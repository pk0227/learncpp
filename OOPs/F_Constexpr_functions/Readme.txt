kon1. Constexpr functions
    -- constexpr keyword : used to create compile-time (symbolic) constants.
    -- constant expressions : expressions that can be evaluated at compile-time rather than runtime.
    
    challenge with constant expressions 
        -- is that function call to a normal function are not allowed in constant expressions.

    -- Constexpr functions can be used in constant expressions.
    -- If a required constant expression contains a constexpr function call, that constexpr function call must evaluate at compile-time.
    -- When a function call is evaluated at compile-time, the compiler will calculate the return value of the function call at compile-time, and then replace the function call with the return value.

    To evaluate at compile-time, other things must also be true:
        -- The call to the constexpr function must have arguments that are known at compile time (e.g. are constant expressions).
        -- All statements and expressions within the constexpr function must be evaluatable at compile-time.
        -- When a constexpr (or consteval) function is being evaluated at compile-time, any other functions it calls are required to be evaluated at compile-time (otherwise the initial function would not be able to return a result at compile-time).

    -- Constexpr functions can also be evaluated at runtime. In such cases, it evaluates just like a normal (non-constexpr) function.
    
    Constexpr/consteval function parameters are not constexpr
        -- A constexpr function parameter would imply the function could only be called with a constexpr argument. 
           But this is not the case -- constexpr functions can be called with non-constexpr arguments when the function is evaluated at runtime.
        -- Because such parameters are not constexpr, they cannot be used in constant expressions within the function.
        -- The parameters of constexpr functions may be declared as const, in which case they are treated as runtime constants.

    Constexpr functions are implicitly inline
        -- The compiler must be able to see the full definition of a constexpr (or consteval) function, not just a forward declaration.
        -- Constexpr/consteval functions used in multiple source files should be defined in a header file so they can be included into each source file.
        -- For constexpr function calls that are only evaluated at runtime, a forward declaration is sufficient to satisfy the compiler. This means you can use a forward declaration to call a constexpr function defined in another translation unit, 
           but only if you invoke it in a context that does not require compile-time evaluation.
    
    Recap
        -- Marking a function as constexpr means it can be used in a constant expression. It does not mean “will evaluate at compile-time”.
        -- A constant expression (which may contain constexpr function calls) is only required to evaluate at compile-time in contexts where a constant expression is required.
        -- In contexts that do not require a constant expression, the compiler may choose whether to evaluate a constant expression (which may contain constexpr function calls) at compile-time or at runtime.
        -- A runtime (non-constant) expression (which may contain constexpr function calls or non-constexpr function calls) will evaluate at runtime.
    -- Note that even non-constexpr functions could be evaluated at compile-time under the as-if rule.

    Key insight
    -- Put another way, we can categorize the likelihood that a function will actually be evaluated at compile-time as follows:
    
    Always (required by the standard):
    -- Constexpr function is called where constant expression is required.
    -- Constexpr function is called from other function being evaluated at compile-time.

    Probably (there’s little reason not to):
    -- Constexpr function is called where constant expression isn’t required, all arguments are constant expressions.

    Possibly (if optimized under the as-if rule):
    -- Constexpr function is called where constant expression isn’t required, some arguments are not constant expressions but their values are known at compile-time.
    -- Non-constexpr function capable of being evaluated at compile-time, all arguments are constant expressions.

    Never (not possible):
    -- Constexpr function is called where constant expression isn’t required, some arguments have values that are not known at compile-time.    
    
2. Forcing a constexpr function to be evaluated at compile-time (Consteval)
    Consteval
        -- C++20 introduces the keyword consteval, which is used to indicate that a function must evaluate at compile-time, otherwise a compile error will result. Such functions are called immediate functions.
        
        if std::is_constant_evaluated() / if consteval
            -- In cases where the compiler is required to evaluate a constant expression at compile-time std::is_constant_evaluated() will true.
            -- However, sometimes the compiler may also be required to evaluate a non-constant expression at compile-time. In such cases, std::is_constant_evaluated() will return false even though the function did evaluate at compile-time.
            -- So std::is_constant_evaluated() really means “the compiler is being forced to evaluate this at compile-time”, not “this is evaluating at compile-time”.
            -- Introduced in C++23, if consteval is a replacement for if (std::is_constant_evaluated()) that provides a nicer syntax and fixes some other issues. 
        -- The downside of consteval functions is that such functions can’t evaluate at runtime, making them less flexible than constexpr functions, which can do either.
        -- it would still be useful to have a convenient way to force constexpr functions to evaluate at compile-time (even when the return value is being used where a constant expression is not required), 
           so that we can explicitly force compile-time evaluation when possible, and runtime evaluation when we can’t.

    -- Constexpr/consteval functions can use non-const local variables.
    
    Constexpr/consteval functions can use function parameters and local variables as arguments in constexpr function calls
        -- When a constexpr (or consteval) function is being evaluated at compile-time, any other functions it calls are required to be evaluated at compile-time.
        -- Perhaps surprisingly, a constexpr or consteval function can use its function parameters (which aren’t constexpr) or even local variables (which may not be const at all) as arguments in a constexpr function call. 
           When a constexpr or consteval function is being evaluated at compile-time, the value of all function parameters and local variables must be known to the compiler (otherwise it couldn’t evaluate them at compile-time). 
           Therefore, in this specific context, C++ allows these values to be used as arguments in a call to a constexpr function, and that constexpr function call can still be evaluated at compile-time. 
        
    Can a constexpr function call a non-constexpr function?
        -- The answer is yes, but only when the constexpr function is being evaluated in a non-constant context. A non-constexpr function may not be called when a constexpr function is evaluating in a constant context 
           (because then the constexpr function wouldn’t be able to produce a compile-time constant value), and doing so will produce a compilation error.    

        -- Avoid calling non-constexpr functions from within a constexpr function if possible.
        -- If your constexpr function requires different behavior for constant and non-constant contexts, conditionalize the behavior with if (std::is_constant_evaluated()) (in C++20) or if consteval (C++23 onward).
        -- Always test your constexpr functions in a constant context, as they may work when called in a non-constant context but fail in a constant context.        

    When should I constexpr a function?
        -- As a general rule, if a function can be evaluated as part of a required constant expression, it should be made constexpr.
        
        A pure function is a function that meets the following criteria:
            -- The function always returns the same return result when given the same arguments
            -- The function has no side effects (e.g. it doesn’t change the value of static local or global variables, doesn’t do input or output, etc…).

        -- Pure functions should generally be made constexpr.

        -- Constexpr functions don’t always need to be pure. In C++23, constexpr functions can use and modify static local variables.
           Since the value of a static local persists across function calls, modifying a static local variable is considered a side-effect.

    Why not constexpr every function?
        There are a few reasons you may not want to constexpr a function:
            -- constexpr signals that a function can be used in a constant expression. If your function cannot be evaluated as part of a constant expression, it should not be marked as constexpr.
            -- constexpr is part of the interface of a function. Once a function is made constexpr, it can be called by other constexpr functions or used in contexts that require constant expressions. Removing the constexpr later will break such code.
            -- constexpr functions can be harder to debug since you can’t breakpoint or step through them in a debugger.

    