1 — Template classes
    -- A template is not a class or a function -- it is a stencil used to create classes or functions. 
       As such, it does not work in quite the same way as normal functions or classes. 
       In most cases, this isn’t much of a issue. However, there is one area that commonly causes problems for developers.

    -- With non-template classes, the common procedure is to put the class definition in a header file, and the member function 
       definitions in a similarly named code file. In this way, the member function definitions are compiled as a separate project file. 
       However, with templates, this does not work.
    
    -- Just like with function templates, the compiler will only instantiate a class template if the class template is used 
       (e.g. as the type of an object like intArray) in a translation unit. In order to perform the instantiation, 
       the compiler must see both the full class template definition (not just a declaration) and the specific template type(s) needed.
    
    -- The easiest way is to simply put all of your template class code in the header file. In this way, when you #include the header, 
       all of the template code will be in one place. The upside of this solution is that it is simple. The downside here is that 
       if the template class is used in many files, you will end up with many local instances of the template class, which can increase your compile and link times.

    -- Another alternative is to use a three-file approach. 
        -- The template class definition goes in the header. (Example : Array.hpp)
        -- The template class member functions goes in the code file. (Example : Array.cpp)
        -- Then you add a third file, which contains all of the instantiated classes you need.  (Example : templates.cpp)

        templates.cpp
        // Ensure the full Array template definition can be seen
        #include "Array.h"
        #include "Array.cpp" // we're breaking best practices here, but only in this one place

        // #include other .h and .cpp template definitions you need here

        template class Array<int>; // Explicitly instantiate template Array<int>
        template class Array<double>; // Explicitly instantiate template Array<double>

        // instantiate other templates here

2 — Template non-type parameters
    -- A template type parameter is a placeholder type that is substituted for a type passed in as an argument.
    -- However, template type parameters are not the only type of template parameters available. 
       Template classes and functions can make use of another kind of template parameter known as a non-type parameter.
        
    Non-type parameters
        -- A template non-type parameter is a template parameter where the type of the parameter is predefined
           and is substituted for a constexpr value passed in as an argument.
        A non-type parameter can be any of the following types:
            -- An integral type
            -- An enumeration type
            -- A pointer or reference to a class object
            -- A pointer or reference to a function
            -- A pointer or reference to a class member function
            -- std::nullptr_t
            -- A floating point type (since C++20)
    
    -- The non-type template type parameters must be constexpr. Otherwise, compiler will issue an error.
  
3 — Function template specialization
    -- When instantiating a function template for a given type, the compiler stencils out a copy of the templated function 
       and replaces the template type parameters with the actual types used in the variable declaration. This means a particular function 
       will have the same implementation details for each instanced type (just using different types). 
       
       While most of the time this is exactly what you want, occasionally there are cases where it is useful to implement 
       a templated function slightly differently for a specific data type.
    -- The non-template function doesn’t need to have the same signature as the function template.
    
    Function template specialization
        -- Another way to achieve a similar result is to use explicit template specialization.
        -- Explicit template specialization (often shortened to template specialization) is a feature that allows us 
           to explicitly define different implementations of a template for specific types or values.
        
        Full specialization     : When all of the template parameters are specialized.
        Partial specialization  : When only some of the template parameters are specialized.
    
        -- In order to specialize a template, the compiler first must have seen a declaration for the primary template.
        -- First, we need a template parameter declaration so the compiler knows we’re doing something related to templates.
        -- However, in case, we don’t actually need any template parameters, so we use an empty pair of angled brackets. 
           Since we have no template parameters in the specialization, this is a full specialization.
        -- On the next line, FUNCTION<TYPE> tells the compiler that we’re specializing the FUNCTION primary template function for TYPE.
        -- The specialization must have the same signature as the primary template.
           (except the specialization substitutes TYPE anywhere the primary template uses T).
        
        Note : if a matching non-template function and a matching template function specialization both exist, 
               the non-template function will take precedence.
               
               Also, full specializations are not implicitly inline,
               so if you define one in a header file, make sure you inline it to avoid ODR violations.
        -- Just like normal functions, function template specializations can be deleted (using = delete).
           Any function calls that resolve to the specialization will produce a compilation error.
        -- We should avoid function template specializations in favor of non-template functions whenever possible.

4 — Class template specialization
    -- It allows us to specialize a template class for a particular data type (or data types, if there are multiple template parameters). 
       In this case, we’re going to use class template specialization to write a customized version of Storage8<bool> that 
       will take precedence over the generic Storage8<T> class.
    
    -- Class template specializations are treated as completely independent classes, even though they are instantiated in the same way as the templated class.
       This means that we can change anything and everything about our specialization class, including the way it’s implemented and even the functions 
       it makes public, just as if it were an independent class.
    
    -- Just like all templates, the compiler must be able to see the full definition of a specialization to use it. 
       Also, defining a class template specialization requires the non-specialized class to be defined first.
    
    -- First, note that our specialized class template starts off with template<>. The template keyword tells the compiler that what follows 
       is a template, and the empty angle braces means that there aren’t any template parameters. In this case, there aren’t any template parameters 
       because we’re replacing the only template parameter (T) with a specific type (bool).

    -- Next, we add <bool> to the class name to denote that we’re specializing a bool version of class Storage8.

    -- Now, when we instantiate an object type Storage<T>, where T is not a bool, we’ll get a version stenciled from the generic templated Storage8<T> class. 
       When we instantiate an object of type Storage8<bool>, we’ll get the specialized version we just created.
    
    Specializing member functions
        -- Explicit function specializations are not implicitly inline, so we should mark our explicit specialization as inline 
           if it is defined it in a header file.
        
        Where to define class template specializations
            -- In order to use a specialization, the compiler must be able to see the full definition of both 
               the non-specialized class and the specialized class.
            -- If the compiler can only see the definition of the non-specialized class, it will use that instead of the specialization.
            
            -- For this reason, specialized classes and functions are often defined in a header file just below the definition of the non-specialized 
               class, so that including a single header includes both the non-specialized class and any specializations. This ensures the specialization 
               can always be seen whenever the non-specialized class can also be seen.

            -- If a specialization is only required in a single translation unit, it can be defined in the source file for that translation unit. 
               Because other translation units will not be able to see the definition of the specialization, they will continue to use the non-specialized version.
            
            Avoid placing template specializations in separate headers that change behavior depending on whether they’re included.
            -- If you forget to include the specialization header, you unintentionally get the primary template;
            -- If another header includes it indirectly, you may get the specialization without meaning to.
               This leads to unpredictable and fragile behavior.

5 — Partial template specialization
    -- Partial template specialization allows us to specialize classes (but not individual functions!) where some, 
       but not all, of the template parameters have been explicitly defined.
    -- Partial template specialization can only be used with classes, not template functions (functions must be fully specialized).

    Partial template specialization for member functions
        -- The limitation on the partial specialization of functions can lead to some challenges when dealing with member functions. 
        -- Because, Partial template specialization for class works
                    Partial template specialization for member function doesn't work without partially specialized class definition.
                
6 — Partial template specialization for pointers
    -- When using class templates with pointer types, subtle and dangerous issues frequently arise regarding ownership, lifetimes, and copying semantics.
    
    The problem with pointers in generic templates:
        -- In a generic class template Storage<T>, the class stores and copies objects of type T by value.
        -- If T is a pointer type (e.g. double*), Storage<double*> simply stores the memory address.
        -- This leads to several serious issues:
           1. Shallow copy vs Deep copy:
              Copying Storage<double*> only copies the pointer address, not the underlying value. Both Storage instances point to the same memory.
           2. Lifetime & Dangling Pointers:
              If the object pointed to is destroyed or falls out of scope, Storage holds a dangling pointer.
           3. Resource Leaks:
              If Storage does not free the pointer in its destructor, dynamically allocated memory is leaked.
              However, if Storage does free the pointer in its destructor (delete m_value), copying Storage causes a double-free crash!
           4. Dereferencing & Printing:
              A generic print() function (std::cout << m_value) prints the raw memory address (e.g. 0x7ffe...) rather than the actual dereferenced value (*m_value).
    
    Partial template specialization for pointer types:
        -- To solve these issues, we can partially specialize the class template specifically for pointer types:
           template <typename T>
           class Storage<T*>
           {
               // Custom implementation specifically designed for pointers
           };
        -- Note that this is a partial specialization because T is still a template parameter, but it is restricted to pointer types (T*).
    
    Managing ownership with smart pointers:
        -- Rather than managing raw pointers manually with new and delete, the best practice is to have Storage<T*> manage ownership safely using std::unique_ptr<T>:
           template <typename T>
           class Storage<T*>
           {
               std::unique_ptr<T> m_value{};
           public:
               Storage(T* val) : m_value{ std::make_unique<T>(val ? *val : 0) } {}
               
               void print() const
               {
                   if (m_value)
                       std::cout << *m_value << '\n';
               }
           };
        -- This creates a deep copy on the heap and guarantees that the allocated memory is automatically freed when Storage<T*> is destroyed, with no risk of memory leaks or double-free crashes.
    
    Restricting pointers completely:
        -- In some designs, storing pointers in a container is fundamentally unsafe or meaningless.
        -- We can explicitly forbid instantiating the template with pointer types or nullptr using a compile-time static_assert:
           template <typename T>
           class Storage
           {
               static_assert(!std::is_pointer_v<T> && !std::is_null_pointer_v<T>, 
                             "Storage<T*> and Storage<nullptr> are disallowed");
               T m_value{};
               // ...
           };
        -- Any attempt to instantiate Storage<int*> or Storage<nullptr_t> will immediately fail compilation with a clear, readable error message.