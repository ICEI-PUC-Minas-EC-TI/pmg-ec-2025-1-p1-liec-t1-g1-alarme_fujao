#include "SevSeg.h" //biblioteca para uso do display de 7 segmentos
#include <Ds1302.h> //Biblioteca RTC DS1302
#include <BluetoothSerial.h> //Biblioteca para Bluetooth
#include <Preferences.h> //Biblioteca que salva dados na memoria do ESP32
//Define os pinos do RTC DS1302
#define CLKpin 23 //Pino CLK
#define DATpin 22 //Pino DAT
#define RSTpin 21 //Pino RST

//Configura o Bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT; //Variavel da Classe BluetoothSerial
Preferences preferences;  //Variavel da Classe Preferences
SevSeg sevseg;  //Variavel da Classe Sevseg
Ds1302 rtc(RSTpin, CLKpin, DATpin);//Varialvel da Classe Ds1302 

int valorRecebido = -1; //Valor recebido do app


const int buzzer = 32; //Pino do buzzer
const int cl = 5; //Pino do led de "dois pontos" do display
bool alarmeAtivo = false;

void recebeSinalBloetooth(){
  if (SerialBT.available()) {
       String receivedString = SerialBT.readStringUntil('\n');
       receivedString.trim(); // Remove espaços em branco
       if (receivedString.length() == 4) {
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
}
void tocaAlarme(){
    preferences.begin("salvarNumero", true);
  Serial.println(preferences.getInt("valorAlarme", 0));
  if (timeNow == preferences.getInt("valorAlarme", 0) && !alarmeAtivo) {
        digitalWrite(buzzer, HIGH);
        alarmeAtivo = true;
  }
  preferences.end();
}
void setup() {
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);
  pinMode(cl, OUTPUT);
  pinMode(botao, INPUT);
  rtc.init(); //Inicializa o RTC

  byte numDigits = 4; //Numero de digitos
  byte digitPins[] = {25, 14, 12, 15}; //Pinos dos digitos do display
  byte segmentPins[] = {26, 13, 4, 18, 19, 27, 2}; //Pinos dos segmentos do Display
  bool resistorsOnSegments = false;  //Define que não há resistores nos segmentos
  byte hardwareConfig = COMMON_CATHODE; //Define o display como Catodo
  bool updateWithDelays = false; //Define o update sem delay
  bool leadingZeros = true; //Define que valores menores que 1000 tem 0 antes do numero
  bool disableDecPoint = true; //Desabilita os pontos

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint); //Inicializa o sevseg
  sevseg.setBrightness(90);//Define o brilho do display

  digitalWrite(cl, 1); //Ativa o "dois pontos"

  SerialBT.begin("ESP32_Alarme");//Inicializa o bluetooth

}

void loop() {
  int timeNow;
  
  Ds1302::DateTime now;
  rtc.getDateTime(&now);
  
  timeNow = now.minute;
  timeNow += (now.hour * 100);

  //Escreve o inteiro de timeNow no display
  sevseg.setNumber(timeNow, 1);
  
  recebeSinalBloetooth();
  tocaAlarme();
  
  sevseg.refreshDisplay();  // Atualiza o display
}
