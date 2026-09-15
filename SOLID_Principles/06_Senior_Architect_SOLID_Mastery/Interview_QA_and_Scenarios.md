# Senior & Staff C++ SOLID Interview Playbook (8–10+ YoE)

> *"Senior engineers know how to apply design patterns and principles. Staff engineers know when to break them."*

---

## 📑 Table of Contents

1. [The Senior Interview Mindset: What Interviewers Evaluate](#1-the-senior-interview-mindset-what-interviewers-evaluate)
2. [Category 1: Core Principles & Architectural Strategy (Q1–Q5)](#category-1-core-principles--architectural-strategy-q1q5)
3. [Category 2: Single Responsibility & Physical Design (Q6–Q9)](#category-2-single-responsibility--physical-design-q6q9)
4. [Category 3: Open/Closed & Polymorphic Dispatch (Q10–Q13)](#category-3-openclosed--polymorphic-dispatch-q10q13)
5. [Category 4: Liskov Substitution & Contract Verification (Q14–Q17)](#category-4-liskov-substitution--contract-verification-q14q17)
6. [Category 5: Interface Segregation & Physical Coupling (Q18–Q21)](#category-5-interface-segregation--physical-coupling-q18q21)
7. [Category 6: Dependency Inversion & Ownership Semantics (Q22–Q25)](#category-6-dependency-inversion--ownership-semantics-q22q25)
8. [Senior System Design Scenarios with SOLID](#8-senior-system-design-scenarios-with-solid)
   - [Scenario 1: Low-Latency Multi-Exchange Trading Gateway](#scenario-1-low-latency-multi-exchange-trading-gateway)
   - [Scenario 2: High-Throughput Distributed Telemetry Engine](#scenario-2-high-throughput-distributed-telemetry-engine)
   - [Scenario 3: Refactoring a 2-Million-Line Legacy C++ Monolith](#scenario-3-refactoring-a-2-million-line-legacy-c-monolith)

---

## 1. The Senior Interview Mindset: What Interviewers Evaluate

When interviewing for Senior, Lead, or Staff C++ positions (8–10+ years of experience), interviewers do not ask you to recite textbook definitions. They evaluate:
- **Architectural Trade-Offs**: Can you articulate the exact cost of an abstraction in nanoseconds, cache lines, binary size, and compilation time?
- **Micro-Architectural Awareness**: Do you understand how virtual dispatch affects instruction caches (L1i), branch target buffers (BTB), and data prefetchers?
- **Pragmatism Over Dogmatism**: Do you know when strict adherence to SOLID causes harmful over-engineering ("Class Explosion")?
- **Physical Design Mastery**: Can you manage build times, header pollution, and ABI stability in large-scale multi-million-line repositories?

---

## Category 1: Core Principles & Architectural Strategy (Q1–Q5)

### Q1: How do you apply OCP and DIP in systems where virtual functions are strictly prohibited?
**Senior Answer**:
In low-latency or embedded systems, we apply OCP and DIP at **compile time** using **C++20 Concepts**, **CRTP (Curiously Recurring Template Pattern)**, and **Policy-Based Design**.
- Under compile-time DIP, the high-level orchestration engine is a class template parameterized by types constrained by C++20 Concepts (e.g., `template <RiskPolicy R, TransportPolicy T> class Gateway`).
- Under compile-time OCP, new exchange drivers or risk rules are added by defining new structs conforming to the concept without touching the engine.
- This provides 100% of the architectural decoupling of DIP/OCP while compiling down to direct inlined instructions with **zero vtable memory overhead and zero indirect branch penalties**.

### Q2: Why is inheritance often considered an anti-pattern in modern C++, and how does that affect SOLID?
**Senior Answer**:
Classical implementation inheritance couples derived classes to base class internal state, causes the **Fragile Base Class Problem**, complicates move semantics, introduces object slicing risks, and forces heap allocation when storing polymorphic objects.
Modern C++ favors:
1. **Value Semantics**: Storing concrete value types contiguously in memory.
2. **Composition & Policy-Based Templates**: Composing orthogonal behaviors at compile time.
3. **Concepts (Structural Typing)**: Decoupling interfaces from class hierarchies.
Under this shift, LSP becomes compile-time concept checking, and OCP is achieved via templates or `std::variant` visitors.

### Q3: What is the "Expression Problem", and how does Modern C++ address it?
**Senior Answer**:
The Expression Problem is the dual challenge of extending a system by adding **new types** versus adding **new operations**:
- **Classical OOP (Interfaces/Vtables)**: Adding new types is trivial (derive a new class). Adding new operations requires modifying the base interface, forcing recompilation of all derived classes.
- **Modern C++ `std::variant` + `std::visit`**: Adding new operations is trivial (write a new visitor struct). Adding new types requires expanding the `std::variant` definition.
In C++, senior architects use OOP for open, dynamic plugin architectures, and `std::variant` for closed, performance-sensitive domain sets (e.g., AST nodes, network message packets).

### Q4: How do the 5 SOLID principles interact when refactoring legacy code?
**Senior Answer**:
Refactoring typically follows an outward-in sequence:
1. **SRP**: Identify the God Object and decompose it into cohesive domain models and isolated operations.
2. **ISP**: Segregate the monolithic class's public methods into client-specific role interfaces.
3. **DIP**: Invert the dependency arrow so callers depend on those newly segregated interfaces.
4. **LSP**: Ensure existing derived classes or replacements fulfill contract invariants.
5. **OCP**: The codebase is now in an open/closed state where future features are added as new plugins rather than edits to existing code.

### Q5: When does applying SOLID become an anti-pattern?
**Senior Answer**:
When applied dogmatically to trivial problems, SOLID results in **Speculative Generality** and **Nano-Class Explosion** (e.g., creating 8 interfaces, 8 classes, and a factory for a simple 20-line utility).
Senior architects apply the **Rule of Three**: Build the simplest concrete working code first. Introduce abstractions only when multiple concrete implementations or independent axes of change are genuinely proven to exist.

---

## Category 2: Single Responsibility & Physical Design (Q6–Q9)

### Q6: How do you quantitatively measure whether a class satisfies SRP?
**Senior Answer**:
Cohesion can be measured quantitatively using the **Henderson-Sellers LCOM (Lack of Cohesion in Methods) metric**:
$$LCOM^* = \frac{\left(\frac{1}{a} \sum_{j=1}^a \mu(A_j)\right) - m}{1 - m}$$
Where $m$ is the number of methods, $a$ is the number of attributes, and $\mu(A_j)$ is the count of methods accessing attribute $A_j$. An $LCOM^*$ value approaching $1.0$ proves mathematically that methods operate on disjoint subsets of state, indicating the class must be split.

### Q7: What is the physical design consequence of violating SRP in a large C++ monorepo?
**Senior Answer**:
Violating SRP binds unrelated header dependencies together. For example, if `UserManager.h` handles business rules, SQL queries, and JSON encoding, it must include database drivers and JSON libraries. Every client including `UserManager.h` transitively includes those heavy headers. A minor database schema tweak forces a multi-hour rebuild cascade across the entire corporate build farm.

### Q8: What is the Single Level of Abstraction Principle (SLAP), and how does it relate to SRP?
**Senior Answer**:
SLAP states that all statements within a function or class should operate at the same conceptual tier. Violating SLAP (e.g., mixing high-level business orchestration like `ValidateRisk()` with raw low-level memory operations like `memcpy(&socket_buf, ...)` inside the same method) is a micro-level SRP violation that destroys code readability and testability.

### Q9: How do you differentiate between an "Anemic Domain Model" and proper SRP?
**Senior Answer**:
An Anemic Domain Model separates all data (dumb structs with getters/setters) from all logic (stateless services). While this may appear to follow SRP, it often destroys **data encapsulation and invariant enforcement**. Proper SRP keeps **intrinsic business invariants** inside the entity (e.g., `Order::addItem()` enforces positive price and quantity), while delegating **extrinsic infrastructure operations** (persistence, serialization, network delivery) to separate services.

---

## Category 3: Open/Closed & Polymorphic Dispatch (Q10–Q13)

### Q10: Explain Herb Sutter's Non-Virtual Interface (NVI) Idiom and why it is superior to public virtual functions.
**Senior Answer**:
In NVI, the base class exposes `public non-virtual` methods and `private virtual` customization hooks.
- The public non-virtual method acts as the **invariant gatekeeper**: it checks preconditions, acquires synchronization locks, logs metrics, calls the private virtual hook, and verifies postconditions.
- Derived classes customize implementation details via the private hook, but they cannot accidentally bypass locks, forget validations, or alter the public API contract.

### Q11: Why are `protected` member variables considered a violation of OCP and encapsulation?
**Senior Answer**:
`protected` variables expose implementation state directly to an infinite number of future derived classes across multiple repositories. If the base class needs to change an internal data structure (e.g., from `std::vector` to `std::deque` for performance), all derived classes break. Protected data is essentially global state bounded only by inheritance. State must always be `private`, with controlled `protected` accessors if customization is required.

### Q12: How does the Fragile Base Class Problem threaten ABI stability in shared C++ libraries?
**Senior Answer**:
In shared libraries (`.so` / `.dll`), adding a new virtual function or a new data member to a base class modifies the `vtable` layout and changes `sizeof(Base)`. If third-party client binaries were compiled against the old header, they will index into the wrong vtable slot or suffer stack/heap buffer overruns at runtime. To prevent this, library architects use the **Pimpl Idiom** to create a physical compilation firewall that maintains strict binary compatibility.

### Q13: What is the performance difference between a virtual dispatch and a CRTP static dispatch in a tight loop?
**Senior Answer**:
- **Virtual Dispatch**: Incurs an indirect load (dereferencing `vptr` -> `vtable`), a dependent branch that can cause a Branch Target Buffer (BTB) miss (~15–20 CPU cycles penalty), and completely inhibits the compiler from inlining the function. In a tight loop of $10^7$ iterations, this can cause a $5\times$ to $10\times$ performance drop.
- **CRTP Static Dispatch**: The compiler knows the exact derived type at compile time. The call is 100% inlined into the caller loop, allowing constant propagation, dead-code elimination, and auto-vectorization (SIMD).

---

## Category 4: Liskov Substitution & Contract Verification (Q14–Q17)

### Q14: Why does `Square` deriving from `Rectangle` violate LSP, and what is the underlying design error?
**Senior Answer**:
In geometry, a square is a rectangle with equal sides. But in OOP, types are defined by their **behavioral invariants under mutation**. A `Rectangle` promises that `setWidth(w)` changes width without mutating height. A `Square` must enforce `width == height`, so mutating width has the side effect of mutating height. Any client written against the `Rectangle` contract expecting independent dimension modification fails when passed a `Square`. The error was modeling static taxonomy instead of behavioral contracts.

### Q15: Why is `std::vector<bool>` widely cited as a failure of LSP in the C++ Standard Library?
**Senior Answer**:
For every standard container `std::vector<T>`, the expression `vec[i]` returns `T&` (a real lvalue reference). However, `std::vector<bool>` is specialized to pack 8 booleans into a single byte. Because individual bits cannot be directly addressed in C++, `vec[i]` returns a temporary proxy object (`std::vector<bool>::reference`). Any generic template function that expects `auto& elem = vec[0];` fails to compile with `std::vector<bool>`, violating behavioral substitutability.

### Q16: Explain the rules of Precondition Contravariance and Postcondition Covariance in Design by Contract.
**Senior Answer**:
- **Preconditions (Contravariant / Weaker)**: A derived class can only require less from its caller, never more. If base accepts any integer, derived cannot reject negative numbers.
- **Postconditions (Covariant / Stronger)**: A derived class can only promise more to its caller, never less. If base promises to return a valid non-null pointer, derived cannot return `nullptr`.

### Q17: How does C++20 enable compile-time verification of LSP?
**Senior Answer**:
Through **C++20 Concepts**, we can statically assert semantic and syntactic requirements before runtime:
```cpp
template <typename T>
concept AccountContract = requires(T a, double amt) {
    { a.withdraw(amt) } noexcept -> std::same_as<bool>;
    { a.getBalance() } noexcept -> std::convertible_to<double>;
};
```
If a substitute class fails to satisfy the `noexcept` specification or return type constraint, compilation halts immediately with an explicit compiler diagnostic.

---

## Category 5: Interface Segregation & Physical Coupling (Q18–Q21)

### Q18: What are the symptoms of an ISP violation in a C++ codebase?
**Senior Answer**:
1. Derived classes contain empty method bodies or throw `std::logic_error("Not implemented")`.
2. Clients must `#include` massive header files containing dozens of data structures they never touch.
3. Classes frequently recompile when completely unrelated features in the shared interface are modified.
4. Vtables contain excessive entries, leading to binary bloat and preventing the linker from removing dead code.

### Q19: Why does Multiple Inheritance in C++ not suffer from the Diamond Problem when applying ISP?
**Senior Answer**:
The Diamond Problem (dreaded diamond) only occurs when a class inherits multiple copies of **data members / state** from a common ancestor without virtual base classes.
Under ISP, role interfaces are **Pure Abstract Classes** consisting strictly of pure virtual functions (`= 0`) and zero data members. Inheriting multiple pure interfaces generates zero state ambiguity and zero memory overhead.

### Q20: How does the Adapter Pattern resolve ISP violations when dealing with third-party legacy APIs?
**Senior Answer**:
When a third-party vendor library supplies a monolithic "God Interface" that cannot be changed, client subsystems should define granular **Role Interfaces** representing only the operations they require. An **Adapter class** wraps the vendor object and implements the role interface, creating an architectural insulation layer that shields application code from vendor bloat.

### Q21: Explain how the C++20 `<ranges>` library applies ISP to iterator categories.
**Senior Answer**:
The STL does not define a single bloated `Iterator` interface. Instead, it segregates iterator capabilities into fine-grained concepts: `input_iterator`, `forward_iterator`, `bidirectional_iterator`, `random_access_iterator`, and `contiguous_iterator`. Algorithms demand only the minimal concept required (e.g., `std::ranges::distance` works on `input_range`, while `std::ranges::sort` requires `random_access_range`), ensuring maximum genericity and compile-time correctness.

---

## Category 6: Dependency Inversion & Ownership Semantics (Q22–Q25)

### Q22: What is the difference between DIP, Dependency Injection (DI), and Inversion of Control (IoC)?
**Senior Answer**:
- **DIP**: The foundational architecture principle stating that high-level policy must depend on abstract interfaces, not concrete implementations.
- **DI**: The design pattern/technique of passing dependencies into a class (e.g., via constructor) rather than having the class instantiate them directly.
- **IoC**: The architectural pattern where execution flow is managed by an external framework (e.g., event loop, application container) rather than by custom application code.

### Q23: When should you inject dependencies via `std::unique_ptr`, `std::shared_ptr`, or `T&` in C++?
**Senior Answer**:
- **`T&` (Non-Owning Reference)**: Default choice for long-lived infrastructure dependencies (connection pools, loggers, thread pools) whose lifecycles strictly outlive the consuming service. Zero runtime memory overhead.
- **`std::unique_ptr<T>`**: Used when the consuming service requires **sole, exclusive ownership** of the dependency's lifecycle.
- **`std::shared_ptr<T>`**: Used **only** when multiple asynchronous threads truly share non-deterministic ownership. Avoid as a general DI default because atomic reference-counting increments/decrements introduce multi-threaded cache contention.

### Q24: Why is the "Service Locator" pattern widely classified as an anti-pattern compared to Constructor Injection?
**Senior Answer**:
The Service Locator hides dependencies inside method implementations (`ServiceLocator::get<IDatabase>()`), producing:
1. **API Dishonesty**: Looking at the constructor does not reveal what dependencies the class actually needs.
2. **Runtime Failures**: Missing registrations fail at runtime instead of compile time.
3. **Hidden Temporal Coupling**: Dependencies must be pre-loaded into the locator in a strict order before any service is instantiated.
4. **Testing Obstacles**: Parallel unit tests can interfere with each other through the shared global locator state.

### Q25: What is the "Composition Root", and where should it reside?
**Senior Answer**:
The Composition Root is the unique location in an application where the entire dependency graph is composed and wired together. It should reside as close to the application entry point as possible (typically inside `main()` or an application bootstrap module). Keeping composition centralized ensures that business logic classes remain completely decoupled from concrete factory and instantiation code.

---

## 8. Senior System Design Scenarios with SOLID

### Scenario 1: Low-Latency Multi-Exchange Trading Gateway
- **Requirement**: Build an order gateway routing orders to NASDAQ (OUCH binary protocol) and CME (iLink3 SBE protocol) under a 250ns latency budget.
- **SOLID Architecture**:
  - **SRP**: Split parsing, risk validation, sequence numbering, and network transmission into separate classes.
  - **OCP & DIP**: Define `ExchangeEncoder` and `RiskPolicy` as **C++20 Concepts**. The gateway is a template parameterized by these policies.
  - **ISP**: Separate market data feeds from order entry interfaces.
  - **LSP**: Concept constraints enforce `noexcept` wire encoding and cache-aligned memory layouts.
  - **Result**: Zero heap allocation, zero vtables, 100% inlined machine code.

### Scenario 2: High-Throughput Distributed Telemetry Engine
- **Requirement**: Capture millions of events per second and route them concurrently to Console, Local Disk (NVMe), and Kafka.
- **SOLID Architecture**:
  - **SRP**: `TelemetryRecord` (state) $\rightarrow$ `IEventFormatter` (formatting) $\rightarrow$ `IEventSink` (I/O) $\rightarrow$ `TelemetryPipeline` (queueing/concurrency).
  - **OCP**: Adding a Datadog or OpenTelemetry sink requires implementing `IEventSink` without altering the background worker loop.
  - **LSP**: Sinks guarantee non-blocking or graceful fallback error handling, preventing worker thread termination.
  - **ISP**: Segregate `IEventSink` from `IFlushable` and `IHealthCheckable`.
  - **DIP**: The pipeline receives sinks via `std::vector<std::unique_ptr<IEventSink>>` in the Composition Root.

### Scenario 3: Refactoring a 2-Million-Line Legacy C++ Monolith
- **Problem**: 45-minute clean build times; modifying a billing function breaks inventory tracking; unit testing requires live Oracle DB.
- **Refactoring Roadmap**:
  1. **Introduce Compilation Firewalls (Pimpl)**: Isolate private data members in heavy legacy headers to halt recompilation cascades.
  2. **Extract Role Interfaces (ISP)**: Define pure abstract interfaces for database and network operations.
  3. **Constructor Injection (DIP)**: Replace hardcoded `OracleConnection::getInstance()` calls with injected interface references.
  4. **Build Hermetic Test Doubles**: Create in-memory mock repositories, enabling unit test suites to run in under 2 seconds.
  5. **Decompose Monolith (SRP)**: Extract domain entities as pure value types, decoupling them from SQL and formatting code.
