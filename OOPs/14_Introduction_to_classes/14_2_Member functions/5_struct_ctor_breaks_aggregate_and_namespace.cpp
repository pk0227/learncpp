// Two best-practice points for member functions on structs:
//
//   (A) A struct with a user-declared CONSTRUCTOR is no longer an aggregate,
//       so it loses aggregate initialization. Keep structs as aggregates by
//       avoiding constructors (and other member functions that disqualify them).
//
//   (B) If a "class type" would have NO data members (only functions), prefer a
//       plain namespace instead -- there is no object state to bundle.

#include <iostream>
#include <type_traits> // std::is_aggregate_v

// ---------------------------------------------------------------------------
// (A) Aggregate vs non-aggregate struct
// ---------------------------------------------------------------------------
struct PointAggregate            // no constructors -> still an aggregate
{
    int x{};
    int y{};
};

struct PointWithCtor             // user-declared constructor -> NOT an aggregate
{
    int x{};
    int y{};
    PointWithCtor(int x_, int y_) : x{ x_ }, y{ y_ } {} // this disqualifies it
};

// ---------------------------------------------------------------------------
// (B) No data members -> prefer a namespace over a struct/class
// ---------------------------------------------------------------------------
namespace mathutils
{
    int square(int n) { return n * n; }
    int cube(int n)   { return n * n * n; }
}

int main()
{
    // Aggregate: direct brace (aggregate) initialization works.
    PointAggregate a{ 1, 2 };
    std::cout << "PointAggregate{1,2} -> (" << a.x << ", " << a.y << ")\n";
    std::cout << "is_aggregate<PointAggregate> = " << std::is_aggregate_v<PointAggregate> << '\n';

    // Non-aggregate: the SAME braces now call the constructor, not aggregate init.
    PointWithCtor b{ 3, 4 };
    std::cout << "PointWithCtor{3,4}  -> (" << b.x << ", " << b.y << ")\n";
    std::cout << "is_aggregate<PointWithCtor>  = " << std::is_aggregate_v<PointWithCtor> << '\n';

    // (B) Functions with no shared state live naturally in a namespace.
    std::cout << "mathutils::square(5) = " << mathutils::square(5) << '\n';
    std::cout << "mathutils::cube(3)   = " << mathutils::cube(3) << '\n';

    return 0;
}
