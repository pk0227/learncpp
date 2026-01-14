# 🧩 Container Adaptors - Restricted Interfaces

> **`stack`, `queue`, `priority_queue` - Wrappers Around Other Containers**

---

## Overview

Container adaptors provide **restricted interfaces** built on top of other containers.

| Adaptor | Interface | Default Container | Operations |
|---------|-----------|------------------|------------|
| `stack` | LIFO (Last In First Out) | `deque` | push, pop, top |
| `queue` | FIFO (First In First Out) | `deque` | push, pop, front, back |
| `priority_queue` | Heap (max by default) | `vector` | push, pop, top |

**Key Properties:**
- **No iterators** (restricted access)
- **Built on other containers** (deque, vector, list)
- **Specific access patterns** (LIFO, FIFO, priority)

---

## 1️⃣ `std::stack` - LIFO (Last In First Out)

### Characteristics
- **LIFO** access pattern
- **No iterators** (can only access top)
- **Default underlying container:** `deque`
- **Can use:** `vector`, `deque`, `list`

### When to Use
✅ Need LIFO behavior  
✅ Function call stack simulation  
✅ Backtracking algorithms  
✅ Expression evaluation  

### Basic Operations

```cpp
#include <stack>

// Declaration
std::stack<int> s;                    // Default: deque
std::stack<int, std::vector<int>> s;  // Using vector
std::stack<int, std::list<int>> s;    // Using list

// Push
s.push(1);
s.push(2);
s.push(3);
s.emplace(4);  // Construct in-place

// Access top
int top = s.top();  // Returns 4 (last pushed)

// Pop
s.pop();  // Removes top element (doesn't return it!)

// Size
size_t size = s.size();
bool empty = s.empty();

// No iterators!
// for (int x : s) { }  // ❌ Won't compile
```

### Complexity

| Operation | Complexity |
|-----------|-----------|
| `push()` | O(1) |
| `pop()` | O(1) |
| `top()` | O(1) |
| `size()` | O(1) |

### Common Patterns

#### Pattern 1: Balanced Parentheses

```cpp
bool isBalanced(const std::string& s) {
    std::stack<char> st;
    
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        } else {
            if (st.empty()) return false;
            
            char top = st.top();
            st.pop();
            
            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{')) {
                return false;
            }
        }
    }
    
    return st.empty();
}
```

#### Pattern 2: Reverse a Sequence

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};
std::stack<int> s;

// Push all
for (int x : v) {
    s.push(x);
}

// Pop all (reversed)
v.clear();
while (!s.empty()) {
    v.push_back(s.top());
    s.pop();
}
// v is now {5, 4, 3, 2, 1}
```

---

## 2️⃣ `std::queue` - FIFO (First In First Out)

### Characteristics
- **FIFO** access pattern
- **No iterators** (can only access front/back)
- **Default underlying container:** `deque`
- **Can use:** `deque`, `list` (NOT `vector` - no `push_front`)

### When to Use
✅ Need FIFO behavior  
✅ BFS (Breadth-First Search)  
✅ Task scheduling  
✅ Buffer management  

### Basic Operations

```cpp
#include <queue>

// Declaration
std::queue<int> q;                    // Default: deque
std::queue<int, std::list<int>> q;    // Using list

// Push (at back)
q.push(1);
q.push(2);
q.push(3);
q.emplace(4);

// Access
int front = q.front();  // Returns 1 (first pushed)
int back = q.back();    // Returns 4 (last pushed)

// Pop (from front)
q.pop();  // Removes front element

// Size
size_t size = q.size();
bool empty = q.empty();
```

### Complexity

| Operation | Complexity |
|-----------|-----------|
| `push()` | O(1) |
| `pop()` | O(1) |
| `front()` / `back()` | O(1) |
| `size()` | O(1) |

### Common Patterns

#### Pattern 1: BFS (Breadth-First Search)

```cpp
void bfs(Graph& g, int start) {
    std::queue<int> q;
    std::unordered_set<int> visited;
    
    q.push(start);
    visited.insert(start);
    
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        
        // Process node
        std::cout << node << " ";
        
        // Add neighbors
        for (int neighbor : g.neighbors(node)) {
            if (visited.find(neighbor) == visited.end()) {
                q.push(neighbor);
                visited.insert(neighbor);
            }
        }
    }
}
```

#### Pattern 2: Level-Order Traversal (Binary Tree)

```cpp
void levelOrder(TreeNode* root) {
    if (!root) return;
    
    std::queue<TreeNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        int level_size = q.size();
        
        for (int i = 0; i < level_size; ++i) {
            TreeNode* node = q.front();
            q.pop();
            
            std::cout << node->val << " ";
            
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        std::cout << "\n";  // New level
    }
}
```

---

## 3️⃣ `std::priority_queue` - Heap (Max-Heap by Default)

### Characteristics
- **Heap** data structure (binary heap)
- **Max-heap by default** (largest element at top)
- **No iterators** (can only access top)
- **Default underlying container:** `vector`
- **Can use:** `vector`, `deque` (NOT `list` - needs random access)

### When to Use
✅ Need priority-based access  
✅ Dijkstra's algorithm  
✅ Huffman coding  
✅ Top K elements  
✅ Median finding  

### Basic Operations

```cpp
#include <queue>

// Declaration (max-heap)
std::priority_queue<int> pq;

// Min-heap
std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;

// Custom comparator
auto cmp = [](int a, int b) { return a > b; };  // Min-heap
std::priority_queue<int, std::vector<int>, decltype(cmp)> custom_pq(cmp);

// Push
pq.push(3);
pq.push(1);
pq.push(4);
pq.push(2);
pq.emplace(5);

// Access top (largest element)
int top = pq.top();  // Returns 5

// Pop (removes largest)
pq.pop();

// Size
size_t size = pq.size();
bool empty = pq.empty();
```

### Complexity

| Operation | Complexity |
|-----------|-----------|
| `push()` | O(log n) |
| `pop()` | O(log n) |
| `top()` | O(1) |
| `size()` | O(1) |

### Heap Operations Explained

```
Max-Heap (default):
       9
      / \
     7   5
    / \ / \
   3  2 4  1

top() returns 9 (largest)

Min-Heap (with std::greater):
       1
      / \
     2   4
    / \ / \
   3  7 5  9

top() returns 1 (smallest)
```

### Common Patterns

#### Pattern 1: Top K Elements

```cpp
std::vector<int> topKFrequent(std::vector<int>& nums, int k) {
    // Count frequencies
    std::unordered_map<int, int> freq;
    for (int num : nums) {
        freq[num]++;
    }
    
    // Min-heap of size k
    auto cmp = [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second > b.second;  // Min-heap by frequency
    };
    std::priority_queue<std::pair<int, int>, 
                        std::vector<std::pair<int, int>>, 
                        decltype(cmp)> pq(cmp);
    
    for (const auto& [num, count] : freq) {
        pq.push({num, count});
        if (pq.size() > k) {
            pq.pop();  // Remove smallest
        }
    }
    
    // Extract results
    std::vector<int> result;
    while (!pq.empty()) {
        result.push_back(pq.top().first);
        pq.pop();
    }
    
    return result;
}
```

#### Pattern 2: Merge K Sorted Lists

```cpp
struct ListNode {
    int val;
    ListNode* next;
};

ListNode* mergeKLists(std::vector<ListNode*>& lists) {
    // Min-heap by value
    auto cmp = [](ListNode* a, ListNode* b) {
        return a->val > b->val;
    };
    std::priority_queue<ListNode*, std::vector<ListNode*>, decltype(cmp)> pq(cmp);
    
    // Add first node from each list
    for (ListNode* head : lists) {
        if (head) pq.push(head);
    }
    
    ListNode dummy(0);
    ListNode* tail = &dummy;
    
    while (!pq.empty()) {
        ListNode* node = pq.top();
        pq.pop();
        
        tail->next = node;
        tail = tail->next;
        
        if (node->next) {
            pq.push(node->next);
        }
    }
    
    return dummy.next;
}
```

#### Pattern 3: Dijkstra's Algorithm

```cpp
std::vector<int> dijkstra(Graph& g, int start) {
    std::vector<int> dist(g.size(), INT_MAX);
    dist[start] = 0;
    
    // Min-heap: {distance, node}
    std::priority_queue<std::pair<int, int>, 
                        std::vector<std::pair<int, int>>, 
                        std::greater<>> pq;
    pq.push({0, start});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (d > dist[u]) continue;  // Already processed
        
        for (auto [v, weight] : g.neighbors(u)) {
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    
    return dist;
}
```

---

## 🎯 Underlying Container Choices

### `stack`

| Container | push/pop | Memory | When to Use |
|-----------|----------|--------|-------------|
| **`deque`** (default) | O(1) | Medium | General purpose |
| `vector` | O(1)* | Low | Memory-constrained |
| `list` | O(1) | High | Need iterator stability |

\* Amortized

### `queue`

| Container | push/pop | Memory | When to Use |
|-----------|----------|--------|-------------|
| **`deque`** (default) | O(1) | Medium | General purpose |
| `list` | O(1) | High | Need iterator stability |
| ❌ `vector` | N/A | N/A | No `push_front` |

### `priority_queue`

| Container | push/pop | Memory | When to Use |
|-----------|----------|--------|-------------|
| **`vector`** (default) | O(log n) | Low | General purpose |
| `deque` | O(log n) | Medium | Rarely needed |
| ❌ `list` | N/A | N/A | No random access |

---

## 🔥 Common Interview Questions

### Q1: Why doesn't `pop()` return the element?

**A:** **Exception safety**. If `pop()` returned the element and copying threw an exception, the element would be lost (already removed from container).

**Solution:** Separate `top()` and `pop()`:
```cpp
int value = s.top();  // Get value (may throw, but element still in stack)
s.pop();              // Remove (doesn't throw)
```

---

### Q2: How to implement a min-heap?

```cpp
// Use std::greater as comparator
std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;

min_heap.push(3);
min_heap.push(1);
min_heap.push(4);

std::cout << min_heap.top();  // 1 (smallest)
```

---

### Q3: Can you iterate over a `stack`/`queue`/`priority_queue`?

**A:** No! They don't provide iterators (by design - restricted interface).

**Workaround:** Copy to temporary container:
```cpp
std::stack<int> s = {1, 2, 3, 4, 5};
std::stack<int> temp = s;  // Copy

while (!temp.empty()) {
    std::cout << temp.top() << " ";
    temp.pop();
}
// Original s is unchanged
```

---

### Q4: `priority_queue` vs `set` for top K elements?

| Feature | priority_queue | set |
|---------|---------------|-----|
| **Top element** | O(1) | O(1) (begin/rbegin) |
| **Insert** | O(log n) | O(log n) |
| **Remove top** | O(log n) | O(log n) |
| **Iterate** | ❌ No | ✅ Yes |
| **Find arbitrary** | ❌ No | ✅ Yes |
| **Memory** | Lower | Higher |

**Use `priority_queue` if:** Only need top element  
**Use `set` if:** Need to iterate or search

---

## 🎓 Key Takeaways

1. **`stack`** - LIFO, built on `deque` by default
2. **`queue`** - FIFO, built on `deque` by default
3. **`priority_queue`** - Max-heap by default, built on `vector`
4. **No iterators** - Restricted access by design
5. **`pop()` doesn't return** - Exception safety
6. **Min-heap** - Use `std::greater` comparator
7. **Custom comparators** - Lambdas or function objects
8. **Heap operations** - O(log n) push/pop, O(1) top

---

## 📚 Next Steps

- [Algorithms Examples](../../Code/algorithms/algorithm_examples.cpp)
- [Interview Problems](../../../Phase2_Selection_Application/Theory/06_Interview_Problems.md)
- [Adaptor Examples](../../Code/containers/adaptors_examples.cpp)

---

**Remember:** Container adaptors provide **restricted interfaces** for specific use cases. Use the right tool for the job!
