#ifndef SERIAL_PACKET_H
#define SERIAL_PACKET_H

#include <stdint.h>

#include <stddef.h> // For size_t

// Declare the function prototype
void uart_write_bytes(const char* buffer, size_t size);


#define SYNC_PATTERN 0xAA55  // Unique sync pattern
#define MAX_WIFI_DATA 256    // Define max array size for CSI data

typedef struct {
    uint16_t sync;         // Sync pattern for packet detection
    uint8_t packet_type;   // Packet type
    int8_t rssi;           // Signal strength
    uint16_t length;       // Length of wifi_data
    int8_t wifi_data[MAX_WIFI_DATA];  // Dynamic array of CSI data (fixed for simplicity)
    uint32_t timestamp;    // Timestamp
    uint16_t crc;         // CRC checksum
} SerialPacket;

// Function prototypes
void serialize_packet(uint8_t *buffer, SerialPacket *packet, uint16_t *size);
uint16_t calculate_crc(const uint8_t *data, uint16_t length);
void send_serial_packet(SerialPacket *packet);
#endif
