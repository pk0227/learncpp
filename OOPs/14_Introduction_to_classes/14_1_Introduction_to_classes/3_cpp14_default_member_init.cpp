// Demonstrates: C++14 -- Default member initializers are allowed in aggregates
// Before C++14, default member initializers made a type non-aggregate.
// Since C++14, a struct/class can have default member initializers AND remain an aggregate.
//
// Compile: g++ -std=c++14 -fsyntax-only 3_cpp14_default_member_init.cpp  (or -std=c++20)

#include <iostream>
#include <string>

// C++14+: This IS still an aggregate even with default member initializers
struct WindowConfig {
    int width  = 800;    // default member initializer
    int height = 600;    // default member initializer
    bool fullscreen = false;
    std::string title = "My App";
};

struct Sensor {
    int id       = -1;
    double value = 0.0;
    bool active  = false;
};

int main() {
    // 1. Empty braces: ALL defaults are used
    WindowConfig w1{};
    std::cout << "w1: " << w1.width << 'x' << w1.height
              << " fullscreen=" << w1.fullscreen
              << " title=" << w1.title << '\n';
    // Output: 800x600 fullscreen=0 title=My App

    // 2. Provide first member: overrides its default; rest use defaults
    WindowConfig w2{1920};
    std::cout << "w2: " << w2.width << 'x' << w2.height << '\n';
    // Output: 1920x600  (height still uses its default 600)

    // 3. Provide all members: all defaults overridden
    WindowConfig w3{2560, 1440, true, "Game"};
    std::cout << "w3: " << w3.width << 'x' << w3.height
              << " fullscreen=" << w3.fullscreen
              << " title=" << w3.title << '\n';
    // Output: 2560x1440 fullscreen=1 title=Game

    // 4. Partial override
    Sensor s1{42};   // id=42, value=0.0 (default), active=false (default)
    std::cout << "s1: id=" << s1.id
              << " value=" << s1.value
              << " active=" << s1.active << '\n';

    // KEY: explicit initializer OVERRIDES the default member initializer
    Sensor s2{99, 3.14, true};  // all defaults overridden
    std::cout << "s2: id=" << s2.id
              << " value=" << s2.value
              << " active=" << s2.active << '\n';

    return 0;
}
