import requests
import datetime

FIREBASE_URL = "https://fireguard360-e8f63-default-rtdb.asia-southeast1.firebasedatabase.app/firebase_status.json"

data = {
    "timestamp": datetime.datetime.now(datetime.timezone.utc).isoformat(),
    "test": "hello from Monty",
    "value": 123
}

try:
    resp = requests.put(FIREBASE_URL, json=data)
    print("Status code:", resp.status_code)
    print("Response:", resp.text)
except Exception as e:
    print("Error:", e)
