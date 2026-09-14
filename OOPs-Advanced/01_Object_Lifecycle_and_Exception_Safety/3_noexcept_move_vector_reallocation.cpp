/**
 * @file 3_noexcept_move_vector_reallocation.cpp
 * @brief Demonstrates the std::vector Reallocation / noexcept Performance Cliff:
 *        - std::vector requires the Strong Exception Guarantee during reallocation.
 *        - If a move constructor is not marked noexcept, std::move_if_noexcept falls back to copy.
 *        - Demonstrates copy count vs move count for noexcept vs non-noexcept classes.
 */

#include <iostream>
#include <vector>
#include <type_traits>

// -----------------------------------------------------------------------------
// Case 1: Move constructor missing 'noexcept'
// -----------------------------------------------------------------------------
class NonNoexceptWidget {
public:
    static inline int copy_count = 0;
    static inline int move_count = 0;

    int id{0};

    NonNoexceptWidget(int val) : id(val) {}

    // Copy constructor
    NonNoexceptWidget(const NonNoexceptWidget& other) : id(other.id) {
        ++copy_count;
    }

    // Move constructor without noexcept!
    NonNoexceptWidget(NonNoexceptWidget&& other) : id(other.id) {
        ++move_count;
    }
};

// -----------------------------------------------------------------------------
// Case 2: Move constructor properly marked 'noexcept'
// -----------------------------------------------------------------------------
class NoexceptWidget {
public:
    static inline int copy_count = 0;
    static inline int move_count = 0;

    int id{0};

    NoexceptWidget(int val) : id(val) {}

    // Copy constructor
    NoexceptWidget(const NoexceptWidget& other) : id(other.id) {
        ++copy_count;
    }

    // Move constructor with noexcept!
    NoexceptWidget(NoexceptWidget&& other) noexcept : id(other.id) {
        ++move_count;
    }
};

int main() {
    std::cout << "std::is_nothrow_move_constructible<NonNoexceptWidget>: " 
              << std::boolalpha << std::is_nothrow_move_constructible_v<NonNoexceptWidget> << "\n";
    std::cout << "std::is_nothrow_move_constructible<NoexceptWidget>:    " 
              << std::is_nothrow_move_constructible_v<NoexceptWidget> << "\n\n";

    // Test 1: Vector of NonNoexceptWidget
    {
        std::vector<NonNoexceptWidget> vec;
        std::cout << "=== Inserting 10 elements into vector<NonNoexceptWidget> ===\n";
        for (int i = 0; i < 10; ++i) {
            vec.emplace_back(i);
        }
        std::cout << "NonNoexceptWidget copies made during reallocations: " 
                  << NonNoexceptWidget::copy_count << "\n";
        std::cout << "NonNoexceptWidget moves made during reallocations:  " 
                  << NonNoexceptWidget::move_count << "\n";
        std::cout << "VERDICT: Move constructor was COMPLETELY BYPASSED during reallocation!\n\n";
    }

    // Test 2: Vector of NoexceptWidget
    {
        std::vector<NoexceptWidget> vec;
        std::cout << "=== Inserting 10 elements into vector<NoexceptWidget> ===\n";
        for (int i = 0; i < 10; ++i) {
            vec.emplace_back(i);
        }
        std::cout << "NoexceptWidget copies made during reallocations: " 
                  << NoexceptWidget::copy_count << "\n";
        std::cout << "NoexceptWidget moves made during reallocations:  " 
                  << NoexceptWidget::move_count << "\n";
        std::cout << "VERDICT: Move semantics FULLY UTILIZED! Zero copies during reallocation!\n";
    }

    return 0;
}
