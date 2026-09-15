/**
 * @file 04_ocp_std_variant_visitor.cpp
 * @brief Demonstrates Modern C++20 Closed-Set Polymorphic OCP using std::variant and std::visit.
 * 
 * In Senior C++ architecture, we distinguish between:
 *   1. Open-Set Polymorphism: New types are added dynamically at runtime (Interfaces/Vtables).
 *   2. Closed-Set Polymorphism: The set of types is known at compile time, but operations are open!
 * 
 * The Expression Problem Dual:
 *   - Classical OOP: Easy to add new types (derive new class), hard to add new operations (modifies interface).
 *   - Functional / Variant Visitor: Easy to add new operations (add new visitor without touching types!).
 * 
 * Why std::variant is favored in modern C++:
 *   - VALUE SEMANTICS: Stored inline in contiguous memory (cache-friendly).
 *   - ZERO HEAP ALLOCATIONS: Unlike `std::vector<std::unique_ptr<Base>>`.
 *   - Type-safe pattern matching with zero vtable pointer overhead.
 * 
 * @standard C++20
 */

#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <iomanip>

// Helper template for std::visit with overloaded lambdas
template <class... Ts>
struct Overloaded : Ts... {
    using Ts::operator()...;
};
// Explicit deduction guide (optional in C++20, but best practice for portability)
template <class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

// ============================================================================
// DOMAIN TYPES: Plain Value Structs (Zero inheritance, zero vtable!)
// ============================================================================
struct CreditCard {
    std::string cardNumber;
    std::string expiry;
};

struct PayPal {
    std::string email;
    std::string authToken;
};

struct CryptoWallet {
    std::string publicAddress;
    std::string currency;
};

// Closed sum type
using PaymentMethodVariant = std::variant<CreditCard, PayPal, CryptoWallet>;

struct PaymentTransaction {
    double amount;
    PaymentMethodVariant method;
};

// ============================================================================
// OPERATION 1: Fee Calculation (Extensible without modifying domain structs!)
// ============================================================================
struct FeeCalculatorVisitor {
    double amount;

    double operator()(const CreditCard&) const noexcept {
        return amount * 0.029 + 0.30;
    }

    double operator()(const PayPal&) const noexcept {
        return amount * 0.035;
    }

    double operator()(const CryptoWallet&) const noexcept {
        return 1.50; // Flat blockchain fee
    }
};

// ============================================================================
// OPERATION 2: JSON Serializer (Added without modifying domain structs!)
// Demonstrates OCP: Completely closed types, completely open operations!
// ============================================================================
struct JsonSerializerVisitor {
    std::string operator()(const CreditCard& cc) const {
        return "{\"type\":\"credit_card\",\"masked_pan\":\"****-" 
             + cc.cardNumber.substr(cc.cardNumber.size() > 4 ? cc.cardNumber.size() - 4 : 0) + "\"}";
    }

    std::string operator()(const PayPal& pp) const {
        return "{\"type\":\"paypal\",\"account\":\"" + pp.email + "\"}";
    }

    std::string operator()(const CryptoWallet& cw) const {
        return "{\"type\":\"crypto\",\"chain\":\"" + cw.currency + "\",\"addr\":\"" + cw.publicAddress + "\"}";
    }
};

int main() {
    std::cout << "=== OCP via std::variant & std::visit (The Modern C++ Dual) ===\n\n";

    // Contiguous vector of variants: ZERO heap allocation for polymorphic dispatch!
    std::vector<PaymentTransaction> transactions = {
        {100.0, CreditCard{"4111222233334444", "12/28"}},
        {250.0, PayPal{"alice@enterprise.com", "tok_live_9921"}},
        {1800.0, CryptoWallet{"0x71C...B29", "ETH"}}
    };

    // 1. Invoking Operation 1 (Fee Calculation) via Functor Visitor
    std::cout << "--- 1. Fee Calculation Operation ---\n";
    for (const auto& tx : transactions) {
        double fee = std::visit(FeeCalculatorVisitor{tx.amount}, tx.method);
        std::cout << "Transaction Amount: $" << tx.amount << " -> Calculated Fee: $" << fee << "\n";
    }

    // 2. Invoking Operation 2 (Serialization) via Functor Visitor
    std::cout << "\n--- 2. JSON Serialization Operation ---\n";
    for (const auto& tx : transactions) {
        std::string json = std::visit(JsonSerializerVisitor{}, tx.method);
        std::cout << "Payload: " << json << "\n";
    }

    // 3. Invoking Operation 3 on the fly via Overloaded Inlined Lambdas!
    std::cout << "\n--- 3. Inline Audit Logging Operation ---\n";
    for (const auto& tx : transactions) {
        std::visit(Overloaded{
            [](const CreditCard& cc) {
                std::cout << "[AUDIT] Card ending in: " << cc.cardNumber.substr(12) << "\n";
            },
            [](const PayPal& pp) {
                std::cout << "[AUDIT] PayPal account: " << pp.email << "\n";
            },
            [](const CryptoWallet& cw) {
                std::cout << "[AUDIT] Crypto transfer via " << cw.currency << "\n";
            }
        }, tx.method);
    }

    std::cout << "\nNotice: We added 3 completely separate operations without touching any domain struct!\n";
    return 0;
}
