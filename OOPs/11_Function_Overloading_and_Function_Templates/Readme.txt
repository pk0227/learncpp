================================================================================
Chapter 11: Function Overloading and Function Templates
================================================================================

11.1 -- Function Overload Differentiation
--------------------------------------------------------------------------------
Function overloading allows us to create multiple functions with the same name, so long as each function can be differentiated by the compiler.

Differentiating Factors for Overloaded Functions:
    1. Number of parameters: Functions with different parameter counts are differentiated.
    2. Type of parameters: Functions with different parameter types are differentiated.
       -- Parameter types include ellipses (...).
       -- Parameter types EXCLUDE typedefs and type aliases (as they are synonyms, not distinct types).
       -- Parameter types EXCLUDE top-level const qualifiers on value parameters (e.g. void print(int) and void print(const int) are identical declarations).
    3. Function-level qualifiers (for member functions):
       -- const and volatile qualifiers (e.g. void print() const vs void print()).
       -- Ref-qualifiers (e.g. void print() & vs void print() &&).

Non-Differentiating Factors:
    -- Return type: Return type is NOT a differentiating factor. You cannot overload functions that differ solely by return type.
    -- Typedefs and type aliases: Do not create distinct types for overloading.
    -- Top-level const on pass-by-value parameters: Ignored in overload differentiation.

NOTE: Return type is not a differentiating factor. So, typedefs, type aliases, const qualifier on value parameters, and return types are not function overloading differentiators.


11.2 -- Function Overload Resolution and Ambiguous Matches
--------------------------------------------------------------------------------
With overloaded functions, there can be many functions with the same name. That function either matches (or can be made to match after type conversions are applied), or it doesn’t (and a compile error results).

Since a function call can only resolve to one of them, the compiler has to determine which overloaded function is the best match. The process of matching function calls to a specific overloaded function is called overload resolution.

Resolving Overloaded Function Calls:
    When a function call is made to an overloaded function, the compiler steps through a sequence of rules to determine which (if any) of the overloaded functions is the best match.
    At each step, the compiler applies a specific set of TYPE CONVERSIONS to the argument(s) in the function call. For each conversion applied, the compiler checks if any of the overloaded functions are now a match. 
    After all the different type conversions have been applied and checked for matches, the step is done. 
       
    The result of any step will be one of three possible outcomes:
    1. No matching functions were found. The compiler moves to the next step in the sequence.
    2. A single matching function was found. This function is considered to be the best match. The matching process is now complete, and subsequent steps are not executed.
    3. More than one matching function was found. The compiler will issue an ambiguous match compile error.

    If the compiler reaches the end of the entire sequence without finding a match, it will generate a compile error.

The 6-Step Overload Resolution Sequence:
    Step 1) Exact match:
        The compiler tries to find an exact match. If no exact match is found, the compiler applies a number of TRIVIAL conversions to the arguments in the function call:
        -- lvalue to rvalue conversions
        -- qualification conversions (e.g. non-const to const)
        -- non-reference to reference conversions

    Step 2) Numeric promotions:
        If no exact match is found, the compiler tries to find a match by applying NUMERIC PROMOTIONS to the argument(s).
        Narrow integral and floating-point types can be automatically PROMOTED to wider types, such as int or double (e.g. char to int, bool to int, float to double).

    Step 3) Numeric conversions:
        If no match is found via numeric promotion, the compiler tries to find a match by applying NUMERIC CONVERSIONS to the arguments (e.g. int to double, double to int, long to int).

    Precedence Rule:
        Sometimes, for a given argument, there may be overloaded functions with parameter types that are exact matches, promotable, or convertible. In such cases, the precedence order is:
        Exact match > Promotion > Conversion

    Step 4) User-defined conversions:
        If no match is found via numeric conversion, the compiler tries to find a match through any USER-DEFINED conversions.
        The constructor of a class (converting constructor) or a user-defined conversion operator (operator T()) acts as a user-defined conversion from other types to that class type or vice versa, and can be used during this step to find matching functions.

    Step 5) Ellipsis:
        If no match is found via user-defined conversion, the compiler will look for a matching function that uses ellipsis (...).

    Step 6) Compile error:
        If no matches have been found by this point, the compiler gives up and issues a compile error stating that no matching function could be found.

Ambiguous Matches:
    If multiple overloaded functions are equally viable at any given step, the compiler halts and emits an ambiguous match error (e.g. calling print(1L) when only print(int) and print(double) exist, because long to int and long to double are both numeric conversions of equal rank).
    Resolution: Disambiguate using explicit casts (static_cast) or distinct literal suffixes (e.g. 0u, 3.14f).


11.3 -- Default Arguments
--------------------------------------------------------------------------------
A default argument is an argument provided in a function declaration that is automatically assigned by the compiler if the caller does not provide one.

Key Properties of Default Arguments:
    -- Default arguments are inserted by the compiler at the site of the function call.
    -- Default arguments are useful when adding new parameters to existing functions: they prevent breaking old function calls by providing a default value, allowing existing code to work unchanged while letting new code pass a custom argument if needed.

Multiple Default Arguments (The Right-to-Left Rule):
    -- If a parameter is given a default argument, all subsequent parameters (to the right) must also be given default arguments:
       void print(int x = 10, int y = 20, int z = 30); // OK
       // void print(int x = 10, int y, int z = 30);   // COMPILE ERROR: y must have a default argument

Redeclaration and Location Rules:
    -- Default arguments cannot be redeclared, and must be declared before use: the default argument can be declared in either the forward declaration or the function definition, but NOT both.
    -- Best practice: Declare the default argument in the forward declaration / header file and not in the function definition, as the forward declaration is more likely to be seen by other files and included before use.
    -- Splitting default arguments across forward declarations: In the same translation unit, subsequent forward declarations can add default arguments to parameters that did not have them previously, provided they are added from right to left.

Default Arguments and Function Overloading:
    -- Default arguments do NOT differentiate overloaded functions.
    -- Default arguments can lead to ambiguous matches when default values allow multiple overloads to be called with the same number of arguments:
       void print(int x = 100);
       void print(double d = 123.45);
       // print(); // COMPILE ERROR: ambiguous call!
    -- Default arguments do NOT work for functions called through function pointers, because function pointers only track the function signature, not default argument values.


11.4 -- Function Templates
--------------------------------------------------------------------------------
A function template is a function-like definition that is used to generate one or more overloaded functions, each with a different set of actual types.

Terminology:
    -- Primary template: The initial function template definition that is used to generate other functions.
    -- Instantiated function (or specialization / function instance): A concrete function generated from the primary template by substituting concrete types for template parameters.
    -- Template types / type template parameters: Placeholder types used in the template parameter list (e.g. typename T or class T).

Template Parameters:
    C++ supports 3 different kinds of template parameters:
    1. Type template parameters (where the parameter represents a type).
    2. Non-type template parameters (where the parameter represents a constexpr value).
    3. Template template parameters (where the parameter represents a template).

Naming Template Parameters:
    -- Use a single capital letter starting with T (e.g. T, U, V, etc.) to name type template parameters that are used in trivial or obvious ways and represent "any reasonable type".
    -- If the type template parameter has a non-obvious usage or specific requirements that must be met, a more descriptive name is warranted (e.g. Allocator or TAllocator).

Function Template Instantiation:
    -- The process of creating functions (with specific types) from function templates is called function template instantiation.
    -- Implicit instantiation occurs when a function is instantiated as the result of a function call.
    -- A function template is only instantiated the first time a function call is made with a specific set of types in each translation unit. Further calls to the function with matching types are routed to the already-instantiated function.
    -- Instantiated functions are implicitly inline, allowing them to exist in multiple translation units without ODR violations.

Template Argument Deduction:
    -- The compiler inspects the types of function call arguments to deduce the required template type arguments automatically (e.g. max(5, 6) deduces T = int).
    -- Non-template preference: The normal function call syntax will prefer a non-template function over an equally viable function instantiated from a template.
    -- Best practice: Favor the normal function call syntax when making calls to a function instantiated from a function template (unless you explicitly need the template version preferred over a non-template function).

Caveats and Restrictions on Templates:
    -- Instantiated functions may not always compile: If the template uses operators (like + or <<) that the instantiated type does not support (e.g. calling addOne(std::string) when operator+(int) is used), compilation fails during template instantiation.
    -- Instantiated functions may not always make sense semantically (e.g. adding raw pointer types).
    -- Modifiable static local variables: Each instantiated function specialization receives its OWN independent static local variable! Static local variables are NOT shared across distinct instantiations of the template.
    -- Function templates can have non-template parameters and default arguments for non-template parameters.

Function Template Specialization and Deleted Functions (= delete):
    -- In cases where a specific type should not be callable, we can define a function template specialization and mark it with = delete:
       template <>
       const char* addOne(const char*) = delete;
    -- = delete means "I forbid this", not "this doesn't exist".
    -- Deleted functions participate in all stages of overload resolution. If a deleted function is chosen as the best match, a compilation error results.

Generic Programming:
    -- Because template types can be replaced with any actual type, template types are sometimes called generic types.
    -- Programming with templates is called generic programming.
    -- Use function templates to write generic code that can work with a wide variety of types whenever you have the need.


11.5 -- Function Templates with Multiple Template Types
--------------------------------------------------------------------------------
Template argument deduction does NOT perform type conversions. If a function template has a single type parameter T:
    template <typename T>
    T max(T x, T y);
Calling max(5, 6.7) fails because the compiler cannot determine whether T should be int or double!

Three Workarounds for Single-Template Type Deduction Conflicts:
    1. Use static_cast to explicitly convert arguments to matching types:
       max(static_cast<double>(5), 6.7);
    2. Provide an explicit template argument:
       max<double>(5, 6.7); // T is explicitly double; 5 converts via numeric conversion
    3. Use multiple template type parameters:
       template <typename T, typename U>
       auto max(T x, U y) -> ...

Multiple Template Type Parameters and Return Type Deduction:
    When using multiple type parameters, the return type must be determined:
    -- In C++14: Use auto return type deduction (e.g. auto max(T x, U y) { return (x < y) ? y : x; }). Note: if conditional operator evaluates to common type, auto deduces that common type.
    -- In C++11: Use trailing return type with std::common_type_t (e.g. auto max(T x, U y) -> std::common_type_t<T, U>).

Abbreviated Function Templates (C++20):
    In C++20, using auto as a function parameter type triggers an abbreviated function template:
    void print(auto x, auto y)
    {
        std::cout << x + y << '\n';
    }
    This is shorthand for:
    template <typename T, typename U>
    void print(T x, U y) { ... }
    -- Each auto parameter receives an INDEPENDENT template type parameter.
    -- There is no concise way to enforce that multiple auto parameters share the same type. For that, standard template syntax (template <typename T> void print(T x, T y)) must be used.

Function Template Overloading and Partial Ordering:
    -- Function templates can be overloaded with non-template functions and other function templates.
    -- The compiler uses "partial ordering of function templates" to determine which template is more specialized and should be preferred.


11.6 -- Non-type Template Parameters (NTTP)
--------------------------------------------------------------------------------
A non-type template parameter is a template parameter with a fixed type that serves as a placeholder for a constexpr value passed in as a template argument.

Permitted Non-Type Template Parameter Types:
    1. An integral type (e.g. int, char, size_t)
    2. An enumeration type
    3. std::nullptr_t
    4. A floating-point type (since C++20)
    5. A pointer or reference to an object
    6. A pointer or reference to a function
    7. A pointer or reference to a member function
    8. A literal class type (structural type, since C++20)

Primary Uses of Non-Type Template Parameters:
    -- Passing compile-time constant values to functions or classes.
    -- Example: std::bitset<8> bits{ 0b0000'0101 }; // <8> is a non-type template parameter defining the bit width.
    -- Fixed-size buffer or array classes (e.g. std::array<T, N>).

Allowed Conversions for Non-Type Template Arguments:
    Only a restricted subset of constexpr conversions are permitted for NTTP arguments:
    -- Integral promotions (e.g. char to int)
    -- Integral conversions (e.g. char to long or int to char)
    -- User-defined conversions to integral types
    -- Lvalue to rvalue conversions

Ambiguity Pitfall with Non-Type Template Arguments:
    Because integral conversions are permitted for non-type arguments, overloaded templates with different integral NTTP types (e.g. print<int N>() and print<char C>()) will result in ambiguous call errors when passed literals like print<5>() or print<'A'>().

Type Deduction for Non-Type Template Parameters (C++17):
    C++17 introduced template <auto N>, allowing the compiler to deduce the type of the non-type template parameter automatically:
    template <auto N>
    void printValue()
    {
        std::cout << N << '\n';
    }
    printValue<5>();   // N is int 5
    printValue<'c'>(); // N is char 'c'


11.7 -- Using Function Templates in Multiple Files
--------------------------------------------------------------------------------
The One Definition Rule (ODR) and Function Templates:
    -- Normal functions cannot have their definitions placed in header files without the inline specifier, otherwise linking multiple source files that include the header causes duplicate symbol link errors.
    -- Template definitions are EXEMPT from the single-definition-per-program ODR rule: having identical template definitions #included into multiple source files is completely valid.
    -- Functions implicitly instantiated from function templates are IMPLICITLY INLINE: they can be instantiated and defined in multiple translation units, provided each definition is identical.
    -- The templates themselves are not inline (inline only applies to functions and variables), but the instantiated functions are.

Best Practice for Template Organization:
    -- Place function template definitions entirely in header files (.h).
    -- #include the template header in any source file that needs to instantiate or call the template.
    -- Do not place function template definitions in .cpp files unless they are only used within that single .cpp file.


================================================================================
Directory Structure and Code Examples
================================================================================
Chapter folder: 11_Function_Overloading_and_Function_Templates/

11_1_Function_overload_differentiation/
    1_no_of_params.cpp
    2_type_of_params.cpp
    3_non_differentiators.cpp

11_2_Function_overload_resolution_and_ambiguous_matches/
    1_trivial-conversion.cpp
    2_numeric_promotions.cpp
    3_numeric_conversions.cpp
    4_precedence.cpp
    5_user-defined-conversion.cpp
    6_ambiguity.cpp
    7_ambiguity.cpp

11_3_Default_arguments/
    1_multiple_default_args.cpp
    2_multiple_default_args.cpp
    3_default_args_redeclaration.cpp
    4_default_args_redeclaration.cpp
    5_default_args_redeclaration.cpp
    6_default_args_redeclaration_headers.cpp
    7_default_args_function_overloading.cpp
    8_default_args_ambiguous_matches.cpp
    9_default_args_ambiguous_matches.cpp
    foo.h

11_4_Function_templates/
    1_function_template.cpp
    2_function_template_equivalent_insight.cpp
    3_function_instatiated_only_once.cpp
    4_function_instatiated_only_once_equivalent_insight.cpp
    5_template_argument_deduction.cpp
    6_function_templates_with_non-template_parameters.cpp
    7_instantiated_functions_may_not_always_compile.cpp
    8_Instantiated_functions_may_not_always_make_sense_semantically.cpp
    9_template_specialization_delete_function.cpp
    10_function_templates_n_default_arguments_for_non-template_parameters.cpp
    11_function_templates_with_modifiable_static_local_variables.cpp

11_5_Function_templates_with_multiple_template_types/
    1_multiple_template_types_need_n_workarounds.cpp
    2_multiple_template_type_parameters.cpp
    3_multiple_template_type_parameters_with_auto.cpp
    4_issue_with_auto_type_deduction.cpp
    5_abbreviated_function_templates.cpp
    6_function_templates_overload.cpp

11_6_Non-type_template_parameters/
    1_non-type_template_parameters.cpp
    2_our_own_non-type_template_parameters.cpp
    3_non-type_template_parameters_use.cpp
    4_Implicit_conversions_for_non-type_template_arguments.cpp
    5_Implicit_conversions_non-type_template_arguments_ambiguity.cpp
    6_type_deduction_non-type_template_parameters.cpp

11_7_Using_function_templates_in_multiple_files/
    add.h
    main.cpp
    implicitly_inline/
        foo.cpp
        main.cpp
        max.h