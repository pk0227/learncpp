# 🧠 Raw Sockets (AF_PACKET)
> **Deep, Practical, Senior-Level Understanding**  
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. What is a Raw Socket? (Real Meaning)

**Standard Definition:**
"A socket that allows direct access to lower-level protocols."

**Senior Definition:**
*   ✅ **"Bypassing the OS TCP/IP Stack"**
*   ✅ **"User-Space Network Driver"**

Normal sockets (`SOCK_STREAM`) strip headers. The OS gives you "Data".
Raw Sockets (`SOCK_RAW` / `AF_PACKET`) give you the **Entire Ethernet Frame**.
*   You see: `[ Eth Header | IP Header | TCP Header | Data ]`.
*   You must manually parse bits.

---

## 🔹 2. Reasonable Code Snippet (Packet Capture)

Requires `sudo` (Root capability `CAP_NET_RAW`).

```cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if_ether.h> // ETH_P_ALL
#include <unistd.h>
#include <iostream>

void createSniffer() {
    // AF_PACKET = Low Level Interface
    // SOCK_RAW = Raw Data
    // htons(ETH_P_ALL) = Capture EVERYTHING (IP, ARP, IPv6...)
    int sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    
    if (sock_raw < 0) {
        perror("Socket Error (Are you Root?)");
        return;
    }

    unsigned char buffer[65536];
    while(true) {
        // Blocks until ANY packet hits the Network Card
        int data_size = recvfrom(sock_raw, buffer, 65536, 0, NULL, NULL);
        if (data_size > 0) {
            std::cout << "Captured " << data_size << " bytes off the wire!\n";
            // Next step: Manually cast buffer to (struct ethhdr*)
        }
    }
}
```

---

## 🔹 3. Deep Pros & Cons

### ✅ Pros: Absolute Visibility
*   **Debugging**: You can see Retransmissions, Checksum errors, and Handshakes exactly as they appear on the wire.
*   **Custom Protocols**: You can implement a non-standard protocol (e.g., OSPF, ICMP custom types) that the Kernel doesn't understand.

### ❌ Cons: Complexity & Security
*   **No "Flows"**: You get a firehose of mixed packets. You must implement your own Hash Map to track "Connection A" vs "Connection B".
*   **Security Risk**: Malware uses Raw Sockets for "Packet Sniffing" (stealing passwords) and "IP Spoofing" (DDoS attacks).

---

## 🔹 4. When to Use / Not Use

### ❌ Do NOT use Raw Sockets when:
*   You just want to write a standard TCP/UDP server. (You will reinvent the wheel: Retries, Windows, Congestion Control).
*   You are running in a restricted container (Kubernetes usually blocks `CAP_NET_RAW` by default).

### ✅ Acceptable use cases:
*   **Security Tools**: Nmap (Scanner), Wireshark (Capture), Snort (IDS).
*   **Infrastructure**: DHCP Clients (Must read broadcast packets before having an IP), Ping (ICMP).

---

## 🔹 5. Relevant Linux Tools

### 🛠️ tcpdump
The standard raw socket tool.
```bash
tcpdump -i eth0 -n -X
```

### 🛠️ setcap (Capabilities)
Allow a binary to use Raw Sockets WITHOUT full `sudo`.
```bash
sudo setcap cap_net_raw+ep ./my_sniffer
```

### 🛠️ ethtool (Promiscuous Mode)
Does the card drop packets not destined for me?
```bash
ip link set eth0 promisc on
# Now you see ALL traffic on the WiFi/Switch (if not switched)
```

---

## 🔹 6. Senior-Level Interview Answer

> "Raw Sockets (`AF_PACKET`) allow me to receive the full Ethernet frame, including L2 and L3 headers. I use them for building network diagnostics or custom protocols like ICMP. A key nuance is **Promiscuous Mode**; without enabling it on the NIC, the hardware drops packets not addressed to my MAC. Also, reading from a Raw Socket is CPU-intensive because every single packet entering the interface triggers a wakeup; for high-performance sniffing, I'd move to `AF_XDP` or `DPDK` to bypass the kernel overhead."

---

## 🔹 7. Interview Question Bank

### Q1. Why does Ping use Raw Sockets?
**Answer**: "Ping uses ICMP. ICMP is a Network Layer protocol (Layer 3), sitting beside TCP/UDP. There is no standard 'socket stream' for ICMP. `ping` opens a `SOCK_RAW` to manually construct the ICMP Echo Request packet."

### Q2. What is `ETH_P_ALL`?
**Answer**: "A constant passed to `socket()`. It tells the Linux Kernel Network Driver: 'Copy EVERY packet you receive, regardless of protocol (ARP, IP, IPv6), and send it to my socket'."

### Q3. Can I write to a Raw Socket?
**Answer**: "Yes. This is called **Packet Injection**. You effectively spoof the sender. You must calculate the CRC Checksums (IP Checksum, TCP Checksum) manually in software, or the receiving router will drop your packet as corrupt."
