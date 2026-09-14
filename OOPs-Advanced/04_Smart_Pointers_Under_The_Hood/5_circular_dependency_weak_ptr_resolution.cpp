/**
 * @file 5_circular_dependency_weak_ptr_resolution.cpp
 * @brief Demonstrates Breaking Circular References with std::weak_ptr:
 *        - Memory leak created by cyclical std::shared_ptr references.
 *        - Clean resolution and destruction using std::weak_ptr.
 *        - Safe atomicity idiom: wp.lock() vs the dangerous wp.expired() race condition.
 */

#include <iostream>
#include <memory>
#include <string>

// -----------------------------------------------------------------------------
// 1. LEAKING CYCLE: Mutually owning shared_ptrs
// -----------------------------------------------------------------------------
struct LeakyB;

struct LeakyA {
    std::shared_ptr<LeakyB> b_ptr;
    ~LeakyA() { std::cout << "  LeakyA destructed!\n"; }
};

struct LeakyB {
    std::shared_ptr<LeakyA> a_ptr;
    ~LeakyB() { std::cout << "  LeakyB destructed!\n"; }
};

void demonstrateLeak() {
    std::cout << "Starting demonstrateLeak()...\n";
    auto a = std::make_shared<LeakyA>();
    auto b = std::make_shared<LeakyB>();
    a->b_ptr = b;
    b->a_ptr = a;
    std::cout << "Exiting demonstrateLeak()...\n";
    // Neither LeakyA nor LeakyB is destructed! Both strong counts remain 1!
}

// -----------------------------------------------------------------------------
// 2. CLEAN RESOLUTION: std::weak_ptr breaks cycle
// -----------------------------------------------------------------------------
struct SafeChild;

struct SafeParent {
    std::string name{"Parent"};
    std::shared_ptr<SafeChild> child; // Downward ownership: strong

    ~SafeParent() { std::cout << "  SafeParent destructed cleanly!\n"; }
};

struct SafeChild {
    std::string name{"Child"};
    std::weak_ptr<SafeParent> parent; // Upward observation: weak (NO cycle!)

    ~SafeChild() { std::cout << "  SafeChild destructed cleanly!\n"; }

    void talkToParent() {
        // Safe access idiom: lock() creates a temporary strong shared_ptr
        if (auto p = parent.lock()) {
            std::cout << "  Child successfully reached " << p->name << " (use_count: " 
                      << p.use_count() << ")\n";
        } else {
            std::cout << "  Child cannot reach parent: Parent object is dead!\n";
        }
    }
};

int main() {
    std::cout << "=== 1. Demonstrating Circular Reference Leak ===\n";
    demonstrateLeak();
    std::cout << "(Notice: NO destructors were printed! Memory leaked permanently!)\n\n";

    std::cout << "=== 2. Demonstrating std::weak_ptr Cycle Resolution ===\n";
    {
        auto p = std::make_shared<SafeParent>();
        auto c = std::make_shared<SafeChild>();
        p->child = c;
        c->parent = p; // Weak reference does NOT increment strong count!

        std::cout << "p use_count: " << p.use_count() << " (Expected: 1)\n";
        std::cout << "c use_count: " << c.use_count() << " (Expected: 2: external + parent)\n";

        c->talkToParent();
        std::cout << "Exiting scope...\n";
    }
    std::cout << "Both objects cleanly destructed without leaks!\n";

    return 0;
}
