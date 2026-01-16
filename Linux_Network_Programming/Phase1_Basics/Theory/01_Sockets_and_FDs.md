# 🧠 Sockets & File Descriptors (The Kernel View)
> **Deep, Practical, Senior-Level Understanding**  
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. What is a Socket? (Real Meaning)

**Standard Definition:**
"An endpoint for communication between two machines."

**Senior Definition:**
*   ✅ **"An Index into the Kernel's Open File Table"**
*   ✅ **"A Kernel Buffer abstracted as a File"**

To a Senior Engineer, a socket is an `int` (File Descriptor) pointing to a `struct socket` in the Kernel. However, **it is NOT a regular file**.
*   **No Offsets**: You cannot `lseek()` a socket. It is a stream, not a disk block.
*   **Readiness**: "Readable" means "Network buffer has data", not "Disk head is positioned".

---

## 🔹 2. The Lifecycle & Production Reality

Many candidates miss the subtlety of `accept()`.

1.  **socket()**: Creates the endpoint. Status: **CLOSED**.
2.  **bind()**: Assigns a local address (IP + Port).
3.  **listen(fd, backlog)**: Marks it as **PASSIVE**. It will never send data. It only spawns others.
    *   *Senior Note*: The `backlog` argument controls the **SYN Queue** (half-open) and **Accept Queue** (established). If this overflows, clients get dropped.
4.  **accept(fd)**: Returns a **NEW File Descriptor**.
    *   *Crucial*: The original `listening_fd` remains open for new connections. The `new_fd` is dedicated to this specific client.

```cpp
// 4. Accept - Crucial distinction
int client_fd = accept(listen_fd, (struct sockaddr*)&addr, &len);
// listen_fd is STILL LISTENING.
// client_fd is the actual connection.
```

---

## 🔹 3. Deep Pros & Cons (The Berkeley API)

### ✅ Pro #1: "Everything is a File" Philosophy
*   **The Benefit**: You can use `read()`, `write()`, `close()`, and `fcntl()` on sockets just like you do on files.
*   **Impact**: Generic code. A Logger class can write to a file or a TCP socket interchangeably.

### ❌ Con #1: The "Stream" Trap
*   **The Issue**: **TCP is a Byte Stream, NOT Message-Based**.
*   **Impact**: `send("Hello")` + `send("World")` gives NO guarantee of arriving as two packets. You might get `recv("He")` then `recv("lloWorld")`. You **MUST** implement Framing (Length Headers or Delimiters).

### ❌ Con #2: Blocking by Default
*   **The Issue**: Default sockets block the thread forever on `recv()`.
*   **Production Reality**: We almost **NEVER** use blocking sockets in C++ servers. We use `fcntl(fd, F_SETFL, O_NONBLOCK)` combined with `epoll` (Linux) or `kqueue` (BSD) to handle 10k+ connections on one thread.

---

## 🔹 4. When to Use / Not Use

### ❌ Do NOT use Raw Sockets when:
*   You just need to fetch a webpage (Use `libcurl` or `Boost.Beast`).
*   You need high-level framing (Use `gRPC` or `ZeroMQ`).

### ✅ Acceptable use cases:
*   Building Custom Protocols (HFT, Game Servers).
*   Low-level Proxies / Load Balancers.

---

## 🔹 5. Relevant Linux Tools

### 🛠️ shutdown vs close (strace interview tip)
*   **`close(fd)`**: Decrements RefCount. Only sends `FIN` if RefCount == 0. Stops BOTH Read and Write.
*   **`shutdown(fd, SHUT_WR)`**: Sends `FIN` immediately (even if other processes have the fd open). Allows "Half-Close": I stop sending, but I can still read your response.

### 🛠️ lsof (List Open Files)
```bash
lsof -p <PID> | grep TCP
# 3u  IPv4 ... (3u = FD 3, Read/Write)
```

---

## 🔹 6. Senior-Level Interview Answer

> "A socket is a file descriptor, but unlike a file, it's non-seekable (`ESPIPE`) and its behavior depends heavily on the **Receive Buffer state**. In production, I treat sockets as **Non-Blocking resources**. I never assume `read()` fills my buffer; I handle **Message Framing** explicitly. Also, I distinguish `close()` from `shutdown()`—`shutdown(SHUT_WR)` is the only correct way to signal 'I am done sending' while keeping the connection open for the acknowledgement."

---

## 🔹 7. Interview Question Bank

### Q1. What happens if I write to a closed socket?
**Answer**: "You get a **SIGPIPE** signal, which crashes the process. I always set `MSG_NOSIGNAL` on `send()` or block the signal globally."

### Q2. `listen(fd, 50)` — What does 50 mean?
**Answer**: "It is the **Backlog**. Historically, it was the sum of SYN_RCVD + ESTABLISHED queues. On modern Linux, it typically limits the **Accept Queue** (fully established connections waiting for `accept()`). If full, the client may receive a connection reset or timeout."

### Q3. Why doesn't `accept()` return the source IP?
**Answer**: "It does! But as an output parameter (`struct sockaddr*`). The return value is strictly the **New File Descriptor** index."
