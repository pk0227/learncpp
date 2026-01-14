# 🌳 Associative Containers - Ordered Containers

> **`set`, `map`, `multiset`, `multimap` - Red-Black Tree Based**

---

## Overview

Associative containers store elements in **sorted order** using Red-Black Trees.

| Container | Stores | Duplicates | Complexity |
|-----------|--------|-----------|-----------|
| `set` | Keys only | ❌ No | O(log n) |
| `multiset` | Keys only | ✅ Yes | O(log n) |
| `map` | Key-value pairs | ❌ No (keys) | O(log n) |
| `multimap` | Key-value pairs | ✅ Yes (keys) | O(log n) |

**Common Properties:**
- **Ordered** iteration (sorted by key)
- **O(log n)** insert, find, erase
- **Bidirectional iterators**
- **Stable iterators** (insertion doesn't invalidate)
- **Red-Black Tree** internal structure

---

## 1️⃣ `std::set` - Unique Sorted Keys

### When to Use
✅ Need unique elements  
✅ Need sorted order  
✅ Need O(log n) operations  
✅ Range queries (`lower_bound`, `upper_bound`)  

### Basic Operations

```cpp
#include <set>

// Declaration
std::set<int> s;
std::set<int> s = {3, 1, 4, 1, 5};  // {1, 3, 4, 5} - duplicates removed, sorted

// Custom comparator (descending order)
std::set<int, std::greater<int>> s_desc = {3, 1, 4, 5};  // {5, 4, 3, 1}

// Insert
auto [it, inserted] = s.insert(2);  // Returns pair<iterator, bool>
// it points to element, inserted = true if new element

s.insert(2);  // No effect if already exists
s.emplace(6);  // Construct in-place

// Find
auto it = s.find(3);  // O(log n)
if (it != s.end()) {
    // Found
}

bool exists = s.count(3);  // Returns 0 or 1 (for set)

// Erase
s.erase(3);        // Erase by value
s.erase(it);       // Erase by iterator
s.clear();         // Remove all

// Size
size_t size = s.size();
bool empty = s.empty();

// Iteration (sorted order)
for (int x : s) {
    std::cout << x << " ";  // 1 3 4 5
}

// Range queries
auto lower = s.lower_bound(3);  // First element >= 3
auto upper = s.upper_bound(3);  // First element > 3
auto range = s.equal_range(3);  // pair of lower_bound and upper_bound

// Find all elements in range [2, 5]
for (auto it = s.lower_bound(2); it != s.upper_bound(5); ++it) {
    std::cout << *it << " ";
}
```

### Complexity

| Operation | Complexity |
|-----------|-----------|
| `insert()` | O(log n) |
| `find()` | O(log n) |
| `erase()` | O(log n) |
| `count()` | O(log n) |
| `lower_bound()` / `upper_bound()` | O(log n) |
| Iteration | O(n) |

### Custom Comparator

```cpp
struct Person {
    std::string name;
    int age;
};

// Comparator function object
struct PersonCompare {
    bool operator()(const Person& a, const Person& b) const {
        return a.age < b.age;  // Sort by age
    }
};

std::set<Person, PersonCompare> people;
people.insert({"Alice", 30});
people.insert({"Bob", 25});
// Sorted by age: Bob(25), Alice(30)

// Lambda comparator (C++11+)
auto cmp = [](const Person& a, const Person& b) {
    return a.age < b.age;
};
std::set<Person, decltype(cmp)> people2(cmp);
```

---

## 2️⃣ `std::map` - Key-Value Pairs (Unique Keys)

### When to Use
✅ Need key-value associations  
✅ Need sorted keys  
✅ Need O(log n) lookup  
✅ Range queries on keys  

### Basic Operations

```cpp
#include <map>

// Declaration
std::map<std::string, int> m;
std::map<std::string, int> m = {{"Alice", 30}, {"Bob", 25}};

// Insert
m.insert({"Charlie", 35});
m.insert(std::make_pair("David", 40));
m.emplace("Eve", 28);

// Access / Insert
m["Frank"] = 32;  // Insert if doesn't exist, update if exists
int age = m["Alice"];  // Returns value, inserts default if key doesn't exist

// Safe access (doesn't insert)
auto it = m.find("Alice");
if (it != m.end()) {
    int age = it->second;  // it->first is key, it->second is value
}

// C++17: try_emplace (doesn't overwrite if exists)
m.try_emplace("Alice", 99);  // No effect if "Alice" exists

// Erase
m.erase("Bob");    // Erase by key
m.erase(it);       // Erase by iterator

// Size
size_t size = m.size();
bool empty = m.empty();

// Iteration (sorted by key)
for (const auto& [key, value] : m) {  // C++17 structured binding
    std::cout << key << ": " << value << "\n";
}

// Traditional iteration
for (auto it = m.begin(); it != m.end(); ++it) {
    std::cout << it->first << ": " << it->second << "\n";
}

// Range queries
auto lower = m.lower_bound("C");  // First key >= "C"
auto upper = m.upper_bound("D");  // First key > "D"
```

### Complexity

| Operation | Complexity |
|-----------|-----------|
| `insert()` / `emplace()` | O(log n) |
| `operator[]` | O(log n) |
| `find()` | O(log n) |
| `erase()` | O(log n) |
| `lower_bound()` / `upper_bound()` | O(log n) |

### Interview Points

**Q: `operator[]` vs `at()` vs `find()`?**

```cpp
std::map<std::string, int> m = {{"Alice", 30}};

// operator[] - inserts if key doesn't exist
int age1 = m["Bob"];  // Inserts {"Bob", 0}, returns 0

// at() - throws if key doesn't exist
try {
    int age2 = m.at("Charlie");  // Throws std::out_of_range
} catch (const std::out_of_range&) {
    // Handle error
}

// find() - safe, doesn't insert
auto it = m.find("David");
if (it != m.end()) {
    int age3 = it->second;
} else {
    // Key doesn't exist
}
```

**Q: When to use `map` vs `unordered_map`?**
- `map`: Need ordering, range queries, predictable O(log n)
- `unordered_map`: Only need lookup, want O(1) average

---

## 3️⃣ `std::multiset` - Sorted Keys (Duplicates Allowed)

### When to Use
✅ Need sorted elements  
✅ Allow duplicates  
✅ Need to count occurrences  

### Basic Operations

```cpp
#include <set>

std::multiset<int> ms = {3, 1, 4, 1, 5, 1};  // {1, 1, 1, 3, 4, 5}

// Insert (always succeeds)
ms.insert(2);
ms.insert(2);  // Both insertions succeed

// Count occurrences
size_t count = ms.count(1);  // Returns 3

// Find (returns iterator to one occurrence)
auto it = ms.find(1);  // Points to one of the 1's

// Erase
ms.erase(1);  // Erases ALL occurrences of 1
ms.erase(it);  // Erases only one occurrence

// Find all occurrences
auto range = ms.equal_range(2);
for (auto it = range.first; it != range.second; ++it) {
    std::cout << *it << " ";  // All 2's
}
```

### Interview Points

**Q: `multiset` vs `vector` + `sort`?**
- `multiset`: Maintains sorted order automatically, O(log n) insert
- `vector` + `sort`: Need to sort after insertions, O(n log n) sort

Use `multiset` if frequent insertions mixed with queries.

---

## 4️⃣ `std::multimap` - Key-Value Pairs (Duplicate Keys Allowed)

### When to Use
✅ Need key-value associations  
✅ Multiple values per key  
✅ Need sorted keys  

### Basic Operations

```cpp
#include <map>

std::multimap<std::string, int> mm;

// Insert (always succeeds)
mm.insert({"Alice", 30});
mm.insert({"Alice", 31});  // Same key, different value
mm.insert({"Bob", 25});

// No operator[] (ambiguous with multiple values)

// Find one occurrence
auto it = mm.find("Alice");  // Points to one of Alice's entries

// Count occurrences
size_t count = mm.count("Alice");  // Returns 2

// Find all values for a key
auto range = mm.equal_range("Alice");
for (auto it = range.first; it != range.second; ++it) {
    std::cout << it->first << ": " << it->second << "\n";
}
// Output:
// Alice: 30
// Alice: 31

// Erase
mm.erase("Alice");  // Erases ALL entries with key "Alice"
mm.erase(it);       // Erases only one entry
```

---

## 🎯 Internal Structure: Red-Black Tree

### Properties
1. **Binary Search Tree** - Left < Parent < Right
2. **Balanced** - Height is O(log n)
3. **Each node is red or black**
4. **Root is black**
5. **Red nodes have black children**
6. **All paths from root to leaf have same number of black nodes**

### Why Red-Black Tree?

| Tree Type | Insert | Find | Erase | Balance |
|-----------|--------|------|-------|---------|
| **Red-Black Tree** | O(log n) | O(log n) | O(log n) | Good (height ≤ 2 log n) |
| AVL Tree | O(log n) | O(log n) | O(log n) | Better (height ≤ 1.44 log n) |
| Unbalanced BST | O(n) worst | O(n) worst | O(n) worst | None |

**STL uses Red-Black Tree because:**
- Faster insertion/deletion than AVL (fewer rotations)
- Good enough search performance
- Simpler implementation

---

## 🔥 Common Interview Questions

### Q1: How to find k-th smallest element in a `set`?

```cpp
std::set<int> s = {3, 1, 4, 1, 5, 9, 2, 6};
int k = 3;

auto it = s.begin();
std::advance(it, k - 1);  // O(k) for set (bidirectional iterator)
std::cout << *it;  // 3rd smallest
```

### Q2: How to find elements in range [a, b]?

```cpp
std::set<int> s = {1, 2, 3, 4, 5, 6, 7, 8, 9};

auto lower = s.lower_bound(3);  // >= 3
auto upper = s.upper_bound(7);  // > 7

for (auto it = lower; it != upper; ++it) {
    std::cout << *it << " ";  // 3 4 5 6 7
}
```

### Q3: How to remove duplicates from a vector?

```cpp
std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5};

// Method 1: Using set
std::set<int> s(v.begin(), v.end());
v.assign(s.begin(), s.end());  // v is now {1, 2, 3, 4, 5, 6, 9}

// Method 2: sort + unique (preserves original order within duplicates)
std::sort(v.begin(), v.end());
v.erase(std::unique(v.begin(), v.end()), v.end());
```

### Q4: How to implement a frequency counter with sorted output?

```cpp
std::vector<std::string> words = {"apple", "banana", "apple", "cherry", "banana", "apple"};

std::map<std::string, int> freq;
for (const auto& word : words) {
    freq[word]++;
}

// Iterate in sorted order (by key)
for (const auto& [word, count] : freq) {
    std::cout << word << ": " << count << "\n";
}
// Output (alphabetical):
// apple: 3
// banana: 2
// cherry: 1
```

---

## 🎓 Key Takeaways

1. **`set`** - Unique sorted keys, O(log n) operations
2. **`map`** - Key-value pairs, unique keys, sorted by key
3. **`multiset`** - Sorted keys, duplicates allowed
4. **`multimap`** - Key-value pairs, duplicate keys allowed
5. **Red-Black Tree** - Self-balancing BST, O(log n) height
6. **Ordered iteration** - Always sorted by key
7. **Stable iterators** - Insertion doesn't invalidate (except erased element)
8. **Range queries** - `lower_bound`, `upper_bound`, `equal_range`

---

## 📚 Next Steps

- [Unordered Containers](file:///home/prashanth/learncpp_workspace/STL/containers/unordered_containers.md)
- [Container Selection Guide](file:///home/prashanth/learncpp_workspace/STL/04_Container_Selection_Guide.md)
- [Set/Map Examples](file:///home/prashanth/learncpp_workspace/STL/examples/containers/set_map_examples.cpp)

---

**Remember:** Use `map`/`set` when you need ordering or range queries. Otherwise, `unordered_map`/`unordered_set` is faster!
