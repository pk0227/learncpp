# 🧠 Reactor Design Pattern
> **Deep, Practical, Senior-Level Understanding**  
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. What is Reactor? (Real Meaning)

**Standard Definition:**
"Handling service requests delivered concurrently to an application by one or more clients."

**Senior Definition:**
*   ✅ **"Non-Blocking Synchronous Event Demultiplexing"**
*   ✅ **"The Node.js / Redis Architecture"**
*   ✅ **"Solving the C10k Problem"**

Instead of **Thread-Per-Client** (which runs out of RAM at 10k users), you have **1 Thread for 10k Clients**. It blocks on *Events* (Data Ready), not *Connection Threads*.

---

## 🔹 2. Modern & Correct C++ Implementation
**The Event Loop (epoll / kqueue)**

```cpp
// Pseudocode for the Loop
while (running) {
    // 1. Demultiplexing (Block until SOMETHING happens)
    // -1 = infinite timeout
    int n = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    
    // 2. Dispatching (Process only active connections)
    for (int i=0; i<n; ++i) {
        if (events[i].events & EPOLLIN) {
            handleRead(events[i].data.ptr);
        }
        else if (events[i].events & EPOLLOUT) {
            handleWrite(events[i].data.ptr);
        }
    }
}
```

---

## 🔹 3. Pros — Deep Explanation

### ✅ Pro #1: Massive Scalability
*   **The Benefit**: A single thread can manage 100,000 idle connections using very little RAM (just OS buffers + socket struct). Use `Thread-Per-Client`, and 100k stacks (1MB each) = 100GB RAM.

### ✅ Pro #2: Zero Locking
*   **The Benefit**: If logic runs on a single thread (like Redis), you don't need mutexes for shared state. No race conditions.

---

## 🔹 4. Cons — Deep Explanation

### ❌ Con #1: "Blocking The Loop"
*   **The Issue**: If one handler calculates Pi to a billion digits (or touches Disk I/O), **NO ONE** else gets served. The server freezes.
*   **Impact**: You CANNOT do CPU-heavy work in a Reactor.

### ❌ Con #2: Callback Hell / Stack Ripping
*   **The Issue**: Linear logic `read() -> process() -> write()` is split into multiple callbacks.
*   **Impact**: You lose the stack trace. Debugging is hard.

---

## 🔹 5. When to Use / Not Use

### ❌ Do NOT use Reactor when:
*   **The task is CPU Intensive**: Video Encoding, Encryption. Use a Thread Pool.
*   **File I/O**: Accessing Disk is notoriously hard to make truly non-blocking. blocking a Reactor on disk read kills throughput.

### ✅ Acceptable use cases:
*   High-Performance Network Servers (Chat, Proxy, Relay).
*   Connections that are mostly **Idle** (waiting for user input).

---

## 🔹 6. Senior-Level Interview Answer

> "I choose Reactor (using `epoll` or `asio`) for I/O bound high-concurrency servers. It beats Thread-Per-Client by avoiding context switching and stack memory usage. However, I act on the rule: **'Never Block the Event Loop'**. Any CPU-heavy task is offloaded to a separate Thread Pool to keep the Reactor responsive."

---

## 🔹 7. Interview Question Bank

### Q1. Reactor vs Proactor?
**Answer**: 
*   **Reactor**: "Data is Ready". **You** read it. (Linux epoll).
*   **Proactor**: "Read Complete". **OS** reads it into your buffer. (Windows IOCP, Linux io_uring).

### Q2. Edge Triggered (ET) vs Level Triggered (LT)?
**Answer**: 
*   **Level Triggered**: `epoll` wakes you *repeatedly* as long as data is there. Safer.
*   **Edge Triggered**: `epoll` wakes you *once* when data arrives. You must read UNTIL `EAGAIN`. Faster, but easy to leave data stranded.
