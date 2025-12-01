1 — Pointers and references to the base class of derived objects
    -- One of the most important and powerful aspects of inheritance -- virtual functions.
    -- Remember that inheritance implies an is-a relationship between two classes. Since a Derived is-a Base, it is appropriate that Derived contain a Base part.
       So, C++ will let us set a Base pointer or reference to a Derived object.
    -- In C++, a function is overridden only if the base class function is marked virtual.
       If it’s not virtual, the function in the derived class does not override it — it hides the base version.
       In function hiding, the derived function is not visible through a base pointer or reference.
       So, when a base pointer/reference points to a derived object, it can only access the base version of the function, because dynamic dispatch does not occur without virtual.
    
    Why would I set a pointer or reference to the base class?
        -- We use base-class pointers/references to enable polymorphism.
           They let us write one generic function that works for all derived types, avoid dozens of overloaded functions, and store different derived objects in a single container.
           Without base pointers, we’d need separate functions and separate arrays for every derived type.
           A template could remove overloads, but it doesn’t enforce the type hierarchy or document intent, and it accepts any type with matching functions.
           But a base pointer normally calls only base-class functions — which is why we make functions virtual, so the correct derived behavior is invoked through the base interface.
        
2 — Virtual functions and polymorphism
    Virtual functions
        -- A virtual function is a special type of member function that, when called, resolves to the most-derived version of the function for the actual type of the object being referenced or pointed to.
    Overrides
        -- A derived function is considered a match if it has the same signature (name, parameter types, and whether it is const) and return type as the base version of the function. Such functions are called overrides.
    
    -- To make a function virtual, simply place the “virtual” keyword before the function declaration.    

    NOTE : Some modern compilers may give an error about having virtual functions and an accessible non-virtual destructor. If this is the case, add a virtual destructor to the base class. In the above program, add this to the definition of Base.

    -- Note that virtual function resolution only works when a virtual member function is called through a pointer or reference to a class type object. This works because the compiler can differentiate the type of the pointer or reference from the type of the object being pointed to or referenced.
    -- Calling a virtual member function directly on an object (not through a pointer or reference) will always invoke the member function belonging to the same type of that object.

    Polymorphism
        -- In programming, polymorphism refers to the ability of an entity to have multiple forms (the term “polymorphism” literally means “many forms”).
        
        Compile-time polymorphism:
            -- refers to forms of polymorphism that are resolved by the compiler. 
               These include function overload resolution, as well as template resolution.

        Runtime polymorphism:
            -- refers to forms of polymorphism that are resolved at runtime. 
               This includes virtual function resolution.
        
    -- The biggest benefit of virtual functions -- the ability to structure your code in such a way that newly derived classes will automatically work with the old code without modification!
    A word of warning: 
        -- The signature of the derived class function must exactly match the signature of the base class virtual function in order for the derived class function to be used. 
           If the derived class function has different parameter types, the program will likely still compile fine, but the virtual function will not resolve as intended. 
        -- If a function is virtual, all matching overrides in derived classes are implicitly virtual.
        -- This does not work the other way around -- a virtual override in a derived class does not implicitly make the base class function virtual.
    
    Return types of virtual functions
        -- Under normal circumstances(except covariant return types), the return type of a virtual function and its override must match. 
    
    Do not call virtual functions from constructors or destructors
    We should not call virtual functions from constructors or destructors. Why?
        -- when a Derived class is created, the Base portion is constructed first. If you were to call a virtual function from the Base constructor, 
           and Derived portion of the class hadn’t even been created yet, it would be unable to call the Derived version of the function 
           because there’s no Derived object for the Derived function to work on. In C++, it will call the Base version instead.

        -- A similar issue exists for destructors. If you call a virtual function in a Base class destructor, 
           it will always resolve to the Base class version of the function, because the Derived portion of the class will already have been destroyed.

    The downside of virtual functions
        -- Since most of the time you’ll want your functions to be virtual, why not just make all functions virtual?
            -- The answer is because it’s inefficient -- resolving a virtual function call takes longer than resolving a regular one.
               Furthermore, to make virtual functions work, the compiler has to allocate an extra pointer for each object of a class that has virtual functions. 
               This adds a lot of overhead to objects that otherwise have a small size. 
        
3 — The override and final specifiers, and covariant return types
    -- To address some common challenges with inheritance, C++ has two inheritance-related identifiers: 
        -- override and final. 
    -- The C++ standard calls them “identifiers with special meaning”, but they are often referred to as “specifiers”.
    -- Override is a fantastic addition that you should use regularly. 

    The override specifier
        -- To help address the issue of functions that are meant to be overrides but aren’t, 
           the override specifier can be applied to any virtual function to tell the compiler to enforce that the function is an override. 
        -- The override specifier is placed at the end of a member function declaration (in the same place where a function-level const goes). 
           If a member function is const and an override, the const must come before override.
        -- If a function marked as override does not override a base class function (or is applied to a non-virtual function), the compiler will flag the function as an error.
        -- There is no performance penalty for using the override specifier.
        -- The override specifier implies virtual. So, there’s no need to tag functions using the override specifier with the virtual keyword.
        -- Use the virtual keyword on virtual functions in a base class.
           Use the override specifier (but not the virtual keyword) on override functions in derived classes. This includes virtual destructors.
        
    The final specifier
        -- There may be cases where you don’t want someone to be able to override a virtual function, or inherit from a class. 
           The final specifier can be used to tell the compiler to enforce this. If the user tries to override a function or inherit from a class that has been specified as final, the compiler will give a compile error.
    
    Covariant return types
        -- There is one special case in which a derived class virtual function override can have a different return type than the base class and still be considered a matching override. 
           If the return type of a virtual function is a pointer or a reference to some class, override functions can return a pointer or a reference to a derived class. 
           These are called covariant return types.
        -- One interesting note about covariant return types: C++ can’t dynamically select types, so we’ll always get the type that matches the actual version of the function being called.
           (
            Whichever virtual function is actually invoked at runtime, its own return type is the one you get.
            C++ does not dynamically ‘adjust’ or ‘compute’ a new return type — it uses the return type of the actual override that is called.
           )
        -- Covariant return types are often used in cases where a virtual member function returns a pointer or reference to the class containing the member function (e.g. Base::getThis() returns a Base*, and Derived::getThis() returns a Derived*). 
           However, this isn’t strictly necessary. Covariant return types can be used in any case where the return type of the override member function is derived from the return type of the base virtual member function.

4 — Virtual destructors, virtual assignment, and overriding virtualization
    Virtual destructors
        -- We should always make your destructors virtual if you’re dealing with inheritance. 
        -- As with normal virtual member functions, if a base class function is virtual, all derived overrides will be considered virtual regardless of whether they are specified as such. 
           It is not necessary to create an empty derived class destructor just to mark it as virtual.
        -- Note that if you want your base class to have a virtual destructor that is otherwise empty, you can define your destructor this way:
            virtual ~Base() = default; // generate a virtual default destructor
    
    Virtual assignment
        -- It is possible to make the assignment operator virtual. However, unlike the destructor case where virtualization is always a good idea, virtualizing the assignment operator really opens up a bag full of worms.

    Ignoring virtualization
        -- There may be cases where you want a Base pointer to a Derived object to call Base-version function instead of Derived-version. 
           To do so, simply use the scope resolution operator:
            Example: std::cout << base.Base::getName() << '\n';
    
    Should we make all destructors virtual?
        -- It’s easy to say yes, so that way you can later use any class as a base class -- but there’s a performance penalty for doing so (a virtual pointer added to every instance of your class). 
           So you have to balance that cost, as well as your intent.
        -- Good practice is that If a class isn’t explicitly designed to be a base class, then it’s generally better to have no virtual members and no virtual destructor. The class can still be used via composition.

        Conventional wisdom
            -- A base class destructor should be either public and virtual, or protected and non-virtual.
               A base class with a protected destructor can’t be deleted using a base class pointer, which prevents deleting a derived class object through a base class pointer.
            
        Now that the final specifier has been introduced into the language, our recommendations are as follows:
            -- If you intend your class to be inherited from, make sure your destructor is virtual and public.
            -- If you do not intend your class to be inherited from, mark your class as final. This will prevent other classes from inheriting from it in the first place, without imposing any other use restrictions on the class itself.
        
        
