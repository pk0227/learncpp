# 1. Single Responsibility Principle (SRP)

> *"A module should have one, and only one, reason to change."*  
> — **Robert C. Martin (Uncle Bob)**

---

## 📑 Table of Contents

1. [Architectural Definition & Senior Mental Model](#1-architectural-definition--senior-mental-model)
2. [Quantitative Cohesion: The LCOM Metric](#2-quantitative-cohesion-the-lcom-metric)
3. [The Single Level of Abstraction Principle (SLAP)](#3-the-single-level-of-abstraction-principle-slap)
4. [Physical Design & Compilation Cascades in C++](#4-physical-design--compilation-cascades-in-c)
5. [Anti-Patterns & Architectural Smells](#5-anti-patterns--architectural-smells)
   - [The God Object (Swiss Army Knife)](#the-god-object-swiss-army-knife)
   - [The Over-Fragmented "Class Explosion" Trap](#the-over-fragmented-class-explosion-trap)
   - [Anemic Domain Model vs Feature Envy](#anemic-domain-model-vs-feature-envy)
6. [Refactoring Strategies: Runtime vs Compile-Time](#6-refactoring-strategies-runtime-vs-compile-time)
   - [Strategy A: Domain Entity + Service Decomposition](#strategy-a-domain-entity--service-decomposition)
   - [Strategy B: Compile-Time Policy-Based Design (Alexandrescu)](#strategy-b-compile-time-policy-based-design-alexandrescu)
7. [📁 Code Examples for Section 1](#7--code-examples-for-section-1)

---

## 1. Architectural Definition & Senior Mental Model

At junior and intermediate levels, developers frequently mistake **Single Responsibility** for *"a class should do only one thing"* (confusing SRP with functional decomposition).

In senior software architecture, the definition is anchored in **actors and sources of change**:
- A **responsibility** is an axis of change.
- A **reason to change** corresponds to a specific **business stakeholder or operational actor** (e.g., DBA, Chief Risk Officer, UI/UX team, DevOps engineer).

```
┌─────────────────────────────────────────────────────────────┐
│                 Monolithic God Class (SMELL)                │
│                                                             │
│   ┌────────────────┐ ┌───────────────┐ ┌────────────────┐  │
│   │ Tax & Pricing  │ │ SQL Database  │ │ JSON & Wire    │  │
│   │ (Finance Team) │ │ (DBA Team)    │ │ (Frontend/API) │  │
│   └────────────────┘ └───────────────┘ └────────────────┘  │
│                      ┌───────────────┐                      │
│                      │ SMTP Alerts   │                      │
│                      │ (DevOps/SRE)  │                      │
│                      └───────────────┘                      │
└─────────────────────────────────────────────────────────────┘
                               ▲
   4 distinct actors force modifications to the same file!
```

> [!IMPORTANT]
> If a class requires modifications when the database schema changes, **AND** when the financial discounting logic changes, **AND** when the API serialization changes, the class violates SRP. It binds multiple unrelated business actors to the same source code artifact.

---

## 2. Quantitative Cohesion: The LCOM Metric

Senior engineers must be able to quantify design quality rather than relying on subjective intuition. In software engineering metrics, cohesion is measured by **LCOM (Lack of Cohesion in Methods)**, originally formalized by Chidamber & Kemerer and later refined by Henderson-Sellers ($LCOM^*$).

Given:
- $m$: Number of member functions (methods) in class $C$.
- $a$: Number of member attributes (fields) in class $C$.
- $\mu(A_j)$: Number of methods in $C$ that access attribute $A_j$.

The Henderson-Sellers metric is calculated as:

$$LCOM^* = \frac{\left(\frac{1}{a} \sum_{j=1}^a \mu(A_j)\right) - m}{1 - m}$$

### Interpreting $LCOM^*$:
- **$LCOM^* = 0$ (Perfect Cohesion)**: Every member function accesses every member attribute. The class represents a unified, indivisible concept.
- **$0 < LCOM^* \le 0.5$ (High Cohesion)**: Healthy distribution of method access across state variables.
- **$LCOM^* \to 1.0$ (Severe Lack of Cohesion)**: Methods operate on completely disjoint subsets of attributes. This proves that the class is a **compound monolith** that must be split into two or more independent classes.

---

## 3. The Single Level of Abstraction Principle (SLAP)

SRP applies not only to classes but also to methods and modules through **SLAP**. A function or class should operate at a uniform level of conceptual abstraction.

```cpp
// ❌ SLAP Violation: Mixing high-level business orchestration with raw byte packing
void ProcessTrade(const Trade& trade) {
    ValidateRiskLimits(trade); // High-level abstraction

    // LOW-LEVEL LEAK: Directly bit-manipulating socket buffer inside business logic!
    uint32_t net_id = htonl(trade.id);
    memcpy(raw_buffer_ + offset_, &net_id, sizeof(net_id));
    offset_ += sizeof(net_id);

    NotifyCompliance(trade);   // High-level abstraction
}
```

By adhering to SLAP, we isolate raw system primitives (I/O, bitwise layout, memory management) from high-level orchestration (business rules, workflows).

---

## 4. Physical Design & Compilation Cascades in C++

In languages like Java or C#, a class violation of SRP primarily impacts logical cohesion. In **C++**, violating SRP directly degrades **physical architecture, build times, and ABI stability** (as formalized by John Lakos in *Large-Scale C++ Software Design*).

### The Blast Radius of Monolithic Headers
When an `OrderManager` mixes business calculations with database persistence and network I/O:
1. `OrderManager.h` must include `<pqxx/pqxx>` (PostgreSQL driver), `<boost/asio.hpp>` (Networking), and `<nlohmann/json.hpp>`.
2. Any downstream translation unit (`.cpp` file) that only needs to calculate a discount must transitively include all those massive vendor headers.
3. A single internal schema tweak or vendor header update triggers a multi-minute **recompilation cascade** across the entire build farm.

```
       [OrderManager.h] (Violates SRP)
       ├── <pqxx/pqxx>           (PostgreSQL client)
       ├── <boost/asio.hpp>      (Sockets / Network)
       └── <nlohmann/json.hpp>   (JSON parser)
            ▲              ▲              ▲
            │              │              │
      [Billing.cpp]  [Pricing.cpp]  [Audit.cpp]
    (Forced to recompile whenever DB driver or Boost changes!)
```

> [!TIP]
> By splitting `OrderManager` into separate headers (`Order.h`, `PricingEngine.h`, `OrderRepository.h`), modules that perform pricing calculations only include `Order.h` and `PricingEngine.h`, reducing include depth and build times by up to **80%**.

---

## 5. Anti-Patterns & Architectural Smells

### The God Object (Swiss Army Knife)
- **Symptom**: A central manager class (e.g., `SessionManager`, `SystemContext`, `DeviceController`) spanning thousands of lines with dozens of disparate methods.
- **Root Cause**: Laziness in finding the correct home for new functionality; developers treat the class as a grab-bag.
- **C++ Consequence**: Heavy contention in version control, impossible-to-mock dependencies, and hidden state mutations.

### The Over-Fragmented "Class Explosion" Trap
- **Symptom**: Creating a new class for every single function (e.g., `OrderPriceCalculator`, `OrderTaxCalculator`, `OrderDiscountCalculator`, `OrderSubtotalCalculator`).
- **Architectural Risk**: Premature micro-abstractions introduce cognitive overload, excessive object allocations, and anemic designs where data and operations are artificially separated.
- **Rule of Thumb**: Group operations that change for the **same business actor** together. A single `PricingEngine` handling subtotal, tax, and discount is cohesive because all three are governed by the Finance actor.

### Anemic Domain Model vs Feature Envy
- **Anemic Domain Model**: A class containing only private getters/setters with zero business invariants, while external services manipulate its internals directly.
- **Feature Envy**: A service method that constantly calls getters on another object to perform a calculation that belongs inside the entity itself.

---

## 6. Refactoring Strategies: Runtime vs Compile-Time

### Strategy A: Domain Entity + Service Decomposition
When runtime flexibility and dependency injection are needed:
1. Extract the **Domain Entity** (`Order`) containing pure state and intrinsic invariants.
2. Extract **Pure Domain Services** (`PricingEngine`) as stateless calculators.
3. Extract **Infrastructure Adapters** (`OrderRepository`, `OrderJsonSerializer`, `EmailNotificationService`).

```cpp
// Focused, cohesive, isolated responsibilities
class PricingEngine {
public:
    struct Breakdown { double subtotal; double tax; double total; };
    [[nodiscard]] Breakdown calculate(const Order& order, double taxRate) const noexcept;
};

class OrderRepository {
public:
    bool save(const Order& order); // SQL encapsulated here
};
```

### Strategy B: Compile-Time Policy-Based Design (Alexandrescu)
In performance-critical domains (HFT, robotics, embedded Linux), creating multiple virtual base classes introduces pointer indirection, cache misses, and disables compiler inlining.

We decompose responsibilities at **compile time** using template policies constrained by C++20 concepts:

```cpp
template <ValidationPolicy Validator, StoragePolicy Storage, LoggingPolicy Logger>
class MarketDataFeed : private Validator, private Storage, private Logger {
public:
    void processIncomingTick(const MarketTick& tick) {
        if (!Validator::isValid(tick)) {
            Logger::log("Tick rejected by validation policy");
            return;
        }
        Storage::store(tick);
    }
};
```

> [!NOTE]
> This pattern preserves strict Single Responsibility (each policy class has one responsibility) while compiling down to **zero runtime overhead**—no vtables, no pointer hops, and 100% compiler inlining.

---

## 7. 📁 Code Examples for Section 1

The following standalone C++20 examples provide concrete, runnable implementations of each concept:

- [`Code/01_srp_violation_god_class.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/01_Single_Responsibility_Principle/Code/01_srp_violation_god_class.cpp): Demonstrates the classic God Object anti-pattern mixing domain calculation, SQL persistence, JSON serialization, and SMTP networking.
- [`Code/02_srp_refactored_clean.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/01_Single_Responsibility_Principle/Code/02_srp_refactored_clean.cpp): Clean, production-grade decomposition into cohesive single-purpose classes using value semantics and isolated services.
- [`Code/03_srp_policy_based_design.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/01_Single_Responsibility_Principle/Code/03_srp_policy_based_design.cpp): Zero-overhead compile-time SRP using Alexandrescu's Policy-Based Design constrained by C++20 Concepts.
