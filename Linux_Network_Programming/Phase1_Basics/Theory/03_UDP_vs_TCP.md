# 🧠 UDP vs TCP (Stream vs Datagram)
> **Deep, Practical, Senior-Level Understanding**  
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. The Core Distinction (Real Meaning)

**Standard Definition:**
"TCP is reliable/connected. UDP is unreliable/connectionless."

**Senior Definition:**
*   ✅ **"Stream Protocol vs Message Protocol"**
*   ✅ **"Head-of-Line Blocking vs Independent Packets"**

The most important difference is NOT reliability (you can build reliability on UDP). It is **Boundaries**.
*   **TCP**: Stream of bytes. No boundaries. `send("Hello")` + `send("World")` might arrive as one `read("HelloWorld")` bytes. Or worse, **Partial Reads**: You might `recv()` just "He" today and "lloWorld" tomorrow. You **MUST** implement Framing (Length Headers).
*   **UDP**: Packets. `sendto("Hello")` always arrives as exactly `"Hello"`. It is all-or-nothing.

---

## 🔹 2. Reasonable Code Snippet (Datagram Semantics)

Note that UDP does **not** listen or accept. It just binds and waits.

```cpp
// UDP Server - No Listen, No Accept
void udpReader(int sockfd) {
    char buffer[1024];
    struct sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);

    while(true) {
        // Blocks until a FULL MESSAGE arrives
        // recvfrom gives us the sender's address for every packet
        int n = recvfrom(sockfd, buffer, 1024, 0, 
                         (struct sockaddr*)&clientAddr, &len);
        
        if (n > 0) {
            std::cout << "Got " << n << " bytes packet\n";
            // Echo back
            sendto(sockfd, buffer, n, 0, 
                   (struct sockaddr*)&clientAddr, len);
        }
    }
}
```

---

## 🔹 3. Deep Pros & Cons

### ✅ TCP Pros / UDP Cons
*   **Congestion Control**: TCP automatically slows down if the network is full. UDP keeps blasting, potentially killing the network (needs manual pacing).
*   **Ordering**: TCP guarantees sequence. UDP can arrive `Packet 2` before `Packet 1`.

### ✅ UDP Pros / TCP Cons
*   **No Head-of-Line Blocking**:
    *   *TCP*: If Packet 1 is lost, Packet 2, 3, 4 wait in the kernel buffer until Packet 1 is retransmitted. The application sees nothing. **Lag Spike**.
    *   *UDP*: Application receives packets 2, 3, 4 immediately. It handles the missing 1 itself (or ignores it).
*   **Zero Handshake**: 0-RTT. Send immediately.

---

## 🔹 4. When to Use / Not Use

### ❌ Do NOT use UDP when:
*   You are recreating TCP features (Sequence, Retries, Congestion) badly.
*   Security Firewalls are strict (TCP port 443 is valid everywhere; random UDP ports are often blocked).

### ✅ Acceptable use cases:
*   **QUIC / HTTP3**: Modern web runs on UDP to fix TCP's legacy issues.
*   **Voice over IP (VoIP)**: If a voice packet is lost, you don't resend it (it's too late). You just play silence.
*   **Service Discovery**: Multicasting "Who is the Printer?" packets.

---

## 🔹 5. Relevant Linux Tools

### 🛠️ nc -u (Netcat UDP)
Generic UDP client/server.
```bash
# Server
nc -u -l -p 9000
# Client
nc -u 127.0.0.1 9000
```

### 🛠️ iperf3
Benchmark Raw Throughput.
```bash
iperf3 -s           # TCP Server
iperf3 -u -s        # UDP Server
```

---

## 🔹 6. Senior-Level Interview Answer

> "I choose between them based on the **Latency** requirement vs **Reliability** requirement. For a Stock Ticker, I use **UDP (Multicast)** because if you miss a price tick, the next one is coming in 1ms anyway—retrying the old one is useless. For a Trading Order, I use **TCP** because financial accuracy is non-negotiable. Also, when working with TCP, I am always careful about 'Message Framing' (Parsing Length Headers) because TCP does not preserve message boundaries."

---

## 🔹 7. Interview Question Bank

### Q1. Can I use connect() with UDP?
**Answer**: "Yes! It doesn't do a handshake. It simply filters incoming packets at the kernel level (only from that IP) and sets a default destination for `send()`, allowing you to use `send()`/`recv()` instead of `sendto()`/`recvfrom()`."

### Q2. What is the Max size of a UDP packet?
**Answer**: "~64KB (2^16). However, sending large UDP packets (> MTU 1500) causes **IP Fragmentation**, which is terrible for reliability (high loss probability). Good UDP protocols stick to < 1400 bytes."

### Q3. Why is QUIC based on UDP?
**Answer**: "TCP is implemented in the Kernel. Updating the OS Kernel is slow (takes years). UDP allows moving the Congestion/Reliability logic into **User Space** (the Application), allowing rapid evolution (like Chrome updates)."
