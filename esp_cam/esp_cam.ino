#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <esp_camera.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <mbedtls/base64.h>

// -------- WiFi ----------
#define WIFI_SSID "Free_wifi"
#define WIFI_PASSWORD "HR@SMB919293"

// -------- Firebase ----------
#define API_KEY "AIzaSyCllG2iY7lsYv6y8msPpj5fh6NrHdMomw0"
#define DATABASE_URL "https://fireguard360-e8f63-default-rtdb.asia-southeast1.firebasedatabase.app/"   

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// -------- ImgBB ----------
String imgbb_api_key = "638443b6206416ba4b41d8bff033cf59";

// -------- Camera Pins (AI Thinker ESP32-CAM) ----------
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// ---------- Base64 Helper ----------
String encodeToBase64(uint8_t *data, size_t length) {
  size_t outputLength;
  unsigned char *outputBuffer = (unsigned char *)malloc(4 * ((length + 2) / 3));
  if (!outputBuffer) return "";

  mbedtls_base64_encode(outputBuffer, 4 * ((length + 2) / 3), &outputLength, data, length);

  String encoded = String((char *)outputBuffer);
  free(outputBuffer);
  return encoded;
}

// ---------- Camera Init ----------
void startCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;  
  config.jpeg_quality = 10;
  config.fb_count = 1;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }
  Serial.println("Camera ready");
}

// ---------- Upload to ImgBB ----------
String uploadToImgBB(uint8_t *image_data, size_t image_len) {
  HTTPClient http;
  String url = "https://api.imgbb.com/1/upload?key=" + imgbb_api_key;
  http.begin(url);

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Encode image to base64
  String imageBase64 = encodeToBase64(image_data, image_len);
  String postData = "image=" + imageBase64;

  int httpResponseCode = http.POST(postData);
  String imageUrl = "";

  if (httpResponseCode == 200) {
    String response = http.getString();
    Serial.println("ImgBB Response: " + response);

    // Parse JSON
    DynamicJsonDocument doc(4096);
    deserializeJson(doc, response);
    imageUrl = doc["data"]["url"].as<String>();
  } else {
    Serial.printf("Error uploading: %d\n", httpResponseCode);
  }

  http.end();
  return imageUrl;
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  startCamera();
}

void loop() {
  // 🔥 Check Firebase Flame Value
  if (Firebase.RTDB.getInt(&fbdo, "/sensors/flame")) {
    int flame = fbdo.intData();
    if (flame == 1) {
      Serial.println("🔥 Fire detected! Capturing image...");
      
      // Capture picture
      camera_fb_t * fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed");
        return;
      }

      // Upload to ImgBB
      String url = uploadToImgBB(fb->buf, fb->len);
      if (url != "") {
        Serial.println("✅ Uploaded to ImgBB: " + url);
        Firebase.RTDB.setString(&fbdo, "/images/fire", url);
      }

      esp_camera_fb_return(fb);
      delay(15000); // wait 15s before next capture
    }
  }
  delay(2000);
}