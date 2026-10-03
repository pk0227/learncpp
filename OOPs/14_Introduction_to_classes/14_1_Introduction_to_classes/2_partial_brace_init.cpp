// Demonstrates: Partial brace initialization of aggregates
// When fewer initializers are provided than members, remaining members
// are VALUE-INITIALIZED (zero for scalars) -- NOT left with indeterminate values.

#include <iostream>
#include <string>

struct Point {
    int x;
    int y;
};

struct Config {
    int width;
    int height;
    int depth;
};

struct Named {
    std::string label;
    int value;
    double ratio;
};

int main() {
    // Case 1: All members explicitly initialized
    Point p1{10, 20};
    std::cout << "p1: x=" << p1.x << ", y=" << p1.y << '\n';  // x=10, y=20

    // Case 2: Only first member initialized -- remainder value-initialized to 0
    Point p2{5};
    std::cout << "p2: x=" << p2.x << ", y=" << p2.y << '\n';  // x=5, y=0

    // Case 3: Empty braces -- ALL members value-initialized to 0
    Point p3{};
    std::cout << "p3: x=" << p3.x << ", y=" << p3.y << '\n';  // x=0, y=0

    // Case 4: Three-member struct with partial init
    Config c1{1920};
    std::cout << "c1: width=" << c1.width
              << ", height=" << c1.height    // value-initialized to 0
              << ", depth=" << c1.depth      // value-initialized to 0
              << '\n';

    // Case 5: Class type member -- value-initialized means default-constructed
    Named n1{"hello"};
    std::cout << "n1: label=" << n1.label
              << ", value=" << n1.value       // 0
              << ", ratio=" << n1.ratio       // 0.0
              << '\n';

    // KEY POINT: without {}, stack variables have INDETERMINATE values (UB to read)
    // Point bad;          // x and y are indeterminate -- reading them is UB
    // Point good{};      // x=0, y=0 -- safe

    return 0;
}
