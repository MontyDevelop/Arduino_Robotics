#include <ESP8266WiFi.h>
#include <espnow.h>

#define RELAY_PIN D4

struct Message {
  bool relayState;
};

Message incomingData;

void OnDataRecv(uint8_t *mac, uint8_t *incomingDataPtr, uint8_t len) {

  memcpy(&incomingData, incomingDataPtr, sizeof(incomingData));

  if (incomingData.relayState) {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Bulb ON");
  }
  else {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Bulb OFF");
  }
}

void setup() {

  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  WiFi.mode(WIFI_STA);

  Serial.print("Receiver MAC Address: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Receiver Ready");
}

void loop() {
}