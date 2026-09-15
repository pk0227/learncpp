# 4. Interface Segregation Principle (ISP)

> *"Clients should not be forced to depend upon interfaces that they do not use."*  
> — **Robert C. Martin (Uncle Bob)**

---

## 📑 Table of Contents

1. [Architectural Definition & Senior Mental Model](#1-architectural-definition--senior-mental-model)
2. [Physical Design & Build Costs: The Lakos Analysis](#2-physical-design--build-costs-the-lakos-analysis)
   - [Header Pollution & Recompilation Cascades](#header-pollution--recompilation-cascades)
   - [Virtual Table (Vtable) Bloat & Dead-Code Linking](#virtual-table-vtable-bloat--dead-code-linking)
3. [The "Not Implemented" Exception Anti-Pattern](#3-the-not-implemented-exception-anti-pattern)
4. [Modern C++ Role Interfaces via Multiple Inheritance](#4-modern-c-role-interfaces-via-multiple-inheritance)
5. [Static ISP: The STL Range & Iterator Concept Hierarchy](#5-static-isp-the-stl-range--iterator-concept-hierarchy)
6. [Refactoring Strategies: Role Interfaces & The Adapter Pattern](#6-refactoring-strategies-role-interfaces--the-adapter-pattern)
7. [📁 Code Examples for Section 4](#7--code-examples-for-section-4)

---

## 1. Architectural Definition & Senior Mental Model

In standard OOP literature, ISP is explained simply: *"Keep your interfaces small."*

For an 8–10 years experienced C++ developer, ISP is fundamentally about **cohesion at interface boundaries and the minimization of client coupling**. When an interface is fat:
- Callers are exposed to methods they do not care about.
- Implementers are forced to write dummy boilerplate or throw runtime exceptions for unsupported operations.
- The interface becomes a coupling nexus that pulls unrelated systems together.

```
       ❌ FAT INTERFACE (ISP VIOLATION)
       ┌────────────────────────────────────────────────────────┐
       │                 IMultiFunctionDevice                   │
       ├────────────────────────────────────────────────────────┤
       │ + printDocument()                                      │
       │ + scanDocument()                                       │
       │ + faxDocument()                                        │
       │ + stapleDocument()                                     │
       └────────────────────────────────────────────────────────┘
          ▲                                    ▲
          │                                    │
    [InkjetPrinter]                    [OfficeScanner]
 (Throws on fax/scan/staple!)         (Throws on print/fax/staple!)
```

---

## 2. Physical Design & Build Costs: The Lakos Analysis

In John Lakos' seminal work *Large-Scale C++ Software Design*, interface segregation is not just an aesthetic OOP concern—it is a **critical build performance and dependency management discipline**.

### Header Pollution & Recompilation Cascades
Consider a fat interface `IDatabaseSession`:
```cpp
// IDatabaseSession.h
#include <vector>
#include <string>
#include <chrono>
#include <mysql/mysql.h>      // Concrete driver leak
#include <openssl/ssl.h>      // Security token leak
#include <aws/core/Aws.h>      // S3 backup leak

class IDatabaseSession {
public:
    virtual void executeQuery(const std::string& query) = 0;
    virtual void exportToS3(const std::string& bucket) = 0; // S3 dependency!
    virtual void rotateSslCerts() = 0;                      // OpenSSL dependency!
};
```
If a component only wants to run a simple SQL query, it is forced to include `IDatabaseSession.h`. As a result:
1. It transitively includes AWS SDK and OpenSSL headers, bloating preprocessed translation units from 5,000 lines to **over 200,000 lines**.
2. Any minor change in the AWS SDK or SSL signature forces the query runner to recompile.

### Virtual Table (Vtable) Bloat & Dead-Code Linking
Every virtual function in a class adds an entry to that class's virtual table.
- A bloated interface with 40 virtual methods creates large vtables for every derived class in the system.
- The linker is forced to retain code for every virtual method because virtual function calls are resolved via runtime pointers, preventing the linker from stripping dead code (even with `-ffunction-sections` and `--gc-sections`).

---

## 3. The "Not Implemented" Exception Anti-Pattern

The most severe symptom of an ISP violation in production code is the presence of:
```cpp
void BasicPrinter::scanDocument(const std::string&) {
    throw std::logic_error("Not implemented: BasicPrinter has no scanner!");
}
```

> [!CAUTION]
> Throwing `std::logic_error` or returning an error code for unsupported interface methods is a **direct violation of the Liskov Substitution Principle (LSP)** triggered by a violation of ISP. The fat interface misled the caller into believing the contract was supported.

---

## 4. Modern C++ Role Interfaces via Multiple Inheritance

In Java or C#, multiple inheritance of classes is prohibited due to the dreaded Diamond Problem. In **C++**, however, multiple inheritance of **pure abstract classes (interfaces with only pure virtual methods and no member variables)** is completely safe and incurs **zero memory overhead**:

```cpp
class IPrinter {
public:
    virtual ~IPrinter() = default;
    virtual void print(const std::string& doc) = 0;
};

class IScanner {
public:
    virtual ~IScanner() = default;
    virtual void scan(const std::string& target) = 0;
};

// Zero vtable diamond overhead! Clean role composition:
class AllInOneOfficeDevice : public IPrinter, public IScanner {
public:
    void print(const std::string& doc) override;
    void scan(const std::string& target) override;
};
```

---

## 5. Static ISP: The STL Range & Iterator Concept Hierarchy

The greatest real-world example of ISP in the C++ language is the **Standard Template Library (STL) Iterator Concept Hierarchy**.

Rather than defining a single monolithic `Iterator` class with all possible operations, the C++ standard segregates capabilities into fine-grained, composable concepts:

```
            std::input_iterator (Read single-pass forward)
                    ▲
            std::forward_iterator (Multi-pass re-readable)
                    ▲
            std::bidirectional_iterator (Supports operator--)
                    ▲
            std::random_access_iterator (O(1) indexing: it + n, it[n])
                    ▲
            std::contiguous_iterator (Strict physical adjacent memory)
```

By segregating these interfaces statically:
- An input stream (like `std::cin` or network sockets) satisfies `std::input_iterator` without being forced to provide dummy `operator--` or `operator[]`.
- Algorithms demand only the minimal required concept: `std::find` requires only `std::input_iterator`, while `std::sort` requires `std::random_access_iterator`.

---

## 6. Refactoring Strategies: Role Interfaces & The Adapter Pattern

When interacting with legacy fat interfaces that cannot be easily rewritten:
1. Define clean, granular **Role Interfaces** (`IReader`, `IWriter`) required by your new subsystems.
2. Implement an **Adapter** that wraps the legacy fat object and exposes only the required role interface to clients.

```cpp
class LegacyGodDevice {
    // 50 monolithic methods
};

class DocumentPrinterAdapter : public IPrinter {
private:
    LegacyGodDevice& device_;
public:
    explicit DocumentPrinterAdapter(LegacyGodDevice& dev) : device_(dev) {}
    void print(const std::string& doc) override {
        device_.ExecutePrintJobV2(doc, 0, false);
    }
};
```

---

## 7. 📁 Code Examples for Section 4

- [`Code/01_isp_violation_fat_interface.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/04_Interface_Segregation_Principle/Code/01_isp_violation_fat_interface.cpp): Monolithic multi-function device interface forcing dummy implementations and runtime errors.
- [`Code/02_isp_refactored_role_interfaces.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/04_Interface_Segregation_Principle/Code/02_isp_refactored_role_interfaces.cpp): Clean segregation into `IPrinter`, `IScanner`, and `IFax` composed via multiple inheritance.
- [`Code/03_isp_concepts_range_hierarchy.cpp`](file:///home/prashanth/Learnings/learncpp/SOLID_Principles/04_Interface_Segregation_Principle/Code/03_isp_concepts_range_hierarchy.cpp): Compile-time static interface segregation modeled after the STL iterator concept hierarchy.
