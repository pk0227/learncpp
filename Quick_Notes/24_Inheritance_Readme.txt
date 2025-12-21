1 — Introduction to inheritance
    -- Inheritance involves creating new objects by directly acquiring the attributes and behaviors of other objects and then extending or specializing them.

2 — Basic inheritance in C++
    -- Inheritance in C++ takes place between classes. In an inheritance (is-a) relationship.
    -- The class being inherited from is called the parent class, base class, or superclass.
    -- The class doing the inheriting is called the child class, derived class, or subclass.
    -- we use a colon, the word “public/protected/private”, and the name of the class we wish to inherit from.

    Inheritance chains
        -- It’s possible to inherit from a class that is itself derived from another class. 
        -- By constructing such inheritance chains, we can create a set of reusable classes that are very general (at the top) and become progressively more specific at each level of inheritance.

    Why is this kind of inheritance useful?
        -- Inheriting from a base class means we don’t have to redefine the information from the base class in our derived classes. 
           We automatically receive the member functions and member variables of the base class through inheritance, and then simply add the additional functions or member variables we want. 
           This not only saves work, but also means that if we ever update or modify the base class (e.g. add new functions, or fix a bug), all of our derived classes will automatically inherit the changes.

3 — Order of construction of derived classes
    -- Because Derived inherits functions and variables from Base, we may assume that the members of Base are copied into Derived. 
       However, this is not true. Instead, we can consider Derived as a two part class: 
        -- one part Derived, and one part Base.
    -- When C++ constructs derived objects, it does so in phases. 
        First, the most-base class (at the top of the inheritance tree) is constructed. 
        Then each child class is constructed in order, until the most-child class (at the bottom of the inheritance tree) is constructed last.
    -- So, when we instantiate an instance of Derived, 
        first the Base portion of Derived is constructed (using the Base default constructor). 
        Once the Base portion is finished, the Derived portion is constructed (using the Derived default constructor). 
    -- The child class often uses variables and functions from the parent, but the parent class knows nothing about the child. 
       Instantiating the parent class first ensures those variables are already initialized by the time the derived class is created and ready to use them.
    
    Order of construction for inheritance chains
        -- In the cases where the classes are derived from other classes, which are themselves derived from other classes,
           C++ always constructs the “first” or “most base” class first. It then walks through the inheritance tree in order and constructs each successive derived class.

4 — Constructors and initialization of derived classes
    -- The only real difference between this case and the non-inherited case is that before the Derived constructor can do anything substantial, the Base constructor is called first. 
       The Base constructor sets up the Base portion of the object, control is returned to the Derived constructor, and the Derived constructor is allowed to finish up its job.
    
        Initializing base class members
            -- C++ prevents classes from initializing inherited member variables in the member initializer list of its constructor.
            
            Why does C++ do this? 
            The answer has to do with const and reference variables. Consider what would happen if member(s) of base class were const. 
            Because const variables must be initialized with a value at the time of creation, the base class constructor must set its value when the variable is created. 
            However, when the base class constructor finishes, the derived class constructor’s member initializer lists are then executed. 
            Each derived class would then have the opportunity to initialize that variable, potentially changing its value! 
            By restricting the initialization of variables to the constructor of the class those variables belong to, C++ ensures that all variables are initialized only once.

            -- However, inherited variables can still have their values changed in the body of the constructor using an assignment unless they are const/reference members.
            -- Fortunately, C++ gives us the ability to explicitly choose which Base class constructor will be called! To do this, simply add a call to the Base class constructor in the member initializer list of the derived class.
            -- Note that it doesn’t matter where in the Derived constructor member initializer list the Base constructor is called -- it will always execute first.

        -- Private members can only be accessed by member functions of the same class. Note that this means derived classes can not access private members of the base class directly! Derived classes will need to use access functions to access private members of the base class.
        
        Inheritance chains
            -- It is worth mentioning that constructors can only call constructors from their immediate parent/base class.
                Example : Consequently, the C constructor could not call or pass parameters to the A constructor directly. The C constructor can only call the B constructor (which has the responsibility of calling the A constructor). 
        
        Destructors
        -- When a derived class is destroyed, each destructor is called in the reverse order of construction. In the above example, when c is destroyed, the C destructor is called first, then the B destructor, then the A destructor.
        -- If your base class has virtual functions, your destructor should also be virtual, otherwise undefined behavior will result in certain cases. 

        -- When constructing a derived class, the derived class constructor is responsible for determining which base class constructor is called. If no base class constructor is specified, the default base class constructor will be used.

5 — Inheritance and access specifiers
    -- Private members can only be accessed by member functions of the same class or friends. This means derived classes can not access private members of the base class directly.

    The protected access specifier
        -- The protected access specifier allows the members declared under it to be accessed by 
            -- the class’s own member functions,
            -- its friends,
            -- its derived classes.
        -- However, protected members are not accessible from outside the class.
    
    So when should I use the protected access specifier?
        -- In general, it’s better to make your members private if you can, and only use protected when derived classes are planned and the cost to build and maintain an interface to those private members is too high.
        -- Favor private members over protected members.

    Different kinds of inheritance, and their impact on access
        -- If we do not choose an inheritance type, C++ defaults to private inheritance (just like members default to private access if you do not specify otherwise).
        -- when members are inherited, the access specifier for an inherited member may be changed (in the derived class only) depending on the type of inheritance used. 
           Put another way, members that were public or protected in the base class may change access specifiers in the derived class.
        
        Keep in mind the following rules as we step through the examples:
            -- A class can always access its own (non-inherited) members.
            -- The public accesses the members of a class based on the access specifiers of the class it is accessing.
            -- A derived class accesses inherited members based on the access specifier inherited from the parent class. This varies depending on the access specifier and type of inheritance used.

        Public inheritance
        -- When you inherit a base class publicly, inherited public members stay public, and inherited protected members stay protected. 
           Inherited private members, which were inaccessible because they were private in the base class, stay inaccessible.
        -- Use public inheritance unless you have a specific reason to do otherwise.

        Protected inheritance
        -- Protected inheritance is the least common method of inheritance. It is almost never used, except in very particular cases.
        -- With protected inheritance, the public and protected members become protected, and private members stay inaccessible.

        Private inheritance
        -- With private inheritance, all members from the base class are inherited as private.
        -- The protected and public members become private, and private members stay inaccessible.
        -- Private inheritance can be useful when the derived class has no obvious relationship to the base class, 
           but uses the base class for implementation internally. In such a case, we probably don’t want the public interface of the base class to be exposed through objects of the derived class.
        
    Final Summary
    -- First, a class (and friends) can always access its own non-inherited members. The access specifiers only affect whether outsiders and derived classes can access those members.
    -- Second, when derived classes inherit members, those members may change access specifiers in the derived class. This does not affect the derived classes’ own (non-inherited) members (which have their own access specifiers). 
       It only affects whether outsiders and classes derived from the derived class can access those inherited members.
    
    +-----------------------------+-------------------------------+-------------------------------+-------------------------------+
    | Access specifier            | Access specifier when         | Access specifier when         | Access specifier when         |
    | in base class               | inherited publicly            | inherited privately           | inherited protectedly         |
    +-----------------------------+-------------------------------+-------------------------------+-------------------------------+
    | Public                      | Public                        | Private                       | Protected                     |
    | Protected                   | Protected                     | Private                       | Protected                     |
    | Private                     | Inaccessible                  | Inaccessible                  | Inaccessible                  |
    +-----------------------------+-------------------------------+-------------------------------+-------------------------------+

6 — Adding new functionality to a derived class
    -- For an example, getValue() function has been defined in Derived class with public access specifier, then, Base doesn't have access to the function.
       This is because there is no getValue() function in Base. Function getValue() belongs to Derived. Because Derived is-a Base, Derived has access to stuff in Base. 
       However, Base does not have access to anything in Derived.
    
7 — Calling inherited functions and overriding behavior
    -- When a member function is called on a derived class object, the compiler first looks to see if any function with that name exists in the derived class. 
       If so, all overloaded functions with that name are considered, and the function overload resolution process is used to determine whether there is a best match. 
       If not, the compiler walks up the inheritance chain, checking each parent class in turn in the same way.
    -- Put another way, the compiler will select the best matching function from the most-derived class with at least one function with that name.

    Calling a base class function and Redefining behaviors
        -- Note that when you redefine a function in the derived class, the derived function does not inherit the access specifier of the function with the same name in the base class. 
           It uses whatever access specifier it is defined under in the derived class. Therefore, a function that is defined as private in the base class can be redefined as public in the derived class, or vice-versa!
    
    Calling Base function using scope resolution operator
        -- In case, if we want to have a derived function call a base function of the same name, simply do a normal function call, but prefix the function with the scope qualifier of the base class. 
           Calling such function without a scope resolution qualifier would default to the same function in the Derived class. This would cause Derived function to call itself, which would lead to an infinite recursion.
        
        Note:
        There’s one bit of trickiness that we can run into when trying to call friend functions in base classes, such as operator<<. 
        Because friend functions of the base class aren’t actually part of the base class, using the scope resolution qualifier won’t work. 
        Instead, we need a way to make our Derived class temporarily look like the Base class so that the right version of the function can be called.

    Overload resolution in derived classes
        -- As noted at the top of the lesson, the compiler will select the best matching function from the most-derived class with at least one function with that name.
            Go through some interesting examples available at https://github.com/pk0227
    
8 — Hiding inherited functionality
    Changing an inherited member’s access level
        -- C++ gives us the ability to change an inherited member’s access specifier in the derived class. 
           This is done by using a using declaration to identify the (scoped) base class member that is having its access changed in the derived class, under the new access specifier.
        -- We can only change the access specifiers of base members the derived class would normally be able to access. 
           Therefore, we can never change the access specifier of a base member from private to protected or public, because derived classes do not have access to private members of the base class.
    
    Hiding functionality
        -- In a derived class, it is possible to hide functionality that exists in the base class, so that it can not be accessed through the derived class. 
           This can be done simply by changing the relevant access specifier.
        -- However, it is worth noting that while public member of base can be made private in the Derived class, it is still public in the Base class. 
           Therefore the encapsulation of the member in Derived can still be subverted by casting to Base& and directly accessing the member.
           The member can be variable, function(the function can be virtual function too)
        -- Perhaps surprisingly, given a set of overloaded functions in the base class, there is no way to change the access specifier for a single overload. You can only change them all.
    
    Deleting functions in the derived class
        -- We can also mark member functions as deleted in the derived class, which ensures they can’t be called at all through a derived object.
        -- Note that the Base version of the function is still accessible though. 
           We can call the function either with Base scope resolution(Base::) operator or by casting to Base.

9 — Multiple inheritance
    Multiple inheritance
        -- Multiple inheritance enables a derived class to inherit members from more than one parent.
        -- To use multiple inheritance, simply specify each base class (just like in single inheritance), separated by a comma.

    Mixins
        -- A mixin (also spelled “mix-in”) is a small class that can be inherited from in order to add properties to a class. 
           The name mixin indicates that the class is intended to be mixed into other classes, not instantiated on its own.
        -- Because mixins are designed to add functionality to the derived class, not to provide an interface, mixins typically do not use virtual functions (covered in the next chapter).
        -- Instead, if a mixin class needs to be customized to work in a particular way, templates are typically used. For this reason, mixin classes are often templatized.
        -- Perhaps surprisingly, a derived class can inherit from a mixin base class using the derived class as a template type parameter. 
           Such inheritance is called Curiously Recurring Template Pattern (CRTP for short), which looks like this,
                // The Curiously Recurring Template Pattern (CRTP)

                template <class T>
                class Mixin
                {
                    // Mixin<T> can use template type parameter T to access members of Derived
                    // via (static_cast<T*>(this))
                };

                class Derived : public Mixin<Derived>
                {
                };

    Problems with multiple inheritance
        -- Increase the complexity of programs and make them a maintenance nightmare.
        -- Ambiguity can result when multiple base classes contain a function with the same name.
        -- Though, there is a way to work around Ambiguity problem (explicitly specifying which version we meant to call using class as prefix),
           The potential for naming conflicts increases exponentially as you inherit more classes, and each of these naming conflicts needs to be resolved explicitly.
        -- More serious is the diamond problem.
            -- This occurs when a class multiply inherits from two classes which each inherit from a single base class. 
               This leads to a diamond shaped inheritance pattern.
    
    Is multiple inheritance more trouble than it’s worth?
        -- Most of the problems that can be solved using multiple inheritance can be solved using single inheritance as well. 
        -- But, there are times and situations when multiple inheritance is the best way to proceed. 
           However, multiple inheritance should be used extremely judiciously.
        -- Many C++ standard libraries use multiple inheritance. 
            Example : The iostream library objects std::cin and std::cout are both implemented using multiple inheritance!
        
        -- Avoid multiple inheritance unless alternatives lead to more complexity.