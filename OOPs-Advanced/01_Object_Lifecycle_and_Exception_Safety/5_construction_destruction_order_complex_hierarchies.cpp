/**
 * @file 5_construction_destruction_order_complex_hierarchies.cpp
 * @brief Demonstrates Construction and Destruction Order in Complex Hierarchies:
 *        - Virtual base classes (initialized first by most-derived class).
 *        - Non-virtual base classes (left-to-right order of inheritance list).
 *        - Data members (order of declaration in class definition).
 *        - Virtual function dispatch suppression inside constructors/destructors.
 */

#include <iostream>

class VirtualBase {
public:
    VirtualBase() { std::cout << "  1. VirtualBase constructed\n"; }
    virtual ~VirtualBase() { std::cout << "  8. VirtualBase destructed\n"; }
};

class Base1 {
public:
    Base1() { 
        std::cout << "  2. Base1 constructed\n"; 
        call_virtual();
    }
    virtual ~Base1() { std::cout << "  7. Base1 destructed\n"; }

    virtual void call_virtual() {
        std::cout << "     -> Base1::call_virtual() invoked (Dynamic dispatch resolves to Base1, NOT Derived!)\n";
    }
};

class Base2 {
public:
    Base2() { std::cout << "  3. Base2 constructed\n"; }
    virtual ~Base2() { std::cout << "  6. Base2 destructed\n"; }
};

class MemberA {
public:
    MemberA() { std::cout << "  4. MemberA constructed\n"; }
    ~MemberA() { std::cout << "  5. MemberA destructed\n"; }
};

class MemberB {
public:
    MemberB() { std::cout << "  4.5 MemberB constructed\n"; }
    ~MemberB() { std::cout << "  4.5 MemberB destructed\n"; }
};

// Derived inherits Base1 (non-virtual), VirtualBase (virtual), Base2 (non-virtual)
class Derived : public Base1, public virtual VirtualBase, public Base2 {
private:
    MemberA m_a;
    MemberB m_b;

public:
    Derived() : Base2(), Base1(), VirtualBase() // Deliberately shuffled order in list!
    {
        std::cout << "  5. Derived constructor body executed\n";
    }

    ~Derived() override {
        std::cout << "  4. Derived destructor body executed\n";
    }

    void call_virtual() override {
        std::cout << "     -> Derived::call_virtual() override invoked!\n";
    }
};

int main() {
    std::cout << "=== Construction Phase ===\n";
    std::cout << "Notice that VirtualBase is constructed FIRST,\n"
              << "followed by Base1 then Base2 (order of class-head inheritance list),\n"
              << "followed by MemberA then MemberB (order of member declaration),\n"
              << "regardless of the order written in the constructor initializer list!\n\n";

    {
        Derived d;
        std::cout << "\n=== Object is fully alive ===\n";
        d.call_virtual(); // Outside constructor, dynamic dispatch invokes Derived's override!
        std::cout << "\n=== Destruction Phase (Exact Reverse Order) ===\n";
    }

    return 0;
}
