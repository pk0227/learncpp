# ⚡ Unordered Containers - Hash-Based Containers

> **`unordered_set`, `unordered_map`, `unordered_multiset`, `unordered_multimap`**

---

## Overview

Unordered containers use **hash tables** for O(1) average-case operations.

| Container | Stores | Duplicates | Average | Worst |
|-----------|--------|-----------|---------|-------|
| `unordered_set` | Keys only | ❌ No | O(1) | O(n) |
| `unordered_multiset` | Keys only | ✅ Yes | O(1) | O(n) |
| `unordered_map` | Key-value pairs | ❌ No | O(1) | O(n) |
| `unordered_multimap` | Key-value pairs | ✅ Yes | O(1) | O(n) |

**Common Properties:**
- **Unordered** iteration (no sorting)
- **O(1) average** insert, find, erase
- **O(n) worst case** (hash collisions)
- **Forward iterators** only
- **Hash table** internal structure
- **Rehashing** invalidates all iterators

---

## 🎯 Hash Table Basics

### Internal Structure

```
Hash Table:
┌─────────────────────────────────────┐
│ Buckets (array of linked lists)    │
├─────────────────────────────────────┤
│ Bucket 0: [elem1] → [elem5] → NULL │
│ Bucket 1: NULL                      │
│ Bucket 2: [elem2] → [elem3] → NULL │
│ Bucket 3: [elem4] → NULL            │
│ ...                                 │
└─────────────────────────────────────┘

hash(key) % bucket_count → bucket index
```

### Key Concepts

**Hash Function:** Maps key to bucket index
```cpp
size_t bucket = hash(key) % bucket_count;
```

**Load Factor:** `size / bucket_count`
- When load factor > max_load_factor (default 1.0), **rehashing** occurs
- Rehashing: Allocate more buckets, rehash all elements

**Collision Handling:** Separate chaining (linked list in each bucket)

---

## 1️⃣ `std::unordered_set` - Unique Keys, Fast Lookup

### When to Use
✅ Need unique elements  
✅ Don't need ordering  
✅ Want O(1) average lookup  
✅ Large datasets  

### Basic Operations

```cpp
#include <unordered_set>

// Declaration
std::unordered_set<int> us;
std::unordered_set<int> us = {3, 1, 4, 1, 5};  // {3, 1, 4, 5} - order undefined

// Insert
auto [it, inserted] = us.insert(2);  // Returns pair<iterator, bool>
us.emplace(6);

// Find
auto it = us.find(3);  // O(1) average
if (it != us.end()) {
    // Found
}

bool exists = (us.count(3) > 0);  // Returns 0 or 1

// Erase
us.erase(3);        // Erase by value
us.erase(it);       // Erase by iterator
us.clear();

// Size
size_t size = us.size();
bool empty = us.empty();

// Iteration (unordered!)
for (int x : us) {
    std::cout << x << " ";  // Order is undefined
}

// Hash table properties
size_t bucket_count = us.bucket_count();
float load_factor = us.load_factor();  // size / bucket_count
float max_load = us.max_load_factor();  // Default 1.0

// Reserve to prevent rehashing
us.reserve(1000);  // Sets bucket_count to accommodate 1000 elements

// Rehash manually
us.rehash(100);  // Set bucket_count to at least 100
```

### Complexity

| Operation | Average | Worst |
|-----------|---------|-------|
| `insert()` | O(1) | O(n) |
| `find()` | O(1) | O(n) |
| `erase()` | O(1) | O(n) |
| `count()` | O(1) | O(n) |

**Worst case** occurs when all elements hash to same bucket (poor hash function or adversarial input).

### Custom Hash Function

```cpp
struct Person {
    std::string name;
    int age;
    
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

// Custom hash function
struct PersonHash {
    size_t operator()(const Person& p) const {
        // Combine hashes of members
        size_t h1 = std::hash<std::string>{}(p.name);
        size_t h2 = std::hash<int>{}(p.age);
        return h1 ^ (h2 << 1);  // Simple combination
    }
};

std::unordered_set<Person, PersonHash> people;
people.insert({"Alice", 30});
```

---

## 2️⃣ `std::unordered_map` - Key-Value, Fast Lookup

### When to Use
✅ Need key-value associations  
✅ Don't need ordering  
✅ Want O(1) average lookup  
✅ Large datasets  

### Basic Operations

```cpp
#include <unordered_map>

// Declaration
std::unordered_map<std::string, int> um;
std::unordered_map<std::string, int> um = {{"Alice", 30}, {"Bob", 25}};

// Insert
um.insert({"Charlie", 35});
um.emplace("David", 40);

// Access / Insert
um["Eve"] = 28;  // Insert if doesn't exist, update if exists
int age = um["Alice"];

// Safe access
auto it = um.find("Alice");
if (it != um.end()) {
    int age = it->second;
}

// C++17: try_emplace
um.try_emplace("Alice", 99);  // No effect if exists

// Erase
um.erase("Bob");
um.erase(it);

// Iteration (unordered!)
for (const auto& [key, value] : um) {
    std::cout << key << ": " << value << "\n";
}

// Reserve to prevent rehashing
um.reserve(1000);
```

### Complexity

Same as `unordered_set`: O(1) average, O(n) worst.

### Interview Points

**Q: When does rehashing occur?**

```cpp
std::unordered_map<int, int> um;
um.max_load_factor(1.0);  // Default

// Rehashing occurs when: size / bucket_count > max_load_factor

// Example:
um.reserve(10);  // bucket_count = 10 (or more)
for (int i = 0; i < 10; ++i) {
    um[i] = i;  // No rehashing (load_factor ≤ 1.0)
}
um[10] = 10;  // May trigger rehashing!
```

**Q: How to prevent rehashing?**

```cpp
std::unordered_map<int, int> um;
um.reserve(expected_size);  // Pre-allocate buckets

// Now insertions won't rehash until expected_size elements
```

---

## 3️⃣ `std::unordered_multiset` - Duplicates Allowed

### Basic Operations

```cpp
#include <unordered_set>

std::unordered_multiset<int> ums = {3, 1, 4, 1, 5, 1};

// Insert (always succeeds)
ums.insert(2);
ums.insert(2);

// Count occurrences
size_t count = ums.count(1);  // Returns 3

// Find one occurrence
auto it = ums.find(1);

// Erase
ums.erase(1);  // Erases ALL occurrences
ums.erase(it);  // Erases only one occurrence

// Find all occurrences
auto range = ums.equal_range(2);
for (auto it = range.first; it != range.second; ++it) {
    std::cout << *it << " ";
}
```

---

## 4️⃣ `std::unordered_multimap` - Duplicate Keys Allowed

### Basic Operations

```cpp
#include <unordered_map>

std::unordered_multimap<std::string, int> umm;

// Insert
umm.insert({"Alice", 30});
umm.insert({"Alice", 31});  // Same key, different value

// No operator[]

// Find all values for a key
auto range = umm.equal_range("Alice");
for (auto it = range.first; it != range.second; ++it) {
    std::cout << it->first << ": " << it->second << "\n";
}

// Count
size_t count = umm.count("Alice");  // Returns 2

// Erase
umm.erase("Alice");  // Erases ALL
umm.erase(it);       // Erases one
```

---

## 🔥 Hash Function Design

### Requirements for Hash Function

1. **Deterministic:** Same input → same output
2. **Fast:** O(1) computation
3. **Uniform distribution:** Minimize collisions
4. **Avalanche effect:** Small input change → large hash change

### Standard Hash Functions

```cpp
std::hash<int>{}(42);
std::hash<std::string>{}("hello");
std::hash<double>{}(3.14);
std::hash<void*>{}(ptr);
```

### Combining Hashes

```cpp
// Method 1: XOR with shift
size_t h1 = std::hash<std::string>{}(s1);
size_t h2 = std::hash<int>{}(i);
size_t combined = h1 ^ (h2 << 1);

// Method 2: Boost hash_combine
template <class T>
inline void hash_combine(size_t& seed, const T& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

size_t hash = 0;
hash_combine(hash, s1);
hash_combine(hash, i);
```

### Custom Hash for User-Defined Types

```cpp
struct Point {
    int x, y;
    
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Specialization of std::hash
namespace std {
    template<>
    struct hash<Point> {
        size_t operator()(const Point& p) const {
            size_t h1 = std::hash<int>{}(p.x);
            size_t h2 = std::hash<int>{}(p.y);
            return h1 ^ (h2 << 1);
        }
    };
}

std::unordered_set<Point> points;  // Now works!
```

---

## ⚠️ Iterator Invalidation

### Rehashing Invalidates ALL Iterators

```cpp
std::unordered_set<int> us;
us.reserve(10);

auto it = us.insert(1).first;

// Check if rehashing will occur
if (us.size() + 1 > us.bucket_count() * us.max_load_factor()) {
    // Rehashing will occur!
    us.insert(2);
    // ⚠️ it is now invalid!
}
```

### Safe Pattern: Reserve Capacity

```cpp
std::unordered_set<int> us;
us.reserve(1000);  // Prevent rehashing for first 1000 elements

auto it = us.insert(1).first;
// Safe to use it as long as size < 1000
```

### Erase Only Invalidates Erased Element

```cpp
std::unordered_set<int> us = {1, 2, 3, 4, 5};
auto it1 = us.find(1);
auto it2 = us.find(2);

us.erase(it1);
// ⚠️ it1 invalid
// ✅ it2 still valid (no rehashing)
```

---

## 🎯 Performance Considerations

### Load Factor Impact

```cpp
std::unordered_map<int, int> um;

// Low load factor (< 0.5): Fast, but wastes memory
um.max_load_factor(0.5);

// High load factor (> 1.0): Slower (more collisions), saves memory
um.max_load_factor(2.0);

// Default (1.0): Good balance
```

### Bucket Count

```cpp
std::unordered_set<int> us;

std::cout << "Bucket count: " << us.bucket_count() << "\n";
std::cout << "Load factor: " << us.load_factor() << "\n";

// See bucket distribution
for (size_t i = 0; i < us.bucket_count(); ++i) {
    std::cout << "Bucket " << i << ": " << us.bucket_size(i) << " elements\n";
}
```

### When Hash Table Performs Poorly

❌ **Poor hash function** - All elements hash to same bucket → O(n) operations  
❌ **High load factor** - Many collisions → O(n) operations  
❌ **Small dataset** - Overhead of hashing not worth it → Use `set`/`map`  
❌ **Need ordering** - Can't iterate in sorted order → Use `set`/`map`  

---

## 🔥 Common Interview Questions

### Q1: `map` vs `unordered_map` - When to use which?

| Feature | map | unordered_map |
|---------|-----|---------------|
| **Ordering** | ✅ Sorted | ❌ Unordered |
| **Average lookup** | O(log n) | O(1) |
| **Worst lookup** | O(log n) | O(n) |
| **Range queries** | ✅ Yes | ❌ No |
| **Memory** | ~3× element | ~4× element |
| **Predictable** | ✅ Always O(log n) | ❌ Worst case O(n) |

**Use `map` when:**
- Need ordering or range queries
- Need predictable performance
- Small datasets (<1000 elements)

**Use `unordered_map` when:**
- Only need lookup/insert/delete
- Large datasets
- Can provide good hash function

---

### Q2: How to handle hash collisions?

**STL uses separate chaining:**
- Each bucket is a linked list
- Colliding elements stored in same bucket's list
- Search within bucket is O(k) where k = bucket size

**Alternative (not used by STL):**
- Open addressing (linear probing, quadratic probing)
- Robin Hood hashing

---

### Q3: What makes a good hash function?

1. **Fast:** O(1) computation
2. **Uniform:** Distributes keys evenly across buckets
3. **Deterministic:** Same input → same output
4. **Avalanche:** Small change in input → large change in hash

**Bad hash function example:**
```cpp
// Always returns 0 - all elements in one bucket!
struct BadHash {
    size_t operator()(int x) const { return 0; }
};
```

---

### Q4: How to count word frequencies efficiently?

```cpp
std::vector<std::string> words = {"apple", "banana", "apple", "cherry"};

// ✅ GOOD: unordered_map (O(n) total)
std::unordered_map<std::string, int> freq;
for (const auto& word : words) {
    freq[word]++;  // O(1) average
}

// ❌ SLOWER: map (O(n log n) total)
std::map<std::string, int> freq;
for (const auto& word : words) {
    freq[word]++;  // O(log n)
}
```

---

## 🎓 Key Takeaways

1. **O(1) average** operations (insert, find, erase)
2. **O(n) worst case** (hash collisions)
3. **Unordered** iteration (no sorting)
4. **Rehashing** invalidates all iterators
5. **Reserve capacity** to prevent rehashing
6. **Custom hash** required for user-defined types
7. **Load factor** affects performance
8. **Use for large datasets** when ordering not needed

---

## 📚 Next Steps

- [Container Adaptors](file:///home/prashanth/learncpp_workspace/STL/containers/container_adaptors.md)
- [Container Selection Guide](file:///home/prashanth/learncpp_workspace/STL/04_Container_Selection_Guide.md)
- [Unordered Examples](file:///home/prashanth/learncpp_workspace/STL/examples/containers/unordered_examples.cpp)

---

**Remember:** `unordered_map` is faster than `map` for pure lookup, but `map` is better when you need ordering or predictable performance!
