// Demonstrates: struct does NOT mean aggregate.
// A struct with private (or protected) data members is NOT an aggregate.
// In C++, whether a type is an aggregate depends strictly on aggregate rules ([dcl.init.aggr]),
// NOT on whether the 'struct' or 'class' keyword was used.
//
// Compile: g++ -std=c++20 1_struct_with_private_not_aggregate.cpp -o /tmp/test && /tmp/test

#include <iostream>
#include <type_traits> // std::is_aggregate_v (C++17+)

// 1. Classic struct aggregate: all members public by default, no user-declared ctors
struct PublicPoint
{
    int x; // public by default
    int y; // public by default
};

// 2. Class keyword CAN be an aggregate if members are made public and rules are met
class ClassAggregate
{
public:
    int x; // explicitly public
    int y; // explicitly public
};

// 3. struct with private members: NOT an aggregate!
// The struct keyword does NOT grant aggregate status if private members exist.
struct PrivatePoint
{
    int x; // public by default

private:
    int y; // private member -> immediately violates condition 1 of [dcl.init.aggr]
};

// 4. struct with ALL private members: also NOT an aggregate
struct AllPrivatePoint
{
private:
    int x;
    int y;
};

int main()
{
    std::cout << std::boolalpha;

    // Compile-time trait verification (std::is_aggregate_v)
    std::cout << "std::is_aggregate_v<PublicPoint>:      "
              << std::is_aggregate_v<PublicPoint> << '\n';     // true

    std::cout << "std::is_aggregate_v<ClassAggregate>:    "
              << std::is_aggregate_v<ClassAggregate> << '\n';   // true

    std::cout << "std::is_aggregate_v<PrivatePoint>:      "
              << std::is_aggregate_v<PrivatePoint> << '\n';     // false!

    std::cout << "std::is_aggregate_v<AllPrivatePoint>:   "
              << std::is_aggregate_v<AllPrivatePoint> << '\n';  // false!

    std::cout << '\n';

    // Aggregate initialization works for PublicPoint and ClassAggregate:
    PublicPoint p1{10, 20};
    std::cout << "p1 (PublicPoint): x=" << p1.x << ", y=" << p1.y << '\n';

    ClassAggregate c1{30, 40};
    std::cout << "c1 (ClassAggregate): x=" << c1.x << ", y=" << c1.y << '\n';

    // The following lines will FAIL to compile if uncommented:
    // PrivatePoint p2{10, 20};
    // Compile error: cannot convert '<brace-enclosed initializer list>' to 'PrivatePoint'
    // Reason: PrivatePoint is NOT an aggregate because it contains private member 'y'.
    // Furthermore, even if it were an aggregate (it isn't), private members cannot be accessed.

    // AllPrivatePoint p3{10, 20};
    // Compile error: cannot convert '<brace-enclosed initializer list>' to 'AllPrivatePoint'
    // Reason: AllPrivatePoint is NOT an aggregate because its members are private.

    std::cout << "\nAttempting aggregate init on PrivatePoint or AllPrivatePoint is a compile error.\n";

    return 0;
}
