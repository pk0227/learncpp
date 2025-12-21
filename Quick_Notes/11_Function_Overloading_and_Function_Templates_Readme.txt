Key topics of this chapter.
--------------------------------------------
1. Differentiating factors are
    a. Number of parameters.
    b. Type of parameters.          (Excludes typedefs, type aliases & const qualifiers. Includes ellipses)
    c. Function-level qualifiers    (For member functions)
        a. const/volatile 
        b. ref-qualifiers

NOTE : return type is not Differentiating factor. So, typedefs, type aliases, const qualifier and return types 
are not function overloading differentiators.

2. Overload resolution
    -- With overloaded functions, there can be many functions That function either matches (or can be made to match after type conversions are applied), 
       or it doesn’t (and a compile error results).
    -- Since a function call can only resolve to one of them, the compiler has to determine which overloaded function is the best match. 
The process of matching function calls to a specific overloaded function is called overload resolution.

Resolving overloaded function calls
    -- When a function call is made to an overloaded function, the compiler steps through a sequence of rules to determine which (if any) of the overloaded functions is the best match.
    -- At each step, the compiler applies a bunch of different TYPE CONVERSIONS to the argument(s) in the function call. For each conversion applied, the compiler checks if any of the overloaded functions are now a match. 
       After all the different type conversions have been applied and checked for matches, the step is done. 
       
       The result will be one of three possible outcomes:
            -- No matching functions were found. The compiler moves to the next step in the sequence.
            -- A single matching function was found. This function is considered to be the best match. The matching process is now complete, and subsequent steps are not executed.
            -- More than one matching function was found. The compiler will issue an ambiguous match compile error.     

        If the compiler reaches the end of the entire sequence without finding a match, it will generate a compile error.

Step 1) The compiler tries to find an exact match. If no exact match found,
        the compiler will apply a number of TRIVIAL conversions to the arguments in the function call.

        The TRIVIAL conversions 
            -- are a set of specific conversion rules that will modify types (without modifying the value) for purposes of finding a match.
            -- These include:
                -- lvalue to rvalue conversions
                -- qualification conversions (e.g. non-const to const)
                -- non-reference to reference conversions

Step 2) If no exact match is found, the compiler tries to find a match by applying NUMERIC PROMOTIONS to the argument(s).
        narrow integral and floating point types can be automatically PROMOTED to wider types, such as int or double.

Step 3) If no match is found via numeric promotion, the compiler tries to find a match by applying NUMERIC CONVERSIONS to the arguments.

Note: Sometimes, for a given argument, there may be overloaded functions with parameter types 
      that are exact matches, promotable, or convertible. In such cases, 
      the precedence order is:

      Exact match > Promotion > Conversion

Step 4) If no match is found via numeric conversion, the compiler tries to find a match through any USER-DEFINED conversions.
        The constructor of a class also acts as a user-defined conversion from other types to that class type, and can be used during this step to find matching functions.
    
Step 5) If no match is found via user-defined conversion, the compiler will look for a matching function that uses ellipsis.

Step 6) If no matches have been found by this point, the compiler gives up and will issue a compile error about not being able to find a matching function.


3. Default arguments
    -- Default arguments are inserted by the compiler at site of the function call.
    -- Default arguments are useful when adding new parameters to existing functions — 
       they prevent breaking old function calls by providing a default value, allowing existing code to work unchanged while letting new code pass a custom argument if needed.
    -- Multiple default arguments : If a parameter is given a default argument, all subsequent parameters (to the right) must also be given default arguments.
    -- Default arguments can not be redeclared, and must be declared before use : the default argument can be declared in either the forward declaration or the function definition, but not both.
       The best practice is to declare the default argument in the forward declaration and not in the function definition, as the forward declaration is more likely to be seen by other files and included before use (particularly if it’s in a header file).
    -- Default arguments and function overloading.
    -- Default arguments can lead to ambiguous matches.
    -- Default arguments don’t work for functions called through function pointers.

4. Function templates
    -- A function template is a function-like definition that is used to generate one or more overloaded functions, each with a different set of actual types.
    -- Primary template : The initial function template that is used to generate other functions is called the primary template.
    -- Instantiated function : The functions generated from the primary template are called instantiated functions.
    -- In a primary function template, we use placeholder types (technically called type template parameters), (informally called template types) 
    -- C++ supports 3 different kinds of template parameters:
        -- Type template parameters (where the template parameter represents a type).
        -- Non-type template parameters (where the template parameter represents a constexpr value).
        -- Template template parameters (where the template parameter represents a template).
    -- Template parameter declaration : defines any template parameters that will be subsequently used.
    -- Naming template parameters
        -- Use a single capital letter starting with T (e.g. T, U, V, etc…) to name type template parameters that are used in trivial or obvious ways and represent “any reasonable type”.
        -- If the type template parameter has a non-obvious usage or specific requirements that must be met, then a more descriptive name is warranted (e.g. Allocator or TAllocator).

5. Function template instantiation:
    -- The process of creating functions (with specific types) from function templates (with template types) is called function template instantiation 
    -- Specialization : A function that is instantiated from a template is technically called a specialization, but in common language is often called a function instance. 
    -- Implicit instantiation : When a function is instantiated due to a function call, it’s called implicit instantiation.
    -- A function template is only instantiated the first time a function call is made in each translation unit. Further calls to the function are routed to the already instantiated function.
    -- Template argument deduction
        -- The normal function call syntax will prefer a non-template function over an equally viable function instantiated from a template.
        -- Favor the normal function call syntax when making calls to a function instantiated from a function template.
           (unless you need the function template version to be preferred over a matching non-template function).
    -- Function templates with non-template parameters.
    -- Instantiated functions may not always compile.
    -- Instantiated functions may not always make sense semantically.
    -- Function template specialization
        -- We can tell the compiler that instantiation of function templates with certain arguments should be disallowed. This is done by using function template specialization, 
           which allow us to overload a function template for a specific set of template arguments, along with = delete
        -- Delete Function
            -- In cases where we have a function that we explicitly do not want to be callable, we can define that function as deleted by using the = delete specifier.
            -- = delete means “I forbid this”, not “this doesn’t exist”.
            -- Deleted function participate in all stages of function overload resolution (not just in the exact match stage). If a deleted function is selected, then a compilation error results.
    -- Function templates and default arguments for non-template parameters.
    -- Beware function templates with modifiable static local variables.
    -- Generic programming
        -- Because template types can be replaced with any actual type, template types are sometimes called generic types. 
           Because templates can be written agnostically of specific types, programming with templates is sometimes called generic programming. 
        -- Use function templates to write generic code that can work with a wide variety of types whenever you have the need.

6. Function templates with multiple template types:
    -- Type conversion is done only when resolving function overloads, not when performing template argument deduction.
        -- Use static_cast to convert the arguments to matching types.
        -- Provide an explicit type template argument
    -- Function templates with multiple template type parameters.
    -- Function templates with type deduction on return type, parameters type.
    -- Abbreviated function templates.
        -- There isn’t a concise way to use abbreviated function templates when you want more than one auto parameter to be the same type.
        -- Feel free to use abbreviated function templates with a single auto parameter, or where each auto parameter should be an independent type (and your language standard is set to C++20 or newer).
    -- Function templates may be overloaded
        -- The rules for determining which of multiple matching function templates should be preferred are called “partial ordering of function templates”.

7. Non-type template parameters
    -- A non-type template parameter is a template parameter with a fixed type that serves as a placeholder for a constexpr value passed in as a template argument.
    -- A non-type template parameter can be any of the following types:
        -- An integral type
        -- An enumeration type
        -- std::nullptr_t
        -- A floating point type (since C++20)
        -- A pointer or reference to an object
        -- A pointer or reference to a function
        -- A pointer or reference to a member function
        -- A literal class type (since C++20)
    -- Non-type template parameters are used primarily when we need to pass constexpr values to functions (or class types),
       so they can be used in contexts that require a constant expression.
    -- The class type std::bitset uses a non-type template parameter to define the number of bits to store 
       because the number of bits must be a constexpr value.
       std::bitset<8> bits{ 0b0000'0101 }; // The <8> is a non-type template parameter
    
    -- In this context, only certain types of constexpr conversions are allowed. The most common types of allowed conversions include:
        -- Integral promotions (e.g. char to int)
        -- Integral conversions (e.g. char to long or int to char)
        -- User-defined conversions (e.g. some program-defined class to int)
        -- Lvalue to rvalue conversions (e.g. some variable x to the value of x)

8. Using function templates in multiple files
    -- Template definitions are exempt from the part of the one-definition rule that requires only one definition per program, 
       so it is not a problem to have the same template definition #included into multiple source files. 
       And functions implicitly instantiated from function templates are implicitly inline, so they can be defined in multiple files, so long as each definition is identical.
    -- The templates themselves are not inline, as the concept of inline only applies to variables and functions.