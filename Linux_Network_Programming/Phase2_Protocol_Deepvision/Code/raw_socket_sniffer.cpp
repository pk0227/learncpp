/**
 * @file raw_socket_sniffer.cpp
 * @brief Raw Packet Sniffer using AF_PACKET (Requires PROMISCUOUS Mode usually)
 *
 * DESIGN PATTERNS:
 * - Raw Socket creation (SOCK_RAW).
 * - recvfrom loop to act as a "User Space Network Driver".
 * - Hexdumping the output.
 *
 * NOTE: RUN WITH SUDO.
 */

#include <arpa/inet.h>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <linux/if_ether.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

void print_hex(const unsigned char *buf, int len) {
  for (int i = 0; i < len; i++) {
    if (i > 0 && i % 16 == 0)
      std::cout << "\n";
    std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)buf[i]
              << " ";
  }
  std::cout << std::dec << "\n";
}

int main() {
  // 1. Create Raw Socket (ETH_P_ALL = Catch everything)
  // REQUIRES ROOT (sudo)
  int sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

  if (sock_raw < 0) {
    perror("Socket Error (Did you run with sudo?)");
    return 1;
  }

  std::cout << "Sniffer started! Waiting for packets...\n";

  unsigned char buffer[65536];
  while (true) {
    // 2. Receive Packet
    int data_size = recvfrom(sock_raw, buffer, 65536, 0, NULL, NULL);

    if (data_size < 0) {
      perror("Recvfrom error");
      return 1;
    }

    std::cout << "\n--- Got " << data_size << " Bytes ---\n";

    // 3. Simple Eth Parsing (Min 14 bytes)
    if (data_size >= 14) {
      struct ethhdr *eth = (struct ethhdr *)buffer;

      printf("[Ethernet] Dest: %02x:%02x:%02x:%02x:%02x:%02x | Src: "
             "%02x:%02x:%02x:%02x:%02x:%02x | Proto: %04x\n",
             eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3],
             eth->h_dest[4], eth->h_dest[5], eth->h_source[0], eth->h_source[1],
             eth->h_source[2], eth->h_source[3], eth->h_source[4],
             eth->h_source[5], ntohs(eth->h_proto));
    }

    // Print first 64 bytes for debug
    int print_len = (data_size > 64) ? 64 : data_size;
    print_hex(buffer, print_len);
  }

  close(sock_raw);
  return 0;
}
