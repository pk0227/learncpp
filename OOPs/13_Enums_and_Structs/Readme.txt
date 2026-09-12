================================================================================
Chapter 13: Enums and Structs
================================================================================

13.0 -- Introduction to Program-Defined (User-Defined) Types
--------------------------------------------------------------------------------
Fundamental types in C++ (such as int, double, bool, char) are insufficient to model complex real-world entities. C++ provides two primary categories of compound types that allow programmers to create program-defined (user-defined) types:
    1. Enumerated types (including unscoped and scoped enumerations)
    2. Class types (including structs, classes, and unions)

Key Rules for Program-Defined Types:
    -- Always terminate type definitions with a semicolon:
       struct Point { int x; int y; }; // Semicolon required!
       enum Color { red, green, blue }; // Semicolon required!
    -- Header Organization: A program-defined type used across multiple code files should be defined in a header file with the same name as the type (e.g. Point.h), and then #included into each source file as needed.
    -- One Definition Rule (ODR) Exemption: Type definitions are partially exempt from the One Definition Rule (ODR). Identical definitions of the same program-defined type can appear in multiple translation units without linker collision.


13.1 -- Unscoped Enumerations
--------------------------------------------------------------------------------
An enumeration (or enum) is a compound data type whose values are restricted to a fixed set of named symbolic constants called enumerators.

Key Concepts:
    -- Enumerator: A specific named symbolic constant belonging to the enumeration (e.g. red, green, blue).
    -- Full Definition Required: Because enumerations are program-defined types, each enumeration must be fully defined before use (a forward declaration enum Color; is not sufficient in older C++, though C++11 allows forward declaration if an explicit underlying type is specified).
    -- Implicitly constexpr: Enumerators are compile-time constants (constexpr).
    -- Distinct Types: Each enumerated type is a distinct type in the C++ type system.
    -- Pass and Return by Value: Because enumerations are small (integral-sized) and cheap to copy, they are typically passed and returned by value.
    -- Bit Flags: Enums are frequently used to define symbolic bit positions for use with std::bitset.

Scope of Unscoped Enumerations and Namespace Pollution:
    -- For unscoped enumerations (enum Color { red, green, blue };), the enumerator names are injected directly into the SAME scope where the enumeration itself is defined.
    -- If defined in the global namespace, red, green, and blue pollute the global namespace.
    -- This significantly increases the probability of naming collisions (e.g. enum Feeling { happy, tired, blue }; collides with Color::blue).
    -- Unscoped enumerations also provide a named scope region: enumerators can be accessed either unqualified (red) or qualified (Color::red).

Mitigating Naming Collisions with Unscoped Enums:
    1. Place unscoped enumerations inside a dedicated namespace (e.g. namespace ColorNames { enum Color { red, green, blue }; }).
    2. Place enums inside classes or structs where they logically belong (e.g. Monster::Type).

Enumerator Values and Value-Initialization:
    -- By default, the first enumerator is assigned value 0, and each subsequent enumerator is incremented by 1.
    -- Best Practice: Avoid assigning explicit integer values to enumerators unless modeling specific protocol codes or bit flags.
    -- Value-Initialization: If an enumeration is value-initialized (e.g. Color c{};), it is zero-initialized to 0, even if no enumerator was explicitly defined with value 0.

Conversions and Underlying Type (Base):
    -- Unscoped enumerations implicitly convert to integral values.
    -- Underlying Type: The specific integral type used to represent enumerator values in memory. By default, compilers typically choose int.
    -- Fixed Underlying Type (C++11): An explicit underlying type can be specified (e.g. enum Color : std::int8_t { Red, Green, Blue };), saving memory.
    -- No Implicit Integer-to-Enum Conversion: An integer will NOT implicitly convert to an unscoped enumeration:
       Color c { 2 }; // COMPILE ERROR: cannot convert int to Color
       Color c { static_cast<Color>(2) }; // OK: explicit static_cast required!

I/O Extraction Operator (>>):
    -- std::cin does not know how to input an enumeration directly.
    -- Workaround: Read into an integer variable, and then static_cast to the enum type (with range validation).


13.2 -- Converting Enumerations to and from Strings
--------------------------------------------------------------------------------
Because enumerators print as integers by default, converting enums to and from human-readable strings is a fundamental task.

Enum to String:
    -- Use a function returning std::string_view with a switch statement:
       std::string_view getColorName(Color c)
       {
           switch (c)
           {
               case Color::red:   return "red";
               case Color::green: return "green";
               case Color::blue:  return "blue";
               default:           return "unknown";
           }
       }

String to Enum:
    -- Use case-insensitive string comparison against known names, returning std::optional<Color> to represent potential lookup failure:
       std::optional<Color> getColorFromString(const std::string& str);


13.3 -- Overloading the I/O Operators for Enumerations
--------------------------------------------------------------------------------
By overloading operator<< and operator>>, enumerations can be streamed directly to and from std::cout and std::cin.

Overloading operator<< (Output):
    std::ostream& operator<<(std::ostream& out, Color c)
    {
        out << getColorName(c);
        return out;
    }

Overloading operator>> (Input):
    std::istream& operator>>(std::istream& in, Color& c)
    {
        std::string str;
        in >> str;
        auto match { getColorFromString(str) };
        if (match)
            c = *match;
        else
            in.setstate(std::ios_base::failbit); // Flag input failure
        return in;
    }


13.4 -- Scoped Enumerations (Enum Classes)
--------------------------------------------------------------------------------
Although unscoped enumerations are distinct types, they lack type safety because they implicitly convert to integers. Consequently, two different unscoped enum types (e.g. Color::red and Fruit::banana) can be compared with == if both have value 0!

The Solution: Scoped Enumerations (enum class):
    Declared using enum class (or enum struct, though enum class is idiomatic):
    enum class Color { red, green, blue };
    enum class Fruit { banana, apple };

Key Properties of Scoped Enumerations:
    1. No Implicit Integer Conversions: Scoped enumerations do NOT implicitly convert to integers or bools.
       Color c { Color::red };
       // int n = c;           // COMPILE ERROR: no implicit conversion
       // if (c == Fruit::apple) // COMPILE ERROR: cannot compare different enum classes!
    2. Strongly Scoped Enumerators: Enumerators are NOT placed in the enclosing scope. They MUST be accessed with scope qualification: Color::red.
    3. Built-In Namespacing: Eliminates namespace pollution and naming collisions.
    4. Explicit Integer Conversion: Converting a scoped enum to an integer requires an explicit cast:
       int val { static_cast<int>(c) };
       In C++20: static_cast<std::underlying_type_t<Color>>(c)
       In C++23: std::to_underlying(c)
    5. Direct-list-initialization from integer (since C++17):
       Color c { 1 }; // OK in C++17 (direct list-init from integral value without static_cast)
       // c = 1;     // COMPILE ERROR: copy assignment from int is still disallowed
    6. 'using enum' Statement (C++20):
       using enum Color; imports all enumerators of Color into the local scope, allowing unqualified access (red) within that block.

Best Practice:
    Favor scoped enumerations (enum class) over unscoped enumerations unless interacting with legacy C APIs.


13.5 -- Structs and Aggregate Initialization
--------------------------------------------------------------------------------
A struct (short for structure) is a program-defined compound type that groups together one or more variables (called member variables or data members).

Aggregate Definition:
    An aggregate is a type that has:
    -- No user-declared or inherited constructors (prior to C++20; in C++20, no user-declared constructors at all)
    -- No private or protected non-static data members
    -- No virtual base classes or virtual member functions

Aggregate Initialization (List Initialization):
    Aggregates allow direct initialization of their members using braced initializer lists {}:
    struct Fraction { int numerator; int denominator; };
    Fraction f1 { 3, 4 }; // numerator = 3, denominator = 4

Missing Initializers in List Initialization:
    If an aggregate is initialized with fewer values than members:
    -- Members with explicit initializers use those values.
    -- Members without explicit initializers use their default member initializer (if one exists).
    -- If no default member initializer exists, the member is value-initialized (zeroed for fundamental types).

Designated Initializers (C++20):
    C++20 introduced designated initializers to explicitly map initialization values to member names:
    struct Point { int x{ 0 }; int y{ 0 }; int z{ 0 }; };
    Point p { .x = 10, .z = 30 }; // y is value-initialized to 0!
    CRITICAL RULE: Designated initializers MUST follow the exact declaration order of members in the struct; out-of-order initializers cause a compilation error.

Assignment:
    Structs can be assigned using initializer lists or designated initializers:
    p = { .x = 5, .y = 15 };


13.6 -- Default Member Initialization
--------------------------------------------------------------------------------
Non-static data members can be given a default value in the struct definition:
struct Employee
{
    int id { 0 };
    double wage { 15.0 };
};

Precedence Hierarchy for Member Initialization:
    1. If an explicit initialization value is provided in {}, that explicit value is used.
    2. If an initializer is omitted and a default member initializer exists, the default value is used.
    3. If an initializer is omitted and no default member initializer exists, value-initialization (zeroing) occurs.
    4. If the struct is default-initialized without braces (Employee e;), members without default member initializers remain UNINITIALIZED (garbage values).

Best Practice:
    Always provide default member initializers for all members, and prefer value-initialization (Point p{};) over default-initialization (Point p;).


13.7 -- Passing and Returning Structs
--------------------------------------------------------------------------------
Passing Structs to Functions:
    -- Structs can contain large amounts of data; passing by value incurs substantial copying overhead.
    -- Best Practice: Pass structs by const lvalue reference (const StructName&) unless the function needs to modify a local copy.

Passing Temporary Structs:
    -- Temporary structs (prvalues) can be passed directly to functions:
       printEmployee(Employee{ 1, 25.0 }); // Direct temporary
       printEmployee({ 1, 25.0 });         // Implicit temporary from braced list
    -- Temporary objects are destroyed at the end of the full expression containing the function call.
    -- Temporaries can only bind to parameters accepting rvalues: pass by value or pass by const reference (cannot bind to non-const reference!).

Returning Structs:
    -- Functions can return structs by value efficiently thanks to Return Value Optimization (RVO) and move semantics.
    -- Modern C++ idiom: return structs from functions to return multiple values cleanly, unpacked via C++17 structured binding:
       auto [id, wage] = getEmployee();


13.8 -- Struct Miscellany and Member Selection
--------------------------------------------------------------------------------
Member Selection Operators:
    -- Use the member selection operator (.) when accessing members from a struct object or reference: employee.id.
    -- Use the member selection from pointer operator (->) when accessing members through a pointer: empPtr->id (shorthand for (*empPtr).id).

Struct Size and Memory Alignment / Padding:
    -- The size of a struct is NOT necessarily the sum of the sizes of its individual members!
    -- CPU hardware accesses memory much faster when data types are aligned to memory addresses that are multiples of their size (e.g. 4-byte ints on 4-byte boundaries, 8-byte doubles on 8-byte boundaries).
    -- Compilers insert invisible padding bytes between members or at the end of the struct to satisfy alignment requirements.
    -- Best Practice: Declare struct members in decreasing order of size (e.g. 8-byte doubles first, then 4-byte ints, then 1-byte chars) to minimize padding overhead.


13.9 -- Class Templates (Struct Templates)
--------------------------------------------------------------------------------
Just as function templates allow writing generic functions, class templates allow writing generic structs and classes.

Motivation:
    -- Aggregate types cannot be overloaded like functions. If you need a Pair of ints, a Pair of doubles, and a Pair of strings, defining separate structs (IntPair, DoublePair) is redundant and unmaintainable.

Syntax:
    template <typename T>
    struct Pair
    {
        T first;
        T second;
    };

    Pair<int> p1 { 1, 2 };
    Pair<double> p2 { 3.4, 5.6 };

Multiple Template Type Parameters:
    template <typename T, typename U>
    struct Pair
    {
        T first;
        U second;
    };
    Pair<int, double> p3 { 1, 2.5 };

Standard Library Pair (std::pair):
    C++ provides std::pair in header <utility>, which is implemented as a class template with two members: first and second.


13.10 -- Class Template Argument Deduction (CTAD) and Deduction Guides
--------------------------------------------------------------------------------
Starting in C++17, when instantiating an object from a class template, the compiler can automatically deduce template type arguments from the constructor or aggregate initializer arguments:
    std::pair p { 1, 2.5 }; // Deduces std::pair<int, double>

Key Rules of CTAD:
    1. CTAD is only triggered if NO template argument list is provided (e.g. std::pair p; not std::pair<> p).
    2. Literal suffixes can be used to guide deduction (e.g. Pair p{ 1.0f, 2.0f } deduces float).
    3. Deduction Guides in C++17:
       In C++17, CTAD worked automatically for classes with constructors, but NOT for aggregate structs!
       To enable CTAD for aggregate class templates in C++17, a deduction guide is required:
       template <typename T, typename U>
       Pair(T, U) -> Pair<T, U>;
    4. C++20 Aggregate CTAD:
       In C++20, the compiler automatically generates deduction guides for aggregates, rendering manual deduction guides unnecessary in most cases.

CTAD Limitations and Restrictions:
    -- CTAD does NOT work for non-static member initialization in class definitions:
       struct Foo {
           // std::pair p { 1, 2 }; // COMPILE ERROR: CTAD not allowed for non-static members
           std::pair<int, int> p { 1, 2 }; // OK
       };
    -- CTAD does NOT work for function parameter types:
       // void print(std::pair p); // COMPILE ERROR: cannot use template without arguments as parameter (use auto or template <typename T, typename U> instead)


================================================================================
Directory Structure and Code Examples
================================================================================
Chapter folder: 13_Enums_and_Structs/

13_1_Unscoped_enumerations/
    1_unscoped_enumerations.cpp
    2_enums_as_parameters.cpp
    3_bit_flags_enums.cpp
    4_scope_and_collisions.cpp
    5_enum_namespace_avoid_collisions.cpp
    6_enum_implicit_conversion.cpp
    7_underlying_type_base.cpp
    8_underlying_type_base_implicit_conversion.cpp

13_2_converting_enumeration_to_n_from_string/
    1_enum_to_string.cpp
    2_string_to_enum.cpp

13_3_Overloading_the_I_O_operators/
    1_i_o_overloading_input_is_number.cpp
    1_i_o_overloading_input_is_string.cpp

13_4_Scoped_enumerations_enum_classes/
    1_need_of_scoped_enums.cpp
    2_no_implicit_conversion.cpp
    3_using_enum.cpp

13_5_Struct_aggregate_initialization/
    1_initializer_as_aggregate.cpp
    1_initializer_as_no_aggregate.cpp
    2_const_class_types_struct.cpp
    3_designated_initializer.cpp
    4_assignment_with_initializer_list.cpp
    5_assignment_with_designated_initializers.cpp
    6_initializing_struct_with_another_struct_of_same_type.cpp

13_6_Default_member_initialization/
    1_initialization_possibilities.cpp

13_7_Passing_and_returning_structs/
    1_passing_temp_structs.cpp
    2_returning_temp_struct.cpp

13_8_Struct_miscellany/
    1_miscellany.cpp
    2_miscellany.cpp
    3_miscellany.cpp

13_9_Class_templates/
    1_aggregate_types_need_templates.cpp
    2_templates_on_aggregate.cpp
    3_templates_on_aggregate.cpp
    4_template_on_std_pair.cpp
    4_templates_on_more_than_one_aggregate.cpp

13_10_Class_template_argument_deduction_CTAD/
    1_CTAD_with_suffix.cpp
    2_CTAD_deduction_guide.cpp
    3_CTAD_deduction_guide.cpp
    4_CTAD_template_default_parameters.cpp
    5_CTAD_with_non_static_members.cpp
    6_CTAD_with_function_parameters.cpp
