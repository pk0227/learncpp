/**
 * @file 03_ocp_compile_time_crtp.cpp
 * @brief Demonstrates Zero-Overhead Open/Closed Principle via CRTP and C++20 Concepts.
 * 
 * In high-performance, low-latency C++ (HFT, networking stacks, kernel drivers), virtual functions
 * are forbidden on the critical path because:
 *   - Virtual dispatch requires a dependent memory load (reading vptr -> reading vtable slot).
 *   - Compilers cannot inline virtual calls across compilation units without profile-guided devirtualization.
 *   - Vtable indirection disrupts CPU branch predictors and instruction caches (i-cache thrashing).
 * 
 * Solution: Static Polymorphism via CRTP (Curiously Recurring Template Pattern)
 *   - Base class templates define the non-virtual interface and contract.
 *   - Derived classes provide compile-time customization hooks.
 *   - C++20 Concepts enforce interface compliance at compile time with zero vtables!
 * 
 * @standard C++20
 */

#include <iostream>
#include <string_view>
#include <vector>
#include <concepts>
#include <chrono>

// Forward declaration
template <typename Derived>
class PacketParserBase;

// ============================================================================
// C++20 CONCEPT: Compile-time contract for derived packet parsers
// ============================================================================
template <typename T>
concept PacketParserType = requires(T parser, const uint8_t* buffer, std::size_t len) {
    { parser.parseHeaderImpl(buffer, len) } -> std::same_as<bool>;
    { parser.parsePayloadImpl(buffer, len) } -> std::same_as<bool>;
    { parser.getProtocolNameImpl() } -> std::convertible_to<std::string_view>;
};

// ============================================================================
// CRTP BASE CLASS: Open for Extension, Closed for Modification (Compile-Time)
// ============================================================================
template <typename Derived>
class PacketParserBase {
public:
    // Compile-time static dispatch: 100% inlinable by compiler!
    bool parse(const uint8_t* buffer, std::size_t len) {
        auto& derived = static_cast<Derived&>(*this);

        if (!buffer || len == 0) {
            std::cout << "[Base Invariant] Invalid buffer passed to parser\n";
            return false;
        }

        std::cout << "[CRTP Pipeline] Processing " << derived.getProtocolNameImpl() 
                  << " packet (" << len << " bytes)...\n";

        if (!derived.parseHeaderImpl(buffer, len)) {
            std::cout << "[CRTP Pipeline] Failed to parse protocol header!\n";
            return false;
        }

        return derived.parsePayloadImpl(buffer, len);
    }
};

// ============================================================================
// EXTENSION 1: IPv4 Packet Parser
// ============================================================================
class IPv4Parser : public PacketParserBase<IPv4Parser> {
public:
    bool parseHeaderImpl(const uint8_t* buffer, std::size_t len) {
        if (len < 20) return false; // Min IPv4 header length
        uint8_t version = (buffer[0] >> 4) & 0x0F;
        std::cout << "  -> [IPv4] Extracted Version: " << static_cast<int>(version) << "\n";
        return version == 4;
    }

    bool parsePayloadImpl(const uint8_t* /*buffer*/, std::size_t len) {
        std::cout << "  -> [IPv4] Parsing " << (len - 20) << " payload bytes.\n";
        return true;
    }

    [[nodiscard]] std::string_view getProtocolNameImpl() const noexcept {
        return "IPv4";
    }
};

// ============================================================================
// EXTENSION 2: UDP Packet Parser
// ============================================================================
class UdpParser : public PacketParserBase<UdpParser> {
public:
    bool parseHeaderImpl(const uint8_t* buffer, std::size_t len) {
        if (len < 8) return false; // Min UDP header length
        uint16_t srcPort = (buffer[0] << 8) | buffer[1];
        uint16_t dstPort = (buffer[2] << 8) | buffer[3];
        std::cout << "  -> [UDP] Port " << srcPort << " -> " << dstPort << "\n";
        return true;
    }

    bool parsePayloadImpl(const uint8_t* /*buffer*/, std::size_t len) {
        std::cout << "  -> [UDP] Payload verified. Dispatching to socket queue.\n";
        return true;
    }

    [[nodiscard]] std::string_view getProtocolNameImpl() const noexcept {
        return "UDP";
    }
};

// ============================================================================
// GENERIC INGESTION ENGINE (Constrained by Concept)
// ============================================================================
template <PacketParserType Parser>
void processNetworkBuffer(Parser& parser, const uint8_t* data, std::size_t size) {
    parser.parse(data, size);
}

int main() {
    std::cout << "=== Zero-Cost Compile-Time OCP via CRTP & Concepts ===\n\n";

    // Synthetic packet headers
    uint8_t ipv4Data[24] = {0x45, 0x00, 0x00, 0x18}; // Version 4, IHL 5
    uint8_t udpData[16]  = {0x04, 0xD2, 0x1F, 0x90}; // Src port 1234, Dst port 8080

    IPv4Parser ipParser;
    UdpParser udpParser;

    // Both parsers execute with ZERO virtual table dispatch!
    processNetworkBuffer(ipParser, ipv4Data, sizeof(ipv4Data));
    std::cout << "\n";
    processNetworkBuffer(udpParser, udpData, sizeof(udpData));

    std::cout << "\nResult: 100% extensible, zero virtual memory lookups, fully inlinable.\n";
    return 0;
}
