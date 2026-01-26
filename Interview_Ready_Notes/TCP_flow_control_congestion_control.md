
🌐 TCP STREAM SOCKETS: SLIDING WINDOW & CONGESTION WINDOW (COMPLETE GUIDE)
================================================================================

A **TCP stream socket** provides a:
- Reliable
- Full-duplex
- Stream-oriented
- Connection-oriented channel

Two critical safety mechanisms make this possible:
1) **Flow Control**  → protects the RECEIVER
2) **Congestion Control** → protects the NETWORK

This document explains BOTH in depth.


1️⃣ FLOW CONTROL — SLIDING WINDOW MECHANISM
================================================================================

----------------------------------------
🔑 WHAT PROBLEM DOES FLOW CONTROL SOLVE?
----------------------------------------
Prevent the sender from overwhelming the **receiver**.

Even if the network is fast:
- Receiver buffer is finite
- Application may read slowly

Without flow control:
- Receiver buffers overflow
- Data loss occurs

----------------------------------------
🧠 CORE IDEA (PLAIN ENGLISH)
----------------------------------------
The receiver tells the sender:

"You may send up to **N bytes** without waiting for acknowledgments."

That **N** is called the **Receiver Window (rwnd)**.

The sender:
- Can send multiple bytes at once
- Does NOT wait for ACK after every send
- Slides the window forward as ACKs arrive

Hence the name: **Sliding Window**

----------------------------------------
📦 WHY IT IS NEEDED FOR STREAM SOCKETS
----------------------------------------
- TCP is **byte-stream oriented**
- No message boundaries
- Reads and writes are decoupled

Sender may send:
- 100 bytes in one call

Receiver may read:
- 25 bytes at a time

Flow control keeps this safe.

----------------------------------------
🧩 HOW SLIDING WINDOW WORKS (STEP-BY-STEP)
----------------------------------------

Assume:
- Receiver window (rwnd) = 5 bytes
- Byte numbers used for clarity

Initial state:
```
Allowed to send bytes: 1 2 3 4 5
```

Sender sends:
```
[1][2][3][4][5]
```

Receiver processes data and sends:
```
ACK = 3
```

Meaning:
"I have received bytes 1 and 2"

Now the window SLIDES:
```
Old window: [1 2 3 4 5]
New window:     [3 4 5 6 7]
```

Sender can now send bytes:
```
6 and 7
```

----------------------------------------
🧠 IMPORTANT DETAILS
----------------------------------------
- Sliding window is measured in **BYTES**, not packets
- Controlled by the **receiver**
- Advertised via TCP header field
- Prevents receiver buffer overflow

----------------------------------------
🎯 INTERVIEW-SAFE ONE-LINER
----------------------------------------
Sliding window is TCP’s flow control mechanism that allows a sender to transmit
multiple bytes without waiting for acknowledgments, limited by the receiver’s
advertised window size.


2️⃣ CONGESTION CONTROL — CONGESTION WINDOW (cwnd)
================================================================================

----------------------------------------
🔑 WHAT PROBLEM DOES CONGESTION CONTROL SOLVE?
----------------------------------------
Prevent the sender from overwhelming the **network**.

Even if:
- Receiver is fast
- Receiver buffer is large

The **network itself** may be slow or congested.

----------------------------------------
🧠 CORE IDEA (PLAIN ENGLISH)
----------------------------------------
TCP continuously asks:

"How much data can the network safely carry right now?"

The answer is maintained as:
```
Congestion Window (cwnd)
```

This window represents:
- Sender’s estimate of network capacity

----------------------------------------
📦 WHY SLIDING WINDOW IS NOT ENOUGH
----------------------------------------

Example:
- Receiver window (rwnd) = 1 MB
- Network capacity = 32 KB

If only sliding window existed:
- Sender sends 1 MB
- Routers drop packets
- Network congestion collapse

Hence:
```
Congestion control is mandatory
```

----------------------------------------
🧩 CONGESTION WINDOW (cwnd)
----------------------------------------
- Maintained by the **sender**
- Limits unacknowledged data in flight
- Adjusted dynamically based on network feedback

----------------------------------------
🚦 CONGESTION CONTROL ALGORITHMS
----------------------------------------

These algorithms control how **cwnd** grows or shrinks.

----------------------------------------
1️⃣ SLOW START
----------------------------------------
- Initial cwnd is small
- cwnd grows exponentially
- Safely probes network capacity

Example:
```
cwnd = 1 → 2 → 4 → 8 → 16 ...
```

----------------------------------------
2️⃣ CONGESTION AVOIDANCE
----------------------------------------
- cwnd grows linearly
- Avoids sudden overload

Example:
```
cwnd = cwnd + 1 per RTT
```

----------------------------------------
3️⃣ FAST RETRANSMIT
----------------------------------------
- Triggered by duplicate ACKs
- Retransmits lost data before timeout

----------------------------------------
4️⃣ FAST RECOVERY
----------------------------------------
- cwnd is reduced but not reset to 1
- Prevents going back to full slow start

----------------------------------------
🧠 HOW TCP DETECTS CONGESTION
----------------------------------------
TCP assumes:
```
Packet loss = congestion
```

Detected via:
- Timeout
- Duplicate ACKs

----------------------------------------
🎯 INTERVIEW-SAFE ONE-LINER
----------------------------------------
Congestion control is TCP’s mechanism to dynamically adjust the sender’s rate
using the congestion window and algorithms like slow start and congestion
avoidance based on network feedback.


3️⃣ SLIDING WINDOW vs CONGESTION WINDOW (CRITICAL DIFFERENCE)
================================================================================

----------------------------------------
📊 COMPARISON TABLE
----------------------------------------

Aspect                | Sliding Window        | Congestion Window
--------------------- | --------------------- | ---------------------
Protects              | Receiver              | Network
Controlled by          | Receiver              | Sender
Window name            | rwnd                  | cwnd
Trigger                | Receiver buffer size  | Loss / RTT / ACKs
Purpose                | Flow control          | Congestion control

----------------------------------------
🧠 ACTUAL TCP SENDING RULE (VERY IMPORTANT)
----------------------------------------

```
Effective Send Window = min(rwnd, cwnd)
```

Meaning:
- Even if receiver allows more
- Sender sends only what network can handle

----------------------------------------
📦 EXAMPLE
----------------------------------------

Assume:
```
rwnd = 64 KB
cwnd = 16 KB
```

Sender can send:
```
16 KB (not 64 KB)
```

Now if network improves:
```
cwnd increases → throughput increases
```

----------------------------------------
🧠 WHY THIS MATTERS FOR STREAM SOCKETS
----------------------------------------
Stream sockets guarantee:
- Reliability
- In-order delivery
- No data loss

These guarantees come from:
- Sliding window → receiver safety
- Congestion control → network safety

----------------------------------------
🎯 FINAL INTERVIEW-GRADE SUMMARY (🔥)
----------------------------------------

Flow control safety in TCP is achieved by the sliding window mechanism using the
receiver window (rwnd), while congestion control safety is achieved by regulating
the sender’s transmission rate using the congestion window (cwnd), dynamically
adjusted by congestion control algorithms such as slow start and congestion
avoidance; the sender transmits data limited by min(rwnd, cwnd).

================================================================================
