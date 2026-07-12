#include "esp_camera.h"
#include <WiFi.h>
#include "fire_model.h"   // Your converted model
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

// ===== WiFi Credentials =====
const char* ssid = "Free_wifi";
const char* password = "HR@SMB919293";

// ===== Camera Config (AI Thinker ESP32-CAM) =====
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

// ===== TensorFlow Lite Setup =====
constexpr int tensorArenaSize = 80 * 1024; // 80KB memory for TFLite
uint8_t tensorArena[tensorArenaSize];

const tflite::Model* model;
tflite::MicroInterpreter* interpreter;
TfLiteTensor* input;
TfLiteTensor* output;

// ===== Function: Initialize Camera =====
void initCamera() {
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
  config.pixel_format = PIXFORMAT_GRAYSCALE;  // Reduce to 1 channel for model
  config.frame_size = FRAMESIZE_64X64;        // Match training size
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // Initialize
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    while (true);
  }
}

// ===== Function: Initialize TFLite =====
void initTensorFlow() {
  model = tflite::GetModel(fire_model);
  static tflite::AllOpsResolver resolver;
  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensorArena, tensorArenaSize);
  interpreter = &static_interpreter;

  interpreter->AllocateTensors();

  input = interpreter->input(0);
  output = interpreter->output(0);

  Serial.println("TensorFlow Lite model loaded!");
}

// ===== Function: Send Alert =====
void sendAlert() {
  WiFiClient client;
  if (client.connect("maker.ifttt.com", 80)) {   // Example: IFTTT webhook
    String url = "/trigger/fire_detected/with/key/YOUR_IFTTT_KEY";
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: maker.ifttt.com\r\n" +
                 "Connection: close\r\n\r\n");
    Serial.println("🔥 Fire Alert Sent!");
  }
}

// ===== Setup =====
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  initCamera();
  initTensorFlow();
}

// ===== Loop =====
void loop() {
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // Preprocess image into model input
  for (int i = 0; i < 64 * 64; i++) {
    input->data.uint8[i] = fb->buf[i]; // assuming grayscale
  }

  // Run inference
  if (interpreter->Invoke() != kTfLiteOk) {
    Serial.println("Invoke failed!");
    return;
  }

  // Get result
  float fire_score = output->data.f[0];  // Adjust index based on model
  Serial.printf("🔥 Fire probability: %.2f\n", fire_score);

  if (fire_score > 0.7) {  // Threshold 70%
    sendAlert();
  }

  esp_camera_fb_return(fb);
  delay(2000);
}
