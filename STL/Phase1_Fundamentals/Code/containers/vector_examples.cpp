/**
 * std::vector Examples - Comprehensive Guide
 * 
 * Compile: g++ -std=c++20 -Wall -Wextra -O2 vector_examples.cpp -o vector_examples
 * Run: ./vector_examples
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

// ============================================================================
// BASIC OPERATIONS
// ============================================================================

void basicOperations() {
    std::cout << "=== BASIC OPERATIONS ===\n";
    
    // Declaration and initialization
    std::vector<int> v1;                      // Empty vector
    std::vector<int> v2(5);                   // 5 elements (default-initialized to 0)
    std::vector<int> v3(5, 42);               // 5 elements, all 42
    std::vector<int> v4 = {1, 2, 3, 4, 5};    // Initializer list
    std::vector<int> v5(v4);                  // Copy constructor
    std::vector<int> v6(std::move(v4));       // Move constructor (v4 is now empty)
    
    // Access
    std::cout << "v3[0] = " << v3[0] << "\n";           // No bounds checking
    std::cout << "v3.at(0) = " << v3.at(0) << "\n";     // Bounds checking (throws)
    std::cout << "v3.front() = " << v3.front() << "\n"; // First element
    std::cout << "v3.back() = " << v3.back() << "\n";   // Last element
    
    // Size and capacity
    std::cout << "Size: " << v3.size() << ", Capacity: " << v3.capacity() << "\n";
    
    std::cout << "\n";
}

// ============================================================================
// MODIFIERS
// ============================================================================

void modifiers() {
    std::cout << "=== MODIFIERS ===\n";
    
    std::vector<int> v;
    
    // push_back and emplace_back
    v.push_back(1);
    v.push_back(2);
    v.emplace_back(3);  // Construct in-place (more efficient for complex types)
    
    std::cout << "After push_back: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
    
    // pop_back
    v.pop_back();
    std::cout << "After pop_back: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
    
    // insert
    v.insert(v.begin() + 1, 99);  // Insert 99 at position 1
    std::cout << "After insert: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
    
    // erase
    v.erase(v.begin() + 1);  // Erase element at position 1
    std::cout << "After erase: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
    
    // resize
    v.resize(5, 42);  // Resize to 5, fill new elements with 42
    std::cout << "After resize(5, 42): ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
    
    // clear
    v.clear();
    std::cout << "After clear: size = " << v.size() << ", capacity = " << v.capacity() << "\n";
    
    std::cout << "\n";
}

// ============================================================================
// CAPACITY MANAGEMENT
// ============================================================================

void capacityManagement() {
    std::cout << "=== CAPACITY MANAGEMENT ===\n";
    
    std::vector<int> v;
    
    std::cout << "Initial: size = " << v.size() << ", capacity = " << v.capacity() << "\n";
    
    // Reserve capacity
    v.reserve(100);
    std::cout << "After reserve(100): size = " << v.size() << ", capacity = " << v.capacity() << "\n";
    
    // Add elements (no reallocation until capacity exceeded)
    for (int i = 0; i < 50; ++i) {
        v.push_back(i);
    }
    std::cout << "After 50 push_back: size = " << v.size() << ", capacity = " << v.capacity() << "\n";
    
    // shrink_to_fit (request to reduce capacity)
    v.shrink_to_fit();
    std::cout << "After shrink_to_fit: size = " << v.size() << ", capacity = " << v.capacity() << "\n";
    
    // Swap idiom to truly free memory
    v.clear();
    std::vector<int>().swap(v);
    std::cout << "After swap idiom: size = " << v.size() << ", capacity = " << v.capacity() << "\n";
    
    std::cout << "\n";
}

// ============================================================================
// ITERATOR INVALIDATION DEMO
// ============================================================================

void iteratorInvalidation() {
    std::cout << "=== ITERATOR INVALIDATION ===\n";
    
    std::vector<int> v = {1, 2, 3, 4, 5};
    
    // Safe erase pattern
    std::cout << "Removing even numbers:\n";
    for (auto it = v.begin(); it != v.end(); ) {
        if (*it % 2 == 0) {
            it = v.erase(it);  // erase returns next valid iterator
        } else {
            ++it;
        }
    }
    
    std::cout << "Result: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
    
    std::cout << "\n";
}

// ============================================================================
// ERASE-REMOVE IDIOM
// ============================================================================

void eraseRemoveIdiom() {
    std::cout << "=== ERASE-REMOVE IDIOM ===\n";
    
    std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};
    
    std::cout << "Original: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
    
    // Remove all 2's
    v.erase(std::remove(v.begin(), v.end(), 2), v.end());
    
    std::cout << "After removing 2's: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
    
    // Remove all even numbers
    v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    v.erase(std::remove_if(v.begin(), v.end(), 
        [](int x) { return x % 2 == 0; }), v.end());
    
    std::cout << "After removing evens: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
    
    std::cout << "\n";
}

// ============================================================================
// ALGORITHMS WITH VECTOR
// ============================================================================

void algorithmsWithVector() {
    std::cout << "=== ALGORITHMS WITH VECTOR ===\n";
    
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    
    // Sort
    std::sort(v.begin(), v.end());
    std::cout << "Sorted: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
    
    // Binary search (requires sorted)
    bool found = std::binary_search(v.begin(), v.end(), 5);
    std::cout << "Binary search for 5: " << (found ? "found" : "not found") << "\n";
    
    // lower_bound and upper_bound
    auto lower = std::lower_bound(v.begin(), v.end(), 5);  // First >= 5
    auto upper = std::upper_bound(v.begin(), v.end(), 5);  // First > 5
    std::cout << "lower_bound(5) index: " << (lower - v.begin()) << "\n";
    std::cout << "upper_bound(5) index: " << (upper - v.begin()) << "\n";
    
    // Reverse
    std::reverse(v.begin(), v.end());
    std::cout << "Reversed: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
    
    // Accumulate (sum)
    int sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "Sum: " << sum << "\n";
    
    // Transform
    std::vector<int> v2(v.size());
    std::transform(v.begin(), v.end(), v2.begin(), 
        [](int x) { return x * 2; });
    std::cout << "Transformed (x2): ";
    for (int x : v2) std::cout << x << " ";
    std::cout << "\n";
    
    std::cout << "\n";
}

// ============================================================================
// CUSTOM TYPES
// ============================================================================

struct Person {
    std::string name;
    int age;
    
    Person(std::string n, int a) : name(std::move(n)), age(a) {}
};

void customTypes() {
    std::cout << "=== CUSTOM TYPES ===\n";
    
    std::vector<Person> people;
    
    // emplace_back constructs in-place (more efficient)
    people.emplace_back("Alice", 30);
    people.emplace_back("Bob", 25);
    people.emplace_back("Charlie", 35);
    
    // Sort by age
    std::sort(people.begin(), people.end(), 
        [](const Person& a, const Person& b) {
            return a.age < b.age;
        });
    
    std::cout << "Sorted by age:\n";
    for (const auto& p : people) {
        std::cout << "  " << p.name << ": " << p.age << "\n";
    }
    
    std::cout << "\n";
}

// ============================================================================
// 2D VECTOR (MATRIX)
// ============================================================================

void twoDimensionalVector() {
    std::cout << "=== 2D VECTOR (MATRIX) ===\n";
    
    // Create 3x4 matrix filled with 0
    std::vector<std::vector<int>> matrix(3, std::vector<int>(4, 0));
    
    // Fill with values
    int value = 1;
    for (auto& row : matrix) {
        for (auto& elem : row) {
            elem = value++;
        }
    }
    
    // Print matrix
    std::cout << "Matrix:\n";
    for (const auto& row : matrix) {
        for (int elem : row) {
            std::cout << elem << " ";
        }
        std::cout << "\n";
    }
    
    std::cout << "\n";
}

// ============================================================================
// PERFORMANCE TIPS
// ============================================================================

void performanceTips() {
    std::cout << "=== PERFORMANCE TIPS ===\n";
    
    // Tip 1: Reserve capacity if you know approximate size
    std::vector<int> v1;
    v1.reserve(1000);  // Prevents multiple reallocations
    for (int i = 0; i < 1000; ++i) {
        v1.push_back(i);
    }
    std::cout << "Tip 1: Reserve capacity to prevent reallocations\n";
    
    // Tip 2: Use emplace_back instead of push_back for complex types
    std::vector<Person> people;
    people.reserve(100);
    people.emplace_back("Alice", 30);  // Constructs in-place
    // people.push_back(Person("Alice", 30));  // Creates temporary, then moves
    std::cout << "Tip 2: Use emplace_back for in-place construction\n";
    
    // Tip 3: Use move semantics for large objects
    std::vector<std::vector<int>> v2;
    std::vector<int> large_vec(10000, 42);
    v2.push_back(std::move(large_vec));  // Move instead of copy
    std::cout << "Tip 3: Use std::move for large objects\n";
    
    // Tip 4: Use swap to truly free memory
    std::vector<int> v3(10000);
    v3.clear();  // size = 0, but capacity still 10000
    std::vector<int>().swap(v3);  // Truly frees memory
    std::cout << "Tip 4: Use swap idiom to free memory\n";
    
    std::cout << "\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    basicOperations();
    modifiers();
    capacityManagement();
    iteratorInvalidation();
    eraseRemoveIdiom();
    algorithmsWithVector();
    customTypes();
    twoDimensionalVector();
    performanceTips();
    
    return 0;
}
