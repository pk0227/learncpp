/**
 * @file icmp_ping.cpp
 * @brief ICMP Echo Request/Reply (Ping) Implementation
 *
 * SENIOR CONCEPTS:
 * - SOCK_RAW (requires CAP_NET_RAW capability)
 * - ICMP header structure and checksum calculation
 * - Sequence number tracking
 * - RTT (Round Trip Time) measurement
 *
 * COMPILE: g++ -std=c++20 -Wall -Wextra icmp_ping.cpp -o icmp_ping
 * RUN: sudo ./icmp_ping <hostname>
 */

#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PACKET_SIZE 64

// ICMP Checksum Calculation (RFC 1071)
uint16_t calculate_checksum(void *buffer, int length) {
  uint16_t *buf = static_cast<uint16_t *>(buffer);
  uint32_t sum = 0;

  // Sum all 16-bit words
  while (length > 1) {
    sum += *buf++;
    length -= 2;
  }

  // Add leftover byte if any
  if (length == 1) {
    sum += *(uint8_t *)buf;
  }

  // Fold 32-bit sum to 16 bits
  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);

  return static_cast<uint16_t>(~sum);
}

class Ping {
private:
  int sock_fd;
  int sequence;
  struct sockaddr_in dest_addr;
  char dest_ip[INET_ADDRSTRLEN];

public:
  Ping(const char *hostname) : sock_fd(-1), sequence(0) {
    // 1. Resolve hostname to IP
    struct hostent *host = gethostbyname(hostname);
    if (!host) {
      std::cerr << "DNS resolution failed for " << hostname << "\n";
      throw std::runtime_error("DNS error");
    }

    // 2. Setup destination address
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    memcpy(&dest_addr.sin_addr, host->h_addr, host->h_length);
    inet_ntop(AF_INET, &dest_addr.sin_addr, dest_ip, INET_ADDRSTRLEN);

    // 3. Create RAW socket (requires root)
    sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock_fd < 0) {
      perror("Socket creation failed (run with sudo)");
      throw std::runtime_error("Socket error");
    }

    // 4. Set receive timeout
    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    std::cout << "PING " << hostname << " (" << dest_ip << "): " << PACKET_SIZE
              << " data bytes\n";
  }

  ~Ping() {
    if (sock_fd >= 0) {
      close(sock_fd);
    }
  }

  void send_ping() {
    char packet[PACKET_SIZE];
    memset(packet, 0, sizeof(packet));

    // Build ICMP header
    struct icmphdr *icmp = reinterpret_cast<struct icmphdr *>(packet);
    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->un.echo.id = getpid(); // Use PID as identifier
    icmp->un.echo.sequence = htons(++sequence);

    // Fill data section with pattern
    for (size_t i = sizeof(struct icmphdr); i < PACKET_SIZE; i++) {
      packet[i] = i & 0xFF;
    }

    // Calculate checksum (MUST be done after filling packet)
    icmp->checksum = 0;
    icmp->checksum = calculate_checksum(packet, PACKET_SIZE);

    // Send packet
    auto send_time = std::chrono::steady_clock::now();

    if (sendto(sock_fd, packet, PACKET_SIZE, 0,
               reinterpret_cast<struct sockaddr *>(&dest_addr),
               sizeof(dest_addr)) <= 0) {
      perror("sendto failed");
      return;
    }

    // Receive reply
    char recv_buffer[1024];
    struct sockaddr_in recv_addr;
    socklen_t addr_len = sizeof(recv_addr);

    ssize_t bytes_received =
        recvfrom(sock_fd, recv_buffer, sizeof(recv_buffer), 0,
                 reinterpret_cast<struct sockaddr *>(&recv_addr), &addr_len);

    auto recv_time = std::chrono::steady_clock::now();

    if (bytes_received < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        std::cout << "Request timeout for icmp_seq " << sequence << "\n";
      } else {
        perror("recvfrom failed");
      }
      return;
    }

    // Parse reply (skip IP header, get ICMP)
    struct iphdr *ip_hdr = reinterpret_cast<struct iphdr *>(recv_buffer);
    int ip_header_len = ip_hdr->ihl * 4;
    struct icmphdr *icmp_reply =
        reinterpret_cast<struct icmphdr *>(recv_buffer + ip_header_len);

    // Verify it's an Echo Reply
    if (icmp_reply->type == ICMP_ECHOREPLY &&
        icmp_reply->un.echo.id == getpid()) {
      auto rtt = std::chrono::duration_cast<std::chrono::milliseconds>(
          recv_time - send_time);

      std::cout << bytes_received - ip_header_len << " bytes from " << dest_ip
                << ": icmp_seq=" << ntohs(icmp_reply->un.echo.sequence)
                << " ttl=" << static_cast<int>(ip_hdr->ttl)
                << " time=" << rtt.count() << " ms\n";
    } else {
      std::cout << "Received non-echo reply (type=" << (int)icmp_reply->type
                << ")\n";
    }
  }

  void run(int count = 4) {
    for (int i = 0; i < count; i++) {
      send_ping();
      if (i < count - 1) {
        sleep(1);
      }
    }
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: sudo " << argv[0] << " <hostname>\n";
    std::cerr << "Example: sudo " << argv[0] << " google.com\n";
    return 1;
  }

  try {
    Ping ping(argv[1]);
    ping.run(4);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
