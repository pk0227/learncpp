/**
 * @file 1_crtp_static_polymorphism_vs_vtable.cpp
 * @brief Demonstrates the Curiously Recurring Template Pattern (CRTP):
 *        - Compile-time static polymorphism vs dynamic virtual dispatch.
 *        - Memory footprint comparison (0 vptr overhead).
 *        - Reusable mixin functionality (per-class instance counter).
 */

#include <iostream>

// -----------------------------------------------------------------------------
// 1. Dynamic Polymorphism (Virtual functions)
// -----------------------------------------------------------------------------
class DynamicBase {
public:
    virtual ~DynamicBase() = default;
    virtual void process() const = 0;
};

class DynamicDerived : public DynamicBase {
public:
    int data{42};
    void process() const override {
        std::cout << "  DynamicDerived::process() [Virtual call, vptr indirection]\n";
    }
};

// -----------------------------------------------------------------------------
// 2. Static Polymorphism via CRTP
// -----------------------------------------------------------------------------
template <typename Derived>
class CRTPBase {
public:
    void process() const {
        // Compile-time static dispatch! Fully inlinable!
        static_cast<const Derived*>(this)->processImpl();
    }
};

class CRTPDerived : public CRTPBase<CRTPDerived> {
public:
    int data{42};
    void processImpl() const {
        std::cout << "  CRTPDerived::processImpl() [Inlinable compile-time dispatch]\n";
    }
};

// -----------------------------------------------------------------------------
// 3. CRTP Mixin: Per-Class Instance Counter
// -----------------------------------------------------------------------------
template <typename T>
class InstanceCounter {
private:
    static inline int s_count{0};

public:
    InstanceCounter() { ++s_count; }
    ~InstanceCounter() { --s_count; }
    static int getLiveCount() { return s_count; }
};

class User : public InstanceCounter<User> {};
class Product : public InstanceCounter<Product> {};

int main() {
    std::cout << "=== Memory Footprint Comparison ===\n";
    std::cout << "sizeof(DynamicDerived): " << sizeof(DynamicDerived) 
              << " bytes (4 bytes data + 4 bytes padding + 8 bytes vptr = 16 bytes)\n";
    std::cout << "sizeof(CRTPDerived):    " << sizeof(CRTPDerived) 
              << " bytes (4 bytes data, ZERO vptr overhead!)\n\n";

    std::cout << "=== Dispatch Execution ===\n";
    DynamicDerived d_dyn;
    d_dyn.process();

    CRTPDerived d_crtp;
    d_crtp.process();

    std::cout << "\n=== CRTP Mixin Instance Counters ===\n";
    User u1, u2, u3;
    Product p1;
    std::cout << "Live User instances:    " << User::getLiveCount() << " (Expected: 3)\n";
    std::cout << "Live Product instances: " << Product::getLiveCount() << " (Expected: 1)\n";

    return 0;
}
