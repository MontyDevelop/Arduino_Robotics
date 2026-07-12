import requests
from time import sleep

ESP_IP = "192.168.29.144"  # replace with the IP shown in Serial Monitor

while True:
    try:
        resp = requests.get(f"http://{ESP_IP}/snapshot", timeout=2)
        if resp.status_code == 200:
            with open("latest.jpg", "wb") as f:
                f.write(resp.content)
            print("Snapshot saved, size:", len(resp.content))
            # Here you could call your classifier on latest.jpg
        else:
            print("Snapshot failed:", resp.status_code)
    except Exception as e:
        print("Error fetching snapshot:", e)
    sleep(1)  # wait 1 second before next
