1 — Introduction to operator overloading
    -- In C++, operators are implemented as functions. 
    -- you can define your own versions of the operators that work with different data types (including classes that you’ve written). Using function overloading to overload operators is called operator overloading.
    
    limitations on operator overloading
        -- First, almost any existing operator in C++ can be overloaded. The exceptions are:
            -- conditional (?:)
            -- sizeof
            -- scope (::)
            -- member selector (.)
            -- pointer member selector (.*)
            -- typeid
            -- the casting operators.
        -- Second, you can only overload the operators that exist. You can not create new operators or rename existing operators. 
            -- For example, you could not create an operator** to do exponents.
        -- Third, at least one of the operands in an overloaded operator must be a user-defined type.
            -- This means you could overload operator+(int, Mystring), but not operator+(int, double).
        -- An overloaded operator should operate on at least one program-defined type (either as a parameter of the function, or the implicit object).
        -- Fourth, it is not possible to change the number of operands an operator supports.
        -- Finally, all operators keep their default precedence and associativity (regardless of what they’re used for) and this can not be changed.
            -- Example, attempting to overload the bitwise XOR operator (^) to do exponentiation. 
               Because, in C++, operator^ has a lower precedence level than the basic arithmetic operators, which causes expressions to evaluate incorrectly.
               In basic mathematics, exponentiation is resolved before basic arithmetic, so 4 + 3 ^ 2 resolves as 4 + (3 ^ 2) => 4 + 9 => 13.
               But, in C++, the arithmetic operators have higher precedence than operator^, so 4 + 3 ^ 2 resolves as (4 + 3) ^ 2 => 7 ^ 2 => 49.
               You’d need to explicitly parenthesize the exponent portion (e.g. 4 + (3 ^ 2)) every time you used it for this to work properly.

            -- When overloading operators, it’s best to keep the function of the operators as close to the original intent of the operators as possible.
        -- If the meaning of an overloaded operator is not clear and intuitive, use a named function instead.
    
    -- Operators that do not modify their operands (e.g. arithmetic operators) should generally return results by value.
    -- Operators that modify their leftmost operand (e.g. pre-increment, any of the assignment operators) should generally return the leftmost operand by reference.

2 — Overloading the arithmetic operators using friend functions
    -- Note that all of the arithmetic operators are binary operators -- meaning they take two operands -- one on each side of the operator. 
    -- All four of these operators are overloaded in the exact same way.
    -- there are three different ways to overload operators: 
        -- the friend function way
        -- the normal function way
        -- the member function way

        Overloading the arithmetic operators using friend functions
            Friend functions can be defined inside the class
                -- Even though friend functions are not members of the class, they can still be defined inside the class if desired.

            Overloading operators for operands of different types and Implementing operators using other operators
                -- It is often possible to define overloaded operators by calling other overloaded operators. You should do so if and when doing so produces simpler code. 
                   In cases where the implementation is trivial (e.g. a single line) it may or may not be worth doing this.

3 — Overloading operators using normal functions
    -- In general, a normal function should be preferred over a friend function if it’s possible to do so with the existing member functions available.
       the less functions touching your classes’s internals, the better
    -- However, don’t add additional access functions just to overload an operator as a normal function instead of a friend function.

4 — Overloading the I/O operators
    Overloading operator<< (Insertion)
        -- We should return reference to std::ostream in insertion operator overloading funcion.
        -- However, if you try to return std::ostream by value, you’ll get a compiler error. 
           This happens because std::ostream specifically disallows being copied.

        -- Any time we want our overloaded binary operators to be chainable in such a manner, the left operand should be returned (by reference). 
           But, it must still exist when the called function returns.
    
    Overloading operator>>
        -- Note that if input stream is set to failure mode, we have to clear failure mode. Otherwise, the next extraction aborts immediately.
           The solution for this is to make our operations transactional.

        Transactional operations can be implemented using a number of different strategies. For example:
            Alter on success: Store the result of each sub-operation. If all sub-operations succeed, replace the relevant data with the stored results. 
            Restore on failure: Copy any data that can be altered. If any sub-operation fails, the changes made by prior sub-operations can be reverted using the data from copy.
            Rollback on failure: If any sub-operation fails, each prior sub-operation is reversed (using an opposite sub-operation). 
                                 This strategy is often used in databases, where the data is too large to back up, and the result of sub-operations can’t be stored. 

        Handling semantically invalid input
            -- In cases where operator>> simply fails to extract anything to a variable, std::cin will automatically be placed in failure mode.
               The caller of this function can then check std::cin to see if it failed and handle that case as appropriate.
            -- But what about cases where the user inputs a value that is extractable but semantically invalid.
               Because std::cin did extract something, it won’t go into failure mode automatically. And then the caller probably won’t realize something went wrong.
               To address this, we can have our overloaded operator>> determine whether any of the values that were extracted are semantically invalid.
               If so, manually put the input stream in failure mode. This can be done by calling std::cin.setstate(std::ios_base::failbit);.

5 — Overloading operators using member functions
    -- The overloaded operator must be added as a member function of the left operand.
    -- The left operand becomes the implicit *this object
    -- All other operands become function parameters.

    So if we can overload an operator as a friend or a member, which should we use? In order to answer that question, there’s a few more things you’ll need to know.
        Not everything can be overloaded as a friend function
            -- The assignment (=), 
            -- subscript ([]), 
            -- function call (()),
            -- member selection (->) operators must be overloaded as member functions.
        
        Not everything can be overloaded as a member function
            -- operator<< 
            -- operator>>
            Because the overloaded operator must be added as a member of the left operand. In this case, the left operand is an object of type std::ostream and std::istream. std::ostream and std::istream are fixed as parts of the standard library.
            Typically, we won’t be able to use a member overload if the left operand is either not a class (e.g. int), or it is a class that we can’t modify (e.g. std::ostream).

        When to use a normal, friend, or member function overload
            The following rules of thumb can help you determine which form is best for a given situation:
                -- If you’re overloading assignment (=), subscript ([]), function call (()), or member selection (->), do so as a member function.
                -- If you’re overloading a unary operator, do so as a member function.
                -- If you’re overloading a binary operator that does not modify its left operand (e.g. operator+), do so as a normal function (preferred) or friend function.
                -- If you’re overloading a binary operator that modifies its left operand, but you can’t add members to the class definition of the left operand (e.g. operator<<, which has a left operand of type ostream), do so as a normal function (preferred) or friend function.
                -- If you’re overloading a binary operator that modifies its left operand (e.g. operator+=), and you can modify the definition of the left operand, do so as a member function.

6 — Overloading unary operators +, -, and !
    -- The positive (+), negative (-) and logical not (!) operators all are unary operators, which means they only operate on one operand. 
       Because they only operate on the object they are applied to, typically unary operator overloads are implemented as member functions. 
    
    -- So, the unary operators take no parameters (it operates on the *this object).

7 — Overloading the comparison operators
    -- Because the comparison operators are all binary operators that do not modify their left operands, we can make our overloaded comparison operators either normal functions or friend functions.
    -- The most common comparison operators are ==, !=, <, >, <=, and >=.

8 — Overloading the increment and decrement operators
    -- Overloading the increment (++) and decrement (--) operators is pretty straightforward, with one small exception. There are actually two versions of the increment and decrement operators: 
        a prefix increment and decrement (e.g. ++x; --y;)
        a postfix increment and decrement (e.g. x++; y--;).
    -- Because the increment and decrement operators are both unary operators and they modify their operands.
       So, they’re best overloaded as member functions. 
    
    Overloading prefix increment and decrement
        -- Note that we return *this. The overloaded increment and decrement operators return the current implicit object so multiple operators can be “chained” together.

    Overloading postfix increment and decrement
        -- Both prefix and postfix increment and decrement operators have the same name (eg. operator++), are unary, and take one parameter of the same type. 
        -- To differentiate the two when overloading, the compiler looks to see if the overloaded operator has an int parameter. 
            -- If the overloaded operator has an int parameter, the operator is a postfix overload.
            -- If the overloaded operator has no parameter, the operator is a prefix overload.
        -- The integer parameter in postfix overload is a dummy parameter.
        -- Because the dummy parameter is not used in the function implementation, we have not even given it a name. 

        -- The difference between the two overload versions is in the value they return. 
            -- The overloaded prefix operators return the object after it has been incremented or decremented. We simply increment or decrement our member variables, and then return *this.
            -- The postfix operators, on the other hand, need to return the state of the object before it is incremented or decremented. 
               We use a temporary variable that holds the value of the object before it is incremented or decremented. Then the object itself can be incremented or decremented. 
               And finally, the temporary variable is returned to the caller.

               Note that this means the return value of the overloaded operator must be a non-reference.
               Also note that this means the postfix operators are typically less efficient than the prefix operators because of the added overhead of instantiating a temporary variable and returning by value instead of reference. 

9 — Overloading the subscript operator
    -- An overloaded operator[] function will always take one parameter: the subscript that the user places between the hard braces.
    -- Note that although you can provide a default value for the function parameter, actually using operator[] without a subscript inside is not considered a valid syntax, so there’s no point.
    -- C++23 adds support for overloading operator[] with multiple subscripts.
    -- operator[] returns a reference for storing value at the given index.
    
    Overloaded operator[] for const objects
        -- we can define a non-const and a const version of operator[] separately. 
           The non-const version will be used with non-const objects, and the const version with const-objects.
        -- Detecting index validity using operator[] overloading.
        -- Pointers to objects and overloaded operator[] don’t mix
        
        The function parameter does not need to be an integral type 
            -- You could define your overloaded operator[] to take a double, a std::string, or whatever else you like.
            -- Overloading operator[] to take a std::string parameter can be useful when writing certain kinds of classes, such as those that use words as indices.
        
        

   


    
