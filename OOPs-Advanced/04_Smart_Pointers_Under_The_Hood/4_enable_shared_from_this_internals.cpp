/**
 * @file 4_enable_shared_from_this_internals.cpp
 * @brief Demonstrates std::enable_shared_from_this Internals and Hazards:
 *        - Safely generating shared_ptr from 'this' without double control blocks.
 *        - The constructor pitfall (throws std::bad_weak_ptr).
 *        - The stack-allocation pitfall (throws std::bad_weak_ptr).
 *        - C++17 weak_from_this().
 */

#include <iostream>
#include <memory>
#include <stdexcept>

class Worker : public std::enable_shared_from_this<Worker> {
public:
    int id;

    Worker(int i, bool callInCtor = false) : id(i) {
        std::cout << "  Worker(" << id << ") constructed.\n";
        if (callInCtor) {
            std::cout << "  Attempting shared_from_this() inside constructor...\n";
            // FATAL: Object is not yet owned by any shared_ptr!
            auto sp = shared_from_this(); 
        }
    }

    ~Worker() {
        std::cout << "  Worker(" << id << ") destructed!\n";
    }

    std::shared_ptr<Worker> getSelf() {
        // Safe: reuses the existing control block!
        return shared_from_this();
    }

    // C++17 weak_from_this()
    std::weak_ptr<Worker> getWeakSelf() {
        return weak_from_this();
    }
};

int main() {
    std::cout << "=== 1. Valid Usage of shared_from_this() ===\n";
    {
        std::shared_ptr<Worker> w1 = std::make_shared<Worker>(1);
        std::cout << "Initial use_count: " << w1.use_count() << "\n";

        std::shared_ptr<Worker> w2 = w1->getSelf();
        std::cout << "After w1->getSelf(), use_count: " << w1.use_count() << " (Shares control block!)\n";
        std::cout << "w1 address: " << w1.get() << ", w2 address: " << w2.get() << "\n";
    }

    std::cout << "\n=== 2. Pitfall: Calling shared_from_this() in Constructor ===\n";
    try {
        // Construction throws std::bad_weak_ptr before ownership can be established:
        auto w_bad = std::make_shared<Worker>(2, true);
    } catch (const std::bad_weak_ptr& e) {
        std::cout << "Caught expected std::bad_weak_ptr: " << e.what() << "\n";
        std::cout << "Rule: Never invoke shared_from_this() during construction!\n";
    }

    std::cout << "\n=== 3. Pitfall: Calling shared_from_this() on Stack Object ===\n";
    try {
        Worker stackWorker(3);
        // stackWorker was never managed by a std::shared_ptr:
        auto sp = stackWorker.getSelf();
    } catch (const std::bad_weak_ptr& e) {
        std::cout << "Caught expected std::bad_weak_ptr: " << e.what() << "\n";
        std::cout << "Rule: Object must be managed by a std::shared_ptr before calling shared_from_this()!\n";
    }

    return 0;
}
