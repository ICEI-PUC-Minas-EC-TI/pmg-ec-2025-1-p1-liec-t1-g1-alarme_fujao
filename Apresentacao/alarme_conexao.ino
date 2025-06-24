#include "SevSeg.h"
#include <Ds1302.h> //Biblioteca RTC DS1302
#include <BluetoothSerial.h>
#include <Preferences.h>
#include <esp_now.h>
#include <WiFi.h>

#define CLKpin 23 //Pino CLK
#define DATpin 22 //Pino DAT
#define RSTpin 21 //Pino RST

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

uint8_t broadcastAddress[] = {0x5C, 0x01, 0x3B, 0x4D, 0x23, 0xF0};
bool receberPress = false;
bool alarmeAtivo = false;

typedef struct receberMandar {
  bool comando;
} receberMandar;
receberMandar BME280Readings;
receberMandar incomingReadings;
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sinal recebido" : "Falha");
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  receberPress = incomingReadings.comando;
}

BluetoothSerial SerialBT;
int valorRecebido = -1;

Preferences preferences;
SevSeg sevseg;

Ds1302 rtc(RSTpin, CLKpin, DATpin);

const int buzzer = 32;
const int cl = 5;

void setup() {
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);
  pinMode(cl, OUTPUT);  //pino do colon (dois pontos)
  rtc.init();

  byte numDigits = 4;
  byte digitPins[] = {25, 14, 12, 15};
  byte segmentPins[] = {26, 13, 4, 18, 19, 27, 2};
  bool resistorsOnSegments = false; 
  byte hardwareConfig = COMMON_CATHODE; 
  bool updateWithDelays = false; 
  bool leadingZeros = true; 
  bool disableDecPoint = true; 

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);

  digitalWrite(cl, 1);

  SerialBT.begin("ESP32_Alarme");

  WiFi.mode(WIFI_STA);
  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

}

void loop() {
  int timeNow;

  BME280Readings.comando = alarmeAtivo;
  
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &BME280Readings, sizeof(BME280Readings));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }

  Ds1302::DateTime now;
  rtc.getDateTime(&now);

  timeNow = now.minute;
  timeNow += (now.hour * 100);

  sevseg.setNumber(timeNow, 1);

  if (SerialBT.available()) {
       String receivedString = SerialBT.readStringUntil('\n');
       receivedString.trim(); // Remove espaços em branco
       if (receivedString.length() == 4 /*&& isDigit(receivedString.charAt(0)) && isDigit(receivedString.charAt(1)) && isDigit(receivedString.charAt(2)) && isDigit(receivedString.charAt(3))*/) {
         int valorRecebido = receivedString.toInt();
         Serial.print("Número recebido: ");
         Serial.println(valorRecebido);
         preferences.begin("salvarNumero", false);
         preferences.putInt("valorAlarme", valorRecebido);
         preferences.end();
       } else {
         Serial.println("Número inválido recebido.");
       }
     }
    preferences.begin("salvarNumero", true);
    if (timeNow == preferences.getInt("valorAlarme", 0) && !alarmeAtivo) {
          digitalWrite(buzzer, HIGH);
          alarmeAtivo = true;
    }
    preferences.end();

  sevseg.refreshDisplay();  // Atualiza o display
}