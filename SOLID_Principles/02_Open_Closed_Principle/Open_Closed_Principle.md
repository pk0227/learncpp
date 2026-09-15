# 2. Open/Closed Principle (OCP)

> *"Software entities (classes, modules, functions, etc.) should be open for extension, but closed for modification."*  
> — **Bertrand Meyer (1988) & Robert C. Martin (1996)**

---

## 📑 Table of Contents

1. [Architectural Definition: Meyer vs Martin](#1-architectural-definition-meyer-vs-martin)
2. [The Expression Problem in Modern C++](#2-the-expression-problem-in-modern-c)
3. [Runtime OCP: The Non-Virtual Interface (NVI) Idiom](#3-runtime-ocp-the-non-virtual-interface-nvi-idiom)
4. [Compile-Time OCP: Zero-Cost CRTP & Concepts](#4-compile-time-ocp-zero-cost-crtp--concepts)
5. [Closed-Set Polymorphic OCP: `std::variant` & Visitors](#5-closed-set-polymorphic-ocp-stdvariant--visitors)
6. [ABI Stability & The Fragile Base Class Problem](#6-abi-stability--the-fragile-base-class-problem)
7. [Hardware & Micro-Architectural Costs of Runtime OCP](#7-hardware--micro-architectural-costs-of-runtime-ocp)
8. [📁 Code Examples for Section 2](#8--code-examples-for-section-2)

---

## 1. Architectural Definition: Meyer vs Martin

To discuss OCP authoritatively at senior and staff levels, you must distinguish between the two historical definitions of the principle:

| Dimension | Bertrand Meyer (1988) | Robert C. Martin / Uncle Bob (1996) |
|:---|:---|:---|
| **Mechanism** | **Implementation Inheritance** | **Polymorphic Abstraction & Interfaces** |
| **Philosophy** | Once a module is published, its interface is closed. Derived classes inherit and override/augment behavior. | Modules depend on abstract interfaces. New implementations satisfy the interface without touching callers. |
| **Modern C++ Risk** | Fragile Base Class Problem, tight coupling to base implementation details, object slicing. | Vtable indirection, heap allocation, cache misses (mitigated via compile-time C++20 techniques). |

```
                       [OCP Architectural Goal]
              New Requirements / New Business Rules Arrive
                                  │
         ┌────────────────────────┴────────────────────────┐
         ▼                                                 ▼
   ❌ BAD (Violates OCP)                              ✅ GOOD (Adheres to OCP)
Edit existing .h/.cpp files                       Create new translation unit (.cpp)
Re-test existing code paths                       Link or inject new type
Risk regression in working logic                  Existing binaries unchanged
```

---

## 2. The Expression Problem in Modern C++

Every software system must balance two opposing dimensions of extension:
1. **Adding new types / entities** (e.g., adding `BitcoinPayment`, `EthereumPayment`).
2. **Adding new operations / behaviors** (e.g., adding `ExportToTaxXml()`, `AuditSecurityCompliance()`).

```
                   Types (Rows) vs Operations (Columns)
                    
                   ┌────────────────────┬────────────────────┐
                   │ CalculateFee()     │ SerializeToJson()  │
┌──────────────────┼────────────────────┼────────────────────┤
│ CreditCard       │                    │                    │
├──────────────────┼────────────────────┼────────────────────┤
│ PayPal           │                    │                    │
├──────────────────┼────────────────────┼────────────────────┤
│ CryptoWallet     │                    │                    │
└──────────────────┴────────────────────┴────────────────────┘
```

- **Classical OOP (Open-Set Polymorphism)**: Makes it easy to add **new types** (rows) by implementing the base interface. However, adding a **new operation** (columns) forces you to modify the base interface and touch every derived class!
- **Modern C++ Functional / Visitor (`std::variant`)**: Makes it easy to add **new operations** (columns) by writing new visitors. However, adding a new type (rows) requires updating the variant type list.

> [!TIP]
> Choose **Classical Interface OCP** when types are open and unknown at compile time (e.g., third-party plugins). Choose **`std::variant` Visitor OCP** when types are bounded and known (e.g., compilers, network protocol packets, financial trade types).

---

## 3. Runtime OCP: The Non-Virtual Interface (NVI) Idiom

In naive C++ object orientation, base classes declare `public virtual` member functions. Herb Sutter formalized why this is a dangerous anti-pattern: it conflates the **public contract** with the **customization hook**.

### The NVI Architecture
1. **Public Interface is Non-Virtual**: Serves as the invariant gatekeeper. It checks preconditions, acquires locks, logs performance metrics, and enforces postconditions.
2. **Virtual Methods are Private or Protected**: Derived classes customize behavior, but they are physically incapable of altering the public calling contract or skipping invariants.

```cpp
class PaymentGateway {
public:
    // Public non-virtual contract (Invariant Gatekeeper)
    void execute(const Transaction& tx) {
        if (tx.amount <= 0.0) throw std::invalid_argument("Invalid amount");
        log_audit(tx);      // Mandatory invariant
        do_execute(tx);     // Dispatch to private hook
        record_metric(tx);  // Mandatory invariant
    }
private:
    // Derived classes override implementation, NOT the contract
    virtual void do_execute(const Transaction& tx) = 0;
};
```

---

## 4. Compile-Time OCP: Zero-Cost CRTP & Concepts

In latency-critical C++ (High-Frequency Trading, game physics, network drivers), runtime virtual dispatch is unacceptable. We achieve OCP at compile time using **CRTP (Curiously Recurring Template Pattern)** and **C++20 Concepts**:

```cpp
template <typename Derived>
class PacketParserBase {
public:
    bool parse(const uint8_t* buffer, std::size_t len) {
        // Invariant checks at compile-time / inline
        if (!buffer || len == 0) return false;
        // Static dispatch: 100% inlined, 0 vtables
        return static_cast<Derived*>(this)->parse_impl(buffer, len);
    }
};

class IPv4Parser : public PacketParserBase<IPv4Parser> {
public:
    bool parse_impl(const uint8_t* buffer, std::size_t len);
};
```

### Advantages for Senior C++ Engineers:
- **Zero Vtable Overhead**: No 8-byte `vptr` per object.
- **Inlining**: The compiler inlines the derived implementation directly into the calling loop.
- **Auto-Vectorization**: The compiler can vectorize loops containing static CRTP calls (impossible across virtual call boundaries).

---

## 5. Closed-Set Polymorphic OCP: `std::variant` & Visitors

When the set of types is closed, `std::variant` combined with `std::visit` provides value-based OCP without dynamic allocation or virtual functions:

```cpp
using PaymentMethod = std::variant<CreditCard, PayPal, Crypto>;

// Adding a new operation WITHOUT modifying any of the domain types!
struct FraudCheckVisitor {
    bool operator()(const CreditCard& cc) const { return verify_cvv(cc); }
    bool operator()(const PayPal& pp)     const { return verify_oauth(pp); }
    bool operator()(const Crypto& cr)     const { return verify_blockchain(cr); }
};
```

---

## 6. ABI Stability & The Fragile Base Class Problem

In shared libraries (`.so` / `.dll`), modifying a base class to extend behavior can destroy binary compatibility:
1. **Vtable Layout Shift**: Adding a virtual function to a base class shifts the indices of existing virtual functions in the vtable.
2. **Object Size Shift**: Adding a data member changes `sizeof(Base)`, causing memory corruption in client binaries that allocate derived objects on the stack.

```
Original Vtable:
[0]: ~Base()
[1]: process()

Modified Base (Added new virtual function in middle):
[0]: ~Base()
[1]: validate()  <-- SHIFTS process() down!
[2]: process()

Unrecompiled Client invokes slot [1] expecting process(), calls validate() -> CRASH!
```

> [!WARNING]
> To maintain strict ABI compatibility under OCP:
> - Never add or reorder virtual functions in public library base classes.
> - Use the **Pimpl Idiom (Pointer to Implementation)** to firewall internal state changes.

---

## 7. Hardware & Micro-Architectural Costs of Runtime OCP

When deciding between Runtime OCP (virtual interfaces) and Compile-Time OCP (CRTP / Concepts), evaluate the micro-architectural trade-offs:

1. **Instruction Cache (i-cache) Thrashing**: Virtual calls jump across disjoint memory pages, evicting hot instructions from L1i cache.
2. **Branch Target Buffer (BTB) Misses**: Indirect calls cannot be resolved by standard static branch predictors. If an array of polymorphic objects contains alternating derived types, the CPU branch predictor fails repeatedly, causing pipeline flushes (~15–20 CPU cycles penalty per miss).
3. **Loss of Inlining & Compiler Optimizations**: Compilers cannot perform constant propagation, dead-code elimination, or loop unrolling across opaque virtual boundaries.

---

## 8. 📁 Code Examples for Section 2

- [`Code/01_ocp_violation_switch_hell.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/02_Open_Closed_Principle/Code/01_ocp_violation_switch_hell.cpp): Demonstrates the classic type-switching anti-pattern violating OCP.
- [`Code/02_ocp_runtime_nvi_polymorphism.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/02_Open_Closed_Principle/Code/02_ocp_runtime_nvi_polymorphism.cpp): Implements Herb Sutter's NVI idiom with Strategy pattern for invariant enforcement.
- [`Code/03_ocp_compile_time_crtp.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/02_Open_Closed_Principle/Code/03_ocp_compile_time_crtp.cpp): Zero-overhead compile-time extensibility via CRTP and C++20 Concepts.
- [`Code/04_ocp_std_variant_visitor.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/02_Open_Closed_Principle/Code/04_ocp_std_variant_visitor.cpp): Modern C++20 closed-set polymorphic extension using `std::variant` and `std::visit`.
