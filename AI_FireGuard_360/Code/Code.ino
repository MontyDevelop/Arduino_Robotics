#include <DHT.h>
#include <Servo.h>

// --------- PIN DEFINITIONS ---------
#define DHTPIN 2          // DHT11 data pin
#define DHTTYPE DHT11     // DHT11 sensor type

#define MQ2_PIN A0        // MQ2 analog output
#define MQ135_PIN A1      // MQ135 analog output
#define FLAME_PIN 7       // Flame sensor digital output
#define PIR_PIN 9         // PIR motion sensor digital output

#define GLED_PIN 3        // Green LED (SAFE)
#define RLED_PIN 4        // Red LED (CRITICAL)
#define YLED_PIN 5        // Yellow LED (WARNING)
#define BUZZER_PIN 6      // Buzzer
#define SERVO_PIN 8       // Servo for door unlock

// --------- THRESHOLDS ---------
#define TEMP_CRITICAL 40.0
#define MQ2_WARNING 250
#define MQ2_CRITICAL 400
#define MQ135_WARNING 250
#define MQ135_CRITICAL 400

#define RISK_WARNING_COUNT 2

// --------- OBJECTS ---------
DHT dht(DHTPIN, DHTTYPE);
Servo doorServo;

// --------- STATE ---------
bool doorUnlocked = false;
unsigned long lastAlertTime = 0;
const unsigned long ALERT_INTERVAL = 5000;

void setup() {
  Serial.begin(9600);
  dht.begin();
  doorServo.attach(SERVO_PIN);

  pinMode(RLED_PIN, OUTPUT);
  pinMode(YLED_PIN, OUTPUT);
  pinMode(GLED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FLAME_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);

  // Start with door locked
  doorServo.write(0);

  Serial.println("=== AI FireGuard 360 Controller with Flame + PIR ===");
}

void loop() {
  delay(1000);

  // ----- READ SENSORS -----
  float humidity = dht.readHumidity();
  float tempC = dht.readTemperature();
  int mq2_raw = analogRead(MQ2_PIN);
  int mq135_raw = analogRead(MQ135_PIN);
  int flameDetected = digitalRead(FLAME_PIN); // LOW = flame detected
  int motionDetected = digitalRead(PIR_PIN);  // HIGH = motion detected

  bool dhtFailed = false;
  if (isnan(humidity) || isnan(tempC)) {
    Serial.println("Warning: DHT read failed.");
    dhtFailed = true;
  }

  // ----- DECIDE RISK LEVELS -----
  int warningCount = 0;
  bool critical = false;

  // Temperature
  if (!dhtFailed) {
    if (tempC >= TEMP_CRITICAL) {
      Serial.println("CRITICAL: High temperature!");
      critical = true;
    } else if (tempC >= (TEMP_CRITICAL - 5)) {
      Serial.println("Warning: Temperature rising.");
      warningCount++;
    }
  }

  // MQ2 gas
  if (mq2_raw >= MQ2_CRITICAL) {
    Serial.println("CRITICAL: Gas concentration very high!");
    critical = true;
  } else if (mq2_raw >= MQ2_WARNING) {
    Serial.println("Warning: Gas level elevated.");
    warningCount++;
  }

  // MQ135 air quality
  if (mq135_raw >= MQ135_CRITICAL) {
    Serial.println("CRITICAL: Poor air quality / CO2 high!");
    critical = true;
  } else if (mq135_raw >= MQ135_WARNING) {
    Serial.println("Warning: Air quality dropping.");
    warningCount++;
  }

  // Flame detection
  if (flameDetected == LOW) {
    Serial.println("CRITICAL: Flame detected!");
    critical = true;
  }

  // PIR motion sensor
  if (motionDetected == HIGH) {
    Serial.println("Motion detected by PIR sensor.");
    if (critical) {
      Serial.println("Emergency confirmed — keeping door unlocked!");
    }
  }

  // Determine overall status
  String status = "SAFE";
  if (critical) {
    status = "CRITICAL";
  } else if (warningCount >= RISK_WARNING_COUNT) {
    status = "WARNING";
  }

  // ----- OUTPUT VALUES -----
  Serial.println("---- SENSOR SUMMARY ----");
  if (!dhtFailed) {
    Serial.print("Temp: "); Serial.print(tempC); Serial.print(" °C  | ");
    Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
  }
  Serial.print("MQ2 (gas): "); Serial.print(mq2_raw);
  Serial.print("  |  MQ135 (air): "); Serial.print(mq135_raw);
  Serial.print("  |  Flame: "); Serial.print(flameDetected == LOW ? "YES" : "NO");
  Serial.print("  |  Motion: "); Serial.println(motionDetected == HIGH ? "YES" : "NO");
  Serial.print("OVERALL STATUS: "); Serial.println(status);
  Serial.println("------------------------");

  // ----- ACTUATORS -----
  if (status == "CRITICAL") {
    showCriticalBehavior();
  } else if (status == "WARNING") {
    showWarningBehavior();
  } else {
    showSafeBehavior();
  }

  Serial.println();
}

// ---- ACTUATOR FUNCTIONS ----
void showSafeBehavior() {
  digitalWrite(GLED_PIN, HIGH);   // Green LED ON
  digitalWrite(YLED_PIN, LOW);
  digitalWrite(RLED_PIN, LOW);
  noTone(BUZZER_PIN);

  if (doorUnlocked) {
    doorServo.write(0);
    doorUnlocked = false;
    Serial.println("Door locked (SAFE mode).");
  }
}

void showWarningBehavior() {
  digitalWrite(GLED_PIN, LOW);
  blinkLED(YLED_PIN, 500); // Yellow LED blink
  digitalWrite(RLED_PIN, LOW);

  if (millis() - lastAlertTime > ALERT_INTERVAL) {
    tone(BUZZER_PIN, 1000, 200);
    lastAlertTime = millis();
  }

  if (doorUnlocked) {
    doorServo.write(0);
    doorUnlocked = false;
  }
}

void showCriticalBehavior() {
  digitalWrite(GLED_PIN, LOW);
  digitalWrite(YLED_PIN, LOW);
  blinkLED(RLED_PIN, 150); // Red LED blink

  tone(BUZZER_PIN, 2000);

  if (!doorUnlocked) {
    doorServo.write(90);
    doorUnlocked = true;
    Serial.println("Door UNLOCKED for evacuation!");
  }
}

// Blink helper
void blinkLED(int ledPin, int intervalMs) {
  static unsigned long lastToggle[10] = {0};
  static bool ledState[10] = {false};

  if (millis() - lastToggle[ledPin] >= intervalMs) {
    ledState[ledPin] = !ledState[ledPin];
    digitalWrite(ledPin, ledState[ledPin] ? HIGH : LOW);
    lastToggle[ledPin] = millis();
  }
}
