# 🐧 Linux Network Programming Roadmap: The Senior Systems Engineer's Path

## 📅 The Strategy: Crawl, Walk, Run
This module transforms you from "Someone who uses HTTP libraries" to "Someone who builds High-Frequency Trading servers".

> [!NOTE]
> **Constraint Checklist**:
> 1.  **Level**: 7+ Years Experience (Senior details).
> 2.  **Format**: Deep Dive Theory + Modern C++ Code.
> 3.  **Content**: Pros/Cons, When to Use/Not Use, Interview Q&A, **Linux Tools (`tcpdump`, `strace`, etc)**.

---

### 🟢 Phase 1: The Core (Crawl)
*Goal: Master the Berkeley Socket API and the fundamental differences between Streams (TCP) and Datagrams (UDP).*

*   **Socket Fundamentals**
    *   [ ] **Theory**: File Descriptors, Kernel Buffers (Recv-Q/Send-Q), and System Calls.
    *   [ ] **Code**: `tcp_echo_server.cpp` (The 'Hello World' of networking).
*   **Protocols & Addressing** (IPv4 vs IPv6)
    *   [ ] **Theory**: TCP Internals (SYN, ACK) & Dual Stack Addressing (IPv4/IPv6).
    *   [ ] **Theory**: UDP (Fast but Unreliable).
    *   [ ] **Theory**: UNIX Domain Sockets (Inter-Process Communication on localhost).
    *   [ ] **Code**: `unix_ipc_server.cpp` and `udp_peer.cpp`.

### 🟡 Phase 2: Protocol Deep Vision (Walk)
*Goal: Peek under the hood. Manipulating headers, understanding byte ordering, and managing states.*

*   **Data Representation**
    *   [ ] **Theory**: Endianness (Big Endian vs Little Endian). struct alignment. serialization.
    *   [ ] **Code**: `packet_serializer.cpp` (Manual header construction).
*   **Application Layer Internals**
    *   [ ] **Theory**: DNS (Recursive vs Iternative), `getaddrinfo`.
    *   [ ] **Theory**: HTTP/1.1 vs HTTP/2 (Text vs Binary).
    *   [ ] **Theory**: SSL/TLS Basics (Handshake, Certificates, overhead).
    *   [ ] **Tools**: `dig`, `curl -v`, `openssl s_client`.
    *   [ ] **Code**: `dns_resolver.cpp` and `simple_http_parser.cpp`.
*   **Network Layer & Debugging**
    *   [ ] **Theory**: ICMP (The Ping Protocol).
    *   [ ] **Theory**: TCP State Machine (TIME_WAIT, CLOSE_WAIT).
    *   [ ] **Tools**: `tcpdump`, `wireshark` (tshark), `ss -tunlp`.
    *   [ ] **Code**: `raw_socket_sniffer.cpp` (Reading IP headers).

### 🔴 Phase 3: High Performance System Design (Run)
*Goal: Solving the C10k Problem. Moving from Blocking I/O to Event-Driven Architecture.*

*   **I/O Models**
    *   [ ] **Theory**: Blocking vs Non-Blocking I/O (`O_NONBLOCK`).
    *   [ ] **Code**: `non_blocking_connect.cpp`.
*   **Multiplexing (The Evolution)**
    *   [ ] **Theory**: `select()` vs `poll()` vs `epoll()`. Why O(1) matters.
    *   [ ] **Code**: `epoll_reactor.cpp` (Level Triggered vs Edge Triggered).
*   **Advanced Topics (Good to Know)**
    *   [ ] **Theory**: Multicast (UDP).
    *   [ ] **Theory**: Netlink, SCTP, and IPsec overview.
    *   [ ] **Code**: `multicast_sender.cpp`.

### ⚔️ Phase 4: Interview Preparation (The Final Boss)
*   [ ] **Network Design Scenarios**: "Design a Chat System", "Design a multiplayer game protocol".
*   [ ] **Top 50 Network Interview Questions**: "What happens when you type google.com?", "TCP Slow Start?"
