#include <ESP8266WiFi.h>

const char* ssid = "MASTER";
const char* password = "master1267";

WiFiServer server(80);

// LED pins
#define RED D1
#define GREEN D2
#define BLUE D5

void setup() {
  Serial.begin(115200);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to hotspot");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected!");
  Serial.print("ESP IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  String request = client.readStringUntil('\r');
  client.flush();

  if (request.indexOf("/red/on") != -1) digitalWrite(RED, HIGH);
  if (request.indexOf("/red/off") != -1) digitalWrite(RED, LOW);

  if (request.indexOf("/green/on") != -1) digitalWrite(GREEN, HIGH);
  if (request.indexOf("/green/off") != -1) digitalWrite(GREEN, LOW);

  if (request.indexOf("/blue/on") != -1) digitalWrite(BLUE, HIGH);
  if (request.indexOf("/blue/off") != -1) digitalWrite(BLUE, LOW);

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");

  client.println("<html>");
  client.println("<head>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
  client.println("</head>");
  client.println("<body style='text-align:center;font-family:Arial;'>");

  client.println("<h2>ESP8266 LED Control</h2>");

  client.println("<h3>Red LED</h3>");
  client.println("<a href='/red/on'><button style='width:120px;height:50px;'>ON</button></a>");
  client.println("<a href='/red/off'><button style='width:120px;height:50px;'>OFF</button></a>");

  client.println("<h3>Green LED</h3>");
  client.println("<a href='/green/on'><button style='width:120px;height:50px;'>ON</button></a>");
  client.println("<a href='/green/off'><button style='width:120px;height:50px;'>OFF</button></a>");

  client.println("<h3>Blue LED</h3>");
  client.println("<a href='/blue/on'><button style='width:120px;height:50px;'>ON</button></a>");
  client.println("<a href='/blue/off'><button style='width:120px;height:50px;'>OFF</button></a>");

  client.println("</body></html>");
}