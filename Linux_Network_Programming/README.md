# 🐧 Linux Network Programming & System Design
> **Mastering Low-Level Networking for High-Performance Systems**  
> *Target Level: Senior Systems Engineer (7+ Years)*

## 🎯 Usage Guide
This module is designed to be consumed in order (Phase 1 -> 3). It bridges the gap between "using a library" and "writing the library".

## 📚 Curriculum Structure

### Phase 1: The Core (Crawl) ✅
*   **Theory**: [Sockets](Phase1_Basics/Theory/01_Sockets_and_FDs.md), [TCP Internals](Phase1_Basics/Theory/02_TCP_Handshake_Internals.md), [UDP vs TCP](Phase1_Basics/Theory/03_UDP_vs_TCP.md), [IPv6](Phase1_Basics/Theory/04_IPv4_vs_IPv6.md), [UDS](Phase1_Basics/Theory/05_UNIX_Domain_Sockets.md).
*   **Code**: [TCP Server](Phase1_Basics/Code/tcp_server.cpp), [UDP Peer](Phase1_Basics/Code/udp_peer.cpp), [IPC](Phase1_Basics/Code/unix_socket_ipc.cpp).

### Phase 2: Protocol Deep Vision (Walk) ✅
*   **Theory**: [Endianness](Phase2_Protocol_Deepvision/Theory/01_Endianness_and_Alignment.md), [State Machine](Phase2_Protocol_Deepvision/Theory/02_TCP_State_Machine.md), [Raw Sockets](Phase2_Protocol_Deepvision/Theory/03_Raw_Sockets.md), [DNS](Phase2_Protocol_Deepvision/Theory/04_DNS_Internals.md), [HTTP/HTTPS](Phase2_Protocol_Deepvision/Theory/05_HTTP_HTTPS.md), [ICMP](Phase2_Protocol_Deepvision/Theory/06_ICMP.md), [SSL/TLS](Phase2_Protocol_Deepvision/Theory/07_SSL_TLS_Basics.md), [IPsec](Phase2_Protocol_Deepvision/Theory/08_IPsec.md).
*   **Code**: [DNS Resolver](Phase2_Protocol_Deepvision/Code/dns_resolver.cpp), [Packet Parser](Phase2_Protocol_Deepvision/Code/packet_parser.cpp), [Sniffer](Phase2_Protocol_Deepvision/Code/raw_socket_sniffer.cpp).

### Phase 3: High Performance (Run) ✅
*   **Theory**: [Blocking vs Non-Blocking](Phase3_High_Performance/Theory/01_Blocking_vs_NonBlocking.md), [Multiplexing (Epoll)](Phase3_High_Performance/Theory/02_Multiplexing_Epoll.md), [C10k Problem](Phase3_High_Performance/Theory/03_C10k_Problem.md).
*   **Code**: [Epoll Reactor](Phase3_High_Performance/Code/epoll_reactor.cpp), [Simple HTTP](Phase3_High_Performance/Code/simple_http_server.cpp).

### Phase 4: Interview Preparation (Fly) ✅
*   **Systems Design**: [Chat/Streaming Architectures](Phase4_Interview_Prep/01_Network_Design_Scenarios.md)
*   **Q&A**: [Top 50 Senior Questions](Phase4_Interview_Prep/02_Top_50_Questions.md)

## 🛠️ Prerequisites
*   **Language**: C++20 (Smart Pointers, Lambdas, Concepts).
*   **OS**: Linux (Ubuntu/CentOS/OpenSUSE).
*   **Tools**: `gcc` (10+), `make`, `tcpdump`, `strace`, `openssl`, `cat`, `curl`.

---
*Created by Antigravity under User Direction*
