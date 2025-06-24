#include <esp_now.h>
#include <WiFi.h>

const int led = 18, botao = 19;

typedef struct struct_message {
  bool comando;
} struct_message;

struct_message msgRecebida;
struct_message msgResposta;

uint8_t peerAddress[] = {0x94, 0x54, 0xC5, 0x6F, 0xCF, 0x48}; // MAC do ESP Alarme

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sinal recebido" : "Falha");
}

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&msgRecebida, incomingData, sizeof(msgRecebida));
  if (msgRecebida.comando == 1) {
    digitalWrite(led, HIGH);    // Marca que o alarme está ativo
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(botao, INPUT);

  WiFi.mode(WIFI_STA);
  esp_now_register_send_cb(OnDataSent);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao iniciar ESP-NOW");
    return;
  }
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  if (digitalRead(botao) == HIGH) {
    msgResposta.comando = true;
    esp_now_send(peerAddress, (uint8_t *)&msgResposta, sizeof(msgResposta));

    digitalWrite(led, LOW); // Desliga o LED
    delay(500);             // debounce simples
  }
}