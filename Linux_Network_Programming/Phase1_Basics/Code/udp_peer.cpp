/**
 * @file udp_peer.cpp
 * @brief UDP Peer-to-Peer Demo
 *
 * SENIOR CONCEPTS:
 * - No Connect, No Accept.
 * - recvfrom() acts as a "Demultiplexer" (receiving from ANYONE).
 * - "Message Oriented" nature (Buffer boundaries matter).
 */

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#define PORT 9091

void receiver_thread(int sockfd) {
  char buffer[1024];
  struct sockaddr_in sender_addr;
  socklen_t len = sizeof(sender_addr);

  std::cout << "[Receiver] Waiting for packets on port " << PORT << "...\n";

  while (true) {
    // BLOCKING CALL - Waits for a packet
    // Unlike TCP, this preserves message boundaries.
    int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                     (struct sockaddr *)&sender_addr, &len);

    if (n > 0) {
      buffer[n] = '\0';
      char ipStr[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &(sender_addr.sin_addr), ipStr, INET_ADDRSTRLEN);

      std::cout << "[Packet Received] From " << ipStr << ":"
                << ntohs(sender_addr.sin_port) << " | Content: " << buffer
                << "\n";
    }
  }
}

int main(int argc, char *argv[]) {
  // 1. Create UDP Socket (SOCK_DGRAM)
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  // 2. Bind (Required to receive)
  struct sockaddr_in local_addr;
  memset(&local_addr, 0, sizeof(local_addr));
  local_addr.sin_family = AF_INET;
  local_addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
  local_addr.sin_port = htons(PORT);

  if (bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
    perror("Bind failed");
    return 1;
  }

  // 3. Start Receiver in Background
  std::thread t(receiver_thread, sockfd);
  t.detach();

  // 4. Sender Logic (Main Thread)
  // To test: Run this, then open another terminal and run `nc -u 127.0.0.1
  // 9091`
  std::cout << "[Sender] Type messages to print to self (Loopback Test):\n";

  struct sockaddr_in dest_addr;
  memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(PORT);
  inet_pton(AF_INET, "127.0.0.1", &dest_addr.sin_addr);

  std::string input;
  while (std::getline(std::cin, input)) {
    sendto(sockfd, input.c_str(), input.length(), 0,
           (struct sockaddr *)&dest_addr, sizeof(dest_addr));
  }

  close(sockfd);
  return 0;
}
