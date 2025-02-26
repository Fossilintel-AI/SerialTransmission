#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "serial_packet.h"


//Here we Generaye random data to test with
void generate_and_send_packets() {
    SerialPacket packet;
    packet.sync = SYNC_PATTERN;

    srand(time(NULL));

    while (1) {
        packet.packet_type = rand() % 256;
        packet.rssi = (rand() % 201) - 100;
        packet.length = rand() % MAX_WIFI_DATA;

        for (int i = 0; i < packet.length; i++) {
            packet.wifi_data[i] = rand() % 256;
        }

        packet.timestamp = (uint32_t) time(NULL);
        send_serial_packet(&packet);

        sleep(1);
    }
}


int main() {
    generate_and_send_packets();
    return 0;
}
