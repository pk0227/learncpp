# Chapter 19: Dynamic Allocation

Dynamic memory allocation allows a running program to request memory from the operating system when needed during runtime, allocating storage from the **heap** rather than the program's limited **stack**.

---

## Table of Contents
1. [1 — Dynamic memory allocation with new and delete](#1--dynamic-memory-allocation-with-new-and-delete)
2. [2 — Dynamically allocating arrays](#2--dynamically-allocating-arrays)
3. [3 — Destructors and RAII](#3--destructors-and-raii)
4. [4 — Pointers to pointers and dynamic multidimensional arrays](#4--pointers-to-pointers-and-dynamic-multidimensional-arrays)
5. [5 — Void pointers](#5--void-pointers)

---

## 1 — Dynamic memory allocation with new and delete

### The Need for Dynamic Memory Allocation
C++ supports three basic types of memory allocation:

| Allocation Type | Scope & Lifetime | When Allocated & Freed | Known at Compile Time? |
|---|---|---|---|
| **Static** | Static and global variables | Allocated once at startup; persists for program duration | Yes |
| **Automatic** | Function parameters and local variables | Allocated when entering a block; freed automatically on block exit | Yes |
| **Dynamic** | Explicit programmer request | Allocated on demand from the heap; persists until explicitly freed | No (runtime sized) |

Both static and automatic allocation share two defining traits:
1. The size of the variable or array **must be known at compile time**.
2. Memory allocation and deallocation **happen automatically** when the variable is instantiated or destroyed.

### The Heap vs The Stack
- **Dynamic memory allocation** is a mechanism for running programs to request memory from the operating system when needed.
- This memory does not come from the program’s limited **stack** memory—instead, it is allocated from a much larger pool of memory managed by the OS called the **heap** (or free store). On modern systems, the heap can be gigabytes in size.

```cpp
int* ptr{ new int }; // Dynamically allocate an integer and assign address to ptr
```

> [!NOTE]
> **Why Heap Access is Slower than Stack Access**:  
> Accessing heap-allocated objects is generally slower than accessing stack-allocated objects. The compiler knows the direct stack offset of stack-allocated objects at compile time. Heap-allocated objects are accessed indirectly through a pointer, requiring a two-step lookup:
> 1. Read the pointer variable to retrieve the heap address.
> 2. Dereference the address to access the value.

> [!TIP]
> **Modern C++ Best Practice**: In modern C++, prefer **smart pointers** (`std::unique_ptr`, `std::shared_ptr`) and standard containers (`std::vector`, `std::string`) over manual dynamic memory allocation using raw `new` and `delete`. Smart pointers manage memory automatically using RAII.

### Deleting a Single Variable
- Use the scalar form of the **`delete`** operator to deallocate a dynamically allocated object:
  ```cpp
  delete ptr;    // Returns the memory pointed to by ptr back to the OS
  ptr = nullptr; // Reset ptr to a null pointer
  ```

> [!WARNING]
> Deleting a pointer that does **not** point to dynamically allocated memory (such as a stack variable's address) causes **undefined behavior** and fatal crashes.

### Dangling Pointers
- A pointer pointing to deallocated memory is a **dangling pointer**.
- Dereferencing or deleting a dangling pointer leads to **undefined behavior**.

> [!TIP]
> **Best Practice**: Set deleted pointers to `nullptr` immediately after `delete`, unless the pointer variable itself is going out of scope immediately afterward.

### Handling `new` Failure
- By default, if `operator new` fails to allocate memory, it throws a **`std::bad_alloc`** exception. If unhandled, the program crashes with an unhandled exception error.
- **Nothrow Allocation**: To have `new` return a null pointer instead of throwing an exception, pass `std::nothrow` (from `<new>`):
  ```cpp
  #include <new>

  int* value{ new (std::nothrow) int }; // Returns nullptr on allocation failure
  if (!value) {
      // Handle allocation failure gracefully
  }
  ```
- **Deleting `nullptr` is completely safe**: In C++, `delete nullptr` is guaranteed to be a safe no-op. There is never a need to wrap `delete` statements in `if (ptr != nullptr)`.

### Placement `new`
- Standard `new` allocates heap memory and then constructs an object in it.
- **Placement `new`** constructs an object in **pre-existing, already allocated memory** without allocating new memory:
  ```cpp
  #include <new> // Required for placement new

  alignas(MyClass) char buffer[sizeof(MyClass)]; // Pre-allocated raw storage
  MyClass* obj = new (buffer) MyClass{ 42 };     // Construct object inside buffer
  ```
- **Crucial Rule**: Because placement `new` did not allocate heap memory, you **must NEVER call `delete` on a placement-new object**!
- To clean up, you must **explicitly call the destructor**:
  ```cpp
  obj->~MyClass(); // Explicit destructor invocation
  ```
- This technique is how containers like `std::vector` allocate uninitialized raw storage and construct elements on-demand.

### Memory Leaks
- A **memory leak** occurs when dynamically allocated memory (`new`, `new[]`, `malloc`) is never released using `delete`, `delete[]`, or `free`.
- The memory remains marked as "in use" by the OS even though the application has lost all pointers to it and can no longer access it.

### 📁 Code Examples for Section 1
- [`2_dynamic_allocation_new_delete.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/19_Dynamic_Allocation_(_Under_Construction_)_/2_dynamic_allocation_new_delete.cpp): Demonstrates scalar `new`, `delete`, preventing dangling pointers by setting to `nullptr`, safe null deletion, and `std::nothrow`.

---

## 2 — Dynamically allocating arrays

- Dynamically allocated arrays allow array lengths to be determined dynamically at runtime:
  ```cpp
  std::size_t length{ 5 };
  int* array{ new int[length]{ 1, 2, 3, 4, 5 } }; // Dynamic C-style array
  ```
- The length parameter of dynamically allocated arrays has type `std::size_t`. Non-constexpr signed integers should be cast to `std::size_t` to avoid narrowing warnings.

> [!TIP]
> If you feel the need to dynamically allocate a `std::array`, use **`std::vector`** instead.

### Dynamically Deleting Arrays: `delete[]`
- Always use the array form of delete: **`delete[]`**.
- Using the scalar `delete` on an array causes **undefined behavior** (data corruption, memory leaks, or crashes).

```cpp
delete[] array; // Deallocates the dynamic array
array = nullptr;
```

#### How does `delete[]` know how many elements to destroy?
- The runtime system stores the allocation size / element count in an internal header preceding the array memory block. When `delete[]` is called, it reads this metadata to call destructors for all elements and free the entire block. This metadata is not directly accessible to the programmer.

### Dynamic Arrays vs Decayed Fixed Arrays
- Fixed-size arrays decay to pointers, losing their size information (`sizeof()` returns pointer size).
- A dynamically allocated array behaves identically to a decayed fixed array: it is merely a pointer to the first element, and the programmer is responsible for tracking its size and freeing it via `delete[]`.

### Resizing Arrays
- C++ does not provide a built-in way to resize an existing dynamic array.
- **Manual Workaround**: Allocate a new array of the desired size, copy elements over, and delete the old array. This manual approach is error-prone, particularly when elements are class types with non-trivial constructors/destructors.
- **Modern Solution**: Use **`std::vector`**, which manages resizing, memory growth, and element copying automatically.

### 📁 Code Examples for Section 2
- [`3_dynamic_arrays_new_delete.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/19_Dynamic_Allocation_(_Under_Construction_)_/3_dynamic_arrays_new_delete.cpp): Demonstrates dynamic array allocation with `new[]`, list initialization, `sizeof` decay behavior, and proper cleanup using `delete[]`.

---

## 3 — Destructors and RAII

### RAII (Resource Acquisition Is Initialization)
- **RAII** is an essential C++ idiom where resource management is bound to the lifetime of an object with automatic storage duration (stack objects).
- In C++, RAII is implemented through classes equipped with constructors and destructors:
  1. **Acquisition**: The resource (memory, file handle, socket, database lock) is acquired in the class **constructor**.
  2. **Usage**: The resource is safely used throughout the object's lifetime.
  3. **Release**: The resource is automatically released in the class **destructor** when the object goes out of scope.
- **Advantage**: Guarantees zero resource leaks, even in the presence of early returns or thrown exceptions.

```cpp
class AutoResource {
    int* m_ptr{};
public:
    AutoResource(int val) : m_ptr{ new int{ val } } {}
    ~AutoResource() {
        delete m_ptr; // Automatically cleaned up when object goes out of scope!
    }
};
```

> [!WARNING]
> **Warning About `std::exit()` and RAII**:  
> Calling `std::exit()` terminates the program **immediately**. Local stack variables are **not destroyed**, and their destructors are **never called**. If your destructors flush buffers, release file locks, or write closing log entries, `std::exit()` will bypass that cleanup.

### 📁 Code Examples for Section 3
- [`1_2D_array_class.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/19_Dynamic_Allocation_(_Under_Construction_)_/1_2D_array_class.cpp): Implements a complete RAII 2D dynamic array class template managing heap memory allocation and guaranteed cleanup in its destructor.

---

## 4 — Pointers to pointers and dynamic multidimensional arrays

### Arrays of Pointers
A pointer to a pointer (`T**`) can hold the address of a dynamically allocated array of pointers:

```cpp
int** array{ new int*[10] }; // Allocate an array of 10 int pointers
```

### Two-Dimensional Dynamically Allocated Arrays
There are three standard ways to dynamically allocate 2D arrays:

#### Approach 1: Pointer to Array (Fixed Rightmost Dimension)
```cpp
int x{ 7 }; // Non-constant rows
auto array{ new int[x][5] }; // Rightmost dimension must be a compile-time constant!
```

#### Approach 2: Array of Pointers (Independent Rows / Jagged Arrays)
When all dimensions must be dynamic, allocate an array of pointers, then allocate each row independently:

```cpp
constexpr int rows{ 10 };
int** array{ new int*[rows] }; // Array of row pointers

for (int r{ 0 }; r < rows; ++r) {
    array[r] = new int[5]; // Allocate 5 columns per row
}
```

This also enables **jagged (non-rectangular or triangular) arrays**:
```cpp
for (int r{ 0 }; r < rows; ++r) {
    array[r] = new int[r + 1]; // Each row has a different column length!
}
```

#### Proper Deallocation Order for Arrays of Pointers:
You must delete the elements in the **exact reverse order** of allocation:

```cpp
for (int r{ 0 }; r < rows; ++r) {
    delete[] array[r]; // 1. Delete each row array first
}
delete[] array;        // 2. Delete the row pointer array last
array = nullptr;
```

> [!CAUTION]
> If you delete `array` before deleting the individual rows (`array[r]`), the pointers to the row allocations are destroyed, and you can no longer free the row memory, resulting in a severe memory leak.

#### Approach 3: Flattening a 2D Array into a Single 1D Array
Because allocating and deallocating arrays of pointers involves multiple heap allocations and pointer chasing, it is often far better to **flatten** an $R \times C$ grid into a single 1D dynamic array of size $R \times C$:

$$\text{1D Index} = (\text{row} \times \text{cols}) + \text{col}$$

```cpp
int* flatArray{ new int[rows * cols] }; // Single heap allocation!

// Accessing grid[r][c]:
flatArray[(r * cols) + c] = 42;

// Deallocation is a single delete[]:
delete[] flatArray;
```

### 📁 Code Examples for Section 4
- [`1_2D_array_class.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/19_Dynamic_Allocation_(_Under_Construction_)_/1_2D_array_class.cpp): Demonstrates allocating dynamic 2D arrays with an array of pointers, reverse-order deallocation in the destructor, and double indexing (`arr[r][c]`) via a proxy class.
- [`4_flattened_and_jagged_arrays.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/19_Dynamic_Allocation_(_Under_Construction_)_/4_flattened_and_jagged_arrays.cpp): Demonstrates dynamic triangular jagged arrays with reverse cleanup, and single-allocation 1D flattened 2D arrays with index mapping formula.

---

## 5 — Void pointers

### What is a Void Pointer?
- A **void pointer** (`void*`), also called a generic pointer, is a pointer that can point to an object of any data type:
  ```cpp
  int n{ 42 };
  void* ptr{ &n }; // Points to an integer
  ```
- Because the compiler does not know what type of object `void*` points to, **direct dereferencing is illegal**:
  ```cpp
  // std::cout << *ptr; // COMPILE ERROR!
  ```
  You must first use `static_cast` to cast it back to the concrete type:
  ```cpp
  int* intPtr{ static_cast<int*>(ptr) };
  std::cout << *intPtr; // OK!
  ```

### Deleting a Void Pointer Causes Undefined Behavior
- Deleting a `void*` is **undefined behavior**:
  ```cpp
  void* vPtr{ new MyClass{} };
  delete vPtr; // ❌ UNDEFINED BEHAVIOR!
  ```
- Because the type is unknown, the compiler cannot call the appropriate destructor for the object, causing resource leaks and corruption.
- **Fix**: Always `static_cast` back to the original pointer type before calling `delete`.

### Pointer Arithmetic on Void Pointers is Disallowed
- Pointer arithmetic is prohibited on `void*` because pointer arithmetic requires knowing `sizeof(T)` to advance the address. Since `sizeof(void)` is incomplete, `vPtr + 1` is an error.

> [!NOTE]
> **Void References Do Not Exist**:  
> A `void&` does not exist in C++ because references must refer to an identifiable object of known type.

### 📁 Code Examples for Section 5
- [`5_void_pointers.cpp`](file:///home/prashanth/Learnings/learncpp/OOPs/19_Dynamic_Allocation_(_Under_Construction_)_/5_void_pointers.cpp): Demonstrates `void*` assignment, `static_cast` conversions, the undefined behavior trap of deleting a `void*`, and deletion safety.
