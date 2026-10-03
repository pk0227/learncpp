// Demonstrates: struct != aggregate -- both struct and class can be aggregates or non-aggregates
// The only language difference between struct and class is the DEFAULT ACCESS LEVEL.
// Whether something is an aggregate depends on the aggregate conditions, not the keyword.
//
// Compile: g++ -std=c++20 -fsyntax-only 3_struct_vs_class_aggregate.cpp

#include <iostream>

// --- AGGREGATES ---

// 1. struct aggregate (classic case): public members, no ctors, no virtual
struct StructAggregate {
    int x;
    int y;
};

// 2. class aggregate: class keyword but public members, no ctors -> still aggregate
class ClassAggregate {
public:          // must be explicit since class defaults to private
    int x;
    int y;
};

// --- NON-AGGREGATES ---

// 3. struct non-aggregate: private member breaks aggregate status
struct StructNonAgg_PrivateMember {
    int x;
private:
    int y;       // private member -> NOT an aggregate
};

// 4. struct non-aggregate: user-declared constructor (C++20 rule)
struct StructNonAgg_Ctor {
    int x;
    int y;
    StructNonAgg_Ctor(int a, int b) : x(a), y(b) {}  // user-declared ctor -> NOT aggregate
};

// 5. struct non-aggregate: = default constructor is user-declared in C++20
struct StructNonAgg_DefaultCtor {
    int x;
    StructNonAgg_DefaultCtor() = default;  // user-DECLARED -> NOT aggregate in C++20
};

// 6. class non-aggregate (typical case): private members by default
class ClassNonAgg {
    int x;       // private by default in class -> NOT an aggregate
    int y;
};

// 7. struct non-aggregate: virtual function
struct StructNonAgg_Virtual {
    int x;
    virtual void f() {}  // virtual function -> NOT an aggregate
};

int main() {
    // Aggregates: can use aggregate initialization {}
    StructAggregate sa{10, 20};
    std::cout << "StructAggregate: x=" << sa.x << " y=" << sa.y << '\n';

    ClassAggregate ca{30, 40};   // class keyword, but still aggregate!
    std::cout << "ClassAggregate:  x=" << ca.x << " y=" << ca.y << '\n';

    // Non-aggregates: {} calls constructor (or fails if no matching ctor)
    StructNonAgg_Ctor nc{5, 6};  // calls the constructor
    std::cout << "NonAgg (ctor):   x=" << nc.x << " y=" << nc.y << '\n';

    // These would fail aggregate initialization (uncomment to see errors):
    // StructNonAgg_PrivateMember bad1{1, 2};    // ERROR: y is private
    // StructNonAgg_DefaultCtor   bad2{42};      // ERROR: no matching ctor for int
    // ClassNonAgg                bad3{1, 2};    // ERROR: members are private

    // KEY TAKEAWAY:
    // struct keyword -> public by default -> often satisfies aggregate conditions naturally
    // class keyword  -> private by default -> usually doesn't satisfy aggregate conditions
    // But EITHER can be an aggregate or non-aggregate depending on members and constructors.

    return 0;
}
