/**
 * @file 1_multiple_inheritance_pointer_adjustment.cpp
 * @brief Demonstrates Pointer Adjustment in Multiple Inheritance:
 *        - Memory layout of multiple base sub-objects.
 *        - Numerical shift of the 'this' pointer during static_cast to secondary base.
 *        - Catastrophic data corruption caused by reinterpret_cast.
 */

#include <iostream>
#include <cstdint>

class Base1 {
public:
    int m_b1{100};
    void printBase1() {
        std::cout << "  Base1::printBase1() called. this = " << this 
                  << ", m_b1 = " << m_b1 << "\n";
    }
};

class Base2 {
public:
    int m_b2{200};
    void printBase2() {
        std::cout << "  Base2::printBase2() called. this = " << this 
                  << ", m_b2 = " << m_b2 << "\n";
    }
};

class Derived : public Base1, public Base2 {
public:
    int m_derived{300};
    void printDerived() {
        std::cout << "  Derived::printDerived() called. this = " << this 
                  << ", m_derived = " << m_derived << "\n";
    }
};

int main() {
    Derived d;

    std::cout << "=== Memory Layout of Derived Object ===\n";
    std::cout << "sizeof(Base1):   " << sizeof(Base1) << " bytes\n";
    std::cout << "sizeof(Base2):   " << sizeof(Base2) << " bytes\n";
    std::cout << "sizeof(Derived): " << sizeof(Derived) << " bytes\n\n";

    Derived* d_ptr = &d;
    Base1* b1_ptr = static_cast<Base1*>(d_ptr);
    Base2* b2_ptr = static_cast<Base2*>(d_ptr);

    std::cout << "=== Pointer Addresses ===\n";
    std::cout << "Derived* address: " << d_ptr << " (Offset +0)\n";
    std::cout << "Base1* address:   " << b1_ptr << " (Offset +0, matches Derived*)\n";
    std::cout << "Base2* address:   " << b2_ptr << " (Offset +" 
              << (reinterpret_cast<uintptr_t>(b2_ptr) - reinterpret_cast<uintptr_t>(d_ptr)) 
              << " bytes! Pointer shifted!)\n\n";

    std::cout << "=== Valid Invocations with static_cast ===\n";
    b1_ptr->printBase1();
    b2_ptr->printBase2(); // Correctly reads m_b2 = 200

    std::cout << "\n=== The DANGER of reinterpret_cast ===\n";
    // reinterpret_cast preserves the raw bit address without applying the offset!
    Base2* b2_bad = reinterpret_cast<Base2*>(d_ptr);
    std::cout << "b2_bad address (reinterpret_cast): " << b2_bad << " (Unshifted! Points to Base1!)\n";
    std::cout << "Attempting to read b2_bad->m_b2: " << b2_bad->m_b2 
              << " (BUG: Read Base1's m_b1 value of 100 instead of 200!)\n";

    return 0;
}
