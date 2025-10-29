Overload resolution
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


