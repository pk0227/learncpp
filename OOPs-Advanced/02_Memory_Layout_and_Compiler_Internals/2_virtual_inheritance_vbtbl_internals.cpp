/**
 * @file 2_virtual_inheritance_vbtbl_internals.cpp
 * @brief Demonstrates Virtual Inheritance and Diamond Problem Internals:
 *        - Memory duplication and member ambiguity in non-virtual diamond hierarchies.
 *        - Shared base sub-object via virtual inheritance and vbptr/vtable offset tables.
 *        - The mandatory rule: Most-derived class initializes the virtual base.
 */

#include <iostream>
#include <string>

// -----------------------------------------------------------------------------
// 1. NON-VIRTUAL DIAMOND (Duplication & Ambiguity)
// -----------------------------------------------------------------------------
struct AnimalNonVirtual {
    int age{5};
};

struct MammalNonVirtual : public AnimalNonVirtual {};
struct WingedNonVirtual : public AnimalNonVirtual {};

struct BatNonVirtual : public MammalNonVirtual, public WingedNonVirtual {
    // Contains TWO AnimalNonVirtual sub-objects!
    // bat.age is ambiguous!
};

// -----------------------------------------------------------------------------
// 2. VIRTUAL INHERITANCE DIAMOND (Single Shared Sub-Object)
// -----------------------------------------------------------------------------
class AnimalVirtual {
public:
    std::string name;
    AnimalVirtual(const std::string& n) : name(n) {
        std::cout << "    AnimalVirtual constructed with name: \"" << name << "\"\n";
    }
};

class MammalVirtual : public virtual AnimalVirtual {
public:
    MammalVirtual() : AnimalVirtual("MammalDefault") {
        std::cout << "    MammalVirtual constructed.\n";
    }
};

class WingedVirtual : public virtual AnimalVirtual {
public:
    WingedVirtual() : AnimalVirtual("WingedDefault") {
        std::cout << "    WingedVirtual constructed.\n";
    }
};

class BatVirtual : public MammalVirtual, public WingedVirtual {
public:
    // The MOST-DERIVED class is directly responsible for initializing AnimalVirtual!
    // The AnimalVirtual initializers in MammalVirtual and WingedVirtual are IGNORED!
    BatVirtual() : AnimalVirtual("BatDirectInitialization") {
        std::cout << "    BatVirtual constructor body reached.\n";
    }
};

int main() {
    std::cout << "=== Non-Virtual Diamond Layout ===\n";
    std::cout << "sizeof(AnimalNonVirtual): " << sizeof(AnimalNonVirtual) << " bytes\n";
    std::cout << "sizeof(BatNonVirtual):    " << sizeof(BatNonVirtual) 
              << " bytes (Contains 2 separate Animal sub-objects!)\n\n";

    BatNonVirtual bat_nv;
    // std::cout << bat_nv.age; // COMPILE ERROR: Request for member 'age' is ambiguous!
    std::cout << "Accessing through explicit path: MammalNonVirtual::age = " 
              << bat_nv.MammalNonVirtual::age << "\n\n";

    std::cout << "=== Virtual Diamond Construction ===\n";
    std::cout << "Constructing BatVirtual:\n";
    BatVirtual bat_v;
    std::cout << "Resulting shared Animal name: \"" << bat_v.name << "\"\n";
    std::cout << "Notice that \"MammalDefault\" and \"WingedDefault\" were completely bypassed!\n";

    return 0;
}
