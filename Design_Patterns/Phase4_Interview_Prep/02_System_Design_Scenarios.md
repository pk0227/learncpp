# 🏙️ System Design Scenarios (Pattern Application)

For a Senior Engineer, you are not asked "Write a Singleton".
You are asked "Design a Logging Library". You must **derive** the patterns.

---

## 📜 Scenario 1: Cross-Platform Logging Library

### Requirements
1.  Must work on Linux, Windows, macOS.
2.  Must support writing to Console, File, and Network DB simultaneously.
3.  Must not block the main application thread (Async logging).
4.  Must allow runtime filtering (Error only vs Debug).

### Suggested Patterns
1.  **Singleton**: (Controversial) Likely for the strictly global `LogManager`.
2.  **Abstract Factory**: To create platform-specific writers (`LinuxFileSink`, `WindowsConsoleSink`).
3.  **Observer (or Chain of Responsibility)**:
    *   The Logger interacts with multiple "Sinks" (FileSink, NetSink).
    *   When a log comes in, notify all Sinks.
4.  **Producer-Consumer (Concurrency)**:
    *   The `log()` function should simple push a string to a `ThreadSafeQueue`.
    *   A background thread (Worker) pops logs and writes to disk (Slow I/O).

### Interview Diagram
```
[Application Thread] -> call log() -> [ThreadSafeQueue]
                                           |
                                     [Background Thread]
                                           |
                                      [LogDispatcher]
                                     /       |       \
                               [FileSink] [DbSink] [ConsoleSink]
```

---

## 📈 Scenario 2: High-Frequency Trading System

### Requirements
1.  Extremely low latency (microseconds matter).
2.  Process market data feeds (Prices).
3.  Execute trades based on algorithms.

### Patterns to USE
1.  **CRTP (Static Polymorphism)**: Virtual functions are banned in the hot path. Use `OrderBook<NasdaqStrategy>` to resolve calls at compile time.
2.  **Object Pool**: Do not `new/delete` orders. Pre-allocate 10,000 Order objects and reuse them to avoid heap fragmentation and malloc latency.
3.  **Ring Buffer (Disruptor)**: Lock-free queues for passing market data to strategy threads.

### Patterns to AVOID
*   **Observer**: Traditional virtual-function observer is too slow. use static listeners.
*   **Exceptions**: Disable them in the hot path.

---

## 🌐 Scenario 3: Web Server (Handling 10k Clients)

### Requirements
1.  Handle 10,000 concurrent connections.
2.  Serve static files and JSON API.

### Architecture
1.  **Reactor Pattern**: Use `epoll` single-thread loop to accept connections and read requests.
2.  **Thread Pool**:
    *   Do NOT process business logic in the Reactor thread.
    *   Reactor -> receives "GET /compute" -> Pushes to ThreadPool.
    *   Worker -> Computes -> Pushes response back to Reactor.
    *   Reactor -> Writes to socket.

### Key Class Design
*   `EventDemultiplexer` (wrapper around `epoll`).
*   `EventHandler` (Interface `.handleRead()`).
*   `Acceptor` (Special handler for new connections).
