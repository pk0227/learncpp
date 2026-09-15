# 5. Dependency Inversion Principle (DIP)

> *"High-level modules should not depend on low-level modules. Both should depend on abstractions.*  
> *Abstractions should not depend on details. Details should depend on abstractions."*  
> — **Robert C. Martin (Uncle Bob)**

---

## 📑 Table of Contents

1. [Architectural Definition: Inverting the Dependency Arrow](#1-architectural-definition-inverting-the-dependency-arrow)
2. [Disambiguation: DIP vs DI vs IoC](#2-disambiguation-dip-vs-di-vs-ioc)
3. [C++ Ownership Semantics in Dependency Injection](#3-c-ownership-semantics-in-dependency-injection)
   - [Exclusive Ownership via std::unique_ptr](#exclusive-ownership-via-stduniqueptr)
   - [Borrowed Non-Owning References (`T&`)](#borrowed-non-owning-references-t)
   - [Shared Ownership via std::shared_ptr: The Performance Cost](#shared-ownership-via-stdsharedptr-the-performance-cost)
4. [Compile-Time DIP: Zero-Cost Static Inversion](#4-compile-time-dip-zero-cost-static-inversion)
5. [The Composition Root Pattern](#5-the-composition-root-pattern)
6. [Anti-Patterns in C++ Dependency Management](#6-anti-patterns-in-c-dependency-management)
   - [The Service Locator Anti-Pattern](#the-service-locator-anti-pattern)
   - [Ambient Context & Global Singletons](#ambient-context--global-singletons)
7. [Hermetic Unit Testing & Test Doubles](#7-hermetic-unit-testing--test-doubles)
8. [📁 Code Examples for Section 5](#8--code-examples-for-section-5)

---

## 1. Architectural Definition: Inverting the Dependency Arrow

In traditional procedural or naive object-oriented systems, source code dependencies follow the flow of control:
- High-level business rules call low-level database libraries.
- Therefore, the high-level business module directly `#include`s and depends on the low-level database module.

```
TRADITIONAL COUPLING (Violates DIP):
[OrderService.h] ──#include──> [PostgresDriver.h]
(Flow of Control: OrderService ──> PostgresDriver)
(Source Dependency: OrderService ──> PostgresDriver)  <-- TIGHT COUPLING!
```

Under the **Dependency Inversion Principle**, the source code dependency arrow is **inverted 180 degrees** relative to the runtime flow of control:

```
INVERTED ARCHITECTURE (Adheres to DIP):
[OrderService.h] ──#include──> [IDatabase.h] <──#include── [PostgresDriver.h]
(Flow of Control: OrderService ──> IDatabase ──> PostgresDriver)
(Source Dependency: OrderService ──> IDatabase <── PostgresDriver)  <-- INVERTED!
```

> [!IMPORTANT]
> The abstraction `IDatabase` belongs to the **high-level module's conceptual boundary**, not the low-level utility. The database conforms to what the business needs, not vice versa.

---

## 2. Disambiguation: DIP vs DI vs IoC

Staff/Senior interviewers regularly test candidates on the distinction between these three terms:

| Concept | Classification | Definition |
|:---|:---|:---|
| **DIP** | **Design Principle** | The rule that high-level policies must depend on abstract contracts rather than concrete details. |
| **DI** | **Design Pattern / Technique** | The tactical mechanism of supplying dependencies from outside (via constructor or method parameter) instead of instantiating them internally (`new MySql()`). |
| **IoC** | **Architectural Paradigm** | Inverting the execution flow so that a framework or lifecycle container calls user code (The "Hollywood Principle": *Don't call us, we'll call you*). |

---

## 3. C++ Ownership Semantics in Dependency Injection

In managed languages (Java/C#), the garbage collector masks object lifetime. In **C++**, the senior engineer must explicitly declare **ownership semantics** when injecting dependencies:

### Exclusive Ownership via std::unique_ptr
Used when the consuming service owns the private, unshared lifecycle of the dependency:
```cpp
class TradingGateway {
private:
    std::unique_ptr<ITransport> transport_;
public:
    explicit TradingGateway(std::unique_ptr<ITransport> t) noexcept
        : transport_(std::move(t)) {}
};
```

### Borrowed Non-Owning References (`T&`)
The preferred and most idiomatic C++ approach for shared infrastructure (such as thread pools, database connection pools, or logging sinks):
```cpp
class OrderService {
private:
    IDatabase& db_; // Non-owning reference
public:
    explicit OrderService(IDatabase& db) noexcept : db_(db) {}
};
```
> [!NOTE]
> When using `T&`, the dependency must strictly outlive the consumer. This is guaranteed by initializing dependencies higher up in the call stack (e.g., in `main()` or the Composition Root).

### Shared Ownership via std::shared_ptr: The Performance Cost
Avoid defaulting to `std::shared_ptr<T>` for dependency injection:
- Every copy of a `std::shared_ptr` executes atomic instructions (`lock xadd` on x86) to increment/decrement the reference count.
- In multi-threaded systems, cache bouncing on the control block reference count degrades throughput.
- Use `std::shared_ptr` **only** when multiple asynchronous threads truly hold independent, nondeterministic ownership of a resource.

---

## 4. Compile-Time DIP: Zero-Cost Static Inversion

In ultra-low-latency C++ (e.g., HFT, kernel bypass, audio DSP), dynamic polymorphism via virtual functions cannot be used. We achieve DIP at **compile time** using C++20 Concepts:

```cpp
template <typename T>
concept RiskCheckPolicy = requires(T r, const Order& o) {
    { r.verifyLimit(o) } -> std::same_as<bool>;
};

// High-level engine depends ONLY on the concept abstraction!
template <RiskCheckPolicy RiskPolicy>
class ExecutionPipeline {
private:
    RiskPolicy riskPolicy_;
public:
    ExecutionPipeline(RiskPolicy p) : riskPolicy_(std::move(p)) {}
    void submit(const Order& order) {
        if (!riskPolicy_.verifyLimit(order)) return; // 100% inlined static call!
    }
};
```

---

## 5. The Composition Root Pattern

A common anti-pattern is scattering `std::make_unique` or factory invocations throughout the application codebase.

The **Composition Root** is the single, centralized location in the application where the entire dependency graph is constructed at startup:

```cpp
int main() {
    // === COMPOSITION ROOT ===
    PostgresRepository database("postgresql://db-primary:5432");
    AwsSesMailer mailer("us-east-1");
    
    // Wire dependencies into high-level business services
    OrderProcessingService orderService(database, mailer);

    // Hand control over to application runtime
    Application app(orderService);
    return app.run();
}
```

---

## 6. Anti-Patterns in C++ Dependency Management

### The Service Locator Anti-Pattern
Instead of injecting dependencies explicitly via constructors, classes query a global registry:
```cpp
// ❌ ANTI-PATTERN: Service Locator
void OrderService::execute() {
    auto db = ServiceLocator::resolve<IDatabase>(); // Hidden dependency!
    db->query();
}
```
*Why it fails*:
1. **API Dishonesty**: The class signature hides its dependencies. Looking at `OrderService::OrderService()` does not reveal that it requires `IDatabase`.
2. **Runtime Fragility**: Missing registrations crash at runtime instead of failing at compile time.
3. **Hidden Temporal Coupling**: Dependencies must be registered in a specific global order before any service is instantiated.

### Ambient Context & Global Singletons
Relying on global singletons (`Database::getInstance()`) creates invisible coupling and makes parallel unit testing impossible due to shared mutable state.

---

## 7. Hermetic Unit Testing & Test Doubles

Dependency Inversion transforms testing from an integration ordeal into blazing-fast **hermetic unit tests**:
- **Stub**: Supplies canned responses to method calls.
- **Mock**: Records method invocations and validates call counts and parameter invariants.
- **Fake**: Provides a lightweight in-memory working implementation (e.g., an in-memory `std::unordered_map` database).

Because `OrderProcessingService` depends only on `IDatabaseRepository`, we test it with an in-memory `MockDatabase` with **zero network connections, zero Docker containers, and execution times under 50 microseconds**.

---

## 8. 📁 Code Examples for Section 5

- [`Code/01_dip_violation_hardwired_deps.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/05_Dependency_Inversion_Principle/Code/01_dip_violation_hardwired_deps.cpp): High-level business service hard-coding MySQL and SMTP details.
- [`Code/02_dip_runtime_constructor_injection.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/05_Dependency_Inversion_Principle/Code/02_dip_runtime_constructor_injection.cpp): Clean runtime constructor injection with abstract interfaces, ownership clarity, and hermetic mock testing.
- [`Code/03_dip_compile_time_templates.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/05_Dependency_Inversion_Principle/Code/03_dip_compile_time_templates.cpp): Zero-cost compile-time dependency inversion using C++20 Concepts and inlined static dispatch.
- [`Code/04_dip_abstract_factory_ioc.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/05_Dependency_Inversion_Principle/Code/04_dip_abstract_factory_ioc.cpp): Decoupled component creation via Abstract Factory and the Composition Root pattern.
