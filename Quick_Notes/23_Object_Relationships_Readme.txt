1 — Object relationships
    -- This process of building complex objects from simpler ones is called object composition.
    -- Object composition models a “has-a” relationship between two objects. 
        -- The complex object is sometimes called the whole, or the parent.
        -- The simpler object is often called the part, child, or component.
    
    Types of object composition
    There are two basic subtypes of object composition: 
        -- composition
        -- aggregation.
        
        -- The term “composition” is often used to refer to both composition and aggregation.
        -- For our convenience, 
            -- object composition -> referring to both
            -- composition        -> composition subtype
        
2 — Composition
    To qualify as a composition, an object and a part must have the following relationship:
        -- The part (member) is part of the object (class)
        -- The part (member) can only belong to one object (class) at a time
        -- The part (member) has its existence managed by the object (class)
        -- The part (member) does not know about the existence of the object (class)
    
    Explanation :
        -- The part is created when the object is created, and destroyed when the object is destroyed. 
           It means the object manages the part’s lifetime.
        -- This is a unidirectional relationship.
    
    Variants on the composition theme.
    For example:
        -- A composition may defer creation of some parts until they are needed. 
           For example, a string class may not create a dynamic array of characters until the user assigns the string some data to hold.
        -- A composition may opt to use a part that has been given to it as input rather than create the part itself.
        -- A composition may delegate destruction of its parts to some other object (e.g. to a garbage collection routine).

    The key point here is that the composition should manage its parts without the user of the composition needing to manage anything.

3 — Aggregation
    -- To qualify as an aggregation, a whole object and its parts must have the following relationship:
        -- The part (member) is part of the object (class)
        -- The part (member) can (if desired) belong to more than one object (class) at a time
        -- The part (member) does not have its existence managed by the object (class)
        -- The part (member) does not know about the existence of the object (class)
    -- It is a unidirectional relationship.
    -- When an aggregation is created, the aggregation is not responsible for creating the parts. When an aggregation is destroyed, the aggregation is not responsible for destroying the parts.
    
    Summarizing composition and aggregation
    Compositions:
        -- Typically use normal member variables
        -- Can use pointer members if the class handles object allocation/deallocation itself
        -- Responsible for creation/destruction of parts

    Aggregations:
        -- Typically use pointer or reference members that point to or reference objects that live outside the scope of the aggregate class
        -- Not responsible for creating/destroying parts
    
    -- It is worth noting that the concepts of composition and aggregation can be mixed freely within the same class.
    -- compositions should be favored over aggregations.

    A few warnings/errata
        -- aggregates and aggregation are different and should not be confused.
        -- Aggregate means
            a struct or class that has no provided constructors, destructors, or overloaded assignment, has all public members, and does not use inheritance -- essentially a plain-old-data struct.
    
    std::reference_wrapper
        -- a list of some kind (e.g. a std::vector) but fixed arrays and the various standard library lists can’t hold references (because list elements must be assignable, and references can’t be reassigned).
            std::vector<const Teacher&> m_teachers{}; // Illegal
        -- Instead of references, we could use pointers, but that would open the possibility to store or pass null pointers.
        -- If we don’t want to allow null pointers. To solve this, there’s std::reference_wrapper.
        -- Essentially, std::reference_wrapper is a class that acts like a reference, but also allows assignment and copying, so it’s compatible with lists like std::vector.

        The good news is that you don’t really need to understand how it works to use it. All you need to know are three things:
            -- std::reference_wrapper lives in the <functional> header.
            -- When you create your std::reference_wrapper wrapped object, the object can’t be an anonymous object (since anonymous objects have expression scope, and this would leave the reference dangling).
            -- When you want to get your object back out of std::reference_wrapper, you use the get() member function.
        
4 — Association
    -- A weaker type of relationship between two otherwise unrelated objects, called an association. 
    -- To qualify as an association, an object and another object must have the following relationship:
        -- The associated object (member) is otherwise unrelated to the object (class)
        -- The associated object (member) can belong to more than one object (class) at a time
        -- The associated object (member) does not have its existence managed by the object (class)
        -- The associated object (member) may or may not know about the existence of the object (class)
    -- A association models as “uses-a” relationship.

    Implementing associations
        -- Most often, associations are implemented using pointers, where the object points at the associated object.
    
    Reflexive association
        -- Sometimes objects may have a relationship with other objects of the same type. This is called a reflexive association.
    
    Associations can be indirect
        -- Any kind of data that allows us to link two objects together suffices. The link needs not strictly be either pointers or references to directly link objects together.
    
    Composition vs aggregation vs association summary
    +-------------------------------------------+--------------+--------------+---------------------------+
    | Property                                  | Composition  | Aggregation  | Association               |
    +-------------------------------------------+--------------+--------------+---------------------------+
    | Relationship type                         | Whole/part   | Whole/part   | Unrelated                 |
    | Members can belong to multiple classes    | No           | Yes          | Yes                       |
    | Members’ existence managed by class       | Yes          | No           | No                        |
    | Directionality                            | Unidirectional | Unidirectional | Uni/Bidirectional     |
    | Relationship verb                         | Part-of      | Has-a        | Uses-a                    |
    +-------------------------------------------+--------------+--------------+---------------------------+

5 — Container classes
    -- A container class is a class designed to hold and organize multiple instances of another type (either another class, or a fundamental type). 
    
    -- Although C++ has built-in array functionality, programmers will often use an array container class (std::array or std::vector) instead because of the additional benefits they provide.
    -- Unlike built-in arrays, array container classes generally provide dynamic resizing (when elements are added or removed), remember their size when they are passed to functions, and do bounds-checking.

    Container classes typically implement a fairly standardized minimal set of functionality. Most well-defined containers will include functions that:
        -- Create an empty container (via a constructor)
        -- Insert a new object into the container
        -- Remove an object from the container
        -- Report the number of objects currently in the container
        -- Empty the container of all objects
        -- Provide access to the stored objects
        -- Sort the elements (optional)
    
    Sometimes certain container classes will omit some of this functionality. For example, arrays container classes often omit the insert and remove functions because they are slow and the class designer does not want to encourage their use.

    Types of containers
    Container classes generally come in two different varieties. 
        -- Value containers are compositions that store copies of the objects that they are holding (and thus are responsible for creating and destroying those copies).
        -- Reference containers are aggregations that store pointers or references to other objects (and thus are not responsible for creation or destruction of those objects).
    
    -- In C++, containers typically only hold one type of data. For example, if you have an array of integers, it will only hold integers. 
    -- Unlike some other languages, many C++ containers do not allow you to arbitrarily mix types. 
    -- If you need containers to hold integers and doubles, you will generally have to write two separate containers to do this (or use templates, which is an advanced C++ feature). 

    Some advanced improvements related to exception handling:
        -- When performing resize or insertion operations, move elements only if their move constructor is noexcept.
        -- Provide a strong exception safety guarantee for resize or insertion operations
    
    One more thing: 
    -- If a class in the standard library meets your needs, use that instead of creating your own. 
       For example, instead of using IntArray, you’re better off using std::vector<int>. It’s battle tested, efficient, and plays nicely with the other classes in the standard library. 
       But sometimes you need a specialized container class that doesn’t exist in the standard library, so it’s good to know how to create your own when you need to.

6 — std::initializer_list
        Class initialization using std::initializer_list
            -- When a compiler sees an initializer list, it automatically converts it into an object of type std::initializer_list.
            -- Therefore, if we create a constructor that takes a std::initializer_list parameter, we can create objects using the initializer list as an input.
            -- std::initializer_list lives in the <initializer_list> header.

        There are a few things to know about std::initializer_list.
            -- Much like std::array or std::vector, you have to tell std::initializer_list what type of data the list holds using angled brackets.
            -- std::initializer_list has a (misnamed) size() function which returns the number of elements in the list.
            -- std::initializer_list is often passed by value. Much like std::string_view, std::initializer_list is a view. Copying a std::initializer_list does not copy the elements in the list.

        Accessing elements of a std::initializer_list
            std::initializer_list does not provide access to the elements of the list via subscripting (operator[]).
            However, there are a number of easy workarounds:
                -- You can use a range-based for loop to iterate over the elements of the list.
                -- Another way is to use the begin() member function to get an iterator to the std::initializer_list. Because this iterator is a random-access iterator, the iterators can be indexed:

        List initialization prefers list constructors over non-list constructors
            -- Non-empty initializer lists will always favor a matching initializer_list constructor over other potentially matching constructors. Consider:
                IntArray a1(5);   // uses IntArray(int), allocates an array of size 5
                IntArray a2{ 5 }; // uses IntArray<std::initializer_list<int>, allocates array of size 1
            -- The a1 case uses direct initialization (which doesn’t consider list constructors), so this definition will call IntArray(int), allocating an array of size 5.
            -- The a2 case uses list initialization (which favors list constructors). Both IntArray(int) and IntArray(std::initializer_list<int>) are possible matches here, but since list constructors are favored, 
               IntArray(std::initializer_list<int>) will be called, allocating an array of size 1 (with that element having value 5).
            -- The same happens to std::vector and other container classes that have both a list constructor and a constructor with a similar type of parameter
                std::vector<int> array(5); // Calls std::vector::vector(std::vector::size_type), 5 value-initialized elements: 0 0 0 0 0
                std::vector<int> array{ 5 }; // Calls std::vector::vector(std::initializer_list<int>), 1 element: 5
            -- List initialization favors matching list constructors over matching non-list constructors.
            
            When initializing a container that has a list constructor:
                -- Use brace initialization when intending to call the list constructor (e.g. because your initializers are element values)
                -- Use direct initialization when intending to call a non-list constructor (e.g. because your initializers are not element values).
            
            -- Adding list constructors to an existing class is dangerous

            Class assignment using std::initializer_list
            -- Note that if you implement a constructor that takes a std::initializer_list, you should ensure you do at least one of the following:
                -- Provide an overloaded list assignment operator
                -- Provide a proper deep-copying copy assignment operator
                -- Delete the copy assignment operator

            -- If you provide list construction, it’s a good idea to provide list assignment as well.