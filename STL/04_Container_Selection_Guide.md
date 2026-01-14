# 🎯 Container Selection Guide - Decision-Making Framework

> **The Most Important Topic for Senior C++ Interviews**

---

## 🚨 Why This Matters

> **90% of senior C++ interviews include container selection questions.**

Interviewers care MORE about your **decision-making process** than your ability to use containers. They want to see you:
- Understand trade-offs
- Justify your choices
- Consider performance implications
- Think about real-world constraints

---

## 🗺️ Container Selection Flowchart

```
START: What do you need to store?
│
├─► Need key-value pairs?
│   │
│   ├─► YES → Need ordering?
│   │   │
│   │   ├─► YES → Need duplicates?
│   │   │   ├─► YES → std::multimap
│   │   │   └─► NO  → std::map
│   │   │
│   │   └─► NO → Need duplicates?
│   │       ├─► YES → std::unordered_multimap
│   │       └─► NO  → std::unordered_map
│   │
│   └─► NO → Need unique elements?
│       │
│       ├─► YES → Need ordering?
│       │   │
│       │   ├─► YES → std::set
│       │   └─► NO  → std::unordered_set
│       │
│       └─► NO → What access pattern?
│           │
│           ├─► Random access → Size known?
│           │   ├─► YES (fixed) → std::array
│           │   └─► NO (dynamic) → std::vector
│           │
│           ├─► Front/back only → std::deque
│           │
│           ├─► Frequent middle insert/delete → std::list
│           │
│           ├─► LIFO (stack) → std::stack
│           │
│           ├─► FIFO (queue) → std::queue
│           │
│           └─► Priority-based → std::priority_queue
```

---

## 📊 Complete Comparison Matrix

### Performance Characteristics

| Container | Access | Insert (end) | Insert (front) | Insert (middle) | Find | Delete | Memory Overhead |
|-----------|--------|--------------|----------------|-----------------|------|--------|-----------------|
| **array** | O(1) | N/A | N/A | N/A | O(n) | N/A | None |
| **vector** | O(1) | O(1)* | O(n) | O(n) | O(n) | O(n) | Low |
| **deque** | O(1) | O(1) | O(1) | O(n) | O(n) | O(n) | Medium |
| **list** | O(n) | O(1) | O(1) | O(1)† | O(n) | O(1)† | High |
| **forward_list** | O(n) | O(1)‡ | O(1) | O(1)† | O(n) | O(1)† | Medium |
| **set/map** | N/A | O(log n) | O(log n) | O(log n) | O(log n) | O(log n) | High |
| **unordered_set/map** | N/A | O(1)** | O(1)** | O(1)** | O(1)** | O(1)** | High |

\* Amortized  
† If you have iterator to position  
‡ No direct access to end  
** Average case; worst case O(n)

### Iterator Categories

| Container | Iterator Category | Invalidation Risk |
|-----------|------------------|-------------------|
| **array** | Random Access, Contiguous | Never (fixed size) |
| **vector** | Random Access, Contiguous | High (reallocation) |
| **deque** | Random Access | Medium |
| **list** | Bidirectional | Low |
| **forward_list** | Forward | Low |
| **set/map** | Bidirectional | Low |
| **unordered_set/map** | Forward | Medium (rehash) |

---

## 🥊 Head-to-Head Comparisons

### 1. `vector` vs `list`

**The Classic Interview Question!**

#### Use `vector` when:
✅ Random access needed (`v[i]`)  
✅ Cache-friendly performance critical  
✅ Mostly append operations  
✅ Infrequent insertions/deletions in middle  
✅ Memory efficiency matters  

#### Use `list` when:
✅ Frequent insertions/deletions in middle  
✅ Iterator stability required (no invalidation)  
✅ Splicing operations needed  
✅ No random access needed  

#### Example Scenario

```cpp
// Scenario: Maintain a sorted list of tasks, frequently add/remove in middle

// ❌ BAD: vector (O(n) for each insert/delete in middle)
std::vector<Task> tasks;
// Insert in middle: O(n) - shifts all elements
tasks.insert(tasks.begin() + pos, task);

// ✅ GOOD: list (O(1) for insert/delete if you have iterator)
std::list<Task> tasks;
// Insert in middle: O(1) if you have iterator
auto it = find_position(tasks, task);
tasks.insert(it, task);  // O(1)!

// But if you need random access frequently:
// ✅ BETTER: vector + sort + binary search
std::vector<Task> tasks;
tasks.push_back(task);
std::sort(tasks.begin(), tasks.end());
auto it = std::lower_bound(tasks.begin(), tasks.end(), target);
```

#### Benchmark Reality Check

```cpp
// Modern CPUs love contiguous memory (cache lines)
// Even with O(n) operations, vector often wins!

// Insert 1000 elements in middle:
// vector: ~50μs  (O(n) but cache-friendly)
// list:   ~200μs (O(1) but cache misses)

// Lesson: Big-O isn't everything! Cache matters!
```

**Interview Answer Template:**
> "I'd choose `vector` by default for cache efficiency, unless I have frequent middle insertions/deletions AND the container is large (>10,000 elements). Even then, I'd benchmark before switching to `list`."

---

### 2. `map` vs `unordered_map`

**Another Top Interview Question!**

#### Use `map` when:
✅ Need sorted/ordered iteration  
✅ Need range queries (`lower_bound`, `upper_bound`)  
✅ Predictable performance required (always O(log n))  
✅ Small datasets (overhead of hashing not worth it)  

#### Use `unordered_map` when:
✅ Only need lookup/insert/delete (no ordering)  
✅ Large datasets (O(1) beats O(log n))  
✅ Can provide good hash function  
✅ Average-case performance acceptable  

#### Example Scenario

```cpp
// Scenario 1: Word frequency counter
// ✅ GOOD: unordered_map (only need counts, no ordering)
std::unordered_map<std::string, int> word_count;
word_count[word]++;

// Scenario 2: Range query - find all keys between 10 and 20
// ✅ GOOD: map (supports range queries)
std::map<int, std::string> data;
auto start = data.lower_bound(10);
auto end = data.upper_bound(20);
for (auto it = start; it != end; ++it) { /* ... */ }

// ❌ BAD: unordered_map (would need to iterate ALL elements)
std::unordered_map<int, std::string> data;
for (const auto& [key, value] : data) {
    if (key >= 10 && key <= 20) { /* ... */ }  // O(n)!
}

// Scenario 3: Sorted output required
// ✅ GOOD: map (already sorted)
std::map<std::string, int> word_count;
for (const auto& [word, count] : word_count) {
    std::cout << word << ": " << count << "\n";  // Alphabetical!
}

// ❌ BAD: unordered_map (need to sort separately)
std::unordered_map<std::string, int> word_count;
std::vector<std::pair<std::string, int>> sorted(word_count.begin(), word_count.end());
std::sort(sorted.begin(), sorted.end());  // Extra O(n log n) work!
```

#### Performance Comparison

| Operation | map | unordered_map |
|-----------|-----|---------------|
| Insert | O(log n) | O(1) average, O(n) worst |
| Lookup | O(log n) | O(1) average, O(n) worst |
| Delete | O(log n) | O(1) average, O(n) worst |
| Iterate (sorted) | O(n) | O(n log n) (need to sort) |
| Range query | O(log n + k) | O(n) |
| Memory | ~3× element size | ~4× element size |

**Interview Answer Template:**
> "I'd use `unordered_map` for pure lookup performance on large datasets, but `map` if I need ordering, range queries, or predictable performance. For small datasets (<1000 elements), the difference is negligible."

---

### 3. `deque` vs `vector`

#### Use `vector` when:
✅ Only append at end  
✅ Random access is primary operation  
✅ Cache locality critical  
✅ Memory contiguity required (e.g., passing to C API)  

#### Use `deque` when:
✅ Need fast insertion at **both** front and back  
✅ Don't need contiguous memory  
✅ Want to avoid iterator invalidation on growth  

#### Example Scenario

```cpp
// Scenario: Sliding window of last N elements
// ✅ GOOD: deque (efficient push_back + pop_front)
std::deque<int> window;
window.push_back(new_value);  // O(1)
if (window.size() > N) {
    window.pop_front();  // O(1)
}

// ❌ BAD: vector (pop_front is O(n))
std::vector<int> window;
window.push_back(new_value);  // O(1)
if (window.size() > N) {
    window.erase(window.begin());  // O(n) - shifts all elements!
}
```

#### Internal Structure

```
vector:  [████████████████████████]  Contiguous
                                     
deque:   [████] [████] [████] [████]  Segmented chunks
         ↑                        ↑
         front                    back
```

**Interview Answer Template:**
> "`vector` is my default choice for dynamic arrays. I'd only use `deque` if I specifically need efficient front insertion/deletion, like implementing a sliding window or a queue."

---

### 4. `set` vs `priority_queue`

**Subtle but Important Difference!**

#### Use `set` when:
✅ Need to **maintain** sorted unique elements  
✅ Need to **iterate** in sorted order  
✅ Need to **search** for specific elements  
✅ Need to **remove** arbitrary elements  

#### Use `priority_queue` when:
✅ Only need **top** element (max or min)  
✅ Don't need to iterate  
✅ Don't need to search  
✅ Only remove top element  

#### Example Scenario

```cpp
// Scenario: Find K largest elements (need to access all K)
// ✅ GOOD: set (can iterate over all elements)
std::set<int, std::greater<int>> top_k;
for (int x : data) {
    top_k.insert(x);
    if (top_k.size() > k) {
        top_k.erase(--top_k.end());  // Remove smallest
    }
}
// Access all K elements
for (int x : top_k) { /* ... */ }

// ❌ BAD: priority_queue (can only access top, need to pop to see others)
std::priority_queue<int> top_k;
// Can't iterate! Must pop to access elements (destructive)

// Scenario: Dijkstra's algorithm (only need minimum)
// ✅ GOOD: priority_queue (only need top element)
std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
while (!pq.empty()) {
    auto node = pq.top();  // Get minimum
    pq.pop();
    // Process...
}

// ❌ OVERKILL: set (don't need iteration or search)
```

**Interview Answer Template:**
> "`priority_queue` is optimized for accessing only the top element (heap operations). If I need to iterate, search, or remove arbitrary elements, I'd use `set`. For algorithms like Dijkstra's, `priority_queue` is perfect."

---

### 5. `array` vs `vector`

#### Use `array` when:
✅ Size known at **compile time**  
✅ Want **stack allocation**  
✅ Need **zero overhead** (no capacity tracking)  
✅ Interfacing with C APIs (fixed-size buffers)  

#### Use `vector` when:
✅ Size known only at **runtime**  
✅ Need **dynamic resizing**  
✅ Size varies during execution  

#### Example Scenario

```cpp
// Scenario: 3D point (always 3 coordinates)
// ✅ GOOD: array (fixed size, stack allocated)
std::array<double, 3> point = {1.0, 2.0, 3.0};

// ❌ OVERKILL: vector (unnecessary heap allocation)
std::vector<double> point = {1.0, 2.0, 3.0};  // Heap allocation!

// Scenario: Read N integers from user
// ✅ GOOD: vector (size unknown at compile time)
int n;
std::cin >> n;
std::vector<int> data(n);

// ❌ CAN'T USE: array (size must be compile-time constant)
// std::array<int, n> data;  // Error: n is not constant!
```

**Interview Answer Template:**
> "If the size is fixed and known at compile time, `array` is better (stack allocation, zero overhead). Otherwise, `vector` is the right choice."

---

## 🎯 Decision-Making Framework

### Step 1: What's Your Access Pattern?

| Pattern | Container |
|---------|-----------|
| Random access by index | `vector`, `deque`, `array` |
| Sequential access only | Any container |
| Key-based lookup | `map`, `unordered_map`, `set`, `unordered_set` |
| Only front/back access | `deque`, `list`, `queue`, `stack` |
| Priority-based access | `priority_queue` |

### Step 2: What Operations Are Frequent?

| Frequent Operation | Container |
|-------------------|-----------|
| Append at end | `vector`, `deque` |
| Insert at front | `deque`, `list`, `forward_list` |
| Insert in middle | `list` (if large), `vector` (if small) |
| Remove from middle | `list` (if large), `vector` (if small) |
| Find element | `set`, `map`, `unordered_set`, `unordered_map` |
| Maintain sorted order | `set`, `map` |
| Get min/max | `priority_queue`, `set` |

### Step 3: What Are Your Constraints?

| Constraint | Consideration |
|------------|---------------|
| Memory limited | Avoid `list` (high overhead), prefer `vector` |
| Cache performance critical | Prefer `vector`, `array`, `deque` |
| Iterator stability required | Use `list`, `set`, `map` |
| Predictable performance | Use `map`, `set` (avoid `unordered_*`) |
| Need contiguous memory | Use `vector`, `array` |
| Fixed size | Use `array` |

---

## 🔥 Real Interview Questions

### Q1: Design a cache with O(1) lookup, insert, and eviction (LRU Cache)

**Answer:**
```cpp
// Combination of unordered_map + list
class LRUCache {
    int capacity;
    std::list<std::pair<int, int>> items;  // {key, value}
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> cache;
    
public:
    int get(int key) {
        if (cache.find(key) == cache.end()) return -1;
        
        // Move to front (most recently used)
        items.splice(items.begin(), items, cache[key]);
        return cache[key]->second;
    }
    
    void put(int key, int value) {
        if (cache.find(key) != cache.end()) {
            items.erase(cache[key]);
        }
        
        items.push_front({key, value});
        cache[key] = items.begin();
        
        if (cache.size() > capacity) {
            auto last = items.back();
            cache.erase(last.first);
            items.pop_back();
        }
    }
};
```

**Why this combination?**
- `unordered_map` - O(1) lookup by key
- `list` - O(1) move to front, O(1) remove from back, iterator stability

---

### Q2: Find median in a stream of integers

**Answer:**
```cpp
class MedianFinder {
    std::priority_queue<int> max_heap;  // Left half (smaller elements)
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;  // Right half
    
public:
    void addNum(int num) {
        max_heap.push(num);
        min_heap.push(max_heap.top());
        max_heap.pop();
        
        if (max_heap.size() < min_heap.size()) {
            max_heap.push(min_heap.top());
            min_heap.pop();
        }
    }
    
    double findMedian() {
        return max_heap.size() > min_heap.size() 
            ? max_heap.top() 
            : (max_heap.top() + min_heap.top()) / 2.0;
    }
};
```

**Why two heaps?**
- Maintain balance: left half ≤ median ≤ right half
- O(log n) insert, O(1) median access
- Alternative: `multiset` (allows iteration, but slower)

---

### Q3: Implement a data structure for range sum queries

**Answer:**
```cpp
// If updates are rare: prefix sum array
class NumArray {
    std::vector<int> prefix;
public:
    NumArray(std::vector<int>& nums) {
        prefix.resize(nums.size() + 1);
        for (int i = 0; i < nums.size(); ++i) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
    }
    
    int sumRange(int left, int right) {
        return prefix[right + 1] - prefix[left];  // O(1)!
    }
};

// If updates are frequent: segment tree or Fenwick tree
// (More complex, but O(log n) update and query)
```

**Why `vector` for prefix sums?**
- Random access needed: O(1)
- Fixed size after construction
- Cache-friendly

---

## 🎓 Key Takeaways

1. **Default to `vector`** unless you have a specific reason not to
2. **`map` vs `unordered_map`** - Ordering vs speed
3. **`list` is rarely the answer** - Cache performance usually beats Big-O
4. **Combine containers** for complex data structures (LRU cache, median finder)
5. **Benchmark if unsure** - Theory vs practice can differ
6. **Consider iterator invalidation** - Affects algorithm correctness
7. **Memory overhead matters** - `list` uses 2-3× more memory than `vector`

---

## 📚 Next Steps

1. [**Iterator Invalidation**](file:///home/prashanth/learncpp_workspace/STL/05_Iterator_Invalidation.md) - Understand invalidation rules
2. [**Interview Problems**](file:///home/prashanth/learncpp_workspace/STL/06_Interview_Problems.md) - Practice container selection
3. [**Container Guides**](file:///home/prashanth/learncpp_workspace/STL/containers/sequence_containers.md) - Deep dive into each container

---

**Remember:** In interviews, **justify your choice** with trade-offs. There's rarely one "correct" answer!
