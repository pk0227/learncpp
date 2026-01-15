# 🧠 Adapter Design Pattern
> **Deep, Practical, Senior-Level Understanding**  
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. What is Adapter? (Real Meaning)

**Standard Definition:**
"Convert the interface of a class into another interface clients expect."

**Senior Definition:**
*   ✅ **"Integration Glue for Incompatible Systems"**
*   ✅ **"Solving the Square Peg in a Round Hole problem"**

It allows you to use a 3rd-party library (e.g., an old C logger) in your modern C++ application (e.g., using `std::string`) without modifying the legacy code.

---

## 🔹 2. Modern & Correct C++ Implementation
**Object Adapter (Composition)** is preferred over Class Adapter (Multiple Inheritance).

```cpp
// 1. Target Interface (What your app expects)
class ILogger {
public:
    virtual void log(std::string_view msg) = 0;
    virtual ~ILogger() = default;
};

// 2. Adaptee (Legacy Lib - cannot change)
class LegacyLogger {
public:
    void printLog(const char* msg) { printf("%s\n", msg); }
};

// 3. The Adapter (Wraps the Legacy object)
class LogAdapter : public ILogger {
    LegacyLogger legacyObj; // Composition
public:
    void log(std::string_view msg) override {
        // Translation Logic: string_view -> const char*
        std::string s(msg); 
        legacyObj.printLog(s.c_str());
    }
};
```

---

## 🔹 3. Pros — Deep Explanation

### ✅ Pro #1: Reusability of Legacy Code
*   **The Benefit**: You can integrate ancient C libraries, binary-only SDKs, or Fortran math logic into modern systems without rewriting them.

### ✅ Pro #2: Single Responsibility Principle
*   **The Benefit**: The Adapter handles **Data Conversion** (marshalling). The Business Logic handles the actual task. You don't pollute your core logic with `reinterpret_cast` or C-style hacks.

---

## 🔹 4. Cons — Deep Explanation

### ❌ Con #1: Runtime Overhead
*   **The Issue**: Every call adds a layer of indirection (Virtual Table + Argument copying/converting).
*   **Impact**: In HFT or deep real-time loops, this small cost adds up.

### ❌ Con #2: Complexity Explosion
*   **The Issue**: If you have 5 incompatible libraries, you end up with 5 different Adapters.
*   **Impact**: The codebase grows with "Wrapper" classes.

---

## 🔹 5. When to Use / Not Use

### ❌ Do NOT use Adapter when:
*   **You OWN the code**. If the "Legacy" class is yours, just Refactor it to match the new interface. Don't wrap garbage; fix it.
*   The interfaces are nearly identical.

### ✅ Acceptable use cases:
*   Integrating 3rd-party Binary SDKs (you cannot change source).
*   Supporting multiple backends (DirectX vs OpenGL) behind a common interface (Renderer).
*   Testing (Adapting a complex system to a Mock interface).

---

## 🔹 6. Senior-Level Interview Answer

> "I use Adapter primarily when integrating 3rd-party libraries or legacy C code. I almost always use the **Object Adapter** (Composition) because it's more flexible and doesn't require **Private Multiple Inheritance**, which is messy in C++. It allows me to keep my core domain logic pure and modern, isolating the ugly type-conversions in the Adapter layer."

---

## 🔹 7. Interview Question Bank

### Q1. Object Adapter vs Class Adapter?
**Answer**: "**Object Adapter** uses Composition (holds the instance). **Class Adapter** uses Multiple Inheritance (inherits from both Interface and Implementation). Object Adapter is generally preferred in C++ to avoid diamond problems and tight coupling."

### Q2. Can Adapter change behavior?
**Answer**: "No. If it changes *behavior*, it's a **Decorator** or **Proxy**. Adapter only changes the *Interface* (how you call it)."

### Q3. What is a Two-Way Adapter?
**Answer**: "An adapter that implements TWO interfaces, allowing the object to assume the guise of either system depending on who is calling it."
