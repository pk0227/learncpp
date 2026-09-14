# 🎁 Utility Types - Modern C++ Helpers

> **`pair`, `tuple`, `optional`, `variant`, `any`**

---

## 📑 Table of Contents

1. [1. std::pair - Two Values](#1-stdpair---two-values)
2. [2. std::tuple - Multiple Values](#2-stdtuple---multiple-values)
3. [3. std::optional (C++17) - Maybe a Value](#3-stdoptional-c17---maybe-a-value)
4. [4. std::variant (C++17) - Type-Safe Union](#4-stdvariant-c17---type-safe-union)
5. [5. std::any (C++17) - Type-Erased Storage](#5-stdany-c17---type-erased-storage)
6. [6. std::string_view (C++17) - Zero-Copy String Slice](#6-stdstring_view-c17---zero-copy-string-slice)
7. [7. std::span (C++20) - Non-Owning Contiguous Buffer View](#7-stdspan-c20---non-owning-contiguous-buffer-view)
8. [8. std::bitset - Fixed-Size Bit Array](#8-stdbitset---fixed-size-bit-array)
9. [Comparison Matrix](#comparison)
10. [When to Use What](#when-to-use-what)
11. [Key Takeaways](#key-takeaways)

---

## 1️⃣ `std::pair` - Two Values

### Basic Usage

```cpp
#include <utility>

// Declaration
std::pair<int, std::string> p1(1, "hello");
std::pair<int, std::string> p2 = {2, "world"};
auto p3 = std::make_pair(3, "test");

// Access
int first = p1.first;
std::string second = p1.second;

// C++17: Structured binding
auto [key, value] = p1;
std::cout << key << ": " << value << "\n";

// Comparison (lexicographical)
std::pair<int, int> a(1, 2);
std::pair<int, int> b(1, 3);
bool less = (a < b);  // true (compares first, then second)
```

### Common Use Cases

```cpp
// 1. Return multiple values
std::pair<bool, int> divide(int a, int b) {
    if (b == 0) return {false, 0};
    return {true, a / b};
}

auto [success, result] = divide(10, 2);

// 2. Map iteration
std::map<std::string, int> m = {{"Alice", 30}, {"Bob", 25}};
for (const auto& [name, age] : m) {
    std::cout << name << ": " << age << "\n";
}

// 3. Insert return value
std::set<int> s;
auto [it, inserted] = s.insert(42);
if (inserted) {
    std::cout << "Inserted " << *it << "\n";
}
```

---

## 2️⃣ `std::tuple` - Multiple Values

### Basic Usage

```cpp
#include <tuple>

// Declaration
std::tuple<int, std::string, double> t1(1, "hello", 3.14);
auto t2 = std::make_tuple(2, "world", 2.71);

// Access
int first = std::get<0>(t1);
std::string second = std::get<1>(t1);
double third = std::get<2>(t1);

// C++17: Structured binding
auto [id, name, score] = t1;

// Size
constexpr size_t size = std::tuple_size<decltype(t1)>::value;  // 3

// Type of element
using FirstType = std::tuple_element<0, decltype(t1)>::type;  // int
```

### Common Use Cases

```cpp
// 1. Return multiple values
std::tuple<bool, int, std::string> process(int x) {
    if (x < 0) return {false, 0, "negative"};
    return {true, x * 2, "success"};
}

auto [ok, result, message] = process(5);

// 2. Store heterogeneous data
std::vector<std::tuple<int, std::string, double>> records;
records.emplace_back(1, "Alice", 95.5);
records.emplace_back(2, "Bob", 87.3);

// 3. Comparison
std::tuple<int, int, int> a(1, 2, 3);
std::tuple<int, int, int> b(1, 2, 4);
bool less = (a < b);  // true (lexicographical)
```

---

## 3️⃣ `std::optional` (C++17) - Maybe a Value

### Purpose
Represents a value that may or may not exist (safer than pointers or special values like -1).

### Basic Usage

```cpp
#include <optional>

// Declaration
std::optional<int> opt1;           // Empty
std::optional<int> opt2 = 42;      // Has value
std::optional<int> opt3 = std::nullopt;  // Explicitly empty

// Check if has value
if (opt2.has_value()) {
    std::cout << "Value: " << opt2.value() << "\n";
}

// Or use operator bool
if (opt2) {
    std::cout << "Value: " << *opt2 << "\n";  // Dereference
}

// value_or - provide default
int val = opt1.value_or(0);  // Returns 0 if empty

// Assign
opt1 = 100;
opt1 = std::nullopt;  // Make empty
opt1.reset();         // Make empty
```

### Common Use Cases

```cpp
// 1. Safe find operation
std::optional<int> find(const std::vector<int>& v, int target) {
    auto it = std::find(v.begin(), v.end(), target);
    if (it != v.end()) {
        return *it;
    }
    return std::nullopt;
}

std::vector<int> v = {1, 2, 3, 4, 5};
if (auto result = find(v, 3)) {
    std::cout << "Found: " << *result << "\n";
} else {
    std::cout << "Not found\n";
}

// 2. Optional parameters
void process(int required, std::optional<int> optional_param = std::nullopt) {
    int value = optional_param.value_or(100);  // Default to 100
    // ...
}

process(1);      // Uses default
process(1, 50);  // Uses 50

// 3. Lazy initialization
class Database {
    mutable std::optional<Connection> conn;
    
public:
    Connection& getConnection() const {
        if (!conn) {
            conn = Connection::create();  // Lazy init
        }
        return *conn;
    }
};
```

---

## 4️⃣ `std::variant` (C++17) - Type-Safe Union

### Purpose
Holds one of several types (type-safe alternative to `union`).

### Basic Usage

```cpp
#include <variant>

// Declaration - can hold int, double, or string
std::variant<int, double, std::string> v;

v = 42;           // Holds int
v = 3.14;         // Now holds double
v = "hello";      // Now holds string

// Check which type
size_t index = v.index();  // 2 (string is 3rd type)

// Get value (throws if wrong type)
try {
    int i = std::get<int>(v);  // Throws: holds string, not int
} catch (const std::bad_variant_access&) {
    std::cout << "Wrong type!\n";
}

// Safe get with pointer
if (auto* p = std::get_if<std::string>(&v)) {
    std::cout << "String: " << *p << "\n";
}

// Check type
if (std::holds_alternative<std::string>(v)) {
    std::cout << "Holds string\n";
}
```

### Visiting Variants

```cpp
// std::visit - apply function to current value
std::variant<int, double, std::string> v = 42;

std::visit([](auto&& arg) {
    std::cout << "Value: " << arg << "\n";
}, v);

// Type-specific handling
std::visit([](auto&& arg) {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T, int>) {
        std::cout << "Int: " << arg << "\n";
    } else if constexpr (std::is_same_v<T, double>) {
        std::cout << "Double: " << arg << "\n";
    } else {
        std::cout << "String: " << arg << "\n";
    }
}, v);
```

### Common Use Cases

```cpp
// 1. Error handling (Result type)
template<typename T, typename E>
using Result = std::variant<T, E>;

Result<int, std::string> divide(int a, int b) {
    if (b == 0) return std::string("Division by zero");
    return a / b;
}

auto result = divide(10, 2);
if (std::holds_alternative<int>(result)) {
    std::cout << "Result: " << std::get<int>(result) << "\n";
} else {
    std::cout << "Error: " << std::get<std::string>(result) << "\n";
}

// 2. Polymorphism without inheritance
using Shape = std::variant<Circle, Square, Triangle>;

double area(const Shape& shape) {
    return std::visit([](const auto& s) { return s.area(); }, shape);
}
```

---

## 5️⃣ `std::any` (C++17) - Type-Erased Storage

### Purpose
Can hold any type (type-erased, runtime type checking).

### Basic Usage

```cpp
#include <any>

// Declaration
std::any a;

a = 42;
a = 3.14;
a = std::string("hello");

// Check type
if (a.type() == typeid(std::string)) {
    std::cout << "Holds string\n";
}

// Get value (throws if wrong type)
try {
    std::string s = std::any_cast<std::string>(a);
    std::cout << s << "\n";
} catch (const std::bad_any_cast&) {
    std::cout << "Wrong type!\n";
}

// Safe get with pointer
if (auto* p = std::any_cast<std::string>(&a)) {
    std::cout << "String: " << *p << "\n";
}

// Check if has value
if (a.has_value()) {
    std::cout << "Has value\n";
}

// Reset
a.reset();  // Now empty
```

### Common Use Cases

```cpp
// 1. Heterogeneous containers
std::vector<std::any> mixed;
mixed.push_back(42);
mixed.push_back(3.14);
mixed.push_back(std::string("hello"));

// 2. Generic property bag
class Properties {
    std::unordered_map<std::string, std::any> props;
    
public:
    template<typename T>
    void set(const std::string& key, const T& value) {
        props[key] = value;
    }
    
    template<typename T>
    std::optional<T> get(const std::string& key) const {
        auto it = props.find(key);
        if (it == props.end()) return std::nullopt;
        
        try {
            return std::any_cast<T>(it->second);
        } catch (const std::bad_any_cast&) {
            return std::nullopt;
        }
    }
};
```

---

## 6️⃣ `std::string_view` (C++17) - Zero-Copy String Slice

### Purpose
`std::string_view` is a non-owning reference to a character buffer, consisting of a pointer `const CharT*` and a length `size_t`. It eliminates unnecessary memory allocations when passing strings to functions or slicing substrings.

### Basic Usage
```cpp
#include <string_view>
#include <string>
#include <iostream>

void printPrefix(std::string_view sv) {
    // Zero copy substring in O(1):
    std::string_view sub = sv.substr(0, 5);
    std::cout << "Prefix: " << sub << " (len: " << sub.size() << ")\n";
}

// Works seamlessly with string literals, std::string, and char arrays without allocation:
printPrefix("Hello, World!");
std::string str = "Modern C++ STL";
printPrefix(str);
```

> [!WARNING]
> **The Lifetime Trap**:
> `std::string_view` does **not** own the characters it points to! Never return a `std::string_view` referencing a temporary:
> ```cpp
> std::string_view badFunction() {
>     std::string temp = "temporary string";
>     return temp; // ⚠️ DANGER! 'temp' is destroyed, returned view dangles!
> }
> ```

---

## 7️⃣ `std::span` (C++20) - Non-Owning Contiguous Buffer View

### Purpose
`std::span` models a contiguous sequence of objects without owning them. It completely replaces C-style pointer-plus-size `(int* arr, size_t n)` interfaces with type-safe, bounds-checked, and range-for compatible views.

### Basic Usage
```cpp
#include <span>
#include <vector>
#include <array>
#include <iostream>

void processBuffer(std::span<const int> s) {
    std::cout << "Size: " << s.size() << ", Bytes: " << s.size_bytes() << "\n";
    for (int x : s) {
        std::cout << x << " ";
    }
    std::cout << "\n";
    
    // O(1) Subspans:
    auto first_half = s.first(s.size() / 2);
}

int raw_arr[] = {1, 2, 3, 4};
std::vector<int> vec = {10, 20, 30};
std::array<int, 2> arr = {100, 200};

// A single function seamlessly accepts raw arrays, vectors, and std::array:
processBuffer(raw_arr);
processBuffer(vec);
processBuffer(arr);
```

### Static vs Dynamic Extent
```cpp
std::span<int> dynamic_span;               // Size determined at runtime
std::span<int, 4> static_span(raw_arr);    // Size baked into type (zero memory overhead: just 1 pointer!)
```

---

## 8️⃣ `std::bitset` - Fixed-Size Bit Array

### Purpose
A fixed-size array of $N$ bits optimized for space and bitwise operations. Stack allocated with zero heap overhead.

### Basic Usage
```cpp
#include <bitset>
#include <iostream>

std::bitset<8> b1(0b10100101);
std::bitset<8> b2("11000011");

// Bitwise operations:
auto b3 = b1 & b2; // Bitwise AND
auto b4 = b1 | b2; // Bitwise OR

// Querying bits:
bool any_set  = b1.any();   // true
bool all_set  = b1.all();   // false
bool none_set = b1.none();  // false
size_t count  = b1.count(); // 4 set bits

// Modifying bits:
b1.set(1);     // Sets bit at index 1 to true
b1.reset(0);   // Sets bit at index 0 to false
b1.flip();     // Inverts all bits
```

---

## 🎯 Comparison

| Type | Purpose | Type Safety | Performance |
|------|---------|-------------|-------------|
| `pair` | 2 values | ✅ Compile-time | ✅ Zero overhead |
| `tuple` | N values | ✅ Compile-time | ✅ Zero overhead |
| `optional` | Maybe a value | ✅ Compile-time | ✅ Minimal overhead |
| `variant` | One of N types | ✅ Compile-time | ✅ Small overhead |
| `any` | Any type | ❌ Runtime | ❌ Higher overhead |

---

## 🔥 When to Use What?

### Use `pair` when:
- Need exactly 2 values
- Common in STL (map, insert return, etc.)

### Use `tuple` when:
- Need more than 2 values
- Want structured bindings

### Use `optional` when:
- Value may or may not exist
- Safer than pointers or special values (-1, nullptr)

### Use `variant` when:
- Value is one of several known types
- Want type-safe union
- Compile-time type checking

### Use `any` when:
- Type truly unknown at compile time
- Need maximum flexibility
- Performance not critical

---

## 🎓 Key Takeaways

1. **`pair`** - Two values, common in STL
2. **`tuple`** - Multiple values, structured bindings
3. **`optional`** - Maybe a value, safer than pointers
4. **`variant`** - Type-safe union, compile-time checked
5. **`any`** - Type-erased, runtime checked
6. **Prefer `optional` over pointers** for "maybe" values
7. **Prefer `variant` over `any`** when types are known
8. **Use structured bindings** (C++17) for cleaner code

---

## 📚 See Also

- [STL Overview](../1_General/01_STL_Overview.md)
- [Quick Reference](../1_General/07_Quick_Reference.md)

---

**Remember:** Modern C++ provides powerful utility types. Use them for safer, cleaner code!
