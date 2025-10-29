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

Overload resolution
    -- With overloaded functions, there can be many functions That function either matches (or can be made to match after type conversions are applied), 
       or it doesn’t (and a compile error results).
    -- Since a function call can only resolve to one of them, the compiler has to determine which overloaded function is the best match. 
The process of matching function calls to a specific overloaded function is called overload resolution.

