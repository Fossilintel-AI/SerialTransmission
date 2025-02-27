import serial
import struct

# Sync Pattern and CRC function used within the application.
SYNC_PATTERN = 0xAA55

def calculate_crc(data):
    crc = 0xFFFF
    for byte in data:
        crc ^= byte
        for _ in range(8):
            if crc & 1:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc >>= 1
    return crc & 0xFFFF


# Open serial port connection
ser = serial.Serial('/dev/ttys006', 115200, timeout=1)

while True:
    #Reaf the available bits
    data = ser.read(512)
    if len(data) < 12:
        continue

    # Try to extract sync pattern
    sync, packet_type, rssi, length = struct.unpack("<HBBH", data[:6])

    if sync != SYNC_PATTERN:
        continue

    wifi_data = list(data[6:6 + length])
    timestamp, crc_received = struct.unpack("<IH", data[6 + length:12 + length])

    #Make sure to Verify CRC
    crc_calculated = calculate_crc(data[:10 + length])

    if crc_calculated != crc_received:
        print("CRC Error! Packet Corrupted")
        continue

    # Print received data
    print(f"Packet Type: {packet_type}, RSSI: {rssi}, Length: {length}")
    print(f"Timestamp: {timestamp}")
    print(f"WiFi Data: {wifi_data}\n")
