#include <ESP8266WiFi.h>
#include <espnow.h>

#define LED_PIN D4

struct Message {
  bool ledState;
};

Message incomingData;

void OnDataRecv(uint8_t *mac, uint8_t *incomingDataPtr, uint8_t len) {
  memcpy(&incomingData, incomingDataPtr, sizeof(incomingData));

  if (incomingData.ledState) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Receiver Ready");
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
}