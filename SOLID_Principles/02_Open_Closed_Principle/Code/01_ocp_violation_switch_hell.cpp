/**
 * @file 01_ocp_violation_switch_hell.cpp
 * @brief Demonstrates an Open/Closed Principle (OCP) violation: Type-switching antipattern ("Switch Hell").
 * 
 * In this codebase, operations on payments (fee calculation, transaction dispatch, receipt generation)
 * are driven by an `enum class PaymentType` evaluated in multiple switch/case blocks.
 * 
 * Why this violates OCP:
 *   - NOT Closed for Modification: Adding a new payment type (e.g., `CryptoPayment` or `ApplePay`)
 *     requires editing, recompiling, and re-testing every single switch statement in existing production code.
 *   - Fragility: If a developer adds an enum value but forgets to update 1 of the 4 switch statements,
 *     the system compiles cleanly but throws runtime errors or executes fallback paths silently.
 *   - Merge Conflicts: Multiple developers adding payment types simultaneously will encounter merge
 *     conflicts on the exact same central function files.
 * 
 * @standard C++20
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include <iomanip>

enum class PaymentType {
    CreditCard,
    PayPal,
    BankWire
    // Adding Crypto requires modifying every switch statement below!
};

struct PaymentRequest {
    PaymentType type;
    double amount;
    std::string sender;
    std::string receiver;
};

// ============================================================================
// ANTI-PATTERN: Centralized Switch / If-Else Dispatcher
// ============================================================================
class PaymentProcessor {
public:
    // Switch Site 1: Fee Calculation
    [[nodiscard]] double calculateTransactionFee(const PaymentRequest& req) const {
        switch (req.type) {
            case PaymentType::CreditCard:
                return req.amount * 0.029 + 0.30; // 2.9% + $0.30
            case PaymentType::PayPal:
                return req.amount * 0.035;        // 3.5% flat
            case PaymentType::BankWire:
                return 15.00;                     // $15 flat wire fee
            default:
                throw std::invalid_argument("Unknown payment type in fee calculation");
        }
    }

    // Switch Site 2: Dispatch Logic
    void executeTransaction(const PaymentRequest& req) const {
        switch (req.type) {
            case PaymentType::CreditCard:
                std::cout << "[CreditCard] Authorizing $" << req.amount 
                          << " via Visa/Mastercard network for " << req.sender << "\n";
                break;
            case PaymentType::PayPal:
                std::cout << "[PayPal] Invoking PayPal OAuth REST API for $" << req.amount 
                          << " from user: " << req.sender << "\n";
                break;
            case PaymentType::BankWire:
                std::cout << "[BankWire] Generating SWIFT MT103 wire transfer for $" << req.amount 
                          << " to: " << req.receiver << "\n";
                break;
            default:
                throw std::invalid_argument("Unknown payment type in transaction dispatch");
        }
    }

    // Switch Site 3: Receipt Formatting
    void printReceipt(const PaymentRequest& req) const {
        double fee = calculateTransactionFee(req);
        double net = req.amount - fee;

        std::cout << "Receipt: Type=";
        switch (req.type) {
            case PaymentType::CreditCard: std::cout << "Credit Card"; break;
            case PaymentType::PayPal:     std::cout << "PayPal";      break;
            case PaymentType::BankWire:   std::cout << "Bank Wire";   break;
            default:                      std::cout << "Unknown";     break;
        }
        std::cout << " | Gross: $" << req.amount << " | Fee: $" << fee << " | Net: $" << net << "\n";
    }
};

int main() {
    std::cout << "=== OCP Violation: Switch Hell Demo ===\n\n";

    PaymentProcessor processor;

    PaymentRequest p1{PaymentType::CreditCard, 100.0, "Alice", "Merchant Corp"};
    PaymentRequest p2{PaymentType::PayPal, 250.0, "Bob", "Service LLC"};
    PaymentRequest p3{PaymentType::BankWire, 5000.0, "Charlie", "Supplier Inc"};

    processor.executeTransaction(p1);
    processor.printReceipt(p1);
    std::cout << "\n";

    processor.executeTransaction(p2);
    processor.printReceipt(p2);
    std::cout << "\n";

    processor.executeTransaction(p3);
    processor.printReceipt(p3);

    std::cout << "\nNotice: Adding 'Crypto' requires modifying calculateTransactionFee(), "
              << "executeTransaction(), and printReceipt() in place!\n";

    return 0;
}
