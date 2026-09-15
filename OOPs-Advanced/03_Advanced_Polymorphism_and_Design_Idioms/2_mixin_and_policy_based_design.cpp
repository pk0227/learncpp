/**
 * @file 2_mixin_and_policy_based_design.cpp
 * @brief Demonstrates Policy-Based Class Design (Alexandrescu Paradigm):
 *        - Decomposing behavior into orthogonal policy classes.
 *        - Compile-time policy assembly with zero runtime overhead.
 *        - Elimination of deep, rigid inheritance hierarchies.
 */

#include <iostream>
#include <string>
#include <mutex>
#include <vector>

// -----------------------------------------------------------------------------
// 1. Threading Policies
// -----------------------------------------------------------------------------
struct SingleThreadedPolicy {
    void lock() const noexcept {}   // Inlined to NOTHING (0 CPU instructions)
    void unlock() const noexcept {}
};

struct MultiThreadedPolicy {
    mutable std::mutex mtx;
    void lock() const { mtx.lock(); }
    void unlock() const { mtx.unlock(); }
};

// -----------------------------------------------------------------------------
// 2. Output Policies
// -----------------------------------------------------------------------------
struct ConsoleOutputPolicy {
    static void write(const std::string& msg) {
        std::cout << "  [Console] " << msg << "\n";
    }
};

struct BufferOutputPolicy {
    static inline std::vector<std::string> buffer;
    static void write(const std::string& msg) {
        buffer.push_back("[Buffer] " + msg);
    }
};

// -----------------------------------------------------------------------------
// 3. The Policy-Assembled Host Class
// -----------------------------------------------------------------------------
template <typename ThreadingPolicy, typename OutputPolicy>
class SmartLogger : private ThreadingPolicy, private OutputPolicy {
public:
    void log(const std::string& message) {
        ThreadingPolicy::lock();
        OutputPolicy::write(message);
        ThreadingPolicy::unlock();
    }
};

int main() {
    std::cout << "=== Policy-Based Class Composition ===\n";

    // Fast, zero-locking logger for single-threaded performance
    using FastConsoleLogger = SmartLogger<SingleThreadedPolicy, ConsoleOutputPolicy>;
    FastConsoleLogger fast_logger;
    fast_logger.log("High-throughput telemetry message (0 lock overhead)");

    // Thread-safe buffered logger
    using SafeBufferLogger = SmartLogger<MultiThreadedPolicy, BufferOutputPolicy>;
    SafeBufferLogger buffer_logger;
    buffer_logger.log("Thread-safe event recorded to buffer");

    std::cout << "Buffer contents:\n";
    for (const auto& entry : BufferOutputPolicy::buffer) {
        std::cout << "  " << entry << "\n";
    }

    return 0;
}
