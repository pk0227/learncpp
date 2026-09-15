/**
 * @file 03_srp_policy_based_design.cpp
 * @brief Demonstrates Zero-Overhead Single Responsibility Principle using Policy-Based Design.
 * 
 * Traditional OOP SRP often leads to class hierarchies connected via virtual pointers,
 * incurring heap allocations, cache misses, and vtable indirection.
 * 
 * In performance-critical domains (HFT, game engines, embedded Linux), we achieve SRP
 * via Andrei Alexandrescu's Policy-Based Design, constrained by C++20 Concepts:
 *   - ValidationPolicy: Responsible SOLELY for boundary and data validation.
 *   - StoragePolicy: Responsible SOLELY for memory management / persistence layout.
 *   - LoggingPolicy: Responsible SOLELY for diagnostic observability.
 * 
 * The host class `MarketDataFeed` orchestrates these policies with:
 *   - ZERO virtual function calls (100% inlined static dispatch).
 *   - ZERO heap allocations (unless required by the storage policy).
 *   - Pluggable single responsibilities swappable at compile time.
 * 
 * @standard C++20
 */

#include <iostream>
#include <vector>
#include <array>
#include <string_view>
#include <concepts>
#include <chrono>

// ============================================================================
// DOMAIN RECORD (Cache-friendly Plain Old Data)
// ============================================================================
struct MarketTick {
    uint32_t symbolId;
    double price;
    uint32_t volume;
    uint64_t timestampNs;
};

// ============================================================================
// C++20 CONCEPTS DEFINING ORTHOGONAL RESPONSIBILITIES
// ============================================================================

template <typename T>
concept ValidationPolicy = requires(T policy, const MarketTick& tick) {
    { policy.isValid(tick) } -> std::same_as<bool>;
};

template <typename T>
concept StoragePolicy = requires(T policy, const MarketTick& tick) {
    { policy.store(tick) } -> std::same_as<void>;
    { policy.count() } -> std::convertible_to<std::size_t>;
};

template <typename T>
concept LoggingPolicy = requires(T policy, std::string_view msg) {
    { policy.log(msg) } -> std::same_as<void>;
};

// ============================================================================
// 1. VALIDATION POLICIES (Single Responsibility: Invariant Checking)
// ============================================================================

// Strict validation policy for live production trading
struct StrictValidationPolicy {
    static bool isValid(const MarketTick& tick) noexcept {
        return tick.price > 0.0 && tick.volume > 0 && tick.symbolId != 0;
    }
};

// Permissive validation for historical replaying / test data
struct PermissiveValidationPolicy {
    static bool isValid(const MarketTick&) noexcept {
        return true; // Accept all ticks
    }
};

// ============================================================================
// 2. STORAGE POLICIES (Single Responsibility: Memory/Storage Strategy)
// ============================================================================

// Fixed-size contiguous circular stack buffer (Zero heap allocation, deterministic)
template <std::size_t Capacity>
class FixedArrayStoragePolicy {
private:
    std::array<MarketTick, Capacity> buffer_{};
    std::size_t head_{0};
    std::size_t count_{0};

public:
    void store(const MarketTick& tick) noexcept {
        buffer_[head_] = tick;
        head_ = (head_ + 1) % Capacity;
        if (count_ < Capacity) ++count_;
    }

    [[nodiscard]] std::size_t count() const noexcept { return count_; }
};

// Dynamic heap vector storage policy for large batch pipelines
class DynamicHeapStoragePolicy {
private:
    std::vector<MarketTick> storage_;

public:
    void store(const MarketTick& tick) {
        storage_.push_back(tick);
    }

    [[nodiscard]] std::size_t count() const noexcept { return storage_.size(); }
};

// ============================================================================
// 3. LOGGING POLICIES (Single Responsibility: Observability / Diagnostics)
// ============================================================================

// Console logger for debugging
struct ConsoleLoggingPolicy {
    static void log(std::string_view msg) {
        std::cout << "[FEED-LOG] " << msg << "\n";
    }
};

// No-op logger: Zero code generated, zero CPU cycles spent
struct NoOpLoggingPolicy {
    static void log(std::string_view) noexcept {}
};

// ============================================================================
// HOST CLASS: Composes Orthogonal Policies via Compile-Time SRP
// ============================================================================
template <
    ValidationPolicy Validator,
    StoragePolicy Storage,
    LoggingPolicy Logger
>
class MarketDataFeed : private Validator, private Storage, private Logger {
public:
    void processIncomingTick(const MarketTick& tick) {
        if (!Validator::isValid(tick)) {
            Logger::log("Invalid market tick rejected by validation policy!");
            return;
        }

        Storage::store(tick);
        Logger::log("Market tick stored successfully.");
    }

    [[nodiscard]] std::size_t getProcessedCount() const noexcept {
        return Storage::count();
    }
};

// ============================================================================
// DEMO
// ============================================================================
int main() {
    std::cout << "=== SRP at Compile-Time: Policy-Based Design (Zero-Overhead) ===\n\n";

    // Configuration A: Ultra-low latency production config
    // Stack-allocated, strict validation, zero logging overhead
    using LowLatencyProdFeed = MarketDataFeed<
        StrictValidationPolicy,
        FixedArrayStoragePolicy<1024>,
        NoOpLoggingPolicy
    >;

    LowLatencyProdFeed prodFeed;
    prodFeed.processIncomingTick(MarketTick{1001, 142.50, 500, 1720000000});
    prodFeed.processIncomingTick(MarketTick{1002, -10.0, 100, 1720000001}); // Rejected silently, 0 logs

    std::cout << "[Prod Feed] Stored ticks: " << prodFeed.getProcessedCount() 
              << " (Compiled without virtual dispatch, no-op logging optimized out)\n\n";

    // Configuration B: Debugging / audit configuration
    // Dynamic heap storage, permissive validation, console logging
    using DebugAuditFeed = MarketDataFeed<
        PermissiveValidationPolicy,
        DynamicHeapStoragePolicy,
        ConsoleLoggingPolicy
    >;

    DebugAuditFeed debugFeed;
    debugFeed.processIncomingTick(MarketTick{2001, 350.75, 1200, 1720000010});
    debugFeed.processIncomingTick(MarketTick{0, 0.0, 0, 1720000011}); // Permissive accepts

    std::cout << "[Debug Feed] Stored ticks: " << debugFeed.getProcessedCount() << "\n";

    return 0;
}
