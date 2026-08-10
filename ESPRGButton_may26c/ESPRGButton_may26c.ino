#include <ESP8266WiFi.h>

const char* ssid = "MASTER";
const char* password = "master1267";

WiFiServer server(80);

#define RED D1
#define GREEN D2
#define BLUE D5

#define BTN_RED D6
#define BTN_GREEN D7
#define BTN_BLUE D8

void allOff() {
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  pinMode(BTN_RED, INPUT_PULLUP);
  pinMode(BTN_GREEN, INPUT_PULLUP);
  pinMode(BTN_BLUE, INPUT_PULLUP);

  allOff();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {

  if (digitalRead(BTN_RED) == LOW) {
    allOff();
    digitalWrite(RED, HIGH);
  }

  if (digitalRead(BTN_GREEN) == LOW) {
    allOff();
    digitalWrite(GREEN, HIGH);
  }

  if (digitalRead(BTN_BLUE) == LOW) {
    allOff();
    digitalWrite(BLUE, HIGH);
  }

  // Web control
  WiFiClient client = server.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  if (request.indexOf("/red") != -1) {
    allOff();
    digitalWrite(RED, HIGH);
  }

  if (request.indexOf("/green") != -1) {
    allOff();
    digitalWrite(GREEN, HIGH);
  }

  if (request.indexOf("/blue") != -1) {
    allOff();
    digitalWrite(BLUE, HIGH);
  }

  if (request.indexOf("/off") != -1) {
    allOff();
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();

  client.println("<h2>ESP8266 RGB Control</h2>");
  client.println("<a href='/red'><button>RED</button></a><br><br>");
  client.println("<a href='/green'><button>GREEN</button></a><br><br>");
  client.println("<a href='/blue'><button>BLUE</button></a><br><br>");
  client.println("<a href='/off'><button>OFF</button></a>");
}