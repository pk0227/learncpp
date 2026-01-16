# 🧠 TCP Handshake & Internals
> **Deep, Practical, Senior-Level Understanding**  
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. What is the 3-Way Handshake? (Real Meaning)

**Standard Definition:**
"SYN, SYN-ACK, ACK. Used to establish a connection."

**Senior Definition:**
*   ✅ **"State Synchronization"**
*   ✅ **"Resource Allocation Negotiation"**

It is not just about "saying hello". It is about:
1.  **Sequence Number Sync**: "My byte stream starts at Index `X`".
2.  **Window Scale Negotiation**: "I have 1GB RAM, I can handle BIG buffers".
3.  **MSS (Max Segment Size)**: "My Ethernet cable can only take 1500 bytes".

---

## 🔹 2. Reasonable Code Snippet (Backlog Queue Control)

How does the Kernel handle the handshake? It uses the `listen` backlog.

```cpp
#include <sys/socket.h>

void startListen(int sockfd) {
    // BACKLOG = 128
    // This controls the size of the "Accept Queue" (Completed connections waiting for your app)
    if (listen(sockfd, 128) < 0) {
        perror("Listen failed");
        return;
    }
}
```

---

## 🔹 3. Deep Internals (The 2 Queues)
The Linux Kernel maintains TWO queues for a listening socket:

1.  **SYN Queue (Incomplete)**:
    *   Client sent `SYN`.
    *   Server sent `SYN-ACK`.
    *   Waiting for Client `ACK`.
    *   *Attack Vector*: **SYN Flood** fills this queue.

2.  **Accept Queue (Complete)**:
    *   3-Way Handshake DONE.
    *   Connection is ESTABLISHED.
    *   Waiting for Application to call `accept()`.
    *   *Action*: `accept()` removes the connection from this queue and returns a **NEW File Descriptor** dedicated to this session.
    *   *Problem*: If App is slow to accept, this fills up, and new SYN packets are dropped (or ignored depending on `tcp_abort_on_overflow`).

---

## 🔹 4. When to Use / Not Use

### ❌ Do NOT use TCP when:
*   **Latency is strictly critical** (Real-time Gaming, Voice/Video). The "Reliability" (Retries) causes lag spikes (Head-of-Line Blocking). Use UDP.
*   **Broadcast/Multicast** is required. TCP is strictly 1-to-1.

### ✅ Acceptable use cases:
*   File Transfers (FTP, HTTP).
*   Remote Shells (SSH).
*   Database Connections (SQL).
*   Anything requiring **Order** and **Integrity**.

---

## 🔹 5. Relevant Linux Tools

### 🛠️ tcpdump (The Truth)
Watch the handshake live.
```bash
sudo tcpdump -i any port 8080 -nn -S
# IP 1.2.3.4 > 5.6.7.8: Flags [S], seq 12345 ...
# IP 5.6.7.8 > 1.2.3.4: Flags [S.], seq 99999, ack 12346 ...
# IP 1.2.3.4 > 5.6.7.8: Flags [.], ack 100000 ...
```
*   `[S]` = SYN, `[.]` = ACK.

### 🛠️ netstat / ss (Queue Inspection)
Check if your queues are overflowing.
```bash
ss -lnt
# Recv-Q   Send-Q
# 0        128
```
*   **For Listening Sockets**: `Recv-Q` is the current size of the Accept Queue. `Send-Q` is the Max Size (Backlog).
*   If `Recv-Q` > `Send-Q`, your app is too slow!

---

## 🔹 6. Senior-Level Interview Answer

> "TCP provides a reliable byte stream, but it's not magic. I pay close attention to the **Backlog Argument** in `listen()`. If I set it too low, or if my Accept Loop is blocked (e.g., doing Disk I/O before accepting), the kernel drops incoming connections even if the specific `epoll` thread is idle. I also check `/proc/sys/net/ipv4/tcp_syncookies` to ensure we are protected against SYN Floods."

---

## 🔹 7. Interview Question Bank

### Q1. What sequence number does a packet assume?
**Answer**: "**Random**. It never starts at 0. This is to prevent confusion with delayed packets from a *previous* connection on the same port, and to prevent Sequence Number Prediction attacks."

### Q2. What is TCP Fast Open (TFO)?
**Answer**: "An optimization (Google logic) to send data **during** the handshake (inside the SYN packet). It saves 1 Round Trip Time (RTT). Requires kernel support and `MSG_FASTOPEN`."

### Q3. Why does the server need to know MSS?
**Answer**: "To avoid **IP Fragmentation**. If TCP sends a packet larger than the MTU (Maximum Transmission Unit, typically 1500 bytes), the Router fragments it. Fragmentation kills performance (if one fragment is lost, the whole packet is resent). MSS ensures TCP segments fit perfectly in the wire."
