/**
 * @file 2_arrow_proxy_idiom_thread_safe_locking.cpp
 * @brief Demonstrates the Arrow Operator (->) Proxy Idiom:
 *        - Recursive chaining of operator-> by the compiler.
 *        - Return-by-value proxy extending lifetime across the full expression.
 *        - Automatically injecting thread-safe RAII mutex locking around member function calls.
 */

#include <iostream>
#include <mutex>
#include <string>

// A business object with normal, non-thread-safe methods
class BankAccount {
private:
    std::string m_owner;
    double m_balance{0.0};

public:
    BankAccount(std::string owner, double initial) 
        : m_owner(std::move(owner)), m_balance(initial) {}

    void deposit(double amount) {
        m_balance += amount;
        std::cout << "    [BankAccount] Deposited $" << amount << ". New balance: $" << m_balance << "\n";
    }

    void withdraw(double amount) {
        m_balance -= amount;
        std::cout << "    [BankAccount] Withdrew $" << amount << ". New balance: $" << m_balance << "\n";
    }

    double getBalance() const { return m_balance; }
};

// Generic Thread-Safe Wrapper using the Arrow Proxy Pattern
template <typename T>
class ThreadSafe {
private:
    T m_object;
    mutable std::mutex m_mutex;

    // RAII Proxy returned by value from operator->
    struct Proxy {
        std::unique_lock<std::mutex> lock;
        T* target_ptr;

        Proxy(std::mutex& mtx, T* ptr) 
            : lock(mtx), target_ptr(ptr) {
            std::cout << "  [Proxy] Mutex LOCKED.\n";
        }

        ~Proxy() {
            std::cout << "  [Proxy] Mutex UNLOCKED (temporary destroyed at end of expression).\n";
        }

        // Compiler recursively chains to this operator-> to reach raw pointer!
        T* operator->() {
            return target_ptr;
        }
    };

public:
    template <typename... Args>
    ThreadSafe(Args&&... args) : m_object(std::forward<Args>(args)...) {}

    // Overloaded operator-> returns proxy by value!
    Proxy operator->() {
        return Proxy(m_mutex, &m_object);
    }
};

int main() {
    std::cout << "=== ThreadSafe Arrow Proxy Execution ===\n";
    ThreadSafe<BankAccount> safeAccount("Alice", 500.0);

    std::cout << "\nExecuting: safeAccount->deposit(250.0);\n";
    // 1. safeAccount.operator->() locks mutex, returns temporary Proxy
    // 2. Proxy.operator->() returns BankAccount*
    // 3. deposit(250.0) executes
    // 4. End of expression: Proxy destructor automatically unlocks mutex!
    safeAccount->deposit(250.0);

    std::cout << "\nExecuting: safeAccount->withdraw(100.0);\n";
    safeAccount->withdraw(100.0);

    return 0;
}
