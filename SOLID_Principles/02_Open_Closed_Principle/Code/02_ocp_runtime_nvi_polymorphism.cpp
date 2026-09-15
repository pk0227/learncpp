/**
 * @file 02_ocp_runtime_nvi_polymorphism.cpp
 * @brief Demonstrates Open/Closed Principle via Herb Sutter's Non-Virtual Interface (NVI) idiom.
 * 
 * Why NVI is the Senior C++ Standard for Runtime Polymorphism:
 *   - Naive public virtual functions conflate the *public interface contract* with the 
 *     *derived implementation customization*.
 *   - In NVI, the base class provides a `public non-virtual` interface method that:
 *       1. Enforces preconditions (e.g., amount > 0, non-empty receiver).
 *       2. Manages system invariants (e.g., acquiring locks, logging, performance profiling).
 *       3. Dispatches to a `private virtual` customization hook (`doExecuteTransaction`).
 *       4. Enforces postconditions (e.g., verifying transaction IDs, auditing).
 * 
 * Adding `CryptoPaymentMethod` or any future payment gateway requires:
 *   - Adding a new derived class in its own `.cpp` file.
 *   - ZERO edits or recompilations of `PaymentMethod` base class or `PaymentProcessor`.
 * 
 * @standard C++20
 */

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <stdexcept>
#include <chrono>

struct TransactionDetails {
    double amount;
    std::string sender;
    std::string receiver;
};

// ============================================================================
// BASE CLASS WITH NON-VIRTUAL INTERFACE (NVI)
// Closed for modification, open for derived extension!
// ============================================================================
class PaymentMethod {
public:
    virtual ~PaymentMethod() = default;

    // Stable Public Non-Virtual Interface (The Contract & Invariant Enforcer)
    [[nodiscard]] double calculateFee(const TransactionDetails& details) const {
        if (details.amount <= 0.0) {
            throw std::invalid_argument("Payment amount must be positive");
        }
        return doCalculateFee(details); // Dispatch to private customization hook
    }

    void process(const TransactionDetails& details) {
        // Precondition checks
        if (details.amount <= 0.0) {
            throw std::invalid_argument("Cannot process non-positive transaction");
        }
        if (details.sender.empty() || details.receiver.empty()) {
            throw std::invalid_argument("Sender and receiver must not be empty");
        }

        std::cout << "[Audit-Pre] Verifying fraud checks and balance for " << details.sender << "...\n";
        auto start = std::chrono::steady_clock::now();

        // Customization hook invocation
        doProcess(details);

        auto end = std::chrono::steady_clock::now();
        auto elapsedUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "[Audit-Post] Transaction completed in " << elapsedUs << " us.\n";
    }

    [[nodiscard]] std::string getMethodName() const {
        return doGetName();
    }

private:
    // Private virtual extension hooks: Derived classes customize behavior, NOT the contract!
    [[nodiscard]] virtual double doCalculateFee(const TransactionDetails& details) const = 0;
    virtual void doProcess(const TransactionDetails& details) = 0;
    [[nodiscard]] virtual std::string doGetName() const = 0;
};

// ============================================================================
// CONCRETE EXTENSION 1: Credit Card
// ============================================================================
class CreditCardPayment : public PaymentMethod {
private:
    [[nodiscard]] double doCalculateFee(const TransactionDetails& details) const override {
        return details.amount * 0.029 + 0.30;
    }

    void doProcess(const TransactionDetails& details) override {
        std::cout << "  -> [CreditCard Gateway] Tokenizing PAN and charging $" 
                  << details.amount << " to " << details.receiver << "\n";
    }

    [[nodiscard]] std::string doGetName() const override {
        return "Credit Card (Visa/Mastercard)";
    }
};

// ============================================================================
// CONCRETE EXTENSION 2: PayPal
// ============================================================================
class PayPalPayment : public PaymentMethod {
private:
    [[nodiscard]] double doCalculateFee(const TransactionDetails& details) const override {
        return details.amount * 0.035;
    }

    void doProcess(const TransactionDetails& details) override {
        std::cout << "  -> [PayPal REST API] Capturing authorized funds for $" 
                  << details.amount << "\n";
    }

    [[nodiscard]] std::string doGetName() const override {
        return "PayPal Express Checkout";
    }
};

// ============================================================================
// NEW EXTENSION: Cryptocurrency (Added with ZERO edits to existing code!)
// ============================================================================
class CryptoPayment : public PaymentMethod {
private:
    [[nodiscard]] double doCalculateFee(const TransactionDetails&) const override {
        return 2.50; // Flat blockchain network gas subsidy
    }

    void doProcess(const TransactionDetails& details) override {
        std::cout << "  -> [Web3 RPC] Broadcasting transaction to Ethereum mempool for $" 
                  << details.amount << " from " << details.sender << "\n";
    }

    [[nodiscard]] std::string doGetName() const override {
        return "Ethereum (USDC Smart Contract)";
    }
};

// ============================================================================
// CLIENT / RUNTIME EXECUTOR
// Completely decoupled and closed for modification
// ============================================================================
class CheckoutService {
public:
    void checkout(PaymentMethod& method, const TransactionDetails& details) {
        std::cout << "=== Initiating Checkout with " << method.getMethodName() << " ===\n";
        double fee = method.calculateFee(details);
        std::cout << "Calculated Fee: $" << fee << " | Total Charged: $" << (details.amount + fee) << "\n";
        method.process(details);
        std::cout << "Checkout Successful.\n\n";
    }
};

int main() {
    std::cout << "=== OCP via NVI (Non-Virtual Interface) Idiom ===\n\n";

    CheckoutService checkout;

    CreditCardPayment cc;
    PayPalPayment pp;
    CryptoPayment crypto; // Extensible without touching CheckoutService or PaymentMethod!

    TransactionDetails t1{100.0, "Alice", "StripeMerchant"};
    TransactionDetails t2{250.0, "Bob", "eBaySeller"};
    TransactionDetails t3{1500.0, "Charlie", "DeFiVault"};

    checkout.checkout(cc, t1);
    checkout.checkout(pp, t2);
    checkout.checkout(crypto, t3);

    return 0;
}
