/**
 * @file 01_solid_hft_order_gateway.cpp
 * @brief Production Case Study: Ultra-Low-Latency HFT Order Gateway with Zero-Cost SOLID Architecture.
 * 
 * Architectural Challenge:
 *   - The business demands a modular, testable, extensible trading gateway supporting multiple
 *     exchanges (CME iLink3, NASDAQ OUCH, Eurex T7).
 *   - However, the trading desk enforces a strict < 250ns latency budget on the critical path.
 *   - Traditional GoF OOP (virtual tables, heap-allocated smart pointers, dynamic dispatch)
 *     is strictly forbidden because of cache misses and indirect branch mispredictions.
 * 
 * The Senior Architect's Solution: Zero-Cost SOLID via C++20 Concepts & Value Semantics
 *   - Single Responsibility (SRP): Risk validation, binary encoding, and socket transmission
 *     are cleanly separated into distinct, orthogonal policy classes.
 *   - Open/Closed (OCP): New exchanges are added by writing new protocol traits/encoders with
 *     zero modifications to the core matching pipeline.
 *   - Liskov Substitution (LSP): Compile-time concepts guarantee behavioral contracts,
 *     `noexcept` invariants, and deterministic return types.
 *   - Interface Segregation (ISP): Finely segregated concepts (`RiskChecker`, `WireEncoder`, `NetworkTransport`).
 *   - Dependency Inversion (DIP): The high-level `OrderExecutionGateway` depends entirely on
 *     concept abstractions, achieving 100% compile-time dependency inversion.
 * 
 * @standard C++20
 */

#include <iostream>
#include <concepts>
#include <string_view>
#include <array>
#include <cstdint>
#include <chrono>

// ============================================================================
// DOMAIN ENTITY: Cache-Line Aligned Order (Zero dynamic allocation)
// ============================================================================
enum class Side : uint8_t { Buy = 1, Sell = 2 };

struct alignas(64) ClientOrder {
    uint64_t orderId;
    uint32_t symbolId;
    uint32_t quantity;
    uint64_t priceInCents;
    Side side;
    uint8_t padding[39]; // Pad to exactly 64 bytes (1 CPU Cache Line)
};
static_assert(sizeof(ClientOrder) == 64, "ClientOrder must fit exactly in one L1D cache line");

// ============================================================================
// 1. SEGREGATED C++20 CONCEPTS (ISP & DIP Abstractions)
// ============================================================================

template <typename T>
concept RiskCheckerPolicy = requires(T checker, const ClientOrder& order) {
    { checker.checkRisk(order) } noexcept -> std::same_as<bool>;
};

template <typename T>
concept ExchangeEncoderPolicy = requires(T encoder, const ClientOrder& order, uint8_t* buffer) {
    { encoder.encodeWireFormat(order, buffer) } noexcept -> std::same_as<std::size_t>;
    { encoder.getExchangeName() } noexcept -> std::convertible_to<std::string_view>;
};

template <typename T>
concept WireTransportPolicy = requires(T transport, const uint8_t* data, std::size_t len) {
    { transport.sendPacket(data, len) } noexcept -> std::same_as<bool>;
};

// ============================================================================
// 2. CONCRETE SRP POLICIES: Risk, Encoders, Transports
// ============================================================================

// --- Risk Policies (SRP: Invariant Verification) ---
struct StrictPreTradeRisk {
    static constexpr uint64_t MAX_NOTIONAL = 5'000'000'00; // $5M limit

    [[nodiscard]] bool checkRisk(const ClientOrder& order) const noexcept {
        uint64_t notional = (order.priceInCents * order.quantity);
        return notional <= MAX_NOTIONAL && order.quantity > 0;
    }
};

// --- Exchange Encoders (SRP & OCP: Extensible Wire Protocols) ---
struct NasdaqOuchEncoder {
    [[nodiscard]] std::size_t encodeWireFormat(const ClientOrder& order, uint8_t* buffer) const noexcept {
        // Simulated binary binary-packing for NASDAQ OUCH 'O' order packet (fixed 40 bytes)
        buffer[0] = 'O'; // Enter Order
        *reinterpret_cast<uint64_t*>(buffer + 1) = order.orderId;
        buffer[9] = static_cast<uint8_t>(order.side);
        *reinterpret_cast<uint32_t*>(buffer + 10) = order.quantity;
        *reinterpret_cast<uint32_t*>(buffer + 14) = order.symbolId;
        *reinterpret_cast<uint64_t*>(buffer + 18) = order.priceInCents;
        return 26;
    }

    [[nodiscard]] std::string_view getExchangeName() const noexcept {
        return "NASDAQ_OUCH";
    }
};

struct CmeILink3Encoder {
    [[nodiscard]] std::size_t encodeWireFormat(const ClientOrder& order, uint8_t* buffer) const noexcept {
        // Simulated SBE (Simple Binary Encoding) for CME iLink3 (fixed 32 bytes)
        buffer[0] = 0x33; // iLink3 message tag
        *reinterpret_cast<uint64_t*>(buffer + 2) = order.orderId;
        *reinterpret_cast<uint64_t*>(buffer + 10) = order.priceInCents;
        *reinterpret_cast<uint32_t*>(buffer + 18) = order.quantity;
        return 22;
    }

    [[nodiscard]] std::string_view getExchangeName() const noexcept {
        return "CME_ILINK3";
    }
};

// --- Transport Policies (SRP: Direct Network I/O) ---
struct KernelBypassMellanoxTransport {
    bool sendPacket(const uint8_t* /*data*/, std::size_t len) noexcept {
        // In real production, interacts directly with libvma / ef_vi / Solarflare Onload
        std::cout << "    [Solarflare ef_vi] DMA pushed " << len << " bytes directly to NIC PCIe ring.\n";
        return true;
    }
};

// ============================================================================
// 3. HIGH-LEVEL PIPELINE: OrderExecutionGateway (DIP Orchestrator)
// 100% Inlined, 0 Heap Allocations, 0 Virtual Functions!
// ============================================================================
template <
    RiskCheckerPolicy RiskPolicy,
    ExchangeEncoderPolicy EncoderPolicy,
    WireTransportPolicy TransportPolicy
>
class OrderExecutionGateway {
private:
    RiskPolicy riskPolicy_;
    EncoderPolicy encoderPolicy_;
    TransportPolicy transportPolicy_;

    // Reusable stack-allocated transmission frame (Zero dynamic allocation!)
    alignas(64) std::array<uint8_t, 256> txBuffer_{};

public:
    OrderExecutionGateway(RiskPolicy r, EncoderPolicy e, TransportPolicy t) noexcept
        : riskPolicy_(std::move(r)), encoderPolicy_(std::move(e)), transportPolicy_(std::move(t)) {}

    bool submitOrder(const ClientOrder& order) noexcept {
        // Step 1: Pre-trade risk check (SRP)
        if (!riskPolicy_.checkRisk(order)) {
            std::cout << "  [GATEWAY ALERT] Order #" << order.orderId << " rejected by Risk Policy!\n";
            return false;
        }

        // Step 2: Protocol serialization (OCP)
        std::size_t wireBytes = encoderPolicy_.encodeWireFormat(order, txBuffer_.data());

        std::cout << "  [GATEWAY] Routing Order #" << order.orderId << " to " 
                  << encoderPolicy_.getExchangeName() << " (" << wireBytes << " bytes)...\n";

        // Step 3: Low-latency physical transmission (DIP)
        return transportPolicy_.sendPacket(txBuffer_.data(), wireBytes);
    }
};

int main() {
    std::cout << "=== Zero-Cost SOLID Case Study: Ultra-Low-Latency HFT Gateway ===\n\n";

    // Gateway Instance A: Wired for NASDAQ OUCH via Kernel-Bypass NIC
    using NasdaqGateway = OrderExecutionGateway<StrictPreTradeRisk, NasdaqOuchEncoder, KernelBypassMellanoxTransport>;
    NasdaqGateway nasdaqGate{StrictPreTradeRisk{}, NasdaqOuchEncoder{}, KernelBypassMellanoxTransport{}};

    // Gateway Instance B: Wired for CME iLink3 via Kernel-Bypass NIC
    using CmeGateway = OrderExecutionGateway<StrictPreTradeRisk, CmeILink3Encoder, KernelBypassMellanoxTransport>;
    CmeGateway cmeGate{StrictPreTradeRisk{}, CmeILink3Encoder{}, KernelBypassMellanoxTransport{}};

    ClientOrder validOrder{
        .orderId = 90001,
        .symbolId = 101, // AAPL
        .quantity = 100,
        .priceInCents = 22000, // $220.00
        .side = Side::Buy,
        .padding = {}
    };

    ClientOrder riskBreachOrder{
        .orderId = 90002,
        .symbolId = 202, // SPY
        .quantity = 500'000,
        .priceInCents = 55000, // $275M (Exceeds $5M risk limit!)
        .side = Side::Buy,
        .padding = {}
    };

    std::cout << "--- 1. Submitting Valid Order to NASDAQ ---\n";
    nasdaqGate.submitOrder(validOrder);

    std::cout << "\n--- 2. Submitting Valid Order to CME ---\n";
    cmeGate.submitOrder(validOrder);

    std::cout << "\n--- 3. Submitting Risk-Breaching Order ---\n";
    nasdaqGate.submitOrder(riskBreachOrder);

    std::cout << "\nArchitectural Triumph: Full SOLID compliance achieved with ZERO vtables and ZERO heap allocations!\n";
    return 0;
}
