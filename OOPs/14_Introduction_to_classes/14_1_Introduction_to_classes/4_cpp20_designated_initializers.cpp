// Demonstrates: C++20 Designated Initializers with aggregates
// Designated initializers allow initializing members BY NAME using .member = value syntax.
// They ONLY work on aggregates (types without user-declared constructors).
// Order must match the declaration order (C++20, unlike C99 which allowed out-of-order).
//
// Compile: g++ -std=c++20 -fsyntax-only 4_cpp20_designated_initializers.cpp

#include <iostream>
#include <string>

struct Point {
    int x;
    int y;
    int z = 0;   // default member initializer (C++14+)
};

struct Employee {
    std::string name;
    int id       = 0;
    double salary = 50000.0;
    bool active  = true;
};

// NON-aggregate -- cannot use designated initializers
struct NonAgg {
    int x;
    NonAgg(int x) : x(x) {}   // user-declared constructor -> not aggregate
};

int main() {
    // 1. Basic designated initializer -- all members by name
    Point p1{.x = 10, .y = 20, .z = 5};
    std::cout << "p1: x=" << p1.x << " y=" << p1.y << " z=" << p1.z << '\n';

    // 2. Partial designated initializer -- unmentioned members use default or value-init
    Point p2{.x = 3, .y = 7};   // z uses its default member initializer (0)
    std::cout << "p2: x=" << p2.x << " y=" << p2.y << " z=" << p2.z << '\n';

    // 3. Skip middle members -- they are value-initialized or use their defaults
    Point p3{.z = 99};           // x=0 (value-init), y=0 (value-init), z=99
    std::cout << "p3: x=" << p3.x << " y=" << p3.y << " z=" << p3.z << '\n';

    // 4. Struct with mixed defaults -- designated initializer overrides selected ones
    Employee e1{.name = "Alice", .salary = 75000.0};
    // id uses its default (0), active uses its default (true)
    std::cout << "e1: name=" << e1.name
              << " id=" << e1.id
              << " salary=" << e1.salary
              << " active=" << e1.active << '\n';

    // 5. Readability benefit: designated init makes the intent clear
    Employee e2{
        .name   = "Bob",
        .id     = 42,
        .salary = 90000.0,
        .active = false
    };
    std::cout << "e2: name=" << e2.name
              << " id=" << e2.id
              << " salary=" << e2.salary
              << " active=" << e2.active << '\n';

    // 6. NON-aggregate: designated initializers do NOT work
    // NonAgg n{.x = 5};  // ERROR: not an aggregate -- uncomment to see error

    // RULE REMINDER: order must match declaration order in C++20
    // Point bad{.y = 2, .x = 1};  // ERROR in C++20: out of declaration order

    return 0;
}
