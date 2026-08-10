#include <ESP8266WiFi.h>
#include <LedControl.h>

const char* ssid = "MASTER";
const char* password = "master1267";

// DIN, CLK, CS
LedControl lc = LedControl(D7, D5, D8, 1);

WiFiServer server(80);

bool matrix[8][8];

void updateDisplay() {
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      lc.setLed(0, y, x, matrix[y][x]);
    }
  }
}

void clearMatrix() {
  memset(matrix, 0, sizeof(matrix));
  lc.clearDisplay(0);
}

void setup() {
  Serial.begin(115200);

  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  String req = client.readStringUntil('\r');
  client.flush();

  if (req.indexOf("/draw") != -1) {
    int xIndex = req.indexOf("x=");
    int yIndex = req.indexOf("y=");

    if (xIndex > 0 && yIndex > 0) {
      int x = req.substring(xIndex + 2, req.indexOf("&", xIndex)).toInt();
      int y = req.substring(yIndex + 2).toInt();

      if (x >= 0 && x < 8 && y >= 0 && y < 8) {
        matrix[y][x] = true;
        updateDisplay();
      }
    }
  }

  if (req.indexOf("/clear") != -1) {
    clearMatrix();
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();

  client.println(R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body{
text-align:center;
font-family:Arial;
}
canvas{
border:2px solid black;
touch-action:none;
}
button{
padding:10px 20px;
margin-top:10px;
font-size:18px;
}
</style>
</head>
<body>

<h2>ESP Drawing Pad</h2>

<canvas id="pad" width="320" height="320"></canvas>
<br>
<button onclick="clearPad()">CLEAR</button>

<script>
let canvas = document.getElementById("pad");
let ctx = canvas.getContext("2d");

ctx.fillStyle="white";
ctx.fillRect(0,0,320,320);

function drawPoint(x,y){
  ctx.fillStyle="black";
  ctx.fillRect(x,y,18,18);

  let gx=Math.floor(x/40);
  let gy=Math.floor(y/40);

  fetch(`/draw?x=${gx}&y=${gy}`);
}

canvas.addEventListener("touchmove", function(e){
  let rect = canvas.getBoundingClientRect();
  let x = e.touches[0].clientX - rect.left;
  let y = e.touches[0].clientY - rect.top;

  drawPoint(x,y);
  e.preventDefault();
});

function clearPad(){
  ctx.clearRect(0,0,320,320);
  fetch('/clear');
}
</script>

</body>
</html>
)rawliteral");
}