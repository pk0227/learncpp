# 🏛️ Network Design Scenarios (System Design)
> **For 7+ Years Senior Roles**
> Focus: Low-Level Architecture, Protocol Choice, Scalability.

---

## 🔹 Scenario 1: Design a Real-Time Chat App (WhatsApp/Discord)

### 1. Requirements
*   **Scale**: 100 Million Active Users.
*   **Latency**: < 100ms.
*   **Features**: 1:1 Chat, Group Chat, Online Status (Presence).

### 2. Protocol Strategy
*   ❌ **HTTP/REST (Short Polling)**: Too much overhead (Headers). Burns battery. 
*   ❌ **Long Polling**: Better, but still holds threads.
*   ✅ **WebSocket (WSS)**: Persistent connection. Bi-directional.
*   ✅ **Custom TCP**: Even lighter than WS. Used by WhatsApp (Erjang).

### 3. Architecture (The "State" Problem)
*   **Gateway Service**: Holds the TCP connection (Epoll Loop).
*   **Routing**: User A is on Gateway-1. User B is on Gateway-99. How do they talk?
*   **Solution**: **Redis Pub/Sub** or **Kafka**. Gateway-1 publishes "Msg for B" to a topic. Gateway-99 subscribes and pushes to User B's socket.

### 4. Advanced (Senior) Nuance: "Presence"
*   **Heartbeats**: Application-level Ping/Pong every 30s.
*   **Zombie Connections**: If TCP FIN is lost (mobile signal drop), the server thinks the user is online.
*   **Fix**: "Last Seen" timestamp in Redis with a TTL (Time To Live). If Heartbeat misses, key expires -> User Offline.

---

## 🔹 Scenario 2: Design a Stock Ticker (High Frequency)

### 1. Requirements
*   **Throughput**: 1 Million updates/sec.
*   **Latency**: Microseconds.
*   **Integrity**: Unimportant (Old prices are useless).

### 2. Protocol Strategy
*   ❌ **TCP**: Retransmission delay (Head-of-Line blocking) is unacceptable. Ack overhead is too high.
*   ✅ **UDP Multicast**: Send ONCE, received by all subscribers simultaneously.
*   **Optimization**: **Kernel Bypass** (Solarflare / DPDK) to read packets directly from NIC ring buffer.

### 3. Architecture
*   **Feed Handler**: Normalizes Exchange Data -> UDP Multicast Group `239.1.1.1`.
*   **Gap Detection**: Each packet has a Sequence ID. If Client sees `1, 2, 4`, it knows it missed `3`.
*   **Recovery**: Client asks a separate "Snapshot Server" (TCP) for the missing state, but continues processing `4, 5, 6` from the live UDP feed.

---

## 🔹 Scenario 3: Design a File Transfer Service (Dropbox)

### 1. Requirements
*   **Reliability**: 100%.
*   **Files**: Large (10GB+).

### 2. Protocol Strategy
*   ✅ **TCP**: Essential for integrity.
*   **Optimization**: **Zero Copy (`sendfile`)**.
    *   *Standard*: Disk -> Kernel Buffer -> User Buffer -> Kernel Buffer -> NIC. (4 Copies).
    *   *Zero Copy*: Disk -> Kernel Buffer -> NIC. (2 Copies, 0 CPU).

### 3. Flow Control
*   **Problem**: If the client is on 3G (Slow), and Server sends fast, Server Memory fills up.
*   **Solution**: TCP Window Size automatically handles this. Application must respect `EAGAIN` (don't read from Disk if Socket is full).

---

## 🔹 Scenario 4: "The Thundering Herd" (Load Balancer)

### 1. The Problem
*   You have 40 Nginx worker processes listening on Port 80.
*   1 new Request arrives.
*   **All 40 process wake up**. 1 accepts, 39 go back to sleep.
*   **Result**: CPU spikes unnecessarily.

### 2. The Solution
*   **EPOLLEXCLUSIVE**: Linux flag that ensures only **ONE** thread is woken up for a new connection.
*   **SO_REUSEPORT**: Allow multiple sockets to bind to port 80. The Kernel hashes the incoming IP and distributes it to a specific worker queue.

---

## 🔹 Summary for Interviews

| Scenario | Protocol | Key Concept | Senior Nuance |
| :--- | :--- | :--- | :--- |
| **Chat** | WebSocket / TCP | Persistent Conn | Heartbeats & Zombie Detection |
| **Video Streaming** | UDP / QUIC | Smoothness > Integrity | Adaptive Bitrate (HLS/DASH) |
| **Stock Trading** | UDP Multicast | Latency | Kernel Bypass (DPDK) |
| **File Server** | TCP | Reliability | Zero Copy (`sendfile`) |
