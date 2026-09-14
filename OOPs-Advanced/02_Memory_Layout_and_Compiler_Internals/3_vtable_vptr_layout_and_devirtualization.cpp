/**
 * @file 3_vtable_vptr_layout_and_devirtualization.cpp
 * @brief Demonstrates Vtable/Vptr Mechanics, Thunks, and Devirtualization:
 *        - Physical inspection of the vptr at offset 0.
 *        - Thunk mechanics: adjusting the 'this' pointer for secondary base vtables.
 *        - Devirtualization using the 'final' specifier.
 */

#include <iostream>
#include <cstdint>

class Base1 {
public:
    virtual ~Base1() = default;
    virtual void func1() { std::cout << "  Base1::func1()\n"; }
};

class Base2 {
public:
    virtual ~Base2() = default;
    virtual void func2() { 
        std::cout << "  Base2::func2() called. this = " << this << "\n"; 
    }
};

class Derived final : public Base1, public Base2 {
public:
    void func1() override { std::cout << "  Derived::func1()\n"; }

    // Derived overrides Base2's virtual function
    void func2() override {
        std::cout << "  Derived::func2() [Thunk adjusted this to: " << this << "]\n";
    }
};

int main() {
    Derived d;

    std::cout << "=== Vptr Inspection in Multiple Inheritance ===\n";
    std::cout << "sizeof(Derived): " << sizeof(Derived) << " bytes\n";

    // In 64-bit Itanium ABI:
    // Offset 0: Primary vptr (Base1 + Derived)
    // Offset 8: Secondary vptr (Base2 sub-object)
    uintptr_t* raw_mem = reinterpret_cast<uintptr_t*>(&d);
    std::cout << "Primary vptr   (at offset 0): 0x" << std::hex << raw_mem[0] << std::dec << "\n";
    std::cout << "Secondary vptr (at offset 8): 0x" << std::hex << raw_mem[1] << std::dec << "\n\n";

    std::cout << "=== Thunk Execution via Secondary Base Pointer ===\n";
    Base2* b2 = &d;
    std::cout << "Derived* actual address: " << &d << "\n";
    std::cout << "Base2* shifted address:  " << b2 << " (+8 bytes)\n";
    std::cout << "Calling b2->func2():\n";
    b2->func2(); // Calls assembly thunk: adjusts this pointer from b2 back to &d!

    std::cout << "\n=== Devirtualization with 'final' ===\n";
    std::cout << "Because Derived is marked 'final', any direct call on Derived instance:\n";
    d.func1(); // Direct call! Compiler eliminates vtable lookup entirely.

    return 0;
}
