# 🧠 Advanced Topics: Multicast, SCTP, Netlink
> **Deep, Practical, Senior-Level Understanding**  
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. Multicast (UDP)

**Concept**: "Radio Station mode".
Instead of sending 1 packet to 1000 clients (1000x bandwidth), you send 1 packet to a **Multicast Group IP** (Class D: `224.0.0.0` to `239.255.255.255`). Switches/Routers replicate it.

### Code Snippet (Joining a Group)
```cpp
struct ip_mreq mreq;
mreq.imr_multiaddr.s_addr = inet_addr("239.0.0.1"); // Group
mreq.imr_interface.s_addr = htonl(INADDR_ANY);      // Local Interface
setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
```

**Use Case**: Stock Market Feeds (Tickers), IPTV.

---

## 🔹 2. SCTP (Stream Control Transmission Protocol)

**Concept**: "TCP + UDP + Multi-Homing".
*   **Message Oriented** (Like UDP).
*   **Reliable** (Like TCP).
*   **Multi-Homing**: One connection uses MULTIPLE IP addresses (WiFi + Ethernet). If one fails, it fails over transparently.

**Use Case**: Telecom Backbones (SS7 over IP), 5G Core Networks. WebRTC Data Channels use SCTP over UDP.

---

## 🔹 3. Netlink (Kernel IPC)

**Concept**: "Talking to the Linux Kernel".
Instead of using `ioctl` (old, messy), Netlink uses a Socket (`AF_NETLINK`) to configure IP addresses, routes, and firewall rules programmatically.

**Use Case**: Implementing a Routing Daemon (BGP/OSPF implementation), or `ip` command replacement in C++.

---

## 🔹 4. Relevant Linux Tools

### 🛠️ ip maddr
Show multicast group memberships.
```bash
ip maddr show
```

### 🛠️ sctp_test
Testing SCTP associations (needs `lksctp-tools`).

### 🛠️ nlmon
Monitor Netlink messages.
```bash
modprobe nlmon
tcpdump -i nlmon0
```
