#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>

#define WIFI_SSID "Free_wifi"
#define WIFI_PASSWORD "HR@SMB919293"

#define DATABASE_URL "https://fireguard360-e8f63-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define API_KEY "AIzaSyCllG2iY7lsYv6y8msPpj5fh6NrHdMomw0"
  
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define DHTPIN 4       
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define MQ2_PIN 34      
#define MQ135_PIN 35   
#define FLAME_PIN 26    
#define PIR_PIN 27    

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  dht.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int mq2Value = analogRead(MQ2_PIN);
  int mq135Value = analogRead(MQ135_PIN);
  int flame = digitalRead(FLAME_PIN); 
  int motion = digitalRead(PIR_PIN);
  String risk = "LOW";
  if (temperature > 40 || mq2Value > 2500 || mq135Value > 2000) {
    risk = "HIGH";
  } else if (temperature > 35 || mq2Value > 2000 || mq135Value > 1500) {
    risk = "MEDIUM";
  }
  if (Firebase.ready()) {
    Firebase.RTDB.setFloat(&fbdo, "/sensors/temperature", temperature);
    Firebase.RTDB.setFloat(&fbdo, "/sensors/humidity", humidity);
    Firebase.RTDB.setInt(&fbdo, "/sensors/mq2", mq2Value);
    Firebase.RTDB.setInt(&fbdo, "/sensors/mq135", mq135Value);
    Firebase.RTDB.setInt(&fbdo, "/sensors/flame", flame);
    Firebase.RTDB.setInt(&fbdo, "/sensors/motion", motion);
    Firebase.RTDB.setString(&fbdo, "/sensors/risk_status", risk);
  }
  delay(2000); 
}
