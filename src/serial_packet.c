#include "serial_packet.h"
#include <string.h> // For memcpy
#include <stdio.h>  // Debugging
#include <stdio.h> // For fwrite

void uart_write_bytes(const char* buffer, size_t size) {
    fwrite(buffer, 1, size, stdout);
}

// CRC16 Calculation Errir Detection
uint16_t calculate_crc(const uint8_t *data, uint16_t length) {
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc >>= 1;
        }
    }
    return crc;
}

// Serializes packet to byte array
void serialize_packet(uint8_t *buffer, SerialPacket *packet, uint16_t *size) {
    memcpy(buffer, &packet->sync, sizeof(packet->sync));
    buffer[2] = packet->packet_type;
    buffer[3] = packet->rssi;
    memcpy(buffer + 4, &packet->length, sizeof(packet->length));
    memcpy(buffer + 6, packet->wifi_data, packet->length);
    memcpy(buffer + 6 + packet->length, &packet->timestamp, sizeof(packet->timestamp));

    // Computing the CRC
    packet->crc = calculate_crc(buffer, 6 + packet->length + sizeof(packet->timestamp));
    memcpy(buffer + 10 + packet->length, &packet->crc, sizeof(packet->crc));

    *size = 12 + packet->length;
}

// Send data over UART
void send_serial_packet(SerialPacket *packet) {
    uint8_t buffer[512];
    uint16_t size;

    serialize_packet(buffer, packet, &size);
    uart_write_bytes((const char*)buffer, size);
}
