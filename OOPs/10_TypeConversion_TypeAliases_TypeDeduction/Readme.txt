================================================================================
Chapter 10: Type Conversion, Type Aliases, and Type Deduction
================================================================================

10.0 -- Introduction to Type Conversion and Implicit Type Conversion
--------------------------------------------------------------------------------
The process of converting data from one type to another type is called "type conversion".

The value of an object is stored as a sequence of bits, and the data type tells the compiler how to interpret those bits into meaningful values. Different data types represent the "same" value differently (e.g., integer 3 as binary 00000000 00000000 00000000 00000011 vs. float 3.0 as IEEE-754 binary 01000000 01000000 00000000 00000000). Copying raw bits between mismatched types via memcpy or reinterpret_cast produces garbage values. Proper type conversion produces a value of the target type that represents the same semantic meaning.

Implicit type conversion (also called automatic type conversion or coercion) is performed automatically by the compiler when one data type is required, but a different data type is supplied.

Key Fundamental Principles of Type Conversion:
    1. Conversions do not change the data being converted. Instead, the conversion process uses that data as input, and produces the converted result.
    2. When converting a value to another type of value, the conversion process produces a temporary object of the target type that holds the result of the conversion.

When Implicit Type Conversion Occurs:
    Implicit type conversion happens in at least six common programming contexts:
    1. Variable initialization: When the initializer expression type differs from the variable's declared type:
       double d { 3 }; // int 3 converted to double 3.0
    2. Variable assignment: When the assigned value differs from the variable type:
       d = 6; // int 6 converted to double 6.0
    3. Function arguments: When an argument type differs from the function parameter type:
       void printDouble(double d);
       printDouble(5); // int 5 converted to double 5.0
    4. Function return statements: When the returned expression type differs from the declared return type:
       float getValue() { return 3.0; } // double 3.0 converted to float 3.0f
    5. Conditional expressions: When a non-boolean expression is used in a condition:
       if (5) { ... } // int 5 converted to bool true
    6. Binary arithmetic operators: When operands have different types (Usual Arithmetic Conversions):
       double result { 4.0 / 3 }; // int 3 converted to double 3.0 before division

Two primary categories of numeric type conversions exist:
    1. Numeric Promotion (safe, always value-preserving)
    2. Numeric Conversion (potentially unsafe, data or precision can be lost)


10.1 -- Numeric Promotions
--------------------------------------------------------------------------------
Numeric promotion is the type conversion of certain narrower numeric types (such as a char or short) to certain wider numeric types (typically int or double) that can be processed efficiently by CPU hardware registers.

Why Numeric Promotions Exist:
    Modern CPUs are designed to perform arithmetic operations most efficiently on their native register size (typically 32-bit or 64-bit words). Processing narrower types (like 8-bit char or 16-bit short) often requires extra masking or emulation instructions. The C++ standard mandates numeric promotions so that narrower types are widened before arithmetic operations are performed.

Numeric Promotion Categories:
    1. Integral promotions
    2. Floating-point promotions

Integral Promotions:
    -- bool, char, signed char, unsigned char, signed short, and unsigned short all get promoted to int.
    -- If int cannot hold the entire range of the type (e.g., unsigned short on systems where int and short have the same size), those get promoted to unsigned int.
    -- wchar_t, char8_t, char16_t, and char32_t also undergo integral promotion to int, unsigned int, long, unsigned long, long long, or unsigned long long (whichever first can hold their entire range).

Floating-Point Promotions:
    -- A value of type float can be converted to a value of type double.

NOTES:
    1. While integral promotion is value-preserving, it does not necessarily preserve the signedness (signed/unsigned) of the type. For example, an unsigned short is promoted to a signed int if signed int can represent all values of unsigned short.
    2. Some widening type conversions (such as char to short, or int to long) are NOT considered numeric promotions. They are numeric conversions, because they do not promote directly to int or double.


10.2 -- Numeric Conversions
--------------------------------------------------------------------------------
Numeric conversions encompass all numeric type conversions that are not numeric promotions. Unlike promotions, numeric conversions are not guaranteed to be safe or value-preserving.

Five Categories of Numeric Conversions:
    1. Converting an integral type to any other integral type (excluding integral promotions, e.g., int to long, or int to short).
    2. Converting a floating-point type to any other floating-point type (excluding floating-point promotions, e.g., double to float, or double to long double).
    3. Converting a floating-point type to any integral type (e.g., double to int).
    4. Converting an integral type to any floating-point type (e.g., int to double).
    5. Converting an integral or floating-point type to a bool (e.g., int to bool, double to bool).

Safety Classifications of Numeric Conversions:
    1. Value-preserving conversions (safe):
       The destination type can represent all values of the source type. No data or precision is lost.
       Examples: short to long, int to double (on 32-bit int architectures).
    2. Reinterpretive conversions (unsafe, but no data lost):
       The bit-pattern is preserved, but the meaning of the value changes.
       Examples: signed int to unsigned int, or unsigned int to signed int. A negative signed int (-5) converted to unsigned int wraps around via modulo arithmetic to a very large positive number (e.g., 4294967291 on 32-bit systems).
    3. Lossy conversions (unsafe, data or precision lost):
       The destination type cannot represent the source value accurately:
       -- Floating-point to integral: The fractional part is completely truncated (e.g., double 3.7 to int 3).
       -- Wider floating-point to narrower floating-point: Rounding or precision loss occurs (e.g., double to float).
       -- Wider integral to narrower integral: High-order bits are discarded, resulting in overflow or wrapping (e.g., int 300 to unsigned char 44).


10.3 -- Narrowing Conversions, List Initialization, and Constexpr Initializers
--------------------------------------------------------------------------------
A narrowing conversion is a potentially unsafe numeric conversion where the target type may not be able to hold all the values of the source type.

The Following Conversions Are Defined as Narrowing:
+-------------------------------+-------------------------------+---------------------------+------------------------------+
| Conversion Type               | Risk Description              | Allowed with constexpr?   | Example                      |
+-------------------------------+-------------------------------+---------------------------+------------------------------+
| Int <- Float                  | Fractional part lost          | ❌ (never allowed)        | int x = 3.14;                |
| Float <- Double               | Precision loss                | ✅ if value fits          | float f{3.0}; // OK          |
| Float <- Int                  | Large int may round           | ✅ if exactly representable| float f{42}; // OK           |
| Smaller Int / Sign Change<-Int| Overflow or reinterpretation  | ✅ if value fits exactly  | unsigned char c{100}; // OK  |
+-------------------------------+-------------------------------+---------------------------+------------------------------+

List Initialization ({}) Disallows Narrowing Conversions:
    One of the primary benefits of brace-initialization (list-initialization) introduced in C++11 is that it strictly prohibits narrowing conversions.

    Compile-time Constant (constexpr) vs. Runtime Variable Rules:
    -- If the source value is constexpr, and the destination type can represent the value exactly without data loss, the compiler allows the initialization:
       unsigned char c1 { 100 }; // OK: 100 is constexpr and fits in unsigned char [0, 255]
       float f1 { 3.0 };         // OK: 3.0 can be represented exactly as a float
    -- If the source value is constexpr, but the destination cannot represent it exactly -> compile error:
       unsigned char c2 { 300 }; // COMPILE ERROR: 300 overflows unsigned char
       int x { 3.14 };           // COMPILE ERROR: fractional part lost
    -- If the source value is NOT constexpr (a runtime variable), the compiler cannot verify if the value fits -> compile error under standard C++ list-initialization:
       int n { 100 };
       unsigned char c3 { n };   // COMPILE ERROR or warning: narrowing from int to unsigned char

Best Practice:
    Because narrowing conversions can be unsafe and are a frequent source of subtle bugs, avoid narrowing conversions whenever possible.
    If you must perform a narrowing conversion, use static_cast to explicitly document and enforce the conversion.


10.4 -- Arithmetic Conversions (Usual Arithmetic Conversions)
--------------------------------------------------------------------------------
In C++, binary operators (such as +, -, *, /, %, <, >, ==) require both of their operands to be of the exact same type. If you supply operands of different types, C++ does not evaluate them independently. Instead, it applies a standardized set of implicit conversions known as the Usual Arithmetic Conversions (UAC) to bring both operands to a common type before performing the operation.

Usual Arithmetic Conversions (UAC) Priority Rules:
    Step 1: Floating-point priority:
       -- If either operand is of type long double, the other operand is converted to long double.
       -- Otherwise, if either operand is of type double, the other operand is converted to double.
       -- Otherwise, if either operand is of type float, the other operand is converted to float.

    Step 2: Integral promotions:
       -- If neither operand is a floating-point type, integral promotions are applied to both operands (e.g., bool, char, short promote to int or unsigned int).

    Step 3: Mixed signed and unsigned integral rules:
       -- If both operands now have the same signedness (both signed or both unsigned), the operand with the narrower type is converted to the wider type.
       -- If the unsigned operand has equal or greater rank (size) than the signed operand, the signed operand is converted to the unsigned type!
          WARNING: This is a major C++ pitfall!
          Example: 5u - 10 evaluates by converting -10 to unsigned int, yielding 4294967291u.
          Example: (-3 < 5u) evaluates to false! Because -3 is converted to unsigned int 4294967293u, which is greater than 5u.
       -- If the signed type can represent all values of the unsigned type, the unsigned operand is converted to the signed type.
       -- Otherwise, both operands are converted to the unsigned counterpart of the signed type.

The typeid Operator:
    Use the typeid operator (defined in header <typeinfo>) to inspect the resulting type of an expression:
    std::cout << typeid(5u - 10).name() << '\n'; // prints mangled type (e.g., 'j' for unsigned int on GCC)
    To obtain human-readable type names on GCC/Clang, use abi::__cxa_demangle from <cxxabi.h>.


10.5 -- Explicit Type Conversion and Casting
--------------------------------------------------------------------------------
Explicit type conversion (casting) is performed when the programmer explicitly requests a conversion using a casting operator.

C++ supports 5 different types of casts:
    1. static_cast
    2. dynamic_cast
    3. const_cast
    4. reinterpret_cast
    5. C-style casts (legacy)
The first four are known as "named casts".

Comparison of Cast Types:
| Cast             | Description                                                                              | Safety Level           |
|------------------|------------------------------------------------------------------------------------------|------------------------|
| static_cast      | Compile-time type conversions between related types (e.g., numeric, upcast/downcast).     | Safe (checked)         |
| dynamic_cast     | Runtime-checked type conversions on pointers/references in polymorphic class hierarchies.| Safe (runtime checked) |
| const_cast       | Adds or removes const or volatile qualifiers.                                            | Unsafe (UB if modified)|
| reinterpret_cast | Reinterprets the raw bit pattern of an object as another type.                           | Unsafe (implementation)|
| C-style casts    | Performs a combination of static_cast, const_cast, and reinterpret_cast.                 | Highly Unsafe          |

Why C-Style Casts Should Be Avoided:
    -- Syntax: (double)x or double(x) (function-style cast).
    -- Inconsistent and dangerous behavior: A C-style cast attempts a static_cast; if that fails, it tries a const_cast, and if that fails, it performs a reinterpret_cast!
    -- Lack of clarity: The reader cannot know whether you intended a benign numeric conversion or a dangerous reinterpret cast.
    -- Searchability: C-style casts are extremely difficult to grep/search for in large codebases.
    -- Access bypass: One unique thing a C-style cast can do that C++ named casts disallow is casting a derived pointer to an inaccessible (privately inherited) base class pointer. This violates encapsulation and should never be used.

static_cast:
    Syntax: static_cast<new_type>(expression)
    -- static_cast<double>(x) produces a temporary object of type double containing the converted value.
    -- Provides compile-time type checking: If no valid conversion exists between the types, the compiler produces a compilation error.
    -- Prevents accidental dangerous conversions: Will not cast away const, and will not reinterpret raw pointers of unrelated types.
    -- Uses direct initialization: Explicit constructors of the target class type are considered.

Casting vs. Initializing a Temporary Object (static_cast<T>(x) vs. T{ x }):
    Three notable differences:
    1. Narrowing conversion handling:
       T{ x } uses list-initialization, which disallows narrowing conversions. If a conversion loses data (like 64-bit int to double, or double to int), T{ x } fails to compile.
       static_cast<T>(x) explicitly indicates that the conversion was intentional, permitting narrowing conversions.
    2. Intent and grep-ability:
       static_cast makes it immediately clear in code reviews and searches that an intentional type conversion is taking place.
    3. Simple type specifiers limitation:
       Direct-list-initialization of a temporary only allows single-word type names (simple type specifiers).
       int{ x } is valid syntax, but unsigned int{ x } is a syntax error. To use multi-word types, one must use static_cast<unsigned int>(x) or define a type alias.

Best Practice:
    Prefer static_cast over initializing a temporary object when a type conversion is desired.


10.6 -- Type Aliases
--------------------------------------------------------------------------------
A type alias is a user-defined identifier that acts as a synonym for an existing type. Type aliases do not introduce new, distinct types; they are merely alternate names for existing types.

Two Ways to Declare Type Aliases:
    1. typedef (legacy C/C++03):
       typedef double distance_t;
       typedef int (*FcnPtr)(double, char);
    2. using alias declaration (modern C++11 and preferred):
       using distance_t = double;
       using FcnPtr = int (*)(double, char);

Why the 'using' Syntax Is Preferred:
    -- Natural reading order: Follows the standard variable assignment syntax: name = value.
    -- Readability with complex types: For function pointers and arrays, the alias name in typedef is buried inside the definition, whereas using puts the alias name cleanly on the left.
    -- Template aliases: using declarations can be templated (alias templates), whereas typedef cannot:
       template <typename T>
       using StringMap = std::map<std::string, T>; // Valid
       // typedef cannot do this directly!

Scope of Type Aliases:
    Type aliases follow standard C++ scoping rules:
    -- Defined inside a function: local scope (only visible within that block).
    -- Defined inside a class/struct: class scope (accessed via ClassName::Alias).
    -- Defined in a namespace or global scope: visible across that namespace/translation unit.

Type Aliases Are NOT Distinct Strong Types:
    Because type aliases are pure synonyms, the compiler treats the alias and the underlying type identically.
    using Miles = double;
    using Kilometers = double;
    Miles m { 10.0 };
    Kilometers k { m }; // Compiles with NO errors! Type aliases do not provide type safety across units.


10.7 -- Type Deduction for Variables and Functions (auto)
--------------------------------------------------------------------------------
Type deduction (also known as type inference) is a feature where the compiler automatically deduces the type of an object or expression from its initializer at compile time.

Type Deduction for Objects:
    -- Basic syntax: auto x { 5 }; // x is deduced as int
    -- Requires an initializer: Type deduction will NOT work for objects without initializers or with empty initializers:
       auto a;     // COMPILE ERROR: initializer required
       auto b { }; // COMPILE ERROR: cannot deduce from empty brace
    -- Function call initializers: If a function returns a non-void type, auto deduces the returned type:
       auto sum { add(5, 6) }; // deduced as return type of add()
    -- Literal suffixes: Can be used to guide deduction:
       auto a { 1.23f }; // float
       auto b { 5u };    // unsigned int
       auto c { 10LL };  // long long

Type Deduction Modifiers and Const Dropping:
    -- auto drops top-level const:
       const int x { 5 };
       auto y { x };       // y is int (const is dropped!)
    -- Supplying const explicitly:
       const auto z { x }; // z is const int
    -- constexpr variables: A constexpr variable is implicitly const; auto drops this const unless explicitly reapplied:
       constexpr double cd { 3.4 };
       auto d { cd }; // d is double

Type Deduction for String Literals:
    -- C-style string literals deduce to const char*, NOT std::string:
       auto s1 { "Hello" }; // type is const char*
    -- To deduce std::string or std::string_view, use the literal suffixes s or sv from namespace std::literals:
       using namespace std::literals;
       auto s2 { "Hello"s };  // std::string
       auto s3 { "Hello"sv }; // std::string_view

Type Deduction for Functions (C++14):
    In C++14, the auto keyword can be used to deduce a function's return type from its return statement:
    auto add(int x, int y)
    {
        return x + y; // deduced as int
    }

    Function Return Type Deduction Restrictions:
    1. All return statements within the function must return expressions of the exact same type; otherwise, deduction fails:
       auto badFcn(bool b)
       {
           if (b) return 5;   // int
           else   return 6.7; // double -> COMPILE ERROR: inconsistent return types
       }
    2. Functions with deduced return types must be fully defined before they can be called. A forward declaration (auto foo();) is not sufficient for the compiler to deduce the return type at the call site.

Trailing Return Type Syntax (C++11):
    C++11 introduced the trailing return type syntax, where the return type is specified after the parameter list:
    auto add(int x, int y) -> int
    {
        return x + y;
    }
    This syntax is particularly beneficial when:
    -- The return type depends on the parameters (e.g., auto multiply(T t, U u) -> decltype(t * u)).
    -- Working with complex member function declarations or lambdas.

Type Deduction for Parameters (C++20 Abbreviated Function Templates):
    Prior to C++20, using auto for function parameter types was illegal.
    In C++20, auto parameters are permitted (e.g., void print(auto x) { ... }).
    IMPORTANT: In this context, auto does NOT perform ordinary type deduction. Instead, it triggers an "abbreviated function template", which is shorthand for template <typename T> void print(T x).


================================================================================
Directory Structure and Code Examples
================================================================================
Chapter folder: 10_TypeConversion_TypeAliases_TypeDeduction/

Top-level:
    1_understanding_conversion_n_need.cpp
    2_when_implicit_conversion_happens.cpp

10_1_Numeric_Promotions/
    3_floating_point_promotion.cpp
    4_Integral_promotions.cpp

10_2_Numeric_Conversions/
    5_Numeric_conversions.cpp
    6_Value_preserving_conversions.cpp
    7_Reinterpretive_conversions.cpp
    8_Lossy_conversions.cpp
    9_unsafe_misc.cpp

10_3_Narrowing_conversions_list_initialization_constexpr_initializers/
    1_float_to_int.cpp
    2_double_to_float.cpp
    3_int_to_float.cpp
    4_int_to_int.cpp

10_4_Arithmetic_conversions/
    1_typeid_example.cpp

10_5_Explicit_type_conversion/
    1_c_and_static_casting.cpp
    2_casting_vs_initializing_temp_obj.cpp
    3_simple_type_specifiers.cpp

10_6_Type_aliases_and_type_deduction/
    1_type_aliases.cpp
    2_type_deduction.cpp
    3_type_deduction_on_functions.cpp
    4_type_deduction_on_functions.cpp
    5_trailing_return_type.cpp
