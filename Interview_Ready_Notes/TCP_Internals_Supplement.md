# TCP Internals: Headers, Flow Control, & State Management
**Supplement to:** System Programming with Linux (Socket Programming Series)
**Context:** This document covers critical theoretical concepts (Protocol Mechanics) that are often assumed in the lectures but are standard interview requirements for Network Engineering roles.

> **Note for the Learner:** This is a **"Gap Filler"** document. While the lectures focus on *how* to use the API (`socket`, `bind`), this document explains *why* the protocol behaves the way it does (Sequence Numbers, Windows, Congestion).

---

## 1. The TCP Segment Structure (The Header)
To understand reliability, we must understand the bits on the wire. A TCP Segment consists of a **Header** (usually 20 bytes) and a **Data** payload.

### The Header Diagram
```text
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          Source Port          |       Destination Port        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                        Sequence Number                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Acknowledgment Number                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Data |           |U|A|P|R|S|F|                               |
| Offset| Reserved  |R|C|S|S|Y|I|            Window             |
|       |           |G|K|H|T|N|N|                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           Checksum            |         Urgent Pointer        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Options                    |    Padding    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                             data                              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

### Field Definitions (Interview Prerequisites)

1.  **Source & Destination Port (16 bits each):** Identifies the sending and receiving applications (e.g., Src: 54154, Dst: 80).
2.  **Sequence Number (32 bits):**
    *   If SYN flag is set: This is the Initial Sequence Number (ISN).
    *   If SYN is clear: This is the byte number of the *first* byte of data in this segment.
    *   *Purpose:* Allows the receiver to reorder packets that arrive out of order and detect duplicates.
3.  **Acknowledgment Number (32 bits):**
    *   Contains the value of the **Next Sequence Number** the sender of this segment is expecting to receive.
    *   *Note:* TCP ACKs are **cumulative**. If you Ack `1000`, you are saying "I have received everything *up to* byte 999."
4.  **Header Length (4 bits):** Length of the header in 32-bit words. Min value is 5 (20 bytes). Max is 15 (60 bytes).
5.  **Flags (Control Bits):**
    *   **URG:** Urgent Pointer field significant.
    *   **ACK:** Acknowledgment field significant (Set on almost all packets after initial handshake).
    *   **PSH (Push):** Asks receiver to push data to the listening application immediately (don't buffer).
    *   **RST (Reset):** Abort the connection (e.g., port closed, confusion).
    *   **SYN:** Synchronize sequence numbers (Connection start).
    *   **FIN:** No more data from sender (Connection end).
6.  **Window Size (16 bits):**
    *   The number of bytes the sender of this segment is willing to accept.
    *   *Purpose:* **Flow Control**. It stops a fast sender from drowning a slow receiver.
7.  **Checksum (16 bits):** Used for error-checking the header and data.
8.  **Urgent Pointer (16 bits):** Points to the end of "Urgent" data. (Rarely used today).

---

## 2. Flow Control (The Sliding Window)

**Problem:** What if Server A sends data at 1Gbps, but Client B reads sequence numbers at 1Mbps? Client B's buffer will overflow, and packets will drop.
**Solution:** TCP Flow Control.

### Mechanism
*   Each side maintains a **Receive Buffer** (RcvBuffer).
*   The **Receive Window (`rwnd`)** is the amount of spare room in that buffer.
*   `rwnd = RcvBuffer - [LastByteReceived - LastByteRead]`
*   Sender Rule: **Unacked Data Sent <= rwnd**.

### The "Sliding" Concept
Imagine a strip of paper with numbered bytes:
```text
      [Sent & Acked]  [Sent, No Ack]  [Can Send]  [Cannot Send]
... | 1  2  3  4  5 | 6  7  8  9 | 10 11 12 | 13 14 ...
                      ^            ^
                Left Edge      Right Edge
```
1.  **Left Edge:** Moves right when ACKs are received. (Window "slides" forward).
2.  **Right Edge:** Defined by `Left Edge + Window Size`.
3.  **Zero Window Probe:** If `rwnd` becomes 0, the sender must stop. It starts a "persist timer" and sends 1-byte "probes" periodically to ask "Is space available yet?".

---

## 3. Congestion Control (Tahoe, Reno, & Cubic)

**Problem:** Flow control protects the *Receiver*. Who protects the *Network* (Routers/Switches)? If everyone blasts data at once, the internet collapses (Congestion Collapse).
**Solution:** Congestion Control.

### Key Variable
*   **CWND (Congestion Window):** A sender-side variable limiting how much data can be in usage.
*   **Effective Window:** `min(rwnd, cwnd)`.
*   *Note:* `rwnd` is advertised by the *receiver*. `cwnd` is calculated by the *sender* based on network behavior.

### The Phases (Standard TCP Reno)

#### A. Slow Start
*   **Goal:** Quickly find multiple of bandwidth.
*   Start `cwnd = 1 MSS` (Maximum Segment Size).
*   For every ACK received, `cwnd` increases by 1 MSS.
*   **Graph:** Exponential Growth (1, 2, 4, 8, 16...).
*   Stops when `cwnd` reaches `ssthresh` (Slow Start Threshold).

#### B. Congestion Avoidance
*   **Goal:** Probe network limit gently.
*   Occurs when `cwnd >= ssthresh`.
*   Increase `cwnd` by `1 / cwnd` for each ACK (or 1 MSS per RTT).
*   **Graph:** Linear Growth.

#### C. Congestion Detection & Reaction
How does TCP know there is congestion? **Packet Loss**.
1.  **Timeout (Severe):** The Retransmission Timer expires.
    *   *Reaction:* Set `ssthresh = cwnd / 2`. Reset `cwnd = 1`. Restart Slow Start. (The "Tahoe" cliff).
2.  **3 Duplicate ACKs (Mild):** The receiver sends multiple ACKs for the *same* old packet (implying one gap in the stream, but subsequent packets arrived).
    *   *Reaction (Fast Retransmit):* Resend the missing segment immediately (don't wait for timer).
    *   *Reaction (Fast Recovery):* Set `ssthresh = cwnd / 2`. Set `cwnd = ssthresh + 3`. Enter Congestion Avoidance. (Skip Slow Start).

---

## 4. The TCP State Machine (Connection Lifecycle)
Understanding these states is crucial for debugging `netstat` output.

### State Transition Diagram (Simplified)

| State | Who? | Description | Transition Trigger |
| :--- | :--- | :--- | :--- |
| **LISTEN** | Server | Waiting for connection. | `bind()` + `listen()` |
| **SYN_SENT** | Client | Sent SYN, waiting for SYN-ACK. | `connect()` |
| **SYN_RCVD** | Server | Received SYN, sent SYN-ACK. | Receive SYN |
| **ESTABLISHED** | Both | Data transfer phase. | Receive ACK |
| **FIN_WAIT_1** | Active Closer | Decided to close. Sent FIN. | `close()` |
| **FIN_WAIT_2** | Active Closer | Received ACK for FIN. Waiting for peer's FIN. | Receive ACK |
| **CLOSE_WAIT** | **Passive Closer** | Recvd FIN, sent ACK. App still reading? | Receive FIN |
| **LAST_ACK** | Passive Closer | App finally called close. Sent FIN. | `close()` |
| **TIME_WAIT** | **Active Closer** | Sent ACK to peer's FIN. Waiting. | Receive FIN |
| **CLOSED** | Both | No connection. | Timeout or Error |

### The "Big Three" Interview Questions

#### Q1: What is the TIME_WAIT state and why is it needed?
*   **Who enters it?** The side that closes the connection *first* (Active Closer).
*   **Duration:** 2 * MSL (Maximum Segment Lifetime). Usually 1-2 minutes.
*   **Reasons:**
    1.  **Reliable ACK:** If the final ACK is lost, the server will resend its FIN. The client must still be there to re-Ack it.
    2.  **Wandering Packets:** Old duplicate packets from the connection might still be bouncing around the internet. `TIME_WAIT` ensures they expire before a new connection (on the same port) accepts them as valid data.

#### Q2: What is CLOSE_WAIT?
*   **Who enters it?** The side that receives the FIN (Passive Closer).
*   **Meaning:** "The other side is done, but *I* am not."
*   **Danger:** If you see thousands of sockets stuck in `CLOSE_WAIT` on your server, your application has a bug. **You forgot to call `close()`** after `read()` returned 0.

#### Q3: Difference between Flow Control and Congestion Control?
*   **Flow Control:** Prevents overwhelming the **Receiver** (Reader too slow). Controlled by `Advertised Window`.
*   **Congestion Control:** Prevents overwhelming the **Network** (Router buffer overflow). Controlled by `Congestion Window (cwnd)`.

---

## 5. Error Control: Principles
*   **Checksum:** Detects bit errors (flipped bits). If bad, packet is discarded (no NACKs in TCP).
*   **Retransmission:**
    *   **RTO (Retransmission TimeOut):** Calculated dynamically based on RTT (Round Trip Time). `RTO = EstimatedRTT + 4 * DevRTT`.
    *   If RTO expires, packet is resent.
