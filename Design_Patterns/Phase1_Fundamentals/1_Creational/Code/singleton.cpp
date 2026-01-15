/**
 * @file singleton_pattern.cpp
 * @brief Thread-Safe Singleton Implementation (Meyers Singleton)
 *
 * THE PROBLEM:
 * Ensure a class has only one instance and provide a global point of access to it.
 * Common bad implementations (Double-Checked Locking with raw pointers) are unsafe in C++
 * before C++11, and still error-prone if not done perfectly with std::atomic.
 *
 * THE SOLUTION (Modern C++):
 * "Meyers Singleton"
 * - Uses a static local variable.
 * - C++11 guarantees that static local variable initialization is THREAD-SAFE.
 * - No need for mutexes, atomic locks, or complex double-checked locking login.
 *
 * WHEN TO USE:
 * - Logging Managers
 * - Configuration Loaders
 * - Hardware Interface Managers (e.g., Driver for a single device)
 */

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

class Logger {
public:
    // 1. The Public Access Point
    // Returns a reference to the single instance.
    static Logger& getInstance() {
        // "Magic Static" - Thread-safe since C++11.
        // It is initialized only once, the first time control passes through its declaration.
        static Logger instance; 
        return instance;
    }

    // Example Business Logic: Thread-safe logging
    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_); // Protect internal state
        logs_.push_back(message);
        std::cout << "[LOG]: " << message << std::endl;
    }

    // Delete Copy Constructor and Assignment Operator
    // This prevents creating copies of the Singleton.
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    // 2. Private Constructor
    // Prevents direct instantiation from outside.
    Logger() {
        std::cout << "Logger Initialized (One Time Only)" << std::endl;
    }

    // Private Destructor (Optional, but usually good practice to keep it automatic)
    // ~Logger() {} 

    std::vector<std::string> logs_;
    std::mutex mutex_;
};

// ===================================
// DEMONSTRATION
// ===================================

void worker_thread(int id) {
    Logger& log = Logger::getInstance(); // All threads get the SAME instance
    log.log("Thread " + std::to_string(id) + " is working...");
}

int main() {
    std::cout << "--- Singleton Pattern Demo ---\n";

    std::vector<std::thread> threads;
    for(int i = 0; i < 5; ++i) {
        threads.emplace_back(worker_thread, i);
    }

    for(auto& t : threads) {
        t.join();
    }

    return 0;
}
