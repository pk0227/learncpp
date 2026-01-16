/**
 * @file packet_parser.cpp
 * @brief Manual Binary Protocol Parsing (Endianness + Struct Packing)
 *
 * SENIOR CONCEPTS:
 * - __attribute__((packed)) to align structs to wire format.
 * - ntohs/ntohl for Endianness correction.
 * - Safety checks (Buffer Length vs Header Size).
 */

#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

// ============================================================================
// 1. Define Protocol Headers (Mock Ethernet/IP/TCP)
// ============================================================================
// Force 1-byte alignment to match wire format strictly
#pragma pack(push, 1)

struct EthernetHeader {
  uint8_t dest_mac[6];
  uint8_t src_mac[6];
  uint16_t ethertype; // Big Endian
};

struct IPHeader {
  uint8_t version_ihl; // Version (4 bits) + Header Length (4 bits)
  uint8_t tos;
  uint16_t total_length; // Big Endian
  uint16_t id;
  uint16_t flags_offset;
  uint8_t ttl;
  uint8_t protocol;
  uint16_t checksum;
  uint32_t src_ip;  // Big Endian
  uint32_t dest_ip; // Big Endian
};

struct TCPHeader {
  uint16_t src_port;  // Big Endian
  uint16_t dest_port; // Big Endian
  uint32_t seq_num;
  uint32_t ack_num;
  uint8_t data_offset; // 4 bits of Data Offset + Reserved
  uint8_t flags;
  uint16_t window;
  uint16_t checksum;
  uint16_t urgent_ptr;
};

#pragma pack(pop)

// ============================================================================
// 2. Parser Logic
// ============================================================================
void parse_packet(const std::vector<uint8_t> &buffer) {
  const uint8_t *raw_data = buffer.data();
  size_t len = buffer.size();
  size_t processed = 0;

  std::cout << "--- Parsing " << len << " bytes ---\n";

  // A. Parse Ethernet
  if (len < sizeof(EthernetHeader))
    return;
  const EthernetHeader *eth =
      reinterpret_cast<const EthernetHeader *>(raw_data);

  uint16_t protocol = ntohs(eth->ethertype);
  std::cout << "[L2] EtherType: 0x" << std::hex << protocol << std::dec << "\n";

  // Move pointer
  processed += sizeof(EthernetHeader);
  if (protocol != 0x0800) { // If not IPv4, stop here for demo
    std::cout << "Non-IPv4 Packet. Stopping.\n";
    return;
  }

  // B. Parse IP
  if (len - processed < sizeof(IPHeader))
    return;
  const IPHeader *ip = reinterpret_cast<const IPHeader *>(raw_data + processed);

  // Bit masking to get Version (High 4 bits)
  int version = (ip->version_ihl >> 4);
  int ihl = (ip->version_ihl & 0x0F) *
            4; // IHL is in 32-bit words, multiply by 4 for bytes

  char src_str[INET_ADDRSTRLEN];
  char dest_str[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(ip->src_ip), src_str, INET_ADDRSTRLEN);
  inet_ntop(AF_INET, &(ip->dest_ip), dest_str, INET_ADDRSTRLEN);

  std::cout << "[L3] IPv" << version << " | Src: " << src_str
            << " -> Dest: " << dest_str << "\n";

  // Move pointer (Variable length header handling!)
  processed += ihl;

  if (ip->protocol == 6) { // TCP
    // C. Parse TCP
    if (len - processed < sizeof(TCPHeader))
      return;
    const TCPHeader *tcp =
        reinterpret_cast<const TCPHeader *>(raw_data + processed);

    std::cout << "[L4] TCP | Port " << ntohs(tcp->src_port) << " -> "
              << ntohs(tcp->dest_port) << "\n";
  }
}

int main() {
  // Mock Packet Data (Hex dump of a real TCP SYN packet)
  std::vector<uint8_t> packet = {
      // Ethernet Header (14 bytes)
      0x00, 0x0c, 0x29, 0x1f, 0x4a, 0x3d, // Dest MAC
      0x00, 0x50, 0x56, 0xc0, 0x00, 0x08, // Src MAC
      0x08, 0x00,                         // EtherType (IPv4)

      // IP Header (20 bytes)
      0x45, 0x00, 0x00, 0x3c, // Ver/IHL, ToS, Len
      0x1c, 0x46, 0x40, 0x00, // ID, Flags
      0x40, 0x06, 0xb1, 0xe6, // TTL, Proto(TCP), Checksum
      0xc0, 0xa8, 0x01, 0x0a, // Src (192.168.1.10)
      0xc0, 0xa8, 0x01, 0x01, // Dest (192.168.1.1)

      // TCP Header (20 bytes)
      0xd0, 0x0e, 0x00, 0x50, // Src Port (53262), Dest (80)
      0x1a, 0x2b, 0x3c, 0x4d, // Seq Num
      0x00, 0x00, 0x00, 0x00, // Ack Num
      0xa0, 0x02, 0xfa, 0xf0, // Offset, Flags(SYN), Window
      0x91, 0x7c, 0x00, 0x00  // Checksum, Urgent
  };

  parse_packet(packet);
  return 0;
}
