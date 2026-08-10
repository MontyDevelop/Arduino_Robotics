#include <ESP8266WiFi.h>
#include <espnow.h>

#define BUTTON_PIN D1

uint8_t peerMac[] = {0x48, 0x3F, 0xDA, 0x87, 0xA4, 0x7E};

struct Message {
  bool ledState;
};

Message data;

bool ledState = false;
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
  esp_now_add_peer(peerMac, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  Serial.println("Transmitter Ready");
}

void loop() {

  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && lastButtonState == HIGH) {

    ledState = !ledState;

    data.ledState = ledState;

    esp_now_send(peerMac, (uint8_t *)&data, sizeof(data));

    Serial.print("LED State Sent: ");
    Serial.println(ledState ? "ON" : "OFF");

    delay(300); // debounce
  }

  lastButtonState = buttonState;
}