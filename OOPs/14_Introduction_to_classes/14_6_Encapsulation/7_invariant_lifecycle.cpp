// Demonstrates: Class invariant lifecycle -- Establish, Maintain, Never Externally Observable Broken
// Also demonstrates: Precondition vs. Postcondition vs. Class Invariant distinction
//
// Invariant lifecycle:
//   - Constructor ESTABLISHES the invariant (throws if impossible)
//   - Member functions MAINTAIN the invariant (valid on entry, valid on exit)
//   - The invariant must hold between ANY two consecutive public function calls
//
// Compile: g++ -std=c++20 -fsyntax-only 7_invariant_lifecycle.cpp

#include <iostream>
#include <stdexcept>
#include <cassert>

// --- Example 1: BankAccount ---
// Invariant: balance >= 0
// This invariant must hold after construction AND after every public operation.
class BankAccount {
    double balance_;

public:
    // Constructor ESTABLISHES the invariant.
    // Precondition: initialBalance >= 0
    // Postcondition: balance_ == initialBalance
    explicit BankAccount(double initialBalance) {
        if (initialBalance < 0.0)
            throw std::invalid_argument("Initial balance cannot be negative");
        balance_ = initialBalance;
        // Invariant is now established: balance_ >= 0
    }

    // deposit() MAINTAINS the invariant.
    // Precondition:  amount > 0        (caller's responsibility)
    // Postcondition: balance_ == old_balance + amount
    // Invariant guarantee: balance_ >= 0 is preserved (adding positive amount keeps it non-negative)
    void deposit(double amount) {
        if (amount <= 0.0)
            throw std::invalid_argument("Deposit amount must be positive");
        balance_ += amount;
        // Invariant still holds: balance_ >= 0
    }

    // withdraw() MAINTAINS the invariant.
    // Precondition:  amount > 0 && amount <= balance_
    // Postcondition: balance_ == old_balance - amount
    // Invariant guarantee: we check before subtracting to ensure balance_ stays >= 0
    void withdraw(double amount) {
        if (amount <= 0.0)
            throw std::invalid_argument("Withdrawal amount must be positive");
        if (amount > balance_)
            throw std::invalid_argument("Insufficient funds");
        balance_ -= amount;
        // Invariant still holds: balance_ >= 0
    }

    double balance() const { return balance_; }
};

// --- Example 2: Fraction ---
// Invariant: denominator != 0
class Fraction {
    int num_;
    int den_;

public:
    // Constructor ESTABLISHES the invariant
    Fraction(int n, int d) {
        if (d == 0)
            throw std::invalid_argument("Denominator cannot be zero");
        num_ = n;
        den_ = d;
    }

    // Setter MAINTAINS the invariant
    void setDenominator(int d) {
        if (d == 0)
            throw std::invalid_argument("Denominator cannot be zero");
        den_ = d;
        // Invariant maintained: den_ != 0
    }

    double value() const { return static_cast<double>(num_) / den_; }
    int numerator()   const { return num_; }
    int denominator() const { return den_; }
};

int main() {
    // BankAccount: invariant established by constructor
    BankAccount acc{100.0};
    std::cout << "Balance: " << acc.balance() << '\n';  // 100

    acc.deposit(50.0);
    std::cout << "After deposit 50: " << acc.balance() << '\n';  // 150

    acc.withdraw(30.0);
    std::cout << "After withdraw 30: " << acc.balance() << '\n'; // 120

    // Precondition violation: amount <= 0 -- throws
    try {
        acc.withdraw(-10.0);
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught: " << e.what() << '\n';
    }

    // Invariant violation attempt: would break balance >= 0 -- throws
    try {
        acc.withdraw(999.0);
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught: " << e.what() << '\n';
    }
    // Invariant still holds after failed operation: balance unchanged
    std::cout << "Balance still: " << acc.balance() << '\n';  // 120

    // Constructor rejects invariant violation
    try {
        BankAccount bad{-50.0};
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught: " << e.what() << '\n';
    }

    // Fraction: invariant (den != 0) enforced by constructor and setter
    Fraction f{3, 4};
    std::cout << "Fraction value: " << f.value() << '\n';  // 0.75

    try {
        Fraction bad{1, 0};  // invariant violation at construction -> throws
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught: " << e.what() << '\n';
    }

    return 0;
}
