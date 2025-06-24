#include <esp_now.h>
#include <WiFi.h>

const int led = 18, botao = 19;

typedef struct struct_message {
  int comando;
} struct_message;

struct_message msgRecebida;
struct_message msgResposta;

uint8_t peerAddress[] = {0x94, 0x54, 0xC5, 0x6F, 0xCF, 0x48}; // MAC do ESP Alarme

bool alarmeAtivo = false;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&msgRecebida, incomingData, sizeof(msgRecebida));
  if (msgRecebida.comando == 1) {
    digitalWrite(led, HIGH); // Feedback visual
    alarmeAtivo = true;      // Marca que o alarme está ativo
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(botao, INPUT);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao iniciar ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  if (alarmeAtivo && digitalRead(botao) == HIGH) {
    msgResposta.comando = 1;
    esp_now_send(broadcastAddress, (uint8_t *)&msgResposta, sizeof(msgResposta));

    digitalWrite(led, LOW); // Desliga o LED
    alarmeAtivo = false;    // Reset do estado
    delay(500);             // debounce simples
  }
}
