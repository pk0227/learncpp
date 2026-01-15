/**
 * @file reactor_pattern.cpp
 * @brief Reactor Pattern (Single-Threaded Event Loop)
 *
 * THE PROBLEM:
 * Blocking I/O (reading from a socket) stops the thread.
 * Thread-per-client doesn't scale to 10k connections (C10k problem).
 *
 * THE SOLUTION:
 * "Inversion of Control" using an Event Demultiplexer (like epoll/select).
 * 1. Register handlers for events (Read/Write).
 * 2. Reactor waits for events (block on select/epoll).
 * 3. Reactor dispatches events to registered handlers synchronously.
 *
 * NOTE: This is a simulation using std::cin as a file descriptor for demo
 * purposes, or just simulating events, to avoid OS-specific system calls in
 * this design pattern demo. In a real Linux system, you would use `epoll_wait`.
 */

#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>

// Using int to represent File Descriptor
using Handle = int;

// Event Types
enum class EventType { READ, WRITE };

// ===================================
// The Event Handler Interface
// ===================================
class EventHandler {
public:
  virtual ~EventHandler() = default;
  virtual void handle_event(Handle handle) = 0;
};

// ===================================
// The Reactor (Event Loop)
// ===================================
class Reactor {
public:
  // Register a handler for a specific handle (FD)
  void register_handler(Handle h, EventHandler *handler) {
    handlers_[h] = handler;
    std::cout << "[Reactor] Registered handler for Handle " << h << "\n";
  }

  // Remove handler
  void remove_handler(Handle h) { handlers_.erase(h); }

  // The Main Event Loop
  void run() {
    std::cout << "[Reactor] Starting Event Loop...\n";
    running_ = true;

    while (running_) {
      // 1. Wait for events (Synchronous Demultiplexing)
      // In real code: int n = epoll_wait(...)
      // Here: We simulate incoming events from "Hardware"
      auto events = wait_for_events();

      // 2. Dispatch events to handlers
      for (auto handle : events) {
        if (handlers_.count(handle)) {
          handlers_[handle]->handle_event(handle);
        }
      }

      // Break for demo
      if (events.empty())
        break;
    }
  }

  void stop() { running_ = false; }

private:
  std::unordered_map<Handle, EventHandler *> handlers_;
  bool running_ = false;

  // Simulation of OS `select` or `epoll_wait`
  std::vector<Handle> wait_for_events() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    static int ticks = 0;
    ticks++;

    // Simulate activity on Handle 1 (Keyboard) and Handle 2 (Network)
    if (ticks < 5) {
      return {1, 2}; // Both behave active
    }
    return {}; // Stop after 5 ticks
  }
};

// ===================================
// Concrete Handlers
// ===================================
class LoggingHandler : public EventHandler {
public:
  void handle_event(Handle h) override {
    std::cout << "  [LoggingHandler] Reading data from Handle " << h << "\n";
  }
};

class NetworkHandler : public EventHandler {
public:
  void handle_event(Handle h) override {
    std::cout << "  [NetworkHandler] Accepted new connection on Handle " << h
              << "\n";
  }
};

// ===================================
// DEMONSTRATION
// ===================================
int main() {
  std::cout << "--- Reactor Pattern Demo ---\n";

  Reactor reactor;

  LoggingHandler logger;
  NetworkHandler network;

  reactor.register_handler(1, &logger);
  reactor.register_handler(2, &network);

  reactor.run();

  return 0;
}
