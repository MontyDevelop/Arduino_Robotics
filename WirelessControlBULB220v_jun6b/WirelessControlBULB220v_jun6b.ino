#include <ESP8266WiFi.h>
#include <espnow.h>

#define BUTTON_PIN D1

uint8_t peerMac[] = {
  0x08, 0xF9, 0xE0, 0x6D, 0x66, 0x80
};

struct Message {
  bool relayState;
};

Message data;

bool relayState = false;
bool lastButtonState = HIGH;

void setup() {

  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  esp_now_add_peer(
    peerMac,
    ESP_NOW_ROLE_SLAVE,
    1,
    NULL,
    0
  );

  Serial.println("Transmitter Ready");
}

void loop() {

  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && lastButtonState == HIGH) {

    relayState = !relayState;

    data.relayState = relayState;

    esp_now_send(
      peerMac,
      (uint8_t *)&data,
      sizeof(data)
    );

    if (relayState)
      Serial.println("Bulb ON Command Sent");
    else
      Serial.println("Bulb OFF Command Sent");

    delay(300);
  }

  lastButtonState = buttonState;
}