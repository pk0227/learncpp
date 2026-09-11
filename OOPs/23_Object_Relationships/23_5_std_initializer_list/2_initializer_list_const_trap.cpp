/**
 * @file 2_initializer_list_const_trap.cpp
 * @brief Demonstrates that elements in std::initializer_list are always const.
 *
 * Concepts covered:
 * 1. Elements of std::initializer_list<T> are const T.
 * 2. Moving from elements silently invokes the copy constructor.
 * 3. Attempting to use std::initializer_list with move-only types (like std::unique_ptr)
 *    fails compilation.
 */

#include <iostream>
#include <initializer_list>
#include <vector>
#include <memory>
#include <utility>

class Tracker
{
public:
    Tracker() { std::cout << "Tracker default constructed\n"; }
    Tracker(const Tracker&) { std::cout << "Tracker COPY constructor called!\n"; }
    Tracker(Tracker&&) noexcept { std::cout << "Tracker MOVE constructor called!\n"; }
};

int main()
{
    std::cout << "--- 1. Testing Move vs Copy with std::initializer_list ---\n";
    Tracker t1, t2;
    std::cout << "\nCreating initializer_list and attempting to move elements into vector:\n";

    // When initializing from std::initializer_list<Tracker>, elements are 'const Tracker'.
    // std::vector's initializer_list constructor cannot move from 'const Tracker'!
    // It must COPY every element:
    std::vector<Tracker> vec{ std::move(t1), std::move(t2) };

    std::cout << "\nNotice: Even though std::move was passed to the braces, COPY constructors ran!\n";
    std::cout << "std::initializer_list elements are always const T.\n\n";

    std::cout << "--- 2. Move-Only Types and Initializer Lists ---\n";
    // The following line would cause a compile-time error because std::unique_ptr cannot be copied:
    // std::vector<std::unique_ptr<int>> badVec{ std::make_unique<int>(1) }; // COMPILE ERROR!

    std::cout << "std::vector<std::unique_ptr<T>> cannot use brace list initialization.\n";
    std::cout << "Alternative: use push_back() or emplace_back() with std::move:\n";
    std::vector<std::unique_ptr<int>> goodVec;
    goodVec.push_back(std::make_unique<int>(42));
    std::cout << "Successfully added move-only type with value: " << *goodVec.front() << '\n';

    return 0;
}
