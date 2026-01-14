/**
 * std::deque Examples - Double-Ended Queue
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -O2 deque_examples.cpp -o
 * deque_examples Run: ./deque_examples
 */

#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>

int main() {
  std::cout << "=== std::deque Examples ===\n\n";

  // 1. Declaration and Initialization
  {
    std::cout << "1. Declaration and Initialization\n";
    std::deque<int> d1;
    std::deque<int> d2(5, 42); // 5 elements, all 42
    std::deque<int> d3 = {1, 2, 3, 4, 5};

    std::cout << "   d2: ";
    for (int x : d2)
      std::cout << x << " ";
    std::cout << "\n   d3: ";
    for (int x : d3)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 2. Front and Back Operations (KEY FEATURE!)
  {
    std::cout << "2. Front and Back Operations (KEY FEATURE!)\n";
    std::deque<int> d;

    d.push_back(3);  // [3]
    d.push_back(4);  // [3, 4]
    d.push_front(2); // [2, 3, 4]
    d.push_front(1); // [1, 2, 3, 4]

    std::cout << "   After pushes: ";
    for (int x : d)
      std::cout << x << " ";

    d.pop_front(); // [2, 3, 4]
    d.pop_back();  // [2, 3]

    std::cout << "\n   After pops: ";
    for (int x : d)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 3. Random Access
  {
    std::cout << "3. Random Access\n";
    std::deque<int> d = {10, 20, 30, 40, 50};

    std::cout << "   d[2] = " << d[2] << "\n";
    std::cout << "   d.at(2) = " << d.at(2) << "\n";
    std::cout << "   d.front() = " << d.front() << "\n";
    std::cout << "   d.back() = " << d.back() << "\n\n";
  }

  // 4. Insert and Erase
  {
    std::cout << "4. Insert and Erase\n";
    std::deque<int> d = {1, 2, 4, 5};

    d.insert(d.begin() + 2, 3); // Insert 3 at position 2
    std::cout << "   After insert: ";
    for (int x : d)
      std::cout << x << " ";

    d.erase(d.begin() + 1); // Erase element at position 1
    std::cout << "\n   After erase: ";
    for (int x : d)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 5. Interview Use Case: Sliding Window
  {
    std::cout << "5. Interview Use Case: Sliding Window\n";
    std::vector<int> data = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;
    std::deque<int> window;

    std::cout << "   Data: ";
    for (int x : data)
      std::cout << x << " ";
    std::cout << "\n   Window size: " << k << "\n";
    std::cout << "   Windows:\n";

    for (int x : data) {
      window.push_back(x);
      if (window.size() > k) {
        window.pop_front(); // O(1) - efficient!
      }
      if (window.size() == k) {
        std::cout << "     [";
        for (size_t i = 0; i < window.size(); ++i) {
          std::cout << window[i];
          if (i < window.size() - 1)
            std::cout << ", ";
        }
        std::cout << "]\n";
      }
    }
    std::cout << "\n";
  }

  // 6. Interview Use Case: Queue Implementation
  {
    std::cout << "6. Interview Use Case: Queue Implementation\n";
    std::deque<int> queue;

    // Enqueue
    queue.push_back(1);
    queue.push_back(2);
    queue.push_back(3);

    std::cout << "   After enqueue: ";
    for (int x : queue)
      std::cout << x << " ";

    // Dequeue
    std::cout << "\n   Dequeued: " << queue.front() << "\n";
    queue.pop_front();

    std::cout << "   After dequeue: ";
    for (int x : queue)
      std::cout << x << " ";
    std::cout << "\n\n";
  }

  // 7. Comparison with vector
  {
    std::cout << "7. Comparison with vector\n";
    std::cout << "   deque advantages:\n";
    std::cout << "   - O(1) push_front/pop_front (vector is O(n))\n";
    std::cout << "   - No reallocation (iterators more stable)\n";
    std::cout << "   deque disadvantages:\n";
    std::cout << "   - Slightly slower random access than vector\n";
    std::cout << "   - Not contiguous in memory\n";
    std::cout << "   - Can't use with C APIs expecting contiguous array\n\n";
  }

  std::cout << "Key Points:\n";
  std::cout << "- Fast insertion/deletion at both ends (O(1))\n";
  std::cout << "- Random access (O(1))\n";
  std::cout << "- No reallocation (unlike vector)\n";
  std::cout << "- Perfect for sliding windows and queues\n";
  std::cout << "- Use when you need fast front operations\n";

  return 0;
}
