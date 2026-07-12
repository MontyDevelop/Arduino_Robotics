import pandas as pd
import joblib
import serial
import time
import requests
import datetime
import serial.tools.list_ports
import warnings

# ------ CONFIG ------
FIREBASE_URL = "https://fireguard360-e8f63-default-rtdb.asia-southeast1.firebasedatabase.app/firebase_status.json"
MODEL_FILENAME = "fire_risk_model.pkl"
SERIAL_PORT = "COM3"  # change if your Arduino shows different port
BAUD_RATE = 9600
# --------------------

# (Optional) suppress sklearn warning
warnings.filterwarnings("ignore", message="X does not have valid feature names*")

# List available ports for debug
print("Available serial ports:")
for p in serial.tools.list_ports.comports():
    print(f"  {p.device} - {p.description}")

# Load trained model
print("Loading model...")
model = joblib.load(MODEL_FILENAME)

# Open serial port
ser = None
for attempt in range(1, 4):
    try:
        print(f"Opening serial port {SERIAL_PORT}, attempt {attempt}...")
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Serial port {SERIAL_PORT} opened.")
        break
    except Exception as e:
        print(f"Failed to open serial port: {e}")
        time.sleep(1)

if ser is None:
    print("Could not open serial port. Exiting.")
    exit(1)

time.sleep(2)  # allow Arduino to reset

# Helper: push to Firebase
def push_to_firebase(prediction, sensors):
    payload = {
        "timestamp": datetime.datetime.now(datetime.timezone.utc).isoformat(),
        "prediction": prediction,
        "temperature": sensors.get("temp"),
        "humidity": sensors.get("humidity"),
        "mq2": sensors.get("mq2"),
        "mq135": sensors.get("mq135"),
        "flame": sensors.get("flame"),
        "pir": sensors.get("pir")
    }
    try:
        print("Pushing to Firebase:", payload)
        response = requests.put(FIREBASE_URL, json=payload)
        print("Firebase response code:", response.status_code)
        print("Firebase response body:", response.text)
    except Exception as e:
        print("Firebase push exception:", e)

# Parser to extract sensor values from Arduino serial text
def parse_line(line):
    try:
        if "Temp:" in line and "Humidity:" in line:
            # Example: Temp: 32.5 °C  | Humidity: 50.0 %
            parts = line.split("|")
            temp_part = parts[0]
            hum_part = parts[1]
            temp = float(temp_part.split("Temp:")[1].split("°")[0].strip())
            humidity = float(hum_part.split("Humidity:")[1].split("%")[0].strip())
            return ("temp_hum", temp, humidity)

        elif "MQ2 (gas):" in line and "MQ135 (air):" in line:
            # Example: MQ2 (gas): 250  |  MQ135 (air): 300  |  Flame: YES  |  Motion: NO
            parts = [p.strip() for p in line.split("|")]
            mq2 = int(parts[0].split("MQ2 (gas):")[1].strip())
            mq135 = int(parts[1].split("MQ135 (air):")[1].strip())
            flame = parts[2].split("Flame:")[1].strip()
            motion = parts[3].split("Motion:")[1].strip()
            return ("mq", mq2, mq135, flame, motion)

    except Exception as e:
        print("Parse error:", e, "for line:", line)
        return None

buffer = {}

print("Starting prediction loop...")
while True:
    raw = ser.readline().decode(errors="ignore").strip()
    if not raw:
        continue

    parsed = parse_line(raw)
    if parsed:
        key = parsed[0]
        if key == "temp_hum":
            _, temp, humidity = parsed
            buffer["temp"] = temp
            buffer["humidity"] = humidity
        elif key == "mq":
            _, mq2, mq135, flame, motion = parsed
            buffer["mq2"] = mq2
            buffer["mq135"] = mq135
            buffer["flame"] = 1 if flame.upper() == "YES" else 0
            buffer["pir"] = 1 if motion.upper() == "YES" else 0

    # When all required values are available
    if all(k in buffer for k in ["temp", "humidity", "mq2", "mq135", "flame", "pir"]):
        # Build DataFrame — include new features if model supports them
        features_df = pd.DataFrame([{
            "temperature": buffer["temp"],
            "humidity": buffer["humidity"],
            "mq2": buffer["mq2"],
            "mq135": buffer["mq135"],
            "flame": buffer["flame"],
            "pir": buffer["pir"]
        }])

        # If your model was not trained with flame/pir, drop them
        expected_cols = model.feature_names_in_ if hasattr(model, "feature_names_in_") else None
        if expected_cols is not None:
            features_df = features_df[[c for c in features_df.columns if c in expected_cols]]

        # Predict
        prediction = model.predict(features_df)[0]
        prediction_norm = prediction.upper()
        print(f"Predicted Risk Level: {prediction_norm}  |  Sensors: {buffer}")

        # Send prediction back to Arduino
        try:
            ser.write((prediction_norm + "\n").encode())
        except Exception:
            pass

        # Push to Firebase
        push_to_firebase(prediction_norm, buffer)

        # Reset buffer
        buffer = {}
