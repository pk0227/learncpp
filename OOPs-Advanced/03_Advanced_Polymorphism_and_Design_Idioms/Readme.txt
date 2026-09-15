================================================================================
MODULE 03: ADVANCED POLYMORPHISM AND DESIGN IDIOMS
================================================================================

This module covers advanced C++ idioms that merge object-oriented programming with 
generic template metaprogramming to achieve high-performance static polymorphism, 
type erasure, and robust API contracts.

--------------------------------------------------------------------------------
TABLE OF CONTENTS
--------------------------------------------------------------------------------
1. Curiously Recurring Template Pattern (CRTP) & Static Polymorphism
   1.1 Mechanics of CRTP: Deriving from a Templated Base
   1.2 Static Polymorphism vs Dynamic Polymorphism (Vtables vs Inlining)
   1.3 Enforcing Compile-Time Interfaces with CRTP
   1.4 The Object Counter and Mixin Pattern via CRTP
2. Policy-Based Class Design and Mixins
   2.1 The Philosophy of Policy-Based Design (Alexandrescu Paradigm)
   2.2 Policies as Template Parameters vs Deep Inheritance Trees
   2.3 Combining Orthogonal Policies (Threading, Storage, Checking)
   2.4 The Zero-Overhead Advantage of Policy Composition
3. The Type Erasure Idiom
   3.1 The Problem: Storing Unrelated Types without a Common Base or Exposed Templates
   3.2 The Tripartite Architecture of Type Erasure:
       - Public Value-Semantics Wrapper
       - Private Abstract Base (Concept)
       - Private Templated Derived (Model)
   3.3 How std::function and std::any Work Internally
   3.4 Small Buffer Optimization (SBO) for Type Erasure
4. Member Function Ref-Qualifiers (& and &&)
   4.1 The Hidden *this Value Category
   4.2 Syntax and Rules for Ref-Qualified Methods
   4.3 Preventing Dangerous Mutations on Rvalue Temporaries
   4.4 Resource Theft Optimization: Move-Enabled Getters
5. Virtual Friend Functions and Double Dispatch
   5.1 Why Friend Functions Cannot Be Virtual
   5.2 The Virtual Friend Idiom (Public Friend Delegating to Protected Virtual)
   5.3 The Double Dispatch Problem (Visitor Pattern)
   5.4 Eliminating dynamic_cast Cascades with Double Dispatch


================================================================================
1. CURIOUSLY RECURRING TEMPLATE PATTERN (CRTP) & STATIC POLYMORPHISM
================================================================================

1.1 Mechanics of CRTP: Deriving from a Templated Base
    -- A class derives from a base class template instantiated with the derived class itself:
       template <typename Derived>
       class Base {
       public:
           void interface() {
               // Upcast 'this' to Derived* and invoke implementation
               static_cast<Derived*>(this)->implementation();
           }
       };
       class MyDerived : public Base<MyDerived> {
       public:
           void implementation() { /* ... */ }
       };

1.2 Static Polymorphism vs Dynamic Polymorphism (Vtables vs Inlining)
    -- Dynamic Polymorphism (virtual functions):
       -- Resolution happens at runtime via vtable indirection.
       -- Incurs indirect call overhead, prevents compiler inlining, and adds 8-byte vptr per object.
    -- Static Polymorphism (CRTP):
       -- Resolution happens at compile time!
       -- The compiler knows the exact derived type, allowing it to COMPLETELY INLINE the implementation.
       -- Zero vptr overhead, zero vtable, zero runtime branch penalties.

1.3 Enforcing Compile-Time Interfaces with CRTP
    -- If a derived class forgets to implement `implementation()`, the code fails compilation 
       with a clear error at compile time rather than relying on runtime pure-virtual crashes.
    -- In C++20, CRTP interfaces can be combined with C++20 Concepts to provide elegant compiler diagnostics.

1.4 The Object Counter and Mixin Pattern via CRTP
    -- CRTP allows adding reusable functionality (mixins) to diverse classes without virtual overhead:
       template <typename T>
       class InstanceCounter {
           static inline int count{0};
       public:
           InstanceCounter() { ++count; }
           ~InstanceCounter() { --count; }
           static int getCount() { return count; }
       };
       class User : public InstanceCounter<User> {};
       class Order : public InstanceCounter<Order> {};
       -- `User` and `Order` each get their own distinct static counter!


================================================================================
2. POLICY-BASED CLASS DESIGN AND MIXINS
================================================================================

2.1 The Philosophy of Policy-Based Design (Alexandrescu Paradigm)
    -- Popularized by Andrei Alexandrescu in "Modern C++ Design".
    -- Instead of building massive, brittle, multi-tiered inheritance hierarchies, a class is 
       assembled from independent "policies" passed as template arguments.

2.2 Policies as Template Parameters vs Deep Inheritance Trees
    -- Traditional OOP creates monolithic classes with multiple flags or subclasses:
       class ThreadSafeFileLogger : public Logger { ... };
       class SingleThreadedConsoleLogger : public Logger { ... };
       -- Leads to combinatorial explosion of subclasses!
    -- Policy-based design breaks behavior into orthogonal concerns:
       template <typename ThreadingPolicy, typename OutputPolicy>
       class SmartLogger : private ThreadingPolicy, private OutputPolicy {
       public:
           void log(const std::string& msg) {
               ThreadingPolicy::lock();
               OutputPolicy::write(msg);
               ThreadingPolicy::unlock();
           }
       };

2.3 Combining Orthogonal Policies (Threading, Storage, Checking)
    -- Users can mix and match policies arbitrarily:
       using FastLogger = SmartLogger<SingleThreadedPolicy, ConsolePolicy>;
       using SecureLogger = SmartLogger<MultiThreadedPolicy, FilePolicy>;
    -- Adding a new policy requires writing one small class, not editing the core class.

2.4 The Zero-Overhead Advantage of Policy Composition
    -- Policies are resolved at compile time.
    -- If `SingleThreadedPolicy::lock()` is empty, the compiler completely inlines and optimizes 
       it away into 0 machine instructions!


================================================================================
3. THE TYPE ERASURE IDIOM
================================================================================

3.1 The Problem: Storing Unrelated Types without a Common Base or Exposed Templates
    -- How do you store diverse objects in a single container (like `std::vector`) if they do NOT 
       share an inheritance hierarchy, without forcing the container to be a template?
    -- Template parameters cannot be varied across elements in a standard container.
    -- Type Erasure provides the solution: value semantics on the outside, polymorphism on the inside.

3.2 The Tripartite Architecture of Type Erasure
    -- Type erasure (as used in `std::function`, `std::any`, `std::move_only_function`) consists of 3 layers:
       1. Concept (Abstract Base):
          struct Concept {
              virtual ~Concept() = default;
              virtual void print() const = 0;
          };
       2. Model (Templated Derived Wrapper):
          template <typename T>
          struct Model : public Concept {
              T m_data;
              Model(T val) : m_data(std::move(val)) {}
              void print() const override { m_data.print(); }
          };
       3. Value-Semantics External Class:
          class Printable {
              std::unique_ptr<Concept> m_impl;
          public:
              template <typename T>
              Printable(T val) : m_impl(std::make_unique<Model<T>>(std::move(val))) {}
              void print() const { m_impl->print(); }
          };

3.3 How std::function and std::any Work Internally
    -- `std::function<void()>` does not know what type of callable it holds (lambda, functor, function pointer).
    -- It wraps whatever you give it in a templated `Model` sub-object, hiding the type from the caller!

3.4 Small Buffer Optimization (SBO) for Type Erasure
    -- Heap allocating `Concept` with `std::unique_ptr` incurs allocation overhead.
    -- Production implementations of `std::function` include an internal byte array (e.g. 32 or 64 bytes) 
       and use placement new for small objects, eliminating heap allocation completely.


================================================================================
4. MEMBER FUNCTION REF-QUALIFIERS (& AND &&)
================================================================================

4.1 The Hidden *this Value Category
    -- Member functions have an implicit parameter: `*this`.
    -- Normally, a member function can be called on both lvalues (`obj.method()`) and 
       rvalue temporaries (`createObj().method()`).

4.2 Syntax and Rules for Ref-Qualified Methods
    -- C++11 allows appending `&` or `&&` to member function declarations:
       void foo() &;  // Only callable if *this is an lvalue!
       void foo() &&; // Only callable if *this is an rvalue (temporary)!

4.3 Preventing Dangerous Mutations on Rvalue Temporaries
    -- Consider a setter or in-place modification method:
       class Matrix {
       public:
           Matrix& transpose() &; // Mutates in place; only allowed on lvalues!
       };
    -- Without `&`:
       getMatrix().transpose(); // Legal! But the transposition is immediately lost when the temporary dies!
    -- With `&`:
       getMatrix().transpose(); // COMPILE ERROR: Cannot bind lvalue ref-qualifier to rvalue!

4.4 Resource Theft Optimization: Move-Enabled Getters
    -- A getter can avoid copying when called on temporary objects:
       class Document {
           std::vector<std::string> m_lines;
       public:
           // Called on lvalue: returns const reference (no copy!)
           const std::vector<std::string>& getLines() const & { return m_lines; }
           // Called on rvalue temporary: MOVES the data out! (zero-copy resource theft!)
           std::vector<std::string> getLines() && { return std::move(m_lines); }
       };
       -- `auto lines = getDoc().getLines();` executes zero copies!


================================================================================
5. VIRTUAL FRIEND FUNCTIONS AND DOUBLE DISPATCH
================================================================================

5.1 Why Friend Functions Cannot Be Virtual
    -- A friend function is a non-member function.
    -- Only non-static member functions can have virtual dispatch because dynamic dispatch 
       requires an implicit `this` pointer pointing to an object with a vtable.

5.2 The Virtual Friend Idiom
    -- To achieve polymorphic behavior for friend operators (like `operator<<`):
       class Base {
       protected:
           virtual void print(std::ostream& os) const { os << "Base"; }
       public:
           friend std::ostream& operator<<(std::ostream& os, const Base& b) {
               b.print(os); // Virtual call inside non-virtual friend!
               return os;
           }
       };

5.3 The Double Dispatch Problem (Visitor Pattern)
    -- Single dynamic dispatch resolves a function based solely on the runtime type of ONE object (`obj->method()`).
    -- Double Dispatch resolves a method based on the runtime types of TWO objects:
       -- Example: `collision(Shape& s1, Shape& s2)` must behave differently for (Circle, Box), (Box, Box), etc.
    -- Naive approach: Nested `if (dynamic_cast<Box*>(&s2))` cascades. Ugly, slow, $O(N \times M)$ maintenance nightmare!
    -- Double Dispatch uses two virtual calls:
       s1.collideWith(s2); -> inside s1: `s2.collideWithCircle(*this);`
       -- Zero casts, 100% type-safe, resolved entirely through vtables!
