# ⚠️ Iterator Invalidation & Pitfalls - Advanced Topics

> **Critical Knowledge for Senior C++ Developers**

---

## 🎯 What is Iterator Invalidation?

**Iterator invalidation** occurs when an iterator no longer points to a valid element or position in a container, typically after a modification operation.

### The Danger

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};
auto it = v.begin() + 2;  // Points to 3

v.push_back(6);  // May trigger reallocation

// ⚠️ DANGER: it may now be invalid!
std::cout << *it;  // Undefined behavior if reallocation occurred!
```

**Why this matters in interviews:**
- Common source of bugs
- Shows understanding of container internals
- Tests knowledge of memory management
- Demonstrates defensive programming skills

---

## 📊 Invalidation Rules by Container

### Quick Reference Table

| Container | Insert | Erase | Reallocation/Rehash | Notes |
|-----------|--------|-------|---------------------|-------|
| **vector** | All if realloc, else from insert point | From erase point to end | All | Capacity check critical |
| **deque** | All | All (except at ends) | N/A | Middle ops invalidate all |
| **list** | None | Only erased | N/A | Most stable |
| **forward_list** | None | Only erased | N/A | Most stable |
| **set/map** | None | Only erased | N/A | Stable except erased |
| **unordered_*** | All if rehash | Only erased | All | Load factor critical |
| **array** | N/A | N/A | N/A | Never (fixed size) |

---

## 1️⃣ `vector` - High Invalidation Risk

### Invalidation Rules

#### A) Insertion

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};
auto it1 = v.begin();
auto it2 = v.begin() + 2;
auto it3 = v.end();

v.insert(v.begin() + 1, 99);

// If NO reallocation (capacity was sufficient):
// ✅ it1 valid (before insertion point)
// ❌ it2 invalid (at or after insertion point)
// ❌ it3 invalid (end iterator always invalidated)

// If reallocation occurred:
// ❌ ALL iterators invalid!
```

#### B) `push_back` / `emplace_back`

```cpp
std::vector<int> v;
v.reserve(10);  // Pre-allocate capacity

auto it = v.begin();
v.push_back(1);  // No reallocation (capacity sufficient)
// ✅ it still valid (but now points to end, not begin!)

v.push_back(2);
// ... (fill to capacity)
v.push_back(11);  // Reallocation!
// ❌ it now invalid!
```

**Key Insight:** Check capacity before insertion!

```cpp
// Safe pattern
if (v.size() == v.capacity()) {
    // Will reallocate - save index instead of iterator
    size_t index = std::distance(v.begin(), it);
    v.push_back(value);
    it = v.begin() + index;  // Reconstruct iterator
} else {
    v.push_back(value);  // Safe, no reallocation
}
```

#### C) Erase

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};
auto it1 = v.begin();
auto it2 = v.begin() + 2;  // Points to 3
auto it3 = v.begin() + 4;  // Points to 5

v.erase(it2);  // Remove 3

// ✅ it1 valid (before erase point)
// ❌ it2 invalid (erased element)
// ❌ it3 invalid (after erase point - elements shifted)
```

**Safe erase pattern:**

```cpp
// ❌ WRONG: Iterator invalidated after erase
for (auto it = v.begin(); it != v.end(); ++it) {
    if (*it == target) {
        v.erase(it);  // it now invalid!
        ++it;  // ⚠️ Undefined behavior!
    }
}

// ✅ CORRECT: Use return value of erase
for (auto it = v.begin(); it != v.end(); ) {
    if (*it == target) {
        it = v.erase(it);  // erase returns next valid iterator
    } else {
        ++it;
    }
}

// ✅ BETTER: Use erase-remove idiom
v.erase(std::remove(v.begin(), v.end(), target), v.end());
```

### Reference and Pointer Invalidation

```cpp
std::vector<int> v = {1, 2, 3};
int& ref = v[1];
int* ptr = &v[2];

v.push_back(4);  // May reallocate

// ⚠️ ref and ptr may now be dangling!
// Accessing them is undefined behavior!
```

---

## 2️⃣ `deque` - Moderate Invalidation Risk

### Invalidation Rules

#### A) Insert/Erase at Ends

```cpp
std::deque<int> d = {1, 2, 3, 4, 5};
auto it = d.begin() + 2;

d.push_back(6);   // ✅ it still valid
d.push_front(0);  // ✅ it still valid (but points to different index!)

// Note: Iterators valid, but indices change!
```

#### B) Insert/Erase in Middle

```cpp
std::deque<int> d = {1, 2, 3, 4, 5};
auto it1 = d.begin();
auto it2 = d.begin() + 2;
auto it3 = d.end();

d.insert(d.begin() + 2, 99);

// ❌ ALL iterators invalid (middle insertion)
```

**Key Insight:** `deque` is only safe for front/back operations!

---

## 3️⃣ `list` / `forward_list` - Low Invalidation Risk

### Invalidation Rules

```cpp
std::list<int> l = {1, 2, 3, 4, 5};
auto it1 = l.begin();
auto it2 = ++l.begin();  // Points to 2
auto it3 = l.end();

l.insert(it2, 99);  // Insert before 2
// ✅ ALL iterators still valid!

l.erase(it2);  // Remove 2
// ✅ it1 valid
// ❌ it2 invalid (erased element)
// ✅ it3 valid
```

**Why so stable?**
- Linked list structure - nodes don't move
- Insertion/deletion only affects links, not other nodes

### Splicing (Unique to `list`)

```cpp
std::list<int> l1 = {1, 2, 3};
std::list<int> l2 = {4, 5, 6};

auto it = l2.begin();  // Points to 4 in l2

l1.splice(l1.end(), l2);  // Move all of l2 to end of l1

// ✅ it still valid, now points to 4 in l1!
// l1 is now {1, 2, 3, 4, 5, 6}
// l2 is now empty
```

---

## 4️⃣ `set` / `map` - Low Invalidation Risk

### Invalidation Rules

```cpp
std::set<int> s = {1, 2, 3, 4, 5};
auto it1 = s.begin();
auto it2 = s.find(3);
auto it3 = s.end();

s.insert(6);
// ✅ ALL iterators still valid!

s.erase(it2);  // Remove 3
// ✅ it1 valid
// ❌ it2 invalid (erased element)
// ✅ it3 valid
```

**Why so stable?**
- Red-Black Tree structure - nodes don't move
- Insertion/deletion only rebalances tree, doesn't relocate nodes

### Common Pitfall: Erasing While Iterating

```cpp
std::map<int, std::string> m = {{1, "a"}, {2, "b"}, {3, "c"}};

// ❌ WRONG
for (auto it = m.begin(); it != m.end(); ++it) {
    if (it->second == "b") {
        m.erase(it);  // it invalidated!
        ++it;  // ⚠️ Undefined behavior!
    }
}

// ✅ CORRECT (C++11+)
for (auto it = m.begin(); it != m.end(); ) {
    if (it->second == "b") {
        it = m.erase(it);  // erase returns next iterator
    } else {
        ++it;
    }
}

// ✅ ALSO CORRECT (C++20+)
std::erase_if(m, [](const auto& pair) {
    return pair.second == "b";
});
```

---

## 5️⃣ `unordered_set` / `unordered_map` - Moderate Invalidation Risk

### Invalidation Rules

#### A) Insertion (No Rehash)

```cpp
std::unordered_set<int> s;
s.reserve(100);  // Pre-allocate buckets

auto it = s.insert(1).first;
s.insert(2);
// ✅ it still valid (no rehash)
```

#### B) Insertion (With Rehash)

```cpp
std::unordered_set<int> s = {1, 2, 3};
auto it = s.find(2);

s.insert(4);  // May trigger rehash!
// ❌ it may be invalid if rehash occurred!
```

**How to check:**

```cpp
auto old_bucket_count = s.bucket_count();
auto it = s.find(2);

s.insert(4);

if (s.bucket_count() != old_bucket_count) {
    // Rehash occurred - it is invalid!
    it = s.find(2);  // Reconstruct iterator
}
```

**Better: Reserve capacity**

```cpp
std::unordered_set<int> s;
s.reserve(1000);  // Avoid rehashing for first 1000 elements

// Now insertions won't invalidate (until load factor exceeded)
```

### Load Factor and Rehashing

```cpp
std::unordered_map<int, int> m;

std::cout << "Load factor: " << m.load_factor() << "\n";
std::cout << "Max load factor: " << m.max_load_factor() << "\n";

// Rehash occurs when: load_factor > max_load_factor
// Default max_load_factor is usually 1.0

// Prevent rehashing
m.reserve(expected_size);  // Sets bucket_count to accommodate expected_size
```

---

## 🐛 Common Bugs and Pitfalls

### Bug 1: Erasing While Iterating (Vector)

```cpp
// ❌ WRONG: Classic bug!
std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};
for (auto it = v.begin(); it != v.end(); ++it) {
    if (*it == 2) {
        v.erase(it);  // Iterator invalidated!
    }
}

// ✅ FIX 1: Use erase return value
for (auto it = v.begin(); it != v.end(); ) {
    if (*it == 2) {
        it = v.erase(it);
    } else {
        ++it;
    }
}

// ✅ FIX 2: Erase-remove idiom (best for vector)
v.erase(std::remove(v.begin(), v.end(), 2), v.end());
```

### Bug 2: Dangling References After Reallocation

```cpp
std::vector<std::string> v = {"hello"};
const std::string& ref = v[0];

v.push_back("world");  // May reallocate!

std::cout << ref;  // ⚠️ Undefined behavior if reallocation occurred!

// ✅ FIX: Don't hold references across modifications
std::cout << v[0];  // Always access through container
```

### Bug 3: Iterator Invalidation in Nested Loops

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};

// ❌ WRONG
for (auto it1 = v.begin(); it1 != v.end(); ++it1) {
    for (auto it2 = v.begin(); it2 != v.end(); ++it2) {
        if (*it1 == *it2) {
            v.erase(it2);  // Invalidates it1 and it2!
        }
    }
}

// ✅ FIX: Use indices for nested loops with modifications
for (size_t i = 0; i < v.size(); ++i) {
    for (size_t j = 0; j < v.size(); ) {
        if (v[i] == v[j] && i != j) {
            v.erase(v.begin() + j);
            // Don't increment j (elements shifted)
        } else {
            ++j;
        }
    }
}
```

### Bug 4: Modifying `unordered_map` While Iterating

```cpp
std::unordered_map<int, int> m = {{1, 10}, {2, 20}, {3, 30}};

// ❌ WRONG: May cause rehash during iteration
for (const auto& [key, value] : m) {
    m[key * 10] = value * 10;  // May rehash and invalidate iterators!
}

// ✅ FIX: Collect changes, apply after iteration
std::vector<std::pair<int, int>> to_add;
for (const auto& [key, value] : m) {
    to_add.push_back({key * 10, value * 10});
}
for (const auto& [key, value] : to_add) {
    m[key] = value;
}
```

### Bug 5: Saving `end()` Iterator

```cpp
std::vector<int> v = {1, 2, 3};
auto end_it = v.end();  // Save end iterator

v.push_back(4);  // Modifies container

if (v.begin() != end_it) {  // ⚠️ end_it may be invalid!
    // ...
}

// ✅ FIX: Always call end() fresh
if (v.begin() != v.end()) {  // Safe
    // ...
}
```

---

## 🛡️ Safe Coding Patterns

### Pattern 1: Use Indices for Modifying Loops

```cpp
// Instead of iterators
for (size_t i = 0; i < v.size(); ++i) {
    if (v[i] == target) {
        v.erase(v.begin() + i);
        --i;  // Adjust for shift
    }
}
```

### Pattern 2: Erase-Remove Idiom

```cpp
// Remove all elements equal to target
v.erase(std::remove(v.begin(), v.end(), target), v.end());

// Remove all elements satisfying predicate
v.erase(std::remove_if(v.begin(), v.end(), 
    [](int x) { return x % 2 == 0; }), v.end());
```

### Pattern 3: Reserve Capacity

```cpp
std::vector<int> v;
v.reserve(1000);  // Pre-allocate

// Now push_back won't invalidate until 1000 elements
for (int i = 0; i < 1000; ++i) {
    v.push_back(i);  // Safe, no reallocation
}
```

### Pattern 4: Use Return Value of `erase`

```cpp
// erase returns iterator to next element
for (auto it = container.begin(); it != container.end(); ) {
    if (should_erase(*it)) {
        it = container.erase(it);  // Safe
    } else {
        ++it;
    }
}
```

### Pattern 5: Collect-Then-Modify

```cpp
// For unordered containers or complex modifications
std::vector<Key> to_erase;
for (const auto& [key, value] : map) {
    if (should_remove(value)) {
        to_erase.push_back(key);
    }
}

for (const auto& key : to_erase) {
    map.erase(key);
}
```

---

## 🔥 Interview Questions

### Q1: Why does `vector::erase` invalidate iterators after the erase point?

**A:** `vector` stores elements contiguously. When you erase an element, all subsequent elements shift left to fill the gap. This means iterators pointing to those elements now point to different elements (or past-the-end), making them invalid.

---

### Q2: How can you safely erase elements from a `vector` while iterating?

**A:** Three approaches:
1. **Use erase return value:** `it = v.erase(it);`
2. **Erase-remove idiom:** `v.erase(std::remove_if(...), v.end());`
3. **Use indices:** `for (size_t i = 0; i < v.size(); ++i)`

---

### Q3: Why doesn't `list::erase` invalidate other iterators?

**A:** `list` is a linked list. Erasing a node only affects that node's links, not other nodes. Other iterators still point to valid nodes.

---

### Q4: When does `unordered_map` invalidate iterators?

**A:** On **rehashing** (when load factor exceeds max load factor). Erase only invalidates the erased element's iterator. To prevent rehashing, use `reserve()`.

---

### Q5: What's wrong with this code?

```cpp
std::vector<int> v = {1, 2, 3};
int& ref = v[0];
v.push_back(4);
std::cout << ref;
```

**A:** `push_back` may trigger reallocation, making `ref` a dangling reference. Accessing it is undefined behavior. Fix: Don't hold references across modifications, or use `reserve()` to prevent reallocation.

---

## 🎓 Key Takeaways

1. **`vector`** - High risk: reallocation invalidates all, erase invalidates from erase point
2. **`deque`** - Moderate risk: middle operations invalidate all
3. **`list`** - Low risk: only erased element invalidated
4. **`set/map`** - Low risk: only erased element invalidated
5. **`unordered_*`** - Moderate risk: rehashing invalidates all
6. **Use erase-remove idiom** for `vector` element removal
7. **Use erase return value** when iterating and erasing
8. **Reserve capacity** to prevent reallocation/rehashing
9. **Don't hold references** across modifications
10. **Indices are safer** than iterators for complex modifications

---

## 📚 Next Steps

1. [**Interview Problems**](06_Interview_Problems.md) - Practice with real scenarios
2. [**Container Examples**](../../Phase1_Fundamentals/Code/containers/) - See safe patterns in code
3. [**Quick Reference**](../../Phase1_Fundamentals/Theory/07_Quick_Reference.md) - Invalidation rules summary

---

**Remember:** Iterator invalidation bugs are **subtle and dangerous**. Always consider invalidation when modifying containers!
