================================================================================
MODULE 01: OBJECT LIFETIME, CONSTRUCTOR FAILURES & EXCEPTION SAFETY
================================================================================

This module covers the critical object lifecycle mechanics, failure modes, and 
exception-safety guarantees that senior C++ engineers must master for mission-critical 
systems and senior/staff-level technical interviews.

--------------------------------------------------------------------------------
TABLE OF CONTENTS
--------------------------------------------------------------------------------
1. The Object Lifetime Invariant and The Constructor Exception Trap
   1.1 When Does an Object's Lifetime Begin? (ISO C++ [basic.life])
   1.2 What Happens When a Constructor Throws?
   1.3 Why the Destructor Never Runs for Partially Constructed Objects
   1.4 The Raw Pointer Resource Leak Hazard
   1.5 Sub-Object Destruction Order During Constructor Failure
   1.6 The Modern Solution: RAII and Rule of Zero
2. Function-Try-Blocks on Constructors
   2.1 Syntax and Structure of Function-Try-Blocks
   2.2 The Inability to Suppress Exceptions (Implicit Rethrow)
   2.3 The "Dangling Sub-Object" Hazard: Accessing Members in Catch Block is UB
   2.4 Legitimate Use Cases: Exception Translation and Logging
3. Move Semantics, noexcept, and the std::vector Reallocation Dilemma
   3.1 The Three Exception Safety Guarantees (Basic, Strong, No-Throw)
   3.2 Why std::vector Reallocation Requires the Strong Guarantee
   3.3 The Role of std::move_if_noexcept and std::is_nothrow_move_constructible
   3.4 The Silent Performance Cliff: Falling Back to Deep Copies
   3.5 Best Practice: Marking Move Constructors and Move Assignment noexcept
4. The Copy-and-Swap Idiom
   4.1 The Deficiencies of Naive Copy Assignment Operators
   4.2 Self-Assignment Branching Overhead vs Robustness
   4.3 Mechanics of Copy-and-Swap: Pass-by-Value and Non-Throwing Swap
   4.4 How Copy-and-Swap Unifies Copy and Move Assignment
   4.5 Exception Safety Guarantees Provided by Copy-and-Swap
5. Construction and Destruction Order in Complex Hierarchies
   5.1 Standard Construction Sequence (Virtual Bases, Non-Virtual Bases, Members)
   5.2 Why Member Initialization Order Depends Solely on Class Declaration Order
   5.3 Standard Destruction Sequence (Exact Mirror Reversal)
   5.4 Virtual Function Dispatch During Construction and Destruction
   5.5 The Pure Virtual Function Call Hazard (__cxa_pure_virtual)


================================================================================
1. THE OBJECT LIFETIME INVARIANT AND THE CONSTRUCTOR EXCEPTION TRAP
================================================================================

1.1 When Does an Object's Lifetime Begin? (ISO C++ [basic.life])
    -- According to the ISO C++ Standard ([basic.life]), the lifetime of an object of class type 
       begins ONLY WHEN its constructor completes execution without throwing an exception.
    -- If a constructor exits by throwing an exception, the object is considered by the runtime
       to have NEVER existed.

1.2 What Happens When a Constructor Throws?
    -- When an exception is thrown from the constructor body or during the initialization of its 
       sub-objects (bases or members), construction is aborted immediately.
    -- Stack unwinding begins from the point of the throw.

1.3 Why the Destructor Never Runs for Partially Constructed Objects
    -- Because an object's lifetime never formally began, the runtime will NEVER invoke the 
       destructor of that class.
    -- RATIONALE: The destructor assumes all class invariants have been established and all 
       members are valid. Running a destructor on a half-initialized object would lead to 
       undefined behavior (e.g., attempting to clean up unallocated pointers or invalid handles).

1.4 The Raw Pointer Resource Leak Hazard
    -- Consider a class that allocates two raw heap buffers in its constructor:
       class Danger {
           int* p1;
           int* p2;
       public:
           Danger() : p1(new int[100]), p2(new int[200]) {
               // Suppose allocation of p2 throws std::bad_alloc, 
               // OR an exception is thrown here in the constructor body.
           }
           ~Danger() {
               delete[] p1;
               delete[] p2;
           }
       };
    -- If p2 throws std::bad_alloc, or if any code in the constructor body throws:
       1. The destructor ~Danger() is NOT called.
       2. The memory pointed to by p1 is LEAKED permanently!
    -- This is the classic "Constructor Exception Trap".

1.5 Sub-Object Destruction Order During Constructor Failure
    -- Although the class destructor itself does not run, C++ guarantees that all fully 
       constructed sub-objects (base classes and data members initialized prior to the throw) 
       are destroyed in the EXACT REVERSE order of their construction.
    -- If a member variable is an object whose constructor completed successfully, its 
       destructor will run during stack unwinding.

1.6 The Modern Solution: RAII and Rule of Zero
    -- To eliminate constructor leaks, never store raw owning pointers in a class with multiple 
       resources.
    -- Replace raw pointers with RAII wrappers like std::unique_ptr or standard containers:
       class Safe {
           std::unique_ptr<int[]> p1;
           std::unique_ptr<int[]> p2;
       public:
           Safe() : p1(std::make_unique<int[]>(100)), p2(std::make_unique<int[]>(200)) {
               // If p2 throws, p1 is already a fully constructed std::unique_ptr sub-object.
               // The runtime invokes p1's destructor during stack unwinding, cleanly freeing p1!
           }
       };


================================================================================
2. FUNCTION-TRY-BLOCKS ON CONSTRUCTORS
================================================================================

2.1 Syntax and Structure of Function-Try-Blocks
    -- Normal try-catch blocks inside the constructor body cannot catch exceptions thrown from 
       the member initializer list.
    -- A "function-try-block" wraps the entire constructor, including the initializer list:
       class Derived : public Base {
           Member m_member;
       public:
           Derived(int x) try 
               : Base(x), m_member(x) 
           {
               // Constructor body
           } 
           catch (const std::exception& e) 
           {
               // Catch block for Base, m_member, or constructor body exceptions
           }
       };

2.2 The Inability to Suppress Exceptions (Implicit Rethrow)
    -- In a normal function, a catch block can handle an exception and return normally without 
       propagating the exception.
    -- IN A CONSTRUCTOR FUNCTION-TRY-BLOCK, THIS IS IMPOSSIBLE:
       -- The standard mandates: If control reaches the end of a constructor catch block, 
          the runtime automatically and implicitly executes `throw;` (rethrowing the original exception).
       -- Even if you put a `return;` statement in the catch block, it is illegal (or results in rethrow).
    -- RATIONALE: An object whose constructor failed cannot be allowed to exist in an incompletely 
       constructed state. The caller must be informed that object creation failed.

2.3 The "Dangling Sub-Object" Hazard: Accessing Members in Catch Block is UB
    -- When a constructor function-try-block catch block is entered:
       1. Any members that were constructed prior to the exception have ALREADY been destroyed by stack unwinding!
       2. The base classes have ALREADY been destroyed!
    -- WARNING: Accessing any non-static member variable or calling any member function inside the 
       catch block of a constructor function-try-block is UNDEFINED BEHAVIOR!
    -- Only static members and function arguments can be safely inspected.

2.4 Legitimate Use Cases: Exception Translation and Logging
    -- If function-try-blocks cannot suppress exceptions and cannot access member variables, why do they exist?
       1. Exception Translation: Catch a low-level third-party library exception thrown in a member/base 
          initializer and rethrow a domain-specific custom exception.
       2. Logging / Auditing: Log construction failure diagnostics at the boundary.
       3. Cleaning up external non-memory resources (e.g., shared memory handles, file locks, or database transactions)
          acquired in the parameter list or outside the sub-object hierarchy.


================================================================================
3. MOVE SEMANTICS, NOEXCEPT, AND THE STD::VECTOR REALLOCATION DILEMMA
================================================================================

3.1 The Three Exception Safety Guarantees (Basic, Strong, No-Throw)
    -- Basic Guarantee: If an exception is thrown, no resources are leaked and all objects remain in a 
       valid (though possibly unspecified) state. Invariants are preserved.
    -- Strong Guarantee ("Commit or Rollback"): If an operation throws, the program state is left 
       exactly as it was before the operation began.
    -- No-Throw (noexcept) Guarantee: The operation is guaranteed to never throw under any circumstance.

3.2 Why std::vector Reallocation Requires the Strong Guarantee
    -- When a std::vector exceeds its capacity during push_back(), it must:
       1. Allocate a new contiguous memory buffer of larger capacity.
       2. Transfer existing elements from the old buffer to the new buffer.
       3. Insert the new element.
       4. Deallocate the old buffer.
    -- std::vector::push_back() promises the STRONG EXCEPTION GUARANTEE.
    -- If copying elements from old to new buffer throws, std::vector can simply deallocate the new buffer; 
       the old buffer and its original elements remain completely untouched and intact.
    -- BUT WHAT IF WE MOVE ELEMENTS?
       -- If an element's move constructor throws mid-way (say on element 4 of 10):
          -- Elements 0 to 3 have already been moved to the new buffer (leaving old elements 0-3 in moved-from state).
          -- Element 4 threw an exception.
          -- Elements 5 to 9 remain in the old buffer.
       -- The old buffer CANNOT be restored because elements 0-3 were already mutated/gutted by move semantics!
       -- The Strong Exception Guarantee is shattered!

3.3 The Role of std::move_if_noexcept and std::is_nothrow_move_constructible
    -- To solve this, C++ introduces `std::move_if_noexcept(x)`:
       -- It checks if `std::is_nothrow_move_constructible<T>::value` is true.
       -- If true, it returns an rvalue reference (static_cast<T&&>(x)), enabling move semantics.
       -- If false (and T is copy constructible), it returns a const lvalue reference (const T&), 
          forcing a deep copy!

3.4 The Silent Performance Cliff: Falling Back to Deep Copies
    -- If a developer implements a move constructor but forgets to mark it `noexcept`:
       class Widget {
       public:
           Widget(Widget&& other); // Missing noexcept!
       };
    -- During std::vector reallocation, the compiler detects that Widget's move constructor could throw.
    -- Result: std::vector SILENTLY IGNORES the move constructor and falls back to calling the copy constructor 
       for every single element in the vector!
    -- This results in catastrophic performance degradation in high-throughput systems, with zero compiler 
       warnings or errors.

3.5 Best Practice: Marking Move Constructors and Move Assignment noexcept
    -- Move operations transfer ownership of pointers and handles; they should almost NEVER throw.
    -- ALWAYS mark move constructors and move assignment operators `noexcept`:
       Widget(Widget&& other) noexcept;
       Widget& operator=(Widget&& other) noexcept;


================================================================================
4. THE COPY-AND-SWAP IDIOM
================================================================================

4.1 The Deficiencies of Naive Copy Assignment Operators
    -- A traditional copy assignment operator usually looks like this:
       MyString& MyString::operator=(const MyString& other) {
           if (this == &other) return *this; // Self-assignment check
           delete[] m_data;                  // Release old resource
           m_data = new char[other.m_len];   // Allocate new resource
           std::memcpy(m_data, other.m_data, other.m_len);
           m_len = other.m_len;
           return *this;
       }
    -- Critical Flaws:
       1. EXCEPTION SAFETY VIOLATION: If `new char[other.m_len]` throws std::bad_alloc, `m_data` has ALREADY 
          been deleted! `*this` is now corrupted with a dangling pointer, violating the Strong Exception Guarantee.
       2. CODE DUPLICATION: Memory allocation and copying logic is duplicated between copy constructor and 
          copy assignment operator.

4.2 Self-Assignment Branching Overhead vs Robustness
    -- In naive code, the `if (this == &other)` check is mandatory to prevent deleting one's own data before reading it.
    -- However, in 99.99% of normal executions, objects are not assigned to themselves. The branch incurs 
       unnecessary branch-prediction overhead in tight loops.

4.3 Mechanics of Copy-and-Swap: Pass-by-Value and Non-Throwing Swap
    -- Step 1: Implement a non-throwing friend swap function using ADL (Argument-Dependent Lookup):
       friend void swap(MyString& first, MyString& second) noexcept {
           using std::swap;
           swap(first.m_data, second.m_data);
           swap(first.m_len, second.m_len);
       }
    -- Step 2: Implement the assignment operator by passing the parameter BY VALUE:
       MyString& operator=(MyString other) noexcept {
           swap(*this, other);
           return *this;
       }

4.4 How Copy-and-Swap Unifies Copy and Move Assignment
    -- Notice that the parameter `other` is passed by value:
       1. If an lvalue is passed: `str1 = str2;`
          -- The copy constructor runs to initialize `other`. If allocation fails, it throws BEFORE entering 
             operator=, leaving `str1` completely untouched!
          -- `swap(*this, other)` swaps `str1`'s old buffer into `other`.
          -- When `operator=` finishes, `other` is destroyed, executing the destructor and freeing the old buffer!
       2. If an rvalue is passed: `str1 = std::move(str2);` or `str1 = MyString("temp");`
          -- The move constructor runs to initialize `other` (zero allocation).
          -- `swap(*this, other)` transfers the new data into `str1`.
          -- Old data in `other` is freed when `other` leaves scope.
    -- One single operator= handles BOTH copy assignment and move assignment efficiently!

4.5 Exception Safety Guarantees Provided by Copy-and-Swap
    -- Strong Exception Guarantee: If memory allocation fails during parameter copy construction, the target 
       object is never modified.
    -- Once inside `operator=`, the swap operation is completely `noexcept`.
    -- Self-assignment is naturally safe: `str1 = str1;` creates a copy of `str1`, swaps with `str1`, and destroys 
       the copy. (Slightly slower for self-assignment, but removes the branch from every normal assignment).


================================================================================
5. CONSTRUCTION AND DESTRUCTION ORDER IN COMPLEX HIERARCHIES
================================================================================

5.1 Standard Construction Sequence (Virtual Bases, Non-Virtual Bases, Members)
    -- The ISO C++ standard specifies a strict, deterministic order of construction:
       1. Virtual Base Classes:
          -- Initialized first, regardless of where they appear in the inheritance hierarchy.
          -- Initialized in depth-first, left-to-right order as declared in the inheritance graph.
          -- CRITICAL: Virtual base classes are ALWAYS initialized directly by the MOST-DERIVED class 
             constructor (intermediate base class initializers for virtual bases are ignored).
       2. Non-Virtual Direct Base Classes:
          -- Initialized in the exact order they appear in the class declaration's base-specifier list 
             (left-to-right).
       3. Non-Static Data Members:
          -- Initialized in the order they are DECLARED in the class definition.
       4. Constructor Body:
          -- The code inside `{ ... }` of the constructor executes last.

5.2 Why Member Initialization Order Depends Solely on Class Declaration Order
    -- What happens if you write:
       class Example {
           int a;
           int b;
       public:
           Example(int val) : b(val), a(b + 1) {} // Order in list: b then a
       };
    -- DANGER: Member `a` is declared BEFORE `b` in the class definition.
    -- The compiler initializes `a` FIRST using `b + 1`. But `b` is uninitialized garbage!
    -- Result: Undefined Behavior!
    -- RATIONALE: The order of destruction must be the exact reverse of construction. If initialization 
       depended on the order written in constructor initializer lists, two different constructors could 
       initialize members in different orders, making a single deterministic destructor order impossible!
    -- Modern compilers (-Wall -Wreorder) issue a warning if the list order does not match declaration order.

5.3 Standard Destruction Sequence (Exact Mirror Reversal)
    -- Destruction occurs in the EXACT REVERSE order of construction:
       1. Constructor body is cleaned up (destructor body runs first).
       2. Non-static data members are destroyed in REVERSE order of their class declaration.
       3. Non-virtual direct base classes are destroyed in REVERSE order of their base-specifier list (right-to-left).
       4. Virtual base classes are destroyed last, in REVERSE order of their construction.

5.4 Virtual Function Dispatch During Construction and Destruction
    -- SENIOR INTERVIEW FAVORITE: What happens when a virtual function is called inside a Base class constructor?
       class Base {
       public:
           Base() { print(); }
           virtual void print() { std::cout << "Base\n"; }
       };
       class Derived : public Base {
           int* data;
       public:
           Derived() : data(new int(42)) {}
           void print() override { std::cout << "Derived: " << *data << "\n"; }
       };
    -- Result: It prints "Base", NOT "Derived"!
    -- WHY?
       -- During the execution of `Base::Base()`, the `Derived` part of the object has NOT been constructed yet!
       -- If dynamic dispatch invoked `Derived::print()`, it would attempt to access `*data`, which is an 
          uninitialized pointer (garbage), causing an immediate segfault or crash.
       -- The C++ standard mandates that during construction of Base, the dynamic type of the object IS Base.
       -- The compiler updates the vptr at each level of the inheritance hierarchy!

5.5 The Pure Virtual Function Call Hazard (__cxa_pure_virtual)
    -- If a base class constructor calls a virtual function that is pure virtual (= 0) and has no body:
       class Abstract {
       public:
           Abstract() { call_pure(); }
           void call_pure() { pure_func(); } // Indirect call to defeat compiler warning
           virtual void pure_func() = 0;
       };
    -- Because the dynamic type is `Abstract`, the vtable contains a pointer to the runtime error handler 
       `__cxa_pure_virtual`.
    -- Program aborts immediately with: "pure virtual method called"!
    -- RULE: NEVER call virtual functions from constructors or destructors.
