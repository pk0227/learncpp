/**
 * @file raii_patterns.cpp
 * @brief RAII (Resource Acquisition Is Initialization) Demonstrations
 *
 * SENIOR CONCEPTS:
 * - Smart pointers (unique_ptr, shared_ptr)
 * - Lock guards for thread safety
 * - Custom RAII wrappers for C APIs
 * - Exception safety guarantees
 *
 * COMPILE: g++ -std=c++20 -pthread -Wall -Wextra raii_patterns.cpp -o
 * raii_patterns RUN: ./raii_patterns
 */

#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

// ============================================================================
// Example 1: RAII for File Handles
// ============================================================================
class FileLogger {
private:
  std::ofstream file; // RAII: fstream handles FILE* internally

public:
  FileLogger(const std::string &filename) : file(filename, std::ios::app) {
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open log file");
    }
    std::cout << "[FileLogger] File opened: " << filename << "\n";
  }

  ~FileLogger() {
    std::cout << "[FileLogger] File closed automatically\n";
    // file.close() is called automatically by std::ofstream destructor
  }

  void log(const std::string &message) {
    if (file.is_open()) {
      file << message << std::endl;
    }
  }

  // Delete copy (files shouldn't be copied)
  FileLogger(const FileLogger &) = delete;
  FileLogger &operator=(const FileLogger &) = delete;
};

// ============================================================================
// Example 2: RAII for Mutex (Thread Safety)
// ============================================================================
class ThreadSafeCounter {
private:
  mutable std::mutex mtx;
  int count = 0;

public:
  void increment() {
    std::lock_guard<std::mutex> lock(mtx); // RAII: Lock acquired
    ++count;
    // Lock automatically released when 'lock' goes out of scope
  }

  int get() const {
    std::lock_guard<std::mutex> lock(mtx);
    return count;
  } // Lock released here (even if exception thrown)
};

// ============================================================================
// Example 3: Custom RAII Wrapper for Socket File Descriptor
// ============================================================================
class SocketHandle {
private:
  int fd;

public:
  explicit SocketHandle(int socket_fd) : fd(socket_fd) {
    std::cout << "[SocketHandle] Acquired FD: " << fd << "\n";
  }

  ~SocketHandle() {
    if (fd >= 0) {
      std::cout << "[SocketHandle] Closing FD: " << fd << "\n";
      // In real code: close(fd);
    }
  }

  int get() const { return fd; }

  // Delete copy, allow move
  SocketHandle(const SocketHandle &) = delete;
  SocketHandle &operator=(const SocketHandle &) = delete;

  SocketHandle(SocketHandle &&other) noexcept : fd(other.fd) {
    other.fd = -1; // Transfer ownership
  }

  SocketHandle &operator=(SocketHandle &&other) noexcept {
    if (this != &other) {
      if (fd >= 0) { /* close(fd); */
      }
      fd = other.fd;
      other.fd = -1;
    }
    return *this;
  }
};

// ============================================================================
// Example 4: Smart Pointers (Memory RAII)
// ============================================================================
class Resource {
public:
  Resource() { std::cout << "[Resource] Constructed\n"; }
  ~Resource() { std::cout << "[Resource] Destroyed\n"; }
  void use() { std::cout << "[Resource] Using resource\n"; }
};

void demonstrate_smart_pointers() {
  std::cout << "\n=== Smart Pointer Demo ===\n";

  {
    // unique_ptr: Exclusive ownership
    std::unique_ptr<Resource> ptr1 = std::make_unique<Resource>();
    ptr1->use();
    // No need to explicitly delete - automatic cleanup
  } // Resource destroyed here

  {
    // shared_ptr: Shared ownership
    std::shared_ptr<Resource> ptr2 = std::make_shared<Resource>();
    {
      std::shared_ptr<Resource> ptr3 = ptr2; // Ref count = 2
      std::cout << "Ref count: " << ptr2.use_count() << "\n";
    } // Ref count = 1
    std::cout << "Ref count: " << ptr2.use_count() << "\n";
  } // Ref count = 0, Resource destroyed
}

// ============================================================================
// Example 5: Exception Safety with RAII
// ============================================================================
void exception_safety_demo() {
  std::cout << "\n=== Exception Safety Demo ===\n";

  try {
    SocketHandle sock(42);
    std::lock_guard<std::mutex> lock(*(new std::mutex())); // BAD! Don't do this

    throw std::runtime_error("Simulated error");

    // Even though exception is thrown:
    // - sock's destructor IS called (FD closed)
    // - lock's destructor IS called (mutex unlocked)
    // - The 'new mutex' is leaked (not RAII!)
  } catch (const std::exception &e) {
    std::cout << "Caught: " << e.what() << "\n";
    std::cout << "But all RAII resources were cleaned up!\n";
  }
}

// ============================================================================
// Main: Demonstrate All Patterns
// ============================================================================
int main() {
  std::cout << "=== RAII Patterns Demo ===\n\n";

  // 1. File RAII
  {
    std::cout << "--- File Logging ---\n";
    FileLogger logger("test.log");
    logger.log("This is a test message");
    logger.log("RAII ensures file is closed");
  } // File automatically closed here

  // 2. Thread-Safe Counter
  {
    std::cout << "\n--- Thread-Safe Counter ---\n";
    ThreadSafeCounter counter;

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
      threads.emplace_back([&counter]() {
        for (int j = 0; j < 100; ++j) {
          counter.increment();
        }
      });
    }

    for (auto &t : threads) {
      t.join();
    }

    std::cout << "Final count: " << counter.get() << " (expected: 1000)\n";
  }

  // 3. Socket RAII
  {
    std::cout << "\n--- Socket Handle ---\n";
    SocketHandle sock(123);
    std::cout << "Using socket FD: " << sock.get() << "\n";
  } // Socket automatically closed

  // 4. Smart Pointers
  demonstrate_smart_pointers();

  // 5. Exception Safety
  exception_safety_demo();

  std::cout << "\n=== All Demos Complete ===\n";
  return 0;
}
