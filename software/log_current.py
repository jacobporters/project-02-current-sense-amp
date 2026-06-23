### python3 log_current.py ###

import socket
import csv
from datetime import datetime

LAPTOP_IP = ''   # blank = listen on all interfaces
UDP_PORT  = 5005
OUTPUT_FILE = 'current_log.csv'

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((LAPTOP_IP, UDP_PORT))

print(f"Listening on port {UDP_PORT}... writing to {OUTPUT_FILE}")
print("Ctrl+C to stop.\n")

with open(OUTPUT_FILE, 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(['timestamp', 'voltage_V', 'current_mA', 'power_mW'])

    try:
        while True:
            data, addr = sock.recvfrom(1024)
            row = data.decode().strip().split(',')
            timestamp = datetime.now().isoformat()
            writer.writerow([timestamp] + row)
            f.flush()  # write immediately, don't buffer
            print(f"{timestamp}  V={row[0]}  I={row[1]}mA  P={row[2]}mW")
    except KeyboardInterrupt:
        print("\nStopped.")
