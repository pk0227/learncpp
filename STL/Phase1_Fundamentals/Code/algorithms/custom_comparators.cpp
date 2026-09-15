/**
 * Custom Comparators, Strict Weak Ordering, and Transparent Lookup in C++ STL
 *
 * Demonstrates:
 * 1. Three Ways to Provide Comparators: Function pointer, Functor Struct, Lambda
 * 2. Strict Weak Ordering Axioms (Why `<=` causes crashes in std::sort)
 * 3. Custom Comparator with std::priority_queue (Min-Heap and Struct Ordering)
 * 4. Transparent Comparators (C++14 is_transparent / std::less<>) for Zero-Allocation Lookup
 *
 * Compile:
 *   g++ -std=c++20 -O2 -Wall -Wextra custom_comparators.cpp -o custom_comparators
 */

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <algorithm>
#include <queue>
#include <set>
#include <functional>

// ============================================================================
// 1. DATA TYPES AND BASIC COMPARATOR FORMS
// ============================================================================

struct Player {
    int id;
    std::string name;
    int score;
};

// Form 1: Free Function Pointer
bool compareByScoreDesc(const Player& a, const Player& b) {
    return a.score > b.score; // Descending score
}

// Form 2: Functor Struct (aggressive inlining, can hold state)
struct PlayerScoreAsc {
    bool operator()(const Player& a, const Player& b) const {
        return a.score < b.score;
    }
};

void demo_comparator_forms() {
    std::cout << "=== 1. Three Comparator Forms ===\n";

    std::vector<Player> roster = {
        {1, "Alice", 85},
        {2, "Bob", 95},
        {3, "Charlie", 70}
    };

    // 1. Function Pointer
    std::sort(roster.begin(), roster.end(), compareByScoreDesc);
    std::cout << "Top score (Function pointer): " << roster.front().name << " (" << roster.front().score << ")\n";

    // 2. Functor Struct
    std::sort(roster.begin(), roster.end(), PlayerScoreAsc{});
    std::cout << "Lowest score (Functor struct): " << roster.front().name << " (" << roster.front().score << ")\n";

    // 3. Modern Lambda (Most idiomatic)
    std::sort(roster.begin(), roster.end(), [](const Player& a, const Player& b) {
        if (a.score != b.score) return a.score > b.score;
        return a.name < b.name; // Tie-breaker by name
    });
    std::cout << "Sorted with Lambda tie-breaker successfully.\n\n";
}

// ============================================================================
// 2. STRICT WEAK ORDERING: WHY NEVER USE '<='
// ============================================================================

/**
 * Strict Weak Ordering requires:
 * 1. Irreflexivity: comp(x, x) == false
 * 2. Asymmetry:     if comp(x, y) == true, then comp(y, x) == false
 * 3. Transitivity:  if comp(x, y) && comp(y, z), then comp(x, z)
 * 4. Transitivity of Equivalence: if equiv(x, y) && equiv(y, z), then equiv(x, z)
 *    where equiv(a, b) is (!comp(a, b) && !comp(b, a))
 *
 * CRITICAL PITFALL:
 * If you write: bool operator()(int a, int b) { return a <= b; }
 * Then:
 *   comp(5, 5) returns TRUE! (Violates Irreflexivity!)
 *   Both comp(5, 5) and comp(5, 5) return true (Violates Asymmetry!)
 *   Equivalence check: (!comp(5, 5) && !comp(5, 5)) -> (false && false) -> false!
 *   std::sort assumes an element is not equal to itself, enters infinite loops,
 *   dereferences past array bounds, and crashes with segmentation faults!
 */

struct SafeCmp {
    bool operator()(int a, int b) const {
        return a < b; // CORRECT: strictly less than
    }
};

void demo_strict_weak_ordering() {
    std::cout << "=== 2. Strict Weak Ordering Verification ===\n";
    SafeCmp cmp;
    int x = 42;
    std::cout << "Irreflexivity check for x=42: comp(42, 42) is " 
              << (cmp(x, x) ? "TRUE (VIOLATION!)" : "FALSE (Correct)") << "\n\n";
}

// ============================================================================
// 3. PRIORITY QUEUE: MIN-HEAP & CUSTOM STRUCTS
// ============================================================================

struct Task {
    int priority;
    std::string title;
};

// For priority_queue, operator() must return TRUE if 'a' has LOWER priority than 'b'
// (because top() yields the element for which nothing is "greater")
struct TaskPriorityCmp {
    bool operator()(const Task& a, const Task& b) const {
        return a.priority > b.priority; // Min-heap behavior: smallest priority number at top!
    }
};

void demo_priority_queue() {
    std::cout << "=== 3. Priority Queue Custom Comparators ===\n";

    // Standard Min-Heap of integers
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;
    min_pq.push(30);
    min_pq.push(10);
    min_pq.push(20);

    std::cout << "Min-heap root (should be 10): " << min_pq.top() << "\n";

    // Custom Task Min-Heap
    std::priority_queue<Task, std::vector<Task>, TaskPriorityCmp> task_queue;
    task_queue.push({3, "Low priority cleanup"});
    task_queue.push({1, "Critical security patch"});
    task_queue.push({2, "Feature release"});

    std::cout << "Next task to execute: [" << task_queue.top().priority << "] " 
              << task_queue.top().title << "\n\n";
}

// ============================================================================
// 4. TRANSPARENT COMPARATORS (is_transparent for Zero-Allocation Lookup)
// ============================================================================

/**
 * Problem with std::set<std::string>:
 * When calling set.find("alice"), pre-C++14 compilers construct a temporary
 * std::string("alice") heap object just to perform the comparison.
 *
 * Solution (C++14): Transparent Comparator with 'is_transparent' tag.
 * std::less<> (or custom struct with using is_transparent = void;) enables
 * heterogeneous lookup: compares std::string directly with std::string_view or const char*!
 */

struct StringLessTransparent {
    using is_transparent = void; // Enables heterogeneous lookup

    bool operator()(std::string_view a, std::string_view b) const {
        return a < b;
    }
};

void demo_transparent_comparators() {
    std::cout << "=== 4. Heterogeneous Transparent Lookup (C++14/C++20) ===\n";

    // Set using transparent comparator
    std::set<std::string, StringLessTransparent> user_set = {
        "admin", "developer", "guest", "root"
    };

    // Lookup using string_view without allocating a std::string temporary
    std::string_view query = "developer";
    auto it = user_set.find(query); // Zero heap allocations!

    if (it != user_set.end()) {
        std::cout << "Found key via zero-copy string_view lookup: " << *it << "\n";
    }

    // std::less<> is the standard transparent comparator:
    std::set<std::string, std::less<>> std_transparent_set = {"alpha", "beta", "gamma"};
    auto it2 = std_transparent_set.find("beta");
    if (it2 != std_transparent_set.end()) {
        std::cout << "Found key via std::less<>: " << *it2 << "\n";
    }
    std::cout << "\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    demo_comparator_forms();
    demo_strict_weak_ordering();
    demo_priority_queue();
    demo_transparent_comparators();

    std::cout << "All custom comparator demonstrations completed successfully.\n";
    return 0;
}
