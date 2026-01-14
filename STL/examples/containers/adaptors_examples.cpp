/**
 * Container Adaptors Examples - stack, queue, priority_queue
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -O2 adaptors_examples.cpp -o
 * adaptors_examples Run: ./adaptors_examples
 */

#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <vector>

int main() {
  std::cout << "=== Container Adaptors Examples ===\n\n";

  // ========== std::stack ==========
  std::cout << "========== std::stack (LIFO) ==========\n\n";

  // 1. Basic stack operations
  {
    std::cout << "1. Basic stack operations\n";
    std::stack<int> s;

    s.push(1);
    s.push(2);
    s.push(3);

    std::cout << "   Top: " << s.top() << "\n";
    std::cout << "   Size: " << s.size() << "\n";

    s.pop();
    std::cout << "   After pop, top: " << s.top() << "\n\n";
  }

  // 2. Interview Use Case: Balanced parentheses
  {
    std::cout << "2. Interview Use Case: Balanced parentheses\n";
    auto isBalanced = [](const std::string &s) {
      std::stack<char> st;
      for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
          st.push(c);
        } else {
          if (st.empty())
            return false;
          char top = st.top();
          st.pop();
          if ((c == ')' && top != '(') || (c == ']' && top != '[') ||
              (c == '}' && top != '{')) {
            return false;
          }
        }
      }
      return st.empty();
    };

    std::cout << "   \"({[]})\" is balanced: "
              << (isBalanced("({[]})") ? "Yes" : "No") << "\n";
    std::cout << "   \"({[})\" is balanced: "
              << (isBalanced("({[})") ? "Yes" : "No") << "\n\n";
  }

  // 3. Interview Use Case: Reverse a string
  {
    std::cout << "3. Interview Use Case: Reverse a string\n";
    std::string str = "Hello";
    std::stack<char> s;

    for (char c : str)
      s.push(c);

    std::string reversed;
    while (!s.empty()) {
      reversed += s.top();
      s.pop();
    }

    std::cout << "   Original: " << str << "\n";
    std::cout << "   Reversed: " << reversed << "\n\n";
  }

  // ========== std::queue ==========
  std::cout << "========== std::queue (FIFO) ==========\n\n";

  // 4. Basic queue operations
  {
    std::cout << "4. Basic queue operations\n";
    std::queue<int> q;

    q.push(1);
    q.push(2);
    q.push(3);

    std::cout << "   Front: " << q.front() << "\n";
    std::cout << "   Back: " << q.back() << "\n";
    std::cout << "   Size: " << q.size() << "\n";

    q.pop();
    std::cout << "   After pop, front: " << q.front() << "\n\n";
  }

  // 5. Interview Use Case: BFS simulation
  {
    std::cout << "5. Interview Use Case: BFS level-order traversal\n";
    // Simulating tree: 1 -> 2, 3 -> 4, 5, 6, 7
    std::queue<int> q;
    q.push(1);

    std::cout << "   Level order: ";
    while (!q.empty()) {
      int node = q.front();
      q.pop();
      std::cout << node << " ";

      // Add children (simulated)
      if (node == 1) {
        q.push(2);
        q.push(3);
      } else if (node == 2) {
        q.push(4);
        q.push(5);
      } else if (node == 3) {
        q.push(6);
        q.push(7);
      }
    }
    std::cout << "\n\n";
  }

  // ========== std::priority_queue ==========
  std::cout << "========== std::priority_queue (Heap) ==========\n\n";

  // 6. Basic priority_queue (max-heap by default)
  {
    std::cout << "6. Basic priority_queue (max-heap)\n";
    std::priority_queue<int> pq;

    pq.push(3);
    pq.push(1);
    pq.push(4);
    pq.push(2);

    std::cout << "   Elements (popped in order): ";
    while (!pq.empty()) {
      std::cout << pq.top() << " ";
      pq.pop();
    }
    std::cout << " (largest first)\n\n";
  }

  // 7. Min-heap
  {
    std::cout << "7. Min-heap (using std::greater)\n";
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;

    min_pq.push(3);
    min_pq.push(1);
    min_pq.push(4);
    min_pq.push(2);

    std::cout << "   Elements (popped in order): ";
    while (!min_pq.empty()) {
      std::cout << min_pq.top() << " ";
      min_pq.pop();
    }
    std::cout << " (smallest first)\n\n";
  }

  // 8. Interview Use Case: Top K elements
  {
    std::cout << "8. Interview Use Case: Top 3 elements\n";
    std::vector<int> nums = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    int k = 3;

    // Min-heap of size k
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq;

    for (int num : nums) {
      pq.push(num);
      if (pq.size() > k) {
        pq.pop(); // Remove smallest
      }
    }

    std::cout << "   Top " << k << " elements: ";
    while (!pq.empty()) {
      std::cout << pq.top() << " ";
      pq.pop();
    }
    std::cout << "\n\n";
  }

  // 9. Interview Use Case: Merge K sorted arrays
  {
    std::cout << "9. Interview Use Case: Merge K sorted arrays\n";
    std::vector<std::vector<int>> arrays = {{1, 4, 7}, {2, 5, 8}, {3, 6, 9}};

    // Min-heap: {value, array_index, element_index}
    using Element = std::tuple<int, int, int>;
    auto cmp = [](const Element &a, const Element &b) {
      return std::get<0>(a) > std::get<0>(b); // Min-heap
    };
    std::priority_queue<Element, std::vector<Element>, decltype(cmp)> pq(cmp);

    // Initialize with first element of each array
    for (size_t i = 0; i < arrays.size(); ++i) {
      if (!arrays[i].empty()) {
        pq.push({arrays[i][0], i, 0});
      }
    }

    std::cout << "   Merged: ";
    while (!pq.empty()) {
      auto [val, arr_idx, elem_idx] = pq.top();
      pq.pop();
      std::cout << val << " ";

      // Add next element from same array
      if (elem_idx + 1 < arrays[arr_idx].size()) {
        pq.push({arrays[arr_idx][elem_idx + 1], arr_idx, elem_idx + 1});
      }
    }
    std::cout << "\n\n";
  }

  // 10. Custom comparator for priority_queue
  {
    std::cout << "10. Custom comparator for priority_queue\n";

    struct Task {
      std::string name;
      int priority;
    };

    auto cmp = [](const Task &a, const Task &b) {
      return a.priority < b.priority; // Higher priority first
    };

    std::priority_queue<Task, std::vector<Task>, decltype(cmp)> pq(cmp);

    pq.push({"Low priority task", 1});
    pq.push({"High priority task", 10});
    pq.push({"Medium priority task", 5});

    std::cout << "   Tasks by priority:\n";
    while (!pq.empty()) {
      auto task = pq.top();
      pq.pop();
      std::cout << "     " << task.name << " (priority: " << task.priority
                << ")\n";
    }
    std::cout << "\n";
  }

  std::cout << "Key Points:\n";
  std::cout << "- stack: LIFO, no iterators, O(1) operations\n";
  std::cout << "- queue: FIFO, no iterators, O(1) operations\n";
  std::cout << "- priority_queue: Heap, no iterators, O(log n) push/pop\n";
  std::cout << "- All built on other containers (deque/vector)\n";
  std::cout << "- Restricted interfaces by design\n";

  return 0;
}
