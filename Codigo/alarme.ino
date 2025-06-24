#include "SevSeg.h"
#include <Ds1302.h> //Biblioteca RTC DS1302
#include <BluetoothSerial.h>
#include <Preferences.h>

#define CLKpin 23 //Pino CLK
#define DATpin 22 //Pino DAT
#define RSTpin 21 //Pino RST

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
int valorRecebido = -1;

Preferences preferences;
SevSeg sevseg;

Ds1302 rtc(RSTpin, CLKpin, DATpin);

const int buzzer = 32;
const int cl = 5;
const int botao = 33;
bool alarmeAtivo = false;
void setup() {
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);
  pinMode(cl, OUTPUT);  //pino do colon (dois pontos)
  pinMode(botao, INPUT);
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

}

void loop() {
  int pressBotao = 0;

  int timeNow;
  
  Ds1302::DateTime now;
  rtc.getDateTime(&now);

  timeNow = now.minute;
  timeNow += (now.hour * 100);

  sevseg.setNumber(timeNow, 1);


  /*if (SerialBT.available() > 0) {
    valorRecebido =(int)SerialBT.read();
     
    else if(pressBotao == 1){
      digitalWrite(buzzer, LOW);
    }
    
  }*/

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
    Serial.println(preferences.getInt("valorAlarme", 0));
    if (timeNow == preferences.getInt("valorAlarme", 0) && !alarmeAtivo) {
          digitalWrite(buzzer, HIGH);
          alarmeAtivo = true;
    }
    preferences.end();

  sevseg.refreshDisplay();  // Atualiza o display
}