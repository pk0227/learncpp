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
        
5 — Early binding and late binding
    Binding and dispatching
        -- In general programming, binding is the process of associating names with such properties.
        -- Function binding (or method binding) is the process that determines what function definition is associated with a function call.
        -- The process of actually invoking a bound function is called dispatching.

        Early binding
            -- In C++, when a direct call is made to a non-member function or a non-virtual member function, the compiler can determine which function definition should be matched to the call. 
               This is sometimes called early binding (or static binding), as it can be performed at compile-time. 
            -- The compiler (or linker) can then generate machine language instructions that tells the CPU to jump directly to the address of the function.
            -- Calls to overloaded functions and function templates can also be resolved at compile-time.
            
            NOTE : In case of switch case statement, which function is actually called is not determined until runtime. However, that is a path of execution issue, not a binding issue.

        Late binding
            -- In some cases, a function call can’t be resolved until runtime. In C++, this is sometimes known as late binding (or in the case of virtual function resolution, dynamic dispatch).
            -- In general programming terminology, the term “late binding” usually means that the function being called can’t be determined based on static type information alone, but must be resolved using dynamic type information.
            -- In C++, the term tends to be used more loosely to mean any function call where the actual function being called is not known by the compiler or linker at the point where the function call is actually being made.
            -- In C++, one way to get late binding is to use function pointers.
               Function pointer 
                -- A type of pointer that points to a function instead of a variable. The function that a function pointer points to can be called by using the function call operator () on the pointer.
                -- The compiler does not know at compile-time what function is being called. Instead, at runtime, an indirect function call is made to whatever function exists at the address held by the function pointer.
            
        -- Late binding is slightly less efficient since it involves an extra level of indirection. With early binding, the CPU can jump directly to the function’s address. 
           With late binding, the program has to read the address held in the pointer and then jump to that address. This involves one extra step, making it slightly slower. 
           However, the advantage of late binding is that it is more flexible than early binding, because decisions about what function to call do not need to be made until runtime.

6 — The virtual table
    -- The C++ standard does not specify how virtual functions should be implemented (this detail is left up to the implementation).
    -- However, C++ implementations typically implement virtual functions using a form of late binding known as the virtual table.
    -- The virtual table is a lookup table of functions used to resolve function calls in a dynamic/late binding manner. 
       The virtual table sometimes goes by other names, such as 
        “vtable”, “virtual function table”, “virtual method table”, or “dispatch table”.
    -- In C++, virtual function resolution is sometimes called dynamic dispatch.
    
    Nomenclature
        -- Early binding/static dispatch = direct function call overload resolution
        -- Late binding = indirect function call resolution
        -- Dynamic dispatch = virtual function override resolution

    How virtual table works
        -- First, every class that uses virtual functions (or is derived from a class that uses virtual functions) has a corresponding virtual table.
        -- This table is simply a static array that the compiler sets up at compile time. 
        -- A virtual table contains one entry for each virtual function that can be called by objects of the class.
        -- Each entry in this table is simply a function pointer that points to the most-derived function accessible by that class.

        -- Second, the compiler also adds a hidden pointer that is a member of the base class, which we will call *__vptr. 
        -- *__vptr is set (automatically) when a class object is created so that it points to the virtual table for that class.
        -- Unlike the this pointer, which is actually a function parameter used by the compiler to resolve self-references, *__vptr is a real pointer member.
        -- Consequently, it makes each class object allocated bigger by the size of one pointer. 
        -- It also means that *__vptr is inherited by derived classes, which is important.
    
        Look at the pictorial representation "virtual_table.png" under 25_5_Early_binding_n_late_binding_virtual_table folder.
    
    Calling a virtual function is slower than calling a non-virtual function for a couple of reasons:
        -- First, we have to use the *__vptr to get to the appropriate virtual table.
        -- Second, we have to index the virtual table to find the correct function to call. 
        -- Only then we can call the function. 
        As a result, we have to do 3 operations to find the function to call, as opposed to 2 operations for a normal indirect function call, or one operation for a direct function call.

7 — Pure virtual functions, abstract base classes, and interface classes
    Pure virtual function (or Abstract function)
        -- A special kind of virtual function that has no body at all!
        -- To create a pure virtual function, rather than define a body for the function, we simply assign the function the value 0.
        -- Adding a pure virtual function to our class means “it is up to the derived classes to implement this function”.

        A pure virtual function has two main consequences:
            -- First, any class with one or more pure virtual functions becomes an abstract base class, which means that it can not be instantiated!
               Because there’s no definition for pure virtual function, what would the function resolve to?
            -- Second, any derived class must define a body for this function, or that derived class will be considered an abstract base class as well.

        Why would we need a pure virtual function?
            -- A pure virtual function is useful when we have a function that we want to put in the base class, but only the derived classes know what it should return.
            -- A pure virtual function makes it so the base class can not be instantiated, and the derived classes are forced to define these functions before they can be instantiated.
            -- This helps ensure the derived classes do not forget to redefine functions that the base class was expecting them to.
        
            NOTE : Any class with pure virtual functions should also have a virtual destructor.

        Pure virtual functions CAN have definitions
            -- In this case, pure virtual function with its definition is still considered a pure virtual function because of the “= 0” (even though it has been given a definition).
               The class containing pure virtual function is still considered an abstract base class (and thus can’t be instantiated). 
               Any class that inherits from such class needs to provide its own definition for the pure virtual function or it will also be considered an abstract base class.
            
            -- This paradigm can be useful when we want our base class to provide a default implementation for a function, but still force any derived classes to provide their own implementation. 
               However, if the derived class is happy with the default implementation provided by the base class, it can simply call the base class implementation directly using base class scope resolution.
            
            NOTE : When providing a definition for a pure virtual function, the definition must be provided separately (not inline).

            -- A destructor can be made pure virtual, but must be given a definition so that it can be called when a derived object is destructed.
        
        Interface classes
            -- An interface class is a class that has no member variables, and where all of the functions are pure virtual! 
            -- Interfaces are useful when you want to define the functionality that derived classes must implement, but leave the details of how the derived class implements that functionality entirely up to the derived class.
            -- Don’t forget to include a virtual destructor for your interface classes, so that the proper derived destructor will be called if a pointer to the interface is deleted.
        
        Pure virtual functions and the virtual table
            -- For consistency, abstract classes still have virtual tables. 
               A constructor or destructor of an abstract class can call a virtual function, 
               and it needs to resolve to the proper function (in the same class, since the derived classes either haven’t been constructed yet or have already been destroyed).

            -- The virtual table entry for a class with a pure virtual function will generally either contain a null pointer, or point to a generic function that prints an error 
               (sometimes this function is named __purecall).
        
            -- In C++, any class that contains virtual functions has a hidden vptr in each object, and this vptr is inherited by derived classes. 
               There is only one vptr per object, but each class has its own vtable, which contains the addresses of its virtual functions. 
               Derived classes reuse the same vptr but point it to their own vtable, allowing dynamic dispatch.
            
        NOTE : In C++, Abstract and Interface classes can have definitions for pure virtual functions. They remain abstract and interface classes, 
               and derived classes must override them but can call that base implementation if needed.
               They both can have virtual/pure virtual destructors.

8 — Virtual base classes
    -- To share a base class, simply insert the “virtual” keyword in the inheritance list of the derived class. 
       This creates what is called a virtual base class, which means there is only one base object. 
       The base object is shared between all objects in the inheritance tree and it is only constructed once. 
       
       Here is an example (without constructors for simplicity) showing how to use the virtual keyword to create a shared base class for Diamond problem:     
        class PoweredDevice
        {
        };

        class Scanner: virtual public PoweredDevice
        {
        };

        class Printer: virtual public PoweredDevice
        {
        };

        class Copier: public Scanner, public Printer
        {
        };

    -- However, this leads to one more problem: if Scanner and Printer share a PoweredDevice base class, who is responsible for creating it?
       The answer, as it turns out, is Copier. The Copier constructor is responsible for creating PoweredDevice. 
       
       NOTE : Consequently, this is one time when Copier is allowed to call a non-immediate-parent constructor directly.
    
    -- First, for the constructor of the most derived class, virtual base classes are always created before non-virtual base classes, which ensures all bases get created before their derived classes.
    -- Second, note that the Scanner and Printer constructors still have calls to the PoweredDevice constructor. 
       When creating an instance of Copier, these constructor calls are simply ignored because Copier is responsible for creating the PoweredDevice, not Scanner or Printer.
       However, if we were to create an instance of Scanner or Printer, those constructor calls would be used, and normal inheritance rules apply.
    -- Third, if a class inherits one or more classes that have virtual parents, the most derived class is responsible for constructing the virtual base class. 
       In this case, Copier inherits Printer and Scanner, both of which have a PoweredDevice virtual base class. Copier, the most derived class, is responsible for creation of PoweredDevice.

        NOTE : Note that this is true even in a single inheritance case: if Copier singly inherited from Printer, and Printer was virtually inherited from PoweredDevice, Copier is still responsible for creating PoweredDevice.
    -- Fourth, all classes inheriting a virtual base class will have a virtual table, even if they would normally not have one otherwise, and thus instances of the class will be larger by a pointer.

    -- Because Scanner and Printer derive virtually from PoweredDevice, Copier will only be one PoweredDevice subobject. 
       Scanner and Printer both need to know how to find that single PoweredDevice subobject, so they can access its members (because after all, they are derived from it). 
       This is typically done through some virtual table magic (which essentially stores the offset from each subclass to the PoweredDevice subobject).

9 — Object slicing
    -- When we assign a Derived object to a Base object, only the Base portion of the Derived object is copied.The Derived portion is not.
    -- That Derived portion has effectively been “sliced off”. Consequently, the assigning of a Derived class object to a Base class object is called object slicing (or slicing for short).
    -- Used conscientiously, slicing can be benign. However, used improperly, slicing can cause unexpected results in quite a few different ways.

    Slicing and functions
        why would we assign derived to base like that?
        -- Consider a pretty simple function with a const base object parameter that is passed by value and we call the function with derived object by mistake.
           Of course, slicing here can all be easily avoided by making the function parameter a reference instead of a pass by value.
           ( yet another reason why passing classes by reference instead of value is a good idea )
    
    Slicing vectors
        -- Yet another area where new programmers run into trouble with slicing is trying to implement polymorphism with std::vector. 
            	std::vector<Base> v{};
	            v.push_back(Base{ 5 });    // add a Base object to our vector
	            v.push_back(Derived{ 6 }); // add a Derived object to our vector
        
        -- Fixing this is a little more difficult. Many new programmers try creating a std::vector of references to an object, like this:
                std::vector<Base&> v{};
            Unfortunately, this won’t compile. The elements of std::vector must be assignable, whereas references can’t be reassigned (only initialized).
        
        -- One way to address this is to make a vector of pointers:
           A few comments about this. 
            -- First, nullptr is now a valid option, which may or may not be desirable.
            -- Second, you now have to deal with pointer semantics, which can be awkward. 
            -- But the upside is that using pointers allows us to put dynamically allocated objects in the vector.

        -- Another option is to use std::reference_wrapper, which is a class that mimics an reassignable reference:

    The Frankenobject
        -- we’ve seen cases where slicing lead to the wrong result because the derived class had been sliced off. Now let’s take a look at another dangerous case where the derived object still exists!
            Consider the following code:
                int main()
                {
                    Derived d1{ 5 };
                    Derived d2{ 6 };
                    Base& b{ d2 };

                    b = d1; // this line is problematic

                    return 0;
                }
        -- The first three lines in the function are pretty straightforward. Create two Derived objects, and set a Base reference to the second one.
        
        -- The fourth line is where things go astray. Since b points at d2, and we’re assigning d1 to b, you might think that the result would be that d1 would get copied into d2 -- and it would, if b were a Derived. 
           But b is a Base, and the operator= that C++ provides for classes isn’t virtual by default. Consequently, the assignment operator that copies a Base is invoked, and only the Base portion of d1 is copied into d2.
        
        -- As a result, you’ll discover that d2 now has the Base portion of d1 and the Derived portion of d2. In this particular example, that’s not a problem (because the Derived class has no data of its own), 
           but in most cases, you’ll have just created a Frankenobject -- composed of parts of multiple objects.

        -- Worse, there’s no easy way to prevent this from happening (other than avoiding assignments like this as much as possible).

    NOTE : If the Base class is not designed to be instantiated by itself (e.g. it is just an interface class), slicing can be avoided by making the Base class non-copyable (by deleting the Base copy constructor and Base assignment operator).

    Conclusion
    Although C++ supports assigning derived objects to base objects via object slicing, in general, this is likely to cause nothing but headaches.
    We should generally try to avoid slicing. Make sure your function parameters are references (or pointers) and try to avoid any kind of pass-by-value when it comes to derived classes.

10 — Dynamic casting
    -- When dealing with polymorphism, you’ll often encounter cases where you have a pointer to a base class, but you want to access some information that exists only in a derived class.
    -- We know that C++ will implicitly let you convert a Derived pointer into a Base pointer (in fact, getObject() does just that). 
       This process is sometimes called upcasting. However, what if there was a way to convert a Base pointer back into a Derived pointer? 
       Then we could call Derived::getName() directly using that pointer, and not have to worry about virtual function resolution at all.
    
    dynamic_cast
        -- C++ provides a casting operator named dynamic_cast that can be used for just this purpose. Although dynamic casts have a few different capabilities, 
           by far the most common use for dynamic casting is for converting base-class pointers into derived-class pointers. This process is called downcasting.
    
    dynamic_cast failure
        -- dynamic_cast will be successful if the Base pointer points to Derived object.
        -- dynamic_cast will fail if the Base pointer points to Base object. Because the conversion can’t be made.
            If a dynamic_cast fails, the result of the conversion will be a null pointer.
        -- Always ensure your dynamic casts actually succeeded by checking for a null pointer result.
    
    Note that because dynamic_cast does some consistency checking at runtime (to ensure the conversion can be made), use of dynamic_cast does incur a performance penalty.
    
    Also note that there are several cases where downcasting using dynamic_cast will not work:
        -- With protected or private inheritance.
        -- For classes that do not declare or inherit any virtual functions (and thus don’t have a virtual table).
        -- In certain cases involving virtual base classes 
    
    Downcasting with static_cast
        -- It turns out that downcasting can also be done with static_cast. 
           The main difference is that static_cast does no runtime type checking to ensure that what you’re doing makes sense.
           This makes using static_cast faster, but more dangerous. 
        -- If you cast a Base* to a Derived*, it will “succeed” even if the Base pointer isn’t pointing to a Derived object. 
           This will result in undefined behavior when you try to access the resulting Derived pointer (that is actually pointing to a Base object).
        
        -- If you’re absolutely sure that the pointer you’re downcasting will succeed, then using static_cast is acceptable.
    
    dynamic_cast and references
        -- dynamic_cast can also be used with references. This works analogously to how dynamic_cast works with pointers.
        -- Because C++ does not have a “null reference”, dynamic_cast can’t return a null reference upon failure. 
           Instead, if the dynamic_cast of a reference fails, an exception of type std::bad_cast is thrown.
    
    When to use dynamic_cast and static_cast?
        -- The answer is quite simple: use static_cast unless you’re downcasting, in which case dynamic_cast is usually a better choice. 
           However, you should also consider avoiding casting altogether and just use virtual functions.
    
    Downcasting vs virtual functions
        -- here are some developers who believe dynamic_cast is evil and indicative of a bad class design. 
           Instead, these programmers say you should use virtual functions.
        -- In general, using a virtual function should be preferred over downcasting. 
           However, there are times when downcasting is the better choice:
           -- When you can not modify the base class to add a virtual function (e.g. because the base class is part of the standard library)
           -- When you need access to something that is derived-class specific (e.g. an access function that only exists in the derived class)
           -- When adding a virtual function to your base class doesn’t make sense
              ( e.g. there is no appropriate value for the base class to return ). 
              ( Using a pure virtual function may be an option here if you don’t need to instantiate the base class. )
            
    A warning about dynamic_cast and RTTI
        -- Run-time type information (RTTI) is a feature of C++ that exposes information about an object’s data type at runtime. 
        -- This capability is leveraged by dynamic_cast.
        
        Why dynamic_cast does not work with protected or private inheritance?
            -- dynamic_cast relies on runtime type information (RTTI) and public inheritance that expresses an is-a relationship.
            -- When inheritance is protected or private, that public is-a relationship is intentionally hidden, so the type conversion is not allowed externally.

        -- Because RTTI has a pretty significant space performance cost, some compilers allow you to turn RTTI off as an optimization. 
           Needless to say, if you do this, dynamic_cast won’t function correctly. 
         
11 — Printing inherited classes using operator<<
     The challenges with operator<< 
        -- Operators are free or non-virtual functions, so they cannot be made virtual directly 
           (except member operators, but most stream operators are free functions).

        -- Overload resolution happens at compile time, preventing polymorphism when the operator is invoked through a base class reference.

        -- Can lead to unexpected behavior where a derived object is handled as a base object.

        Requires workarounds such as:
            -- Calling a virtual function from within the operator
            -- Using double dispatch or visitor patterns
            -- Avoiding such operators for polymorphic output and using named virtual functions instead

        The main challenge with operators in polymorphism is that operator overloading does not participate in virtual dispatch by default. 
        Operator overload resolution is based on the static type of the object, not the dynamic type, which prevents expected runtime 
        polymorphic behavior unless the operator internally calls a virtual function.
    
    Can we make operator<< virtual?
        -- No. There are a number of reasons for this.
        -- First, only member functions can be virtualized -- this makes sense, since only classes can inherit from other classes, 
           and there’s no way to override a function that lives outside of a class.
        -- We can overload non-member functions, but not override them.
        -- Because we typically implement operator<< as a friend, and friends aren’t considered member functions, 
           a friend version of operator<< is ineligible to be virtualized.
        
        -- Second, even if we could virtualize operator<< there’s the problem that the function parameters for Base::operator<< and Derived::operator<< 
           differ (the Base version would take a Base parameter and the Derived version would take a Derived parameter). 
           Consequently, the Derived version wouldn’t be considered an override of the Base version, and thus be ineligible for virtual function resolution.
        
    The solution: Delegating to a virtual member function
        -- The solution to this challenge is surprisingly simple and elegant: we have friend operator<< delegate to a virtual member function!
        
        How it works:
        1. In the Base class:
           -- Declare friend operator<<(std::ostream& out, const Base& b) taking a const Base&.
           -- Inside this operator<<, instead of directly streaming data members, delegate the work by calling a virtual member function:
              return b.print(out);
           -- Declare and define the virtual member function:
              virtual std::ostream& print(std::ostream& out) const
              {
                  out << "Base";
                  return out;
              }
        2. In the Derived class:
           -- Override print(std::ostream& out) const override:
              std::ostream& print(std::ostream& out) const override
              {
                  out << "Derived";
                  // Output derived specific members here
                  return out;
              }
        
        Why this works:
        -- When operator<< is invoked on a Base& that actually refers to a Derived object:
           - Overload resolution binds the call to Base's friend operator<< at compile time (since the reference type is Base&).
           - Inside operator<<, calling b.print(out) performs dynamic dispatch via the vtable because print() is virtual.
           - C++ resolves the call to the most-derived override (Derived::print()) at runtime!
           - Derived::print() can also invoke Base::print(out) to print base-class fields first, ensuring clean composition.
        
        Alternative lightweight approach:
        -- If classes only need to identify their type name, Base can define:
           virtual std::string_view getType() const { return "Base"; }
           and friend operator<< streams ref.getType().
        -- However, delegating to virtual std::ostream& print(std::ostream& out) const is far more flexible because it allows each derived class to stream complex formatted data members directly into the stream.