/**
 * @file 03_dip_compile_time_templates.cpp
 * @brief Demonstrates Zero-Overhead Compile-Time Dependency Inversion via C++20 Concepts.
 * 
 * The Senior Systems Architecture Challenge:
 *   - How do you invert dependencies in ultra-low latency systems where virtual functions,
 *     dynamic memory allocations, and pointer indirections are strictly forbidden?
 * 
 * The Solution: Static Dependency Inversion
 *   - The abstraction is defined as a C++20 Concept (`DatabaseSink`, `AlertSink`).
 *   - The high-level service is a class template parameterized by types satisfying the concepts.
 *   - Concrete production types and test mocks satisfy the concepts statically.
 *   - The compiler completely inlines the dependency calls with ZERO runtime indirection!
 * 
 * @standard C++20
 */

#include <iostream>
#include <concepts>
#include <string_view>
#include <vector>
#include <cstdint>
#include <utility>

// ============================================================================
// 1. COMPILE-TIME ABSTRACTIONS (C++20 Concepts)
// ============================================================================

template <typename T>
concept DatabaseSink = requires(T db, std::string_view key, uint64_t val) {
    { db.writeMetric(key, val) } -> std::same_as<void>;
};

template <typename T>
concept AlertSink = requires(T alert, std::string_view msg) {
    { alert.sendAlert(msg) } -> std::same_as<void>;
};

// ============================================================================
// 2. HIGH-LEVEL DOMAIN SERVICE (Depends SOLELY on Concepts, Zero Concrete Types!)
// ============================================================================
template <DatabaseSink DB, AlertSink Alert>
class OrderRiskEngine {
private:
    DB db_;
    Alert alert_;
    uint64_t maxOrderLimit_{1'000'000};

public:
    OrderRiskEngine(DB db, Alert alert)
        : db_(std::forward<DB>(db)), alert_(std::forward<Alert>(alert)) {}

    bool evaluateOrder(std::string_view symbol, uint64_t value) {
        if (value > maxOrderLimit_) {
            alert_.sendAlert("Risk Breach: Order exceeded max threshold!");
            return false;
        }

        // Fast static inline call
        db_.writeMetric(symbol, value);
        return true;
    }
};

// ============================================================================
// 3. LOW-LEVEL PRODUCTION SINKS
// ============================================================================
struct SharedMemoryDatabaseSink {
    void writeMetric(std::string_view key, uint64_t val) noexcept {
        std::cout << "  -> [SHM Sink] Wrote " << key << "=" << val << " to POSIX shared memory ring.\n";
    }
};

struct FiberUdpAlertSink {
    void sendAlert(std::string_view msg) noexcept {
        std::cout << "  -> [Kernel Bypass UDP] Alert fired to risk desk: " << msg << "\n";
    }
};

// ============================================================================
// 4. COMPILE-TIME TEST MOCK
// ============================================================================
struct CompileTimeMockDB {
    std::size_t writeCount{0};
    void writeMetric(std::string_view, uint64_t) noexcept {
        ++writeCount;
    }
};

struct CompileTimeMockAlert {
    std::size_t alertCount{0};
    void sendAlert(std::string_view) noexcept {
        ++alertCount;
    }
};

int main() {
    std::cout << "=== Zero-Cost Compile-Time DIP via C++20 Concepts ===\n\n";

    // 1. Production Assembly (100% Inlined, 0 Vtables!)
    std::cout << "--- 1. High-Performance Production Execution ---\n";
    OrderRiskEngine<SharedMemoryDatabaseSink, FiberUdpAlertSink> prodEngine(
        SharedMemoryDatabaseSink{}, FiberUdpAlertSink{}
    );

    prodEngine.evaluateOrder("AAPL", 500'000);
    prodEngine.evaluateOrder("TSLA", 2'500'000); // Risk breach

    // 2. Unit Testing via Static Mocks
    std::cout << "\n--- 2. Static Unit Testing ---\n";
    CompileTimeMockDB mockDb;
    CompileTimeMockAlert mockAlert;

    OrderRiskEngine<CompileTimeMockDB&, CompileTimeMockAlert&> testEngine(mockDb, mockAlert);
    testEngine.evaluateOrder("MSFT", 100'000);

    std::cout << "Mock DB write count: " << mockDb.writeCount << "\n";
    std::cout << "Static DIP achieved: High-level engine completely decoupled with zero runtime cost!\n";

    return 0;
}
