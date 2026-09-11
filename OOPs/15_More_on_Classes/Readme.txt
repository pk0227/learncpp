1.  The hidden “this” pointer and member function chaining
    -- When a member function is called, how does C++ keep track of which object it was called on?

    The hidden this pointer
        -- Inside every member function, the keyword this is a const pointer that holds the address of the current implicit object.
        -- All non-static member functions have a this pointer that holds the address of the implicit object (the constness of this depends on whether the member function itself is const or not).
        
        'this' always points to the object being operated on
            -- Each member function has a single this pointer parameter that points to the implicit object.
            -- Because this is just a function parameter (and not a member), it does not make instances of your class larger memory-wise.

        Explicitly referencing this
            -- First, if you have a member function that has a parameter with the same name as a data member, you can disambiguate them by using this.

        Returning *this for function chaining
            --  The primary reason to do this is to allow member functions to be “chained” together, so several member functions can be called on the same object in a single expression! This is called function chaining.
            --  Returning *this by value in method chaining is generally discouraged because it may create unnecessary object copies—especially in older C++ versions or when copy elision is disabled—whereas returning *this by reference (or returning this as a pointer) avoids copying and is the recommended, efficient, and idiomatic approach.
            --  If returning this as a pointer is used, Because this always points to the implicit object, we don’t need to check whether it is a null pointer before dereferencing it.
        
        Resetting a class back to default state
            -- Constructors are only for initialization of new objects, and should not be called directly. Doing so will result in unexpected behavior.
            -- The best way to reset a class back to a default state is to create a reset() member function, have that function create a new object (using the default constructor), and then assign that new object to the current implicit object.
        
        this and const objects
            -- For non-const member functions, this is a const pointer to a non-const value (meaning this cannot be pointed at something else, but the object pointing to may be modified).
            -- With const member functions, this is a const pointer to a const value (meaning the pointer cannot be pointed at something else, nor may the object being pointed to be modified).
        
        Why this a pointer and not a reference
            -- when this was added to C++, references didn’t exist yet.
            -- If this were added to the C++ language today, it would undoubtedly be a reference instead of a pointer.

        Modern C++ evolution: Explicit object parameter ("Deducing this" in C++23)
            -- In C++23, C++ introduced explicit object parameters (syntax: void foo(this Self&& self)).
            -- This allows member functions to explicitly declare the implicit object as a parameter, enabling value category and constness deduction without writing multiple const/ref-qualified overloads.

2. Classes and header files
    -- C++ allows us to separate the “declaration” portion of the class from the “implementation” portion by defining member functions outside of the class definition.
    -- Note that the prototypes for these member functions still exist inside the class definition (as these functions need to be declared as part of the class type definition), but the actual implementation will be outside.
    -- we must prefix the member function names with the name of the class type (In case of Date class, Date::) while defining member functions outside of class.
    -- we left the access functions defined inside the class definition. Because access functions are typically only one line, defining these functions inside the class definition adds minimal clutter.
       For this reason, the definitions of access functions (and other trivial, one-line functions) are often left inside the class definition.
    
    Putting class definitions in a header file
        -- Unlike functions, which only need a forward declaration to be used, the compiler typically needs to see the full definition of a class (or any program-defined type) in order for the type to be used. 
           This is because the compiler needs to understand how members are declared in order to ensure they are used properly, and it needs to be able to calculate how large objects of that type are in order to instantiate them. 
           So our header files usually contain the full definition of a class rather than just a forward declaration of the class.

    Naming your class header and code files
        -- Most often, classes are defined in header files of the same name as the class, and any member functions defined outside of the class are put in a .cpp file of the same name as the class.
        -- Trivial member functions (such as access functions, constructors with empty bodies, etc…) can be defined inside the class definition.
        -- Prefer to define non-trivial member functions in a source file with the same name as the class.
    
    Doesn’t defining a class in a header file violate the one-definition rule if the header is #included more than once?
        -- Including a class definition more than once into a single translation unit is still an ODR violation. However, header guards (or #pragma once) will prevent this from happening.

        Inline member functions
            member functions defined inside class (inline default)
            -- Member functions defined inside the class definition are implicitly inline. Inline functions are exempt from the one definition per program part of the one-definition rule.
            
            member functions defined outside class (NOT inline default)
            -- Member functions defined outside the class definition are not implicitly inline (and thus are subject to the one definition per program part of the one-definition rule). 
               This is why such functions are usually defined in a code file (where they will only have one definition across the whole program).
            -- Alternatively, member functions defined outside the class definition can be left in the header file if they are made inline (using the inline keyword).

            Inline expansion of member functions
            -- The compiler must be able to see a full definition of a function in order to perform inline expansion. Most often, such functions (e.g. access functions) are defined inside the class definition. 
               However, if you want to define a member function outside the class definition, but still want it to be eligible for inline expansion, you can define it as an inline function just below the class definition (in the same header file). 
               That way the definition of the function is accessible to anybody who #includes the header.
            
            So why not put everything in a header file?
            -- First, as mentioned above, defining members inside the class definition clutters up your class definition.
            -- Second, if you change any of the code in the header, then you’ll need to recompile every file that includes that header. This can have a ripple effect, where one minor change causes the entire program to need to recompile.

            header-only library files
            -- in modern C++, classes or libraries are increasingly being distributed as “header-only”, meaning all of the code for the class or library is placed in a header file. This is done primarily to make distributing and using such files easier, 
               as a header only needs to be #included, whereas a code file needs to be explicitly added to every project that uses it, so that it can be compiled. If intentionally creating a header-only class or library for distribution, 
               all non-trivial member functions can be made inline and placed in the header file beneath the class definition.
            
            -- Finally, for template classes, template member functions defined outside the class are almost always defined inside the header file, beneath the class definition. Just like non-member template functions, the compiler needs to see the full template definition in order to instantiate it.

            -- Put any default arguments for member functions inside the class definition.

            3rd Party libraries
            -- Many open source software packages provide both .h and .cpp files for you to compile into your program. However, most commercial libraries provide only .h files and a precompiled library file. 
               There are several reasons for this:
                1) It’s faster to link a precompiled library than to recompile it every time you need it.
                2) A single copy of a precompiled library can be shared by many applications, whereas compiled code gets compiled into every executable that uses it (inflating file sizes).
                3) Intellectual property reasons (you don’t want people stealing your code).

3. Nested types (member types)
    -- To create a nested type, you simply define the type inside the class, under the appropriate access specifier.
    -- Define any nested types at the top of your class type.
    -- Since the class itself acts as a scope region, it’s somewhat redundant to use a scoped enumerator as well.

    Nested typedefs and type aliases
        -- Class types can also contain nested typedefs or type aliases.

    Nested classes and access to outer class members
        -- In C++, a nested class does not have access to the this pointer of the outer (containing) class, so nested classes can not directly access the members of the outer class.
        -- This is because a nested class can be instantiated independently of the outer class (and in such a case, there would be no outer class members to access!)
        -- However, because nested classes are members of the outer class, they can access any private members of the outer class that are in scope.
        -- There is one case where nested classes are more commonly used. In the standard library, most iterator classes are implemented as nested classes of the container they are designed to iterate over. 
           For example, std::string::iterator is implemented as a nested class of std::string. We’ll cover iterators in a future chapter.
        
        Nested types and forward declarations
        --  A nested type can be forward declared within the class that encloses it. The nested type can then be defined later, either within the enclosing class, or outside of it.
        -- However, a nested type cannot be forward declared prior to the definition of the enclosing class.
        -- While you can forward declare a nested type after the definition of the enclosing class, since the enclosing class will already contain a declaration for the nested type, doing so is redundant.

4. Introduction to destructors
    -- classes have another type of special member function that is called automatically when an object of a non-aggregate class type is destroyed. This function is called a destructor. Destructors are designed to allow a class to do any necessary clean up before an object of the class is destroyed.

    Destructor naming
     Like constructors, destructors have specific naming rules:
        -- The destructor must have the same name as the class, preceded by a tilde (~).
        -- The destructor can not take arguments.
        -- The destructor has no return type.
        -- A class can only have a single destructor.

    -- Generally you should not call a destructor explicitly (as it will be called automatically when the object is destroyed), since there are rarely cases where you’d want to clean up an object more than once.
    -- Destructors may safely call other member functions since the object isn’t destroyed until after the destructor executes.
    -- Remember that static variables (including global variables and static local variables) are constructed at program startup and destroyed at program shutdown.

    An implicit destructor
    -- If a non-aggregate class type object has no user-declared destructor, the compiler will generate a destructor with an empty body. This destructor is called an implicit destructor, and it is effectively just a placeholder.
    -- If your class does not need to do any cleanup on destruction, it’s fine to not define a destructor at all, and let the compiler generate an implicit destructor for your class.

    A warning about the std::exit() function
    -- the std::exit() function, can be used to terminate your program immediately. When the program is terminated immediately, the program just ends. Local variables are not destroyed first, and because of this, no destructors will be called. Be wary if you’re relying on your destructors to do necessary cleanup work in such a case.
    -- Unhandled exceptions will also cause the program to terminate, and may not unwind the stack before doing so. If stack unwinding does not happen, destructors will not be called prior to the termination of the program.

5. Class templates with member functions
    Type template parameters in member functions
        -- Type template parameters defined as part of a class template parameter declaration can be used both as the type of data members and as the type of member function parameters.
        -- we don’t need deduction guides for CTAD to work with non-aggregate classes. A matching constructor provides the compiler with the information it needs to deduce the template parameters from the initializers.
        -- when we define a member function for a class template outside of the class template definition, we need to resupply a template parameter declaration (template <typename T>) so the compiler knows what T is.
        -- when we define a member function outside of the class, we need to qualify the member function name with the fully templated name of the class template (Pair<T>::isEqual, not Pair::isEqual).
    
    Injected class names
        -- Within the scope of a class, the unqualified name of the class is called an injected class name.
        -- In a class template, the injected class name serves as shorthand for the fully templated name.
        -- we noted that CTAD doesn’t work with function parameters (as it is argument deduction, not parameter deduction). However, using an injected class name as a function parameter is okay, as it is shorthand for the fully templated name, not a use of CTAD.
        -- Any member function templates defined outside the class definition should be defined just below the class definition (in the same file).
        -- Functions implicitly instantiated from templates are implicitly inline. This includes both non-member and member function templates. 
           Therefore, there is no issue including member function templates defined in header files into multiple code files, as the functions instantiated from those templates will be implicitly inline (and the linker will de-duplicate them).

6.  Static member variables
        -- static member variables are shared by all objects of the class.

        Static members are not associated with class objects
            -- static members exist even if no objects of the class have been instantiated! This makes sense: they are created at the start of the program and destroyed at the end of the program, so their lifetime is not bound to a class object like a normal member.
            -- Static members are global variables that live inside the scope region of the class.
            -- Access static members using the class name and the scope resolution operator (::).
        
        Defining and initializing static member variables
            -- When we declare a static member variable inside a class type, it is a pure declaration, NOT a definition.
            -- Because static member variables are essentially global variables, you must explicitly define the static member outside of the class, in the global scope.
                int Something::s_value{ 1 }; // define and initialize s_value to 1
            -- This line serves two purposes: it instantiates the static member variable (just like a global variable), and initializes it. In this case, we’re providing the initialization value 1. If no initializer is provided, static member variables are zero-initialized by default.
            -- Note that this static member definition is not subject to access controls: you can define and initialize the value even if it’s declared as private (or protected) in the class.

            -- For non-template classes, if the class is defined in a header (.h) file, the static member definition is usually placed in the associated code file for the class.
               Alternatively, the member can also be defined as inline and placed below the class definition in the header (this is useful for header-only libraries).

            -- If the class is defined in a source (.cpp) file, the static member definition is usually placed directly underneath the class. 

            -- Do not put the static member definition in a header file (much like a global variable, if that header file gets included more than once, you’ll end up with multiple definitions, which will cause a linker error).

            -- For template classes, the (templated) static member definition is typically placed directly underneath the template class definition in the header file (this doesn’t violate the ODR because such definitions are implicitly inline).
            
            -- Static member variables in a class template are instantiated separately for each template specialization, so each type gets its own independent copy.
        
        Initialization of static member variables inside the class definition
            -- when the static member is a constant integral type (which includes char and bool) or a const enum, the static member can be initialized inside the class definition.
            -- static variables can be initialized inside the class definition regardless of whether they are constant or not using inline keyword.
            -- Because constexpr members are implicitly inline (as of C++17), static constexpr members can also be initialized inside the class definition without explicit use of the inline keyword.
            -- Make your static members inline or constexpr so they can be initialized inside the class definition.
            -- Only static members may use type deduction (auto and CTAD)
            -- Non-static members may not use auto or CTAD.

        The Static Initialization Order Fiasco
            -- In C++, static variables within a single translation unit are initialized in order of definition.
            -- However, the relative initialization order of static variables in DIFFERENT translation units is undefined!
            -- If a static member variable in File A accesses an uninitialized static member variable in File B during startup, it causes undefined behavior.
            -- Solution: Use the "Construct On First Use" idiom (Meyers' Singleton), where the static object is wrapped inside a static function as a local static variable:
               static Something& getSharedInstance()
               {
                   static Something s_instance{}; // C++11 guarantees thread-safe initialization on first call
                   return s_instance;
               } 

7.  Static member functions
        -- Just as static member variables belong to the class rather than to objects of the class, static member functions also belong to the class rather than to any object.
        -- If a static member variable is public, it can be accessed directly using the class name and the scope resolution operator.
        -- what if a static member variable is private?

        Static member functions
            -- Member variables aren’t the only type of member that can be made static. Member functions can be made static as well.
            -- Because static member functions are not associated with a particular object, they can be called directly by using the class name and the scope resolution operator (e.g. Something::getValue()). 
               Like static member variables, they can also be called through objects of the class type, though this is not recommended.

        Static member functions have no this pointer
            -- First, because static member functions are not attached to an object, they have no this pointer.
            -- Second, static member functions can directly access other static members (variables or functions), but not non-static members. This is because non-static members must belong to a class object, and static member functions have no class object to work with.

        Static members defined outside the class definition
            -- Static member functions can also be defined outside of the class declaration. This works the same way as for normal member functions.
            -- member functions defined inside the class definition are implicitly inline. Member functions defined outside the class definition are not implicitly inline, but can be made inline by using the inline keyword. 
               Therefore a static member function that is defined in a header file should be made inline so as not to violate the One Definition Rule (ODR) if that header is then included into multiple translation units.
        
        Monostates(pure static classes)
            -- A class with all static members.
            
            some potential downsides
                -- First, because all static members are instantiated only once, there is no way to have multiple copies of a pure static class.
                -- Like global variables are dangerous, the same holds true for pure static classes.
            
            Pure static classes vs namespaces
                -- Pure static classes have a lot of overlap with namespaces. Both allow you to define variables with static duration and functions within their scope region. However, one significant difference is that classes have access controls while namespaces do not.
                -- In general, a static class is preferable when you have static data members and/or need access controls. Otherwise, prefer a namespace.
            
        -- C++ does not support static constructors

8. Friend non-member functions
    -- In C++, a friend is a class or function (member or non-member) that has been granted full access to the private and protected members of another class. In this way, a class can selectively give other classes or functions full access to their members without impacting anything else.
    -- The friend declaration is not affected by access controls, so it does not matter where within the class body it is placed.
    -- Defining a friend non-member inside a class.
    -- There were times we might prefer to use a non-member function over a member function.

    Multiple friends
        -- A function can be a friend of more than one class at the same time.
    
    Doesn’t friendship violate the principle of data hiding?
        -- No. Friendship is granted by the class doing the data hiding with the expectation that the friend will access its private members. 
        -- Used properly, friendship can make a program more maintainable by allowing functionality to be separated when it makes sense from a design perspective. Or when it makes more sense to use a non-member function instead of a member function.
        -- However, because friends have direct access to the implementation of a class, changes to the implementation of the class will typically necessitate changes to the friends as well. If a class has many friends (or those friends have friends), this can lead to a ripple effect.
        -- When implementing a friend function, prefer to use the public interface over direct access to members whenever possible. This will help insulate your friend function from future implementation changes and lead to less code needing to be modified and/or retested later.
        -- A friend function should prefer to use the class interface over direct access whenever possible.
        -- Prefer non-friend functions to friend functions.
    
9. Friend classes and friend member functions
    -- A friend class is a class that can access the private and protected members of another class.
    -- friendship is not reciprocal.
    -- Class friendship is also not transitive. If class A is a friend of B, and B is a friend of C, that does not mean A is a friend of C.
    -- Nor is friendship inherited. If class A makes B a friend, classes derived from B are not friends of A.
    -- The friend declaration is placed inside the class that is GRANTING friendship (not inside the friend class/function). The class grants access to its own private members.

    Friend member functions
        -- Instead of making an entire class a friend, you can grant friendship to a specific member function of another class.
        -- However, making a member function a friend requires a strict 4-step declaration order to resolve circular dependencies:
           Step 1: Forward declare the class containing the private members (e.g. class BankAccount;).
           Step 2: Fully define the class that contains the friend function (e.g. class BalanceCheck), but only DECLARE the friend function (do not define its body yet, because BankAccount is still incomplete).
           Step 3: Fully define the class containing private members (BankAccount), and include the friend declaration:
                   friend int BalanceCheck::getBankAccountDeposits(const BankAccount& acc) const;
           Step 4: Finally, DEFINE the friend function body outside of BalanceCheck, after BankAccount has been fully defined.

10. Ref qualifiers
        -- we know how calling access functions that return references to data members can be dangerous when the implicit object is an rvalue.
        -- The root of the challenge illustrated above is that we only want one function to service two different cases (one where our implicit object is an lvalue, and one where our implicit object is an rvalue).
        -- C++11 introduced a little known feature called a ref-qualifier that allows us to overload a member function based on whether it is being called on an lvalue or an rvalue implicit object.
        -- If the implicit object is a non-const rvalue, use std::move to try to move m_name.

        Some notes about ref-qualified member functions
            -- First, for a given function, non-ref-qualified overloads and ref-qualified overloads cannot coexist. Use one or the other.
            -- Second, similar to how a const lvalue reference can bind to an rvalue, if only a const lvalue-qualified function exists, it will accept either lvalue or rvalue implicit objects.
            -- Third, either qualified overload can be explicitly deleted (using = delete), which prevents calls to that function. For example, deleting the rvalue-qualified version prevents use of the function with rvalue implicit objects.
            -- Ref-qualifier syntax: use & for lvalue-qualified and && for rvalue-qualified overloads.
               e.g. std::string& getName() &  { return m_name; }          // lvalue objects
                    std::string  getName() && { return std::move(m_name); } // rvalue objects
        
        So why don’t we recommend using ref-qualifiers?
            -- While ref-qualifiers are neat, there are some downsides to using them in this way.
            -- Adding rvalue overloads to every getter that returns a reference adds clutter to the class, to mitigate against a case that isn’t that common and is easily avoidable with good habits.
            -- Having an rvalue overload return by value means we have to pay for the cost of a copy (or move) even in cases where we could have used a reference safely (e.g. in case 1 of the example at the top of the lesson).

            Additionally:
                -- Most C++ developers are not aware of this feature (which can lead to errors or inefficiencies in use).
                -- The standard library typically does not make use of this feature.
            
            -- Based on all of the above, It is not recommended to use of ref-qualifiers as a best practice. Instead, It is always recommended to use the result of an access function immediately and not saving returned references for use later.
            