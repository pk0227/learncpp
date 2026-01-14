# 🧰 Functors & Lambdas - Function Objects in STL

> **Customizing Algorithm Behavior**

---

## Overview

STL algorithms can be customized using **function objects** (functors) and **lambdas**.

**Three forms of callable objects:**
1. **Function pointers** - Traditional C-style
2. **Functors** - Class with `operator()`
3. **Lambdas** - Anonymous functions (C++11+)

---

## 1️⃣ Function Pointers

### Basic Usage

```cpp
// Function
bool isEven(int x) {
    return x % 2 == 0;
}

std::vector<int> v = {1, 2, 3, 4, 5, 6};

// Use with algorithm
auto it = std::find_if(v.begin(), v.end(), isEven);
```

### Limitations
- ❌ Can't store state
- ❌ Can't be inlined easily
- ❌ Verbose for simple operations

---

## 2️⃣ Functors (Function Objects)

### Definition

A **functor** is a class that overloads `operator()`.

```cpp
struct IsEven {
    bool operator()(int x) const {
        return x % 2 == 0;
    }
};

std::vector<int> v = {1, 2, 3, 4, 5, 6};
auto it = std::find_if(v.begin(), v.end(), IsEven{});
```

### Advantages
✅ Can store state  
✅ Can be inlined by compiler  
✅ Type-safe  
✅ Can have multiple overloads  

### Stateful Functor Example

```cpp
struct CountAbove {
    int threshold;
    int count = 0;
    
    CountAbove(int t) : threshold(t) {}
    
    bool operator()(int x) {
        if (x > threshold) {
            count++;
            return true;
        }
        return false;
    }
};

std::vector<int> v = {1, 5, 3, 8, 2, 9, 4};
CountAbove counter(5);
std::count_if(v.begin(), v.end(), std::ref(counter));
std::cout << "Count above 5: " << counter.count << "\n";
```

---

## 3️⃣ Lambdas (Modern C++)

### Basic Syntax

```cpp
// [capture](parameters) -> return_type { body }

auto isEven = [](int x) { return x % 2 == 0; };

std::vector<int> v = {1, 2, 3, 4, 5, 6};
auto it = std::find_if(v.begin(), v.end(), isEven);

// Or inline
auto it2 = std::find_if(v.begin(), v.end(), 
    [](int x) { return x % 2 == 0; });
```

### Capture Modes

```cpp
int threshold = 5;

// [=] - Capture by value (copy)
auto above1 = [=](int x) { return x > threshold; };

// [&] - Capture by reference
auto above2 = [&](int x) { return x > threshold; };

// [threshold] - Capture specific variable by value
auto above3 = [threshold](int x) { return x > threshold; };

// [&threshold] - Capture specific variable by reference
auto above4 = [&threshold](int x) { return x > threshold; };

// [=, &count] - Capture all by value, except count by reference
int count = 0;
auto above5 = [=, &count](int x) {
    if (x > threshold) count++;
    return x > threshold;
};

// [this] - Capture this pointer (in member functions)
class MyClass {
    int threshold = 5;
    
    void process(std::vector<int>& v) {
        auto above = [this](int x) { return x > threshold; };
        std::count_if(v.begin(), v.end(), above);
    }
};
```

### Mutable Lambdas

```cpp
// By default, captured variables are const
// Use 'mutable' to modify them

int count = 0;
auto counter = [count]() mutable {
    count++;  // OK with mutable
    return count;
};

std::cout << counter() << "\n";  // 1
std::cout << counter() << "\n";  // 2
std::cout << count << "\n";      // 0 (original unchanged)
```

### Generic Lambdas (C++14)

```cpp
// auto parameters - works with any type
auto print = [](const auto& x) {
    std::cout << x << "\n";
};

print(42);
print("hello");
print(3.14);
```

---

## 🎯 Standard Functors

STL provides predefined functors in `<functional>`.

### Arithmetic Functors

```cpp
#include <functional>

std::plus<int>{}(3, 4);        // 7
std::minus<int>{}(10, 3);      // 7
std::multiplies<int>{}(3, 4);  // 12
std::divides<int>{}(10, 2);    // 5
std::modulus<int>{}(10, 3);    // 1
std::negate<int>{}(5);         // -5
```

### Comparison Functors

```cpp
std::less<int>{}(3, 5);           // true
std::greater<int>{}(3, 5);        // false
std::less_equal<int>{}(3, 3);     // true
std::greater_equal<int>{}(5, 3);  // true
std::equal_to<int>{}(3, 3);       // true
std::not_equal_to<int>{}(3, 5);   // true
```

### Logical Functors

```cpp
std::logical_and<bool>{}(true, false);  // false
std::logical_or<bool>{}(true, false);   // true
std::logical_not<bool>{}(true);         // false
```

### Using with Containers

```cpp
// Descending order set
std::set<int, std::greater<int>> s = {3, 1, 4, 1, 5};
// s is {5, 4, 3, 1}

// Min-heap priority_queue
std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;
```

---

## 🔥 Common Use Cases

### 1. Custom Sorting

```cpp
struct Person {
    std::string name;
    int age;
};

std::vector<Person> people = {
    {"Alice", 30},
    {"Bob", 25},
    {"Charlie", 35}
};

// Sort by age
std::sort(people.begin(), people.end(),
    [](const Person& a, const Person& b) {
        return a.age < b.age;
    });

// Sort by name
std::sort(people.begin(), people.end(),
    [](const Person& a, const Person& b) {
        return a.name < b.name;
    });
```

### 2. Filtering

```cpp
std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};

// Remove evens
v.erase(std::remove_if(v.begin(), v.end(),
    [](int x) { return x % 2 == 0; }), v.end());

// Keep only numbers > 5
v.erase(std::remove_if(v.begin(), v.end(),
    [](int x) { return x <= 5; }), v.end());
```

### 3. Transformation

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};
std::vector<int> result(v.size());

// Square each element
std::transform(v.begin(), v.end(), result.begin(),
    [](int x) { return x * x; });

// result is {1, 4, 9, 16, 25}
```

### 4. Accumulation with Custom Operation

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};

// Product of all elements
int product = std::accumulate(v.begin(), v.end(), 1,
    [](int acc, int x) { return acc * x; });
// product = 120

// Concatenate strings
std::vector<std::string> words = {"Hello", " ", "World"};
std::string result = std::accumulate(words.begin(), words.end(), std::string(""),
    [](const std::string& acc, const std::string& s) {
        return acc + s;
    });
// result = "Hello World"
```

---

## 🎓 Key Takeaways

1. **Lambdas** are the modern way (C++11+)
2. **Functors** when you need state or reusability
3. **Function pointers** for C compatibility
4. **Capture by value `[=]`** for safety
5. **Capture by reference `[&]`** for performance
6. **`mutable`** to modify captured variables
7. **Standard functors** (`std::less`, `std::greater`, etc.)
8. **Generic lambdas** (C++14) for type flexibility

---

## 📚 See Also

- [Algorithms Overview](file:///home/prashanth/learncpp_workspace/STL/algorithms/algorithms_overview.md)
- [Custom Comparators Example](file:///home/prashanth/learncpp_workspace/STL/examples/algorithms/custom_comparators.cpp)

---

**Remember:** Lambdas are powerful and concise. Use them for custom algorithm behavior!
