# 🧠 Extensible Factory (Self-Registering / Plugin Factory)
> **Deep, Practical, Senior-Level Understanding**
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. What is the Extensible Factory? (Real Meaning)

**Standard Definition:**
"A factory that allows new product types to be registered at runtime, without modifying the factory itself."

**Senior Definition:**
*   ✅ **"Open-Closed Factory: Open for new types, Closed for modification"**
*   ✅ **"The Backbone of Plugin and DLL/SO-based Extension Systems"**
*   ✅ **"Self-Registering Types via Static Initializers"**

**The problem with regular Factory:**
```cpp
// Regular Factory — must be modified every time a new type is added!
std::unique_ptr<Shape> ShapeFactory::create(const std::string& type) {
    if (type == "circle")    return std::make_unique<Circle>();
    if (type == "rectangle") return std::make_unique<Rectangle>();
    // Adding "hexagon"? MUST edit this file. Breaks Open-Closed Principle.
}
```

**The Extensible Factory solution:**
```cpp
// Extensible Factory — NEVER needs modification for new types!
factory.registerType("hexagon", []() { return std::make_unique<Hexagon>(); });
auto hex = factory.create("hexagon");  // Works! Factory unchanged.
```

---

## 🔹 2. Modern & Correct C++ Implementation

### A. The Factory (Registry Core)

```cpp
#include <functional>
#include <unordered_map>
#include <shared_mutex>
#include <memory>

class ShapeFactory {
public:
    using Creator = std::function<std::unique_ptr<Shape>()>;

    // Thread-safe singleton access
    static ShapeFactory& instance() {
        static ShapeFactory factory;  // Magic static, thread-safe init
        return factory;
    }

    // Register a new type (called by plugins, static initializers, or tests)
    void registerType(std::string name, Creator creator) {
        std::unique_lock lock(mutex_);       // Exclusive write lock
        registry_[std::move(name)] = std::move(creator);
    }

    // Create an instance by name
    std::unique_ptr<Shape> create(const std::string& name) const {
        std::shared_lock lock(mutex_);       // Shared read lock (concurrent OK)
        auto it = registry_.find(name);
        if (it == registry_.end()) {
            throw std::runtime_error("Unknown type: " + name);
        }
        return it->second();                 // Call the registered creator
    }

    bool isRegistered(const std::string& name) const {
        std::shared_lock lock(mutex_);
        return registry_.contains(name);
    }

    std::vector<std::string> registeredTypes() const {
        std::shared_lock lock(mutex_);
        std::vector<std::string> names;
        for (const auto& [k, v] : registry_) names.push_back(k);
        return names;
    }

private:
    ShapeFactory() = default;
    std::unordered_map<std::string, Creator> registry_;
    mutable std::shared_mutex mutex_;       // Reader-writer lock
};
```

### B. Self-Registration Idiom (The Magic)

```cpp
// Helper struct: registering in the constructor = registration at static init time
struct ShapeRegistrar {
    ShapeRegistrar(std::string name, ShapeFactory::Creator creator) {
        ShapeFactory::instance().registerType(std::move(name), std::move(creator));
    }
};

// In Circle.cpp — self-registers with no main() involvement:
static ShapeRegistrar g_circleReg("circle", []() {
    return std::make_unique<Circle>();
});

// In Triangle.cpp — a plugin loaded via dlopen() can also self-register!
static ShapeRegistrar g_triangleReg("triangle", []() {
    return std::make_unique<Triangle>();
});
```

**Why this is powerful**: When a shared library (`.so` / `.dll`) is loaded at runtime via `dlopen()`, its static initializers run, causing self-registration. The factory gains new types without any recompilation.

### C. Plugin System (Runtime DLL Loading)

```cpp
// Host application
void loadPlugin(const std::string& path) {
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    // Static initializers in the .so run → new types auto-register
    // ShapeFactory::instance() now has new types!
}

// Usage after loading plugin
auto s = ShapeFactory::instance().create("hexagon"); // New type from plugin!
```

---

## 🔹 3. Pros — Deep Explanation

### ✅ Pro #1: True Open-Closed Principle
*   **The Benefit**: The factory code is **never modified** when new types are added. A plugin DLL can add 10 new types by just registering them.
*   **Contrast**: Regular Factory requires editing a `switch/if-else` chain, recompiling, and redeploying the entire application.

### ✅ Pro #2: Plugin Architecture Enablement
*   **The Benefit**: Third-party developers can add types to your system without touching your source code. They ship a `.so` file.
*   **Real-World**: This is how Photoshop plugins, game engine asset importers, and IDE language servers work.

### ✅ Pro #3: Thread-Safe by Design
*   **The Benefit**: With `std::shared_mutex`, `create()` calls (reads) are fully concurrent. Multiple threads can create objects simultaneously.
*   **Write Lock**: Only `registerType()` needs an exclusive lock — and that typically only happens at startup.

### ✅ Pro #4: Testability
*   **The Benefit**: In unit tests, you can register mock implementations: `factory.registerType("circle", []() { return std::make_unique<MockCircle>(); })`.
*   **Impact**: Replaces the need for a separate mock framework dependency.

---

## 🔹 4. Cons — Deep Explanation

### ❌ Con #1: Static Initialization Order Fiasco (SIOF)
*   **The Issue**: C++ doesn't guarantee the order of static initialization across translation units. `g_circleReg` (in Circle.cpp) might try to call `ShapeFactory::instance()` before it's constructed.
*   **Solution**: Use the **Meyers Singleton** pattern for the factory (a local static in `instance()`). Local statics are initialized on first access, not at program startup — guaranteeing the factory exists when any `Registrar` constructor runs.

### ❌ Con #2: Registration Completeness is Invisible
*   **The Issue**: If a plugin `.so` is not loaded, its types are not registered. There's no compile-time check that all expected types exist.
*   **Solution**: Add `isRegistered()` checks at startup, or use a manifest/config file to declare required plugins.

### ❌ Con #3: Symbol Visibility in Shared Libraries
*   **The Issue**: Static variables in `.so` files may be stripped by the linker if they appear "unused" (no code directly references them).
*   **Solution**: Use `__attribute__((used))` (GCC) or link with `--whole-archive`. Or expose an explicit `registerAll()` function from the plugin.

---

## 🔹 5. When to Use / Not Use

### ❌ Do NOT use Extensible Factory when:
*   The set of types is small, fixed, and never changes. Regular Abstract Factory is simpler.
*   You don't need runtime extension. Over-engineering for a 3-type system.

### ✅ Acceptable use cases:
*   **Plugin Systems**: CAD software tools, IDE extensions, game asset loaders.
*   **Serialization Frameworks**: `fromJSON({"type": "circle", ...})` — look up the type name, create the right object.
*   **Network Protocols**: Packet dispatch — map packet type IDs to handler factory functions.
*   **Dependency Injection Containers**: Register implementations for interfaces; resolve at runtime.
*   **Unit Testing**: Register mock types to replace real implementations without code changes.

---

## 🔹 6. Senior-Level Interview Answer

> "The Extensible Factory solves the fundamental weakness of regular Factory: it doesn't require modification when new types are added. I implement it as a thread-safe registry using `std::shared_mutex` — reads (creation) are concurrent, writes (registration) are exclusive. The self-registration idiom using static initializers is particularly powerful: when a shared library is `dlopen()`'d, its static initializers run, automatically populating the factory. The one pitfall I always guard against is the Static Initialization Order Fiasco — I use the Meyers Singleton for the factory itself so it's guaranteed to exist before any Registrar constructor fires."

---

## 🔹 7. Interview Question Bank

### Q1. Extensible Factory vs Abstract Factory?
**Answer**: "**Abstract Factory** hardcodes a family of related products into one factory class. Adding a new product family requires a new factory class. **Extensible Factory** uses a dynamic registry — new types are added by calling `registerType()`, not by modifying or creating factory classes. Extensible Factory is more flexible; Abstract Factory provides stronger compile-time guarantees about product families."

### Q2. What is the Static Initialization Order Fiasco and how do you prevent it?
**Answer**: "C++ initializes static objects in an undefined order across translation units. If `g_circleReg` (in Circle.cpp) calls `ShapeFactory::instance()` before `ShapeFactory` is initialized (in Factory.cpp), it's undefined behavior. The fix: implement `instance()` with a **local static** (`static ShapeFactory f; return f;`). Local statics are initialized on first call — guaranteed to exist by the time any caller accesses them."

### Q3. How does this pattern enable plugin architectures?
**Answer**: "When a plugin `.so` is loaded via `dlopen()`, all its static initializers run. If a plugin's static initializer registers new types with the factory, those types become available immediately — with zero changes to the host application. This is exactly how editor plugins (Photoshop, Blender, Unreal Engine) add new capabilities: the plugin `.dll` auto-registers its content providers."

### Q4. How do you make unregistration thread-safe?
**Answer**: "Use `std::unique_lock` for `unregisterType()` just like for `registerType()`. However, unregistration is dangerous if other threads might be mid-creation. A safer approach: never unregister. Instead, use **versioning** or **tombstone markers** in the registry, or unload plugins only during controlled shutdown."

### Q5. How do you unit test the Extensible Factory?
**Answer**: "In the test setup, call `factory.registerType('circle', []() { return std::make_unique<MockCircle>(); })`. This replaces the real `Circle` with a mock for the duration of the test. Since each test can register its own version, you get full isolation. Use RAII to clean up registrations after each test if the factory is a singleton."
