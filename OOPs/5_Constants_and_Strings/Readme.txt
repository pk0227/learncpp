=================================================================
📘 C++ NUMERAL SYSTEMS & LITERALS
=================================================================

    🔹 1 — INTEGER BASES & LITERAL PREFIXES
    -------------------------------------------------------------
        - Decimal (base 10): Default numbers with no prefix (0–9)
        - Hexadecimal (base 16): Prefix 0x or 0X (digits 0–9, a–f, A–F)
        - Octal (base 8): Prefix 0 (digits 0–7)
          ⚠️ WARNING: A leading zero indicates an octal literal in C++ (e.g. 044 is decimal 36, not 44)!
        - Binary (base 2): Prefix 0b or 0B (digits 0–1, introduced in C++14)

    🔹 2 — DIGIT SEPARATORS (C++14)
    -------------------------------------------------------------
        - Single quote (') can be used as a digit separator to make large literals readable:
          int million { 1'000'000 };
          int binaryByte { 0b1010'0101 };
          int hexWord { 0x00FF'AABB };

    🔹 3 — OUTPUTTING BINARY WITH std::bitset
    -------------------------------------------------------------
        - Standard stream output does not provide a native manipulator for binary prior to C++20/C++23.
        - std::bitset<N> (in <bitset>) prints binary representations directly:
          std::bitset<8> b{ 0b1010'0101 };
          std::cout << b << '\n'; // 10100101
        - Modern C++:
          - C++20: std::format("{:b}", 36)
          - C++23: std::println("{:b}", 36)

=================================================================
📘 C++ COMPILE-TIME PROGRAMMING & CONSTEXPR
=================================================================

    🔹 1 — WHY COMPILE-TIME PROGRAMMING
    -------------------------------------------------------------
        ✅ Enables explicit compile-time evaluation
        ✅ Compiler enforces compile-time requirement
        ✅ Benefits:
            • ⚡ Performance: faster, smaller binaries
            • 🛡️ Predictability: compiler errors if evaluation fails
            • ✅ Safety: undefined behavior prevented
            • 🐛 Quality: catch bugs early

    🔹 2 — CORE FEATURES IN C++
    -------------------------------------------------------------
        - constexpr variables
        - constexpr functions
        - templates
        - static_assert
        - All rely on constant expressions

    🔹 3 — CONSTANT EXPRESSIONS
    -------------------------------------------------------------
        ✅ Expression evaluatable at compile-time
        ✅ Must consist of literals, constexpr objects, operators, and constexpr function calls
        ❌ Non-constant expressions: runtime variables or non-constexpr function calls

    🔹 4 — CONST vs CONSTEXPR VARIABLES
    -------------------------------------------------------------
        const:
            • Value cannot change after initialization
            • May be runtime or compile-time
            • ❌ Not guaranteed compile-time

        constexpr:
            • Must be compile-time constant
            • ✅ Always usable in constant expressions
            • Implicitly const

        Example:
            constexpr int x{5};   // compile-time constant
            const int y{userInput}; // runtime constant

    🔹 5 — FUNCTION PARAMETERS
    -------------------------------------------------------------
        - Function parameters initialized at runtime
        - const parameter → runtime constant
        - Parameters cannot be constexpr
        - Compile-time arguments → use non-type template parameters

    🔹 6 — CONSTEXPR FUNCTIONS
    -------------------------------------------------------------
        ✅ Can be used in constant expressions
        ⚡ Evaluates at compile-time when required, otherwise runs at runtime

        Example:
            constexpr int cmax(int x, int y) {
                return (x > y) ? x : y;
            }

        Comparison:
            int max(int x, int y);          // runtime-only
            constexpr int cmax(int x, int y); // compile-time capable

        Usage:
            int m1{ max(5,6) };             // ✅ runtime
            const int m2{ max(5,6) };       // ✅ runtime
            constexpr int m3{ max(5,6) };   // ❌ error

            int m4{ cmax(5,6) };            // ✅ runtime
            const int m5{ cmax(5,6) };      // ✅ runtime
            constexpr int m6{ cmax(5,6) };  // ✅ compile-time

    🔹 7 — CONSTANT EXPRESSION RULES
    -------------------------------------------------------------
        ✅ Allowed:
            • Literals: 5, 1.2
            • Operators on constant expressions
            • sizeof(...)
            • const integral variables with constant initializers (legacy)
            • constexpr variables
            • constexpr function calls (with constant arguments)

        ❌ Not allowed:
            • Non-const variables
            • const non-integral (e.g., const double)
            • Non-constexpr function returns
            • Function parameters
            • std::cout, std::cin
            • new, delete, throw, typeid

    🔹 8 — EVALUATION TIMING
    -------------------------------------------------------------
        - Constant expressions are CAPABLE of compile-time evaluation
        - Compiler required to evaluate at compile-time only when context demands
        - Categories:
            • Never → non-constant expression
            • Possibly → optimizable runtime expression
            • Likely → constant expression
            • Always → constant expression required

    🔹 9 — BEST PRACTICES
    -------------------------------------------------------------
        ✅ Use constexpr whenever possible for compile-time constants
        ✅ Use const for runtime constants
        ✅ constexpr works for non-integral types (e.g., double)
        ✅ Use constexpr functions for compile-time computations
        ✅ Prefer non-type template parameters for compile-time arguments

    🔹 10 — KEY INSIGHTS
    -------------------------------------------------------------
        ⚠️ const ≠ compile-time guarantee
        ✅ constexpr enforces compile-time usability
        ⚡ Compile-time evaluation requires values always known at compile-time
        🛑 Compiler errors occur if compile-time requirements not met

=================================================================
📘 C++ STRINGS: std::string & std::string_view
=================================================================

    🔹 1 — C-STYLE STRINGS
    -------------------------------------------------------------
        - "Hello, world!" (const char[])
        - C-style string variables:
            • Hard to assign new values
            • Risk of undefined behavior if sizes mismatch
        ✅ Modern C++: Prefer std::string or std::string_view

    🔹 2 — INTRODUCING std::string
    -------------------------------------------------------------
        #include <string>
        std::string str{};                  // empty string
        std::string name{"Alex"};           // initialize with literal
        name = "John";                      // assign new value

        - Numeric characters are text, not numbers:
            • std::string myID{"45"};   // "45" != 45

    🔹 3 — INPUT / OUTPUT
    -------------------------------------------------------------
        #include <iostream>
        #include <string>

        - std::cout << name << '\n';       // output string
        - std::cin >> name;                 // stops at first whitespace
        - std::getline(std::cin >> std::ws, name); // full-line input

        • std::ws: skips leading whitespace when mixing >> and getline()

    🔹 4 — STRING LENGTH
    -------------------------------------------------------------
        - name.length();                    
        - int len = static_cast<int>(name.length()); 
        - C++20: std::ssize(name) returns signed integral

    🔹 5 — DYNAMIC MEMORY & PERFORMANCE
    -------------------------------------------------------------
        - std::string allocates memory dynamically
        - Handles variable-length strings
        ❌ Copies are expensive → avoid passing by value
        ✅ Prefer std::string_view for read-only function parameters
        ✅ Returning by value is OK due to move semantics

    🔹 6 — STRING LITERALS AS std::string
    -------------------------------------------------------------
        #include <string>
        using namespace std::string_literals;

        "foo"     // C-style
        "foo"s    // std::string literal
        ⚠️ Use sparingly; convenient for type deduction

    🔹 7 — CONSTEXPR STRINGS
    -------------------------------------------------------------
        ❌ constexpr std::string variable NOT supported in C++17 or earlier
        ✅ In C++20, std::string gained transient allocation support in constexpr functions (must be freed before compile-time ends)
        ✅ However, std::string objects cannot be stored as constexpr variables across compilation units
        ✅ Use std::string_view for compile-time string constants

    🔹 8 — BEST PRACTICES
    -------------------------------------------------------------
        ✅ Prefer std::string over C-style strings
        ✅ Use std::getline(std::cin >> std::ws, var) for full-line input
        ✅ Avoid passing std::string by value; use std::string_view
        ✅ Use move semantics when returning local std::string
        ✅ Use "s" suffix for std::string literals if needed
        • Length functions:
            - .length() → size_t
            - std::ssize() → signed type

    🔹 9 — EXAMPLES
    -------------------------------------------------------------
        #include <iostream>
        #include <string>
        using namespace std::string_literals;

        int main() {
            std::string name{"Alex"};
            std::cout << "My name is: " << name << '\n';

            name = "Jason";
            std::cout << "Now: " << name << '\n';

            std::string fullName{};
            std::cout << "Enter full name: ";
            std::getline(std::cin >> std::ws, fullName);

            std::cout << fullName << " has "
                      << name.length() << " characters\n";

            std::string strLiteral = "Hello"s; // std::string literal
        }

=================================================================
📘 C++ STRINGS: std::string_view
=================================================================

    🔹 1 — PROBLEM WITH std::string
    -------------------------------------------------------------
        ❌ Copying std::string is expensive
        - Initializing std::string copies initializer
        - Passing by value → another copy

        Example:
            #include <string>
            std::string s{"Hello, world!"};
            std::cout << s << '\n';

            void printString(std::string str) { std::cout << str << '\n'; }
            printString(s); // 2 copies total

    🔹 2 — INTRODUCING std::string_view (C++17)
    -------------------------------------------------------------
        ✅ Read-only access without copying
        ✅ Can view C-style strings, std::string, or another std::string_view

        Example:
            #include <string_view>
            void printSV(std::string_view str) { std::cout << str << '\n'; }

            std::string_view s{"Hello, world!"};
            printSV(s); // no copies

    🔹 3 — FLEXIBLE INITIALIZATION
    -------------------------------------------------------------
        - Initialize from:
            • C-style string: std::string_view s1{"Hello"};
            • std::string: std::string_view s2{s};
            • std::string_view: std::string_view s3{s2};

        - Function parameters accept:
            printSV("Hello"); // C-style
            printSV(s);       // std::string
            printSV(s2);      // std::string_view

    🔹 4 — std::string_view → std::string
    -------------------------------------------------------------
        ❌ Implicit conversion NOT allowed
        ✅ Explicit conversion:
            std::string s{sv};
            printString(static_cast<std::string>(sv));

    🔹 5 — ASSIGNMENT
    -------------------------------------------------------------
        - Reassigning changes view, not original string

            std::string name{"Alex"};
            std::string_view sv{name};
            sv = "John";        // views "John"
            std::cout << name;  // prints "Alex"

    🔹 6 — LITERALS
    -------------------------------------------------------------
        - C-style: "foo"
        - std::string: "foo"s
        - std::string_view: "foo"sv

        using namespace std::string_literals;
        using namespace std::string_view_literals;

        std::cout << "foo\n";
        std::cout << "goo\n"s;
        std::cout << "moo\n"sv;

    🔹 7 — CONSTEXPR std::string_view
    -------------------------------------------------------------
        ✅ Fully supported → preferred for symbolic constants
        constexpr std::string_view s{"Hello, world!"};
        std::cout << s << '\n';

=================================================================
📘 C++ STRINGS: std::string vs std::string_view (Concise Notes)
=================================================================

    🔹 1 — OWNERS AND VIEWERS ANALOGY
    -------------------------------------------------------------
        ✅ std::string = owner
            - Makes a copy of initializer → independent
            - Responsible for memory management
            - Example:
                  std::string s{"Hello"}; // s owns the string

        ✅ std::string_view = viewer
            - Read-only access, no copy → cheap
            - Depends on the original string
            - Example:
                  std::string_view sv{s}; // sv views s

        ⚠️ Key insight:
            - Owners manage data; viewers just observe
            - Dangling view if original string destroyed

    🔹 2 — std::string: EXPENSIVE COPY
    -------------------------------------------------------------
        Example: passing by value creates copies

            void printString(std::string str) {
                std::cout << str << '\n';
            }

            std::string s{"Hello"};
            printString(s); // ✅ copies s → expensive

        💡 Tip: Use only when ownership or modification is required

    🔹 3 — std::string_view: SAFE AND LIGHTWEIGHT
    -------------------------------------------------------------
        - Read-only access without copying
        - Can view C-style strings, std::string, or another std::string_view
        - Must ensure viewed string outlives the view

        Example: function parameter
            void printSV(std::string_view str) {
                std::cout << str << '\n';
            }

            printSV("Hello");  // ✅ C-style string
            printSV(s);        // ✅ std::string
            printSV(sv);       // ✅ std::string_view

        ⚠️ Warning:
            - Dangling view occurs if original string is destroyed or modified
            - ❌ Do NOT initialize with temporary std::string

    🔹 4 — IMPROPER USE CASES
    -------------------------------------------------------------
        ❌ Viewing destroyed local string
            { std::string s{"Hello"}; sv = s; } // s destroyed → sv dangling

        ❌ Viewing temporary return value
            std::string_view name{ getName() }; // dangling

        ❌ Viewing std::string literal via "s" suffix
            std::string_view name{"Alex"s}; // dangling

        ❌ Modifying string invalidates views
            s = "New"; // sv now invalid

        ✅ Revalidating invalid view:
            sv = s; // now sv valid again

    🔹 5 — RETURNING std::string_view
    -------------------------------------------------------------
        ✅ Safe:
            - Return C-style string literals
            - Return std::string_view parameter (exists in caller scope)

        ⚠️ Warning:
            - Return view of local variable → dangling
            - Temporary objects must be used immediately

    🔹 6 — VIEW MODIFICATION FUNCTIONS
    -------------------------------------------------------------
        - remove_prefix(n): removes n chars from left
        - remove_suffix(n): removes n chars from right

        Example:
            std::string_view str{"Peach"};
            str.remove_prefix(1); // "each"
            str.remove_suffix(2); // "ea"
            str = "Peach";        // reset view

    🔹 7 — SUBSTRINGS AND NULL-TERMINATION
    -------------------------------------------------------------
        - Can view substrings without copying
        - May or may not be null-terminated
        - Length is tracked internally via an internal pointer and length member
        ⚠️ CRITICAL WARNING: Do not assume std::string_view is null-terminated!
            - Calling sv.data() on a substring or sliced view and passing it to C-style functions
              expecting null-terminated strings (strlen, printf("%s"), std::atoi, fopen) causes buffer overrun!
            - If a null-terminated string is required, convert explicitly: std::string{sv}

    🔹 8 — QUICK GUIDE: WHEN TO USE std::string vs std::string_view
    -------------------------------------------------------------
        Variables:
            std::string ✅ → modify, store user input, store return value
            std::string_view ✅ → read-only view, symbolic constant, non-dangling return value

        Function parameters:
            std::string ❌ → rare: needs modification or C++14 compatibility
            std::string_view ✅ → read-only, works with non-null-terminated strings

        Return types:
            std::string ✅ → local variable, function call returning std::string
            std::string_view ✅ → C-style string literal, view parameter

    🔹 9 — KEY INSIGHTS
    -------------------------------------------------------------
        std::string:
            - Expensive to copy → avoid by-value passing
            - Modifying invalidates views
            - Safe to return local variable by value

        std::string_view:
            - Lightweight, read-only
            - Can view string literals or existing strings
            - Dangling views = undefined behavior
            - May or may not be null-terminated
