/**
 * @file iterator.cpp
 * @brief Iterator Pattern — Custom Traversal Without Exposing Internals
 *
 * THE PROBLEM:
 * You have a custom data structure (e.g., a NumberRange, a BinaryTree, a
 * custom ring buffer). Clients want to traverse it with a range-based for
 * loop, pass it to STL algorithms (std::find, std::for_each), or compose
 * it with std::views — without you exposing the internal implementation.
 *
 * THE SOLUTION:
 * Implement begin() / end() and a nested Iterator struct with:
 *   - operator*()  → dereference (get current value)
 *   - operator++() → advance to next element
 *   - operator!=() → sentinel check (end condition)
 *
 * THREAD SAFETY:
 * - Iterating a container while another thread MODIFIES it = Data Race.
 * - Iterating with const_iterator on a const container = thread-safe.
 * - For concurrent read + write: protect the container with shared_mutex.
 *
 * COMPILE: g++ -std=c++20 -Wall -Wextra iterator.cpp -o iterator
 * RUN: ./iterator
 */

#include <algorithm>
#include <iostream>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

// ============================================================================
// Example 1: Integer Range — A Lazy Generator
// Generates integers [from, to) without allocating a vector.
// ============================================================================
template <typename T>
class NumberRange {
    T from_, to_, step_;

public:
    NumberRange(T from, T to, T step = 1)
        : from_(from), to_(to), step_(step) {}

    // --- Nested Iterator (STL iterator_traits compliant) ---
    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const T*;
        using reference         = const T&;

        T current;
        T step;

        Iterator(T val, T s) : current(val), step(s) {}

        T          operator*()  const { return current; }
        Iterator&  operator++() { current += step; return *this; }
        bool       operator!=(const Iterator& other) const {
            return current < other.current;
        }
    };

    Iterator begin() const { return Iterator{from_, step_}; }
    Iterator end()   const { return Iterator{to_,   step_}; }
};

// ============================================================================
// Example 2: Custom Singly-Linked List with Full Iterator
// ============================================================================
template <typename T>
class LinkedList {
    struct Node {
        T data;
        std::unique_ptr<Node> next;
        Node(T d) : data(std::move(d)) {}
    };

    std::unique_ptr<Node> head_;
    size_t size_ = 0;

public:
    void pushFront(T value) {
        auto newNode = std::make_unique<Node>(std::move(value));
        newNode->next = std::move(head_);
        head_ = std::move(newNode);
        ++size_;
    }

    size_t size() const { return size_; }

    // --- Forward Iterator ---
    struct Iterator {
        Node* current;

        explicit Iterator(Node* node) : current(node) {}

        T&         operator*()  const { return current->data; }
        Iterator&  operator++() { current = current->next.get(); return *this; }
        bool       operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    // Const version for read-only traversal
    struct ConstIterator {
        const Node* current;

        explicit ConstIterator(const Node* node) : current(node) {}

        const T&       operator*()  const { return current->data; }
        ConstIterator& operator++() { current = current->next.get(); return *this; }
        bool           operator!=(const ConstIterator& other) const {
            return current != other.current;
        }
    };

    Iterator      begin()  { return Iterator{head_.get()}; }
    Iterator      end()    { return Iterator{nullptr}; }
    ConstIterator begin()  const { return ConstIterator{head_.get()}; }
    ConstIterator end()    const { return ConstIterator{nullptr}; }
    ConstIterator cbegin() const { return ConstIterator{head_.get()}; }
    ConstIterator cend()   const { return ConstIterator{nullptr}; }
};

// ============================================================================
// Example 3: Binary Tree — Pre-Order Traversal Iterator
// ============================================================================
struct BSTNode {
    int value;
    std::unique_ptr<BSTNode> left, right;
    BSTNode(int v) : value(v) {}
};

// Collects pre-order traversal into a vector for demonstration
std::vector<int> preOrder(const BSTNode* node) {
    if (!node) return {};
    std::vector<int> result = {node->value};
    auto leftResult  = preOrder(node->left.get());
    auto rightResult = preOrder(node->right.get());
    result.insert(result.end(), leftResult.begin(),  leftResult.end());
    result.insert(result.end(), rightResult.begin(), rightResult.end());
    return result;
}

// ============================================================================
// DEMONSTRATION
// ============================================================================
int main() {
    std::cout << "=== Iterator Pattern Demo ===\n\n";

    // ------------------------------------------------------------------
    // Part 1: NumberRange — Range-Based For Loop
    // ------------------------------------------------------------------
    std::cout << "--- Part 1: NumberRange — Lazy Generator ---\n";

    NumberRange<int> range(1, 11);  // [1, 10]
    std::cout << "  Integers 1 to 10: ";
    for (int n : range) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    NumberRange<int> evens(2, 21, 2); // [2, 4, 6, ..., 20]
    std::cout << "  Even numbers 2 to 20: ";
    for (int n : evens) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // ------------------------------------------------------------------
    // Part 2: STL Algorithm Compatibility
    // ------------------------------------------------------------------
    std::cout << "\n--- Part 2: STL Algorithm Compatibility ---\n";

    NumberRange<int> nums(1, 16);

    // std::find works because Iterator supports operator* and operator++
    auto it = std::find(nums.begin(), nums.end(), 7);
    std::cout << "  std::find(7) in [1,15]: found=" << *it << "\n";

    // Count elements divisible by 3
    auto count = std::count_if(nums.begin(), nums.end(),
                               [](int n) { return n % 3 == 0; });
    std::cout << "  std::count_if(divisible by 3) in [1,15]: " << count << "\n";

    // ------------------------------------------------------------------
    // Part 3: Custom LinkedList with Iterator
    // ------------------------------------------------------------------
    std::cout << "\n--- Part 3: Custom LinkedList Iterator ---\n";

    LinkedList<std::string> list;
    list.pushFront("World");
    list.pushFront("Beautiful");
    list.pushFront("Hello");

    std::cout << "  List (pushFront order): ";
    for (const auto& item : list) {
        std::cout << item << " ";
    }
    std::cout << "\n";

    // Mutable iteration
    std::cout << "  List (uppercased via mutable iterator): ";
    for (auto& item : list) {
        item[0] = static_cast<char>(std::toupper(item[0]));
        std::cout << item << " ";
    }
    std::cout << "\n";

    // const_iterator — safe for concurrent reading
    const LinkedList<std::string>& constList = list;
    std::cout << "  List via cbegin/cend (read-only): ";
    for (auto cit = constList.cbegin(); cit != constList.cend(); ++cit) {
        std::cout << *cit << " ";
    }
    std::cout << "\n";

    // ------------------------------------------------------------------
    // Part 4: C++20 Ranges with vector (Modern Idiom)
    // ------------------------------------------------------------------
    std::cout << "\n--- Part 4: C++20 Ranges (Lazy Pipelines) ---\n";

    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Filter evens, square them, take first 3 — NO intermediate vector!
    std::cout << "  First 3 squared even numbers from [1..10]: ";
    auto pipeline = data
        | std::views::filter([](int n) { return n % 2 == 0; })
        | std::views::transform([](int n) { return n * n; })
        | std::views::take(3);

    for (int n : pipeline) {
        std::cout << n << " ";  // 4, 16, 36
    }
    std::cout << "\n";

    // Reverse iteration
    std::cout << "  Reversed data: ";
    for (int n : data | std::views::reverse) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // ------------------------------------------------------------------
    // Part 5: Binary Tree Iterator (Traversal)
    // ------------------------------------------------------------------
    std::cout << "\n--- Part 5: Binary Search Tree Pre-Order Traversal ---\n";

    //        5
    //       / .
    //      3   8
    //     / .   .
    //    1   4   9
    auto root = std::make_unique<BSTNode>(5);
    root->left = std::make_unique<BSTNode>(3);
    root->right = std::make_unique<BSTNode>(8);
    root->left->left = std::make_unique<BSTNode>(1);
    root->left->right = std::make_unique<BSTNode>(4);
    root->right->right = std::make_unique<BSTNode>(9);

    auto traversal = preOrder(root.get());
    std::cout << "  Pre-Order: ";
    for (int v : traversal) {
        std::cout << v << " ";  // 5 3 1 4 8 9
    }
    std::cout << "\n";

    std::cout << "\n=== Demo Complete ===\n";
    return 0;
}
