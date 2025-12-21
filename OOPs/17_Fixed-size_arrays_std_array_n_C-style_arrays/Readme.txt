1 — Introduction to std::array
    -- Containers provide storage for a collection of unnamed objects (called elements).
    -- Arrays allocate their elements contiguously in memory, and allow fast, direct access to any element via subscripting.
    -- C++ has three different array types that are commonly used: std::vector, std::array, and C-style arrays.
        Fixed-size arrays
            -- The length of the array be known at the point of instantiation, and that length cannot be changed afterward. 
            -- C-style arrays and std::array are both fixed-size arrays.
            -- Dynamic arrays can be resized at runtime. std::vector is a dynamic array.
    
    So why not use dynamic arrays for everything?
        -- Dynamic arrays are convenient but involve tradeoffs.
        -- std::vector is slightly slower than fixed-size arrays and has very limited constexpr support. In modern C++, this lack of 
           constexpr is the main drawback, since constexpr arrays enable more robust and better-optimized code.

        Best practice:
            -- Use std::array for constexpr arrays, and std::vector for non-constexpr arrays.
    
    Defining a std::array
            #include <array>  // for std::array
            std::array<int, 5> a {};  // a std::array of 5 ints
        -- std::array declaration has two template arguments. 
            The first (int) is a type template argument defining the type of the array.
            The second (5) is an integral non-type template argument defining the array length.
    
    The length of a std::array must be a constant expression
        -- Unlike std::vector, which can be resized at runtime, std::array has a fixed size that must be a constant expression, typically
           an integer literal, a constexpr variable, or an unscoped enumerator.
                std::array<int, 7> a {}; // Using a literal constant
                
                constexpr int len { 8 };
                std::array<int, len> b {}; // Using a constexpr variable

                std::array<int, Colors::max_colors> c {}; // Using an enumerator

        NOTE : non-const variables and runtime constants cannot be used for the length.

        -- Surprisingly, A std::array can have length 0, in which case it contains no data. Accessing elements (including via operator[])
           is undefined behavior. Use empty() to check whether a std::array is zero-length.
        
    Aggregate initialization of a std::array
        -- std::array is an aggregate (no constructors), so it uses aggregate initialization with brace-enclosed initializer lists.
        -- Elements are initialized in order, starting from index 0.
        -- If no initializer is provided, elements are default-initialized (often uninitialized for fundamental types).
        -- Prefer value initialization with empty braces {} to ensure elements are initialized.
        -- Too many initializers → compile-time error.
        -- Too few initializers → remaining elements are value-initialized.

        Best practice:
            -- Use brace initialization and prefer {} when no explicit initial values are given.

        std::array<int, 5> a{1, 2, 3, 4, 5}; // fully initialized
        std::array<int, 5> b{1, 2};         // b[2], b[3], b[4] are value-initialized
        std::array<int, 5> c{};             // all elements value-initialized (zeros)
        std::array<int, 5> a;   // default-initialized → elements are uninitialized
        std::array<int, 5> b{}; // value-initialized → all elements are zero-initialized (preferred)

    Const and constexpr std::array
        -- A std::array can be declared const; all its elements are treated as const.
        -- std::array fully supports constexpr, allowing compile-time initialization and better optimization.
        -- constexpr support is a key advantage of std::array over std::vector.

        const std::array<int, 5> a{2, 3, 5, 7, 11};      // read-only array
        constexpr std::array<int, 5> b{2, 3, 5, 7, 11};  // compile-time array
    
    Class template argument deduction (CTAD) for std::array
        -- Since C++17, CTAD lets the compiler deduce both the element type and array size of a std::array from its initializers.
        -- This syntax is preferred when available; pre-C++17 requires explicit template arguments.
        -- CTAD cannot partially deduce template arguments (neither just type nor just size).

        constexpr std::array a1{9, 7, 5, 3, 1};   // std::array<int, 5>
        constexpr std::array a2{9.7, 7.31};      // std::array<double, 2>

        // Not allowed (partial deduction):
        // std::array<int> a3{1, 2, 3};  // error
        // std::array<5>   a4{1, 2, 3};  // error
    
    Omitting just the array length using std::to_array
        -- TAD (function template argument deduction) supports partial template argument omission, unlike CTAD.
        -- Since C++20, std::to_array allows omitting the array length, or even both type and length.
        -- std::to_array creates a temporary array and copies it, so it is less efficient than direct std::array construction.
        -- Use std::to_array only when the element type can’t be easily deduced (e.g. short).

        constexpr auto a1 = std::to_array<int, 5>({9, 7, 5}); // type and size specified
        constexpr auto a2 = std::to_array<int>({9, 7, 5});    // type specified, size deduced
        constexpr auto a3 = std::to_array({9, 7, 5});         // type and size deduced

        constexpr auto s  = std::to_array<short>({9, 7, 5});  // useful for non-literal types

        Guideline:
        Prefer direct std::array (with CTAD) when possible; use std::to_array only when necessary.
    
    Accessing array elements using operator[]
        -- The most common way to access std::array elements is with the subscript operator (operator[]).
        -- Indexing is zero-based.
        -- operator[] performs no bounds checking; invalid indices cause undefined behavior.
    
2 — std::array length and indexing
    The length of a std::array has type std::size_t
        -- std::array is a class template with a non-type template parameter for its size:
                template<typename T, std::size_t N>
                struct array;

        -- The array length N has type std::size_t (an unsigned integral type).
        -- When defining a std::array, N must be a constexpr value of type std::size_t or convertible to it.
        -- Signed integral constants are allowed, as they are converted to std::size_t at compile time without narrowing.

        Aside (C++23):
            -- A std::size_t literal suffix (UZ) was added to help with type deduction. 
            -- Example: 0 deduces as int, while 0UZ deduces as std::size_t.

    The length and indices of std::array have type size_type, which is always std::size_t
        -- std::array defines a nested type size_type, which represents the container’s size and indices.
        -- For std::array, size_type is always an alias of std::size_t.
        -- The array length template parameter uses std::size_t directly because size_type is not yet defined at that point.
        -- Everywhere else in std::array, size_type is used instead of std::size_t.
    
    Getting the length of a std::array
        -- There are three common ways to get a std::array’s length:
            -- arr.size()
            -- Member function
            -- Returns size_type (alias for std::size_t, unsigned)
        -- std::size(arr) (C++17)
            -- Non-member function
            -- Equivalent to arr.size()
            -- Returns std::size_t
        -- std::ssize(arr) (C++20)
            -- Non-member function  
            -- Returns a signed integral type (usually std::ptrdiff_t)

        Example:    
            constexpr std::array arr{9, 7, 5, 3, 1};
            arr.size();        // unsigned size_type
            std::size(arr);    // unsigned size_type
            std::ssize(arr);   // signed integral type

        Note:
            std::ssize() is the only option that returns a signed length.

    Getting the length of a std::array as a constexpr value
        -- Because a std::array’s length is constexpr, size(), std::size(), and std::ssize() all return constexpr values, even when called
           on a non-constexpr object.
        -- These values can be used in constant expressions and implicitly converted to int without narrowing.
                std::array arr{9, 7, 5, 3, 1};
                constexpr int length{ std::size(arr) }; // OK
        -- Important caveat (pre-C++23):
            When a std::array is passed to a function by (const) reference, these functions do not yield a constexpr value due to a 
            language defect.
                void f(const std::array<int, 5>& arr) {
                    // constexpr int n{ std::size(arr) }; // error (pre-C++23)
                }
            This defect is fixed in C++23 (P2280), but compiler support may be limited.
            
            Workaround:
                -- Use a function template with the array length as a non-type template parameter:
                    template <auto N>
                        void printLength(const std::array<int, N>&) {
                    std::cout << N << '\n';
                }
        
    Subscripting std::array using operator[] or the at() member function
        -- operator[] is the most common indexing a std::array; it performs no bounds checking → invalid index causes undefined behavior.
        -- at() provides runtime bounds checking, but is generally discouraged in favor of explicit checks or compile-time bounds safety.
        -- Both operator[] and at() expect an index of type size_type (std::size_t).
        -- Using a constexpr index is safe: it’s converted to std::size_t without narrowing.
        -- Using a non-constexpr signed index may cause a narrowing-conversion warning when converted to std::size_t.
        (See also: the unsigned index issue discussed with std::vector.)

    std::get() does compile-time bounds checking for constexpr indices
        -- operator[] performs no bounds checking; at() performs runtime checking.
        -- To get compile-time bounds checking, use std::get<Index>(array) with a constexpr index.
        -- std::get takes the index as a non-type template parameter and enforces bounds via static_assert.
        -- Calling std::get with an out-of-bounds index causes a compile-time error.
        -- Only constexpr indices are allowed because template arguments must be constant expressions.

            constexpr std::array prime{2, 3, 5, 7, 11};
            std::cout << std::get<3>(prime); // OK → 7
            // std::cout << std::get<9>(prime); // Compile-time error: index out of bounds

3 — Passing and returning std::array
        -- Passing by value makes an expensive copy; prefer passing by const reference.
        -- The element type and length are part of the type, so function parameters must specify both (e.g., std::array<int,5>).
        -- CTAD does not apply to function parameters, so the compiler cannot deduce the template arguments there.
                void passByRef(const std::array<int, 5>& arr) {
                    std::cout << arr[0] << '\n';
                }

                int main() {
                    std::array arr{9, 7, 5, 3, 1}; // CTAD deduces std::array<int, 5>
                    passByRef(arr);
                }
        
    Using function templates to pass std::array of different element types or lengths
        -- To write a function that accepts any std::array type or length, use a function template parameterizing both element type T 
           and length N:
                    template <typename T, std::size_t N>
                    void passByRef(const std::array<T, N>& arr) {
                        static_assert(N != 0); // optional: fail on zero-length arrays
                        std::cout << arr[0] << '\n';
                    }

                    int main() {
                    std::array arr{9, 7, 5, 3, 1};        // std::array<int, 5>
                    std::array arr2{1, 2, 3, 4, 5, 6};    // std::array<int, 6>
                    std::array arr3{1.2, 3.4, 5.6, 7.8, 9.9}; // std::array<double, 5>

                    passByRef(arr);   // OK
                    passByRef(arr2);  // OK
                    passByRef(arr3);  // OK
                }
        -- T is the element type; N is the non-type template parameter for length (std::size_t required, not int).
        -- A single function template instantiates different functions for arrays of different types and lengths.
        -- Optional: Template only one parameter (e.g., length) if the element type is fixed:
    
                template <std::size_t N>
                void passByRef(const std::array<int, N>& arr) {
                    std::cout << arr[0] << '\n';
                }
        -- This works for any length of std::array<int, N>.
        -- It won’t work for arrays with other element types (e.g., double).

        -- In C++20, a non-type template parameter can use auto to deduce its type from the argument.
        -- This simplifies writing function templates for std::array, avoiding the need to explicitly specify std::size_t.
                template <typename T, auto N> // now using auto to deduce type of N
    
    Static asserting on array length
        -- Using operator[] with a fixed index (e.g., arr[3]) can lead to undefined behavior if the array is too short.
        -- Template parameters are compile-time constants, allowing compile-time checks.
        Solution 1: Use std::get<Index>()
            -- Performs compile-time bounds checking.
                template <typename T, std::size_t N>
                void printElement3(const std::array<T, N>& arr) {
                    std::cout << std::get<3>(arr) << '\n'; // compile-time check
                }
            -- Passing an array shorter than 4 elements causes a compile-time error.
        
        Solution 2: Use static_assert
            -- Validate array length explicitly:
                template <typename T, std::size_t N>
                void printElement3(const std::array<T, N>& arr) {
                    static_assert(N > 3, "Array must have at least 4 elements");
                    std::cout << arr[3] << '\n';
                }

            -- Fails to compile if the array is too short.

        Key point:
            -- static_assert(std::size(arr) > 3) won’t compile pre-C++23; use the template parameter N for compile-time checks.

    Returning std::array from functions
        -- Passing a std::array to a function is simple: pass by (const) reference.
        -- Returning a std::array by value makes a copy (or moves elements if they are move-capable). 
           Unlike std::vector, std::array itself is not moveable as a whole.

        Two conventional options for returning std::array:
            -- Return by value – simple, elements may be moved if possible.
            -- Return via output reference parameter – avoids copying/moving the array itself, can be more efficient in some cases.

        Choice depends on array size, element type, and performance requirements.

        Return by value
        -- Appropriate when:
            -- Array is small.
            -- Elements are cheap to copy or move.
            -- Not in a performance-sensitive context.
            
            Pros:
            -- Conventional and intuitive.
            -- Can initialize an array in a single statement.
            
            Cons:
            -- Copies all elements (may be expensive).
            -- Template arguments must be explicitly supplied.

            Example:
                template <typename T, std::size_t N>
                std::array<T, N> inputArray() {
                    std::array<T, N> arr{};
                    for (std::size_t i = 0; i < N; ++i) std::cin >> arr[i];
                    return arr;
                }

                std::array<int, 5> arr{inputArray<int, 5>()};
        
        -- Return via out-parameter
            -- Pass the array by non-const reference to modify it.

            Pros:
            -- No copying, more efficient for large arrays.

            Cons:
            -- Non-conventional; not obvious that function modifies the argument.
            -- Cannot initialize an array in a single statement.
            -- Cannot produce temporaries.

            Example:
                template <typename T, std::size_t N>
                void inputArray(std::array<T, N>& arr) {
                    for (std::size_t i = 0; i < N; ++i) std::cin >> arr[i];
                }

                std::array<int, 5> arr{};
                inputArray(arr);

        -- Consider std::vector instead
            -- std::vector is move-capable, so returning by value is cheap.
            -- For large arrays or non-constexpr cases, using std::vector may be preferable.    

        Key insight:
        Use return by value for small, cheap-to-copy arrays; use out-parameters for large arrays; consider std::vector 
        if you need move semantics or flexibility.

4 — std::array of class types, and brace elision
    Defining and assigning to a std::array of structs
        -- Works exactly like normal assignment.
            std::array<House, 3> houses{};
            houses[0] = {13, 1, 7};
            houses[1] = {14, 2, 5};
            houses[2] = {15, 2, 4};
        
        Reason: the compiler already knows each element is a House, so {…} is implicitly converted to House.
    
    Initializing with explicit element type (CTAD)
        -- You can initialize directly if you explicitly construct each element.
            constexpr std::array houses{
                House{13, 1, 7},
                House{14, 2, 5},
                House{15, 2, 4}
            }; // deduced as std::array<House, 3>
        
        -- No extra braces needed because each initializer is clearly a House.
    
    Why this does NOT work
        // ❌ does NOT compile
        constexpr std::array<House, 3> houses{
            {13, 1, 7},
            {14, 2, 5},
            {15, 2, 4}
        };

        Reason:
        std::array is an aggregate containing one C-style array member.
        The compiler treats {13,1,7} as initializing the entire internal array, not one element — extra initializers then cause an error.
    
    Correct way: double braces
        -- You must first initialize the internal C-style array, then its elements.
            constexpr std::array<House, 3> houses {{
                {13, 1, 7},
                {14, 2, 5},
                {15, 2, 4}
            }};
    
        -- Outer {} → initializes the std::array object
        -- Inner {} → initializes the internal C-style array
        -- Inner-inner {} → initializes each House
    
    Key insight (remember this)
        -- When initializing a std::array of structs / classes / arrays without explicitly naming the element type, double braces are 
           required.
        -- This is a consequence of aggregate initialization. Other containers (std::vector, std::list) do not have this issue.

    One-line rule to memorize
        -- Assignment: single braces ✅
        -- Initialization with House{}: single braces ✅
        -- Initialization without House{}: double braces required ✅

    Brace elision (why ints work with single braces)
            constexpr std::array<int, 5> a{1, 2, 3, 4, 5};   // OK
            constexpr std::array<int, 5> b{{1, 2, 3, 4, 5}}; // Also OK
        Why?
            -- Scalars (int, double, etc.) allow brace elision
            -- Structs / arrays do not, unless the type is explicitly named
    
        Key takeaway:
            -- If a std::array element needs multiple values (struct, array) and you don’t explicitly write the type, use double braces.

5 — Arrays of references via std::reference_wrapper
    Arrays can hold objects, not references
        -- Array elements must be objects and assignable.
        -- References are not objects and cannot be reseated, so:
                std::array<int&, 2> arr; // ❌ illegal: array of references
    What actually happens here
            int& r1 = x;
            int& r2 = y;
            std::array arr{ r1, r2 }; // ✅ this is std::array<int, 2>
        -- References decay to the referred value
        -- This creates an array of values, not references

    Workaround: std::reference_wrapper
    If you really need an array of references, use std::reference_wrapper<T>.
        #include <array>
        #include <functional>

        int x{1}, y{2}, z{3};

        std::array<std::reference_wrapper<int>, 3> arr{ x, y, z };

        Key properties of std::reference_wrapper
            -- Behaves like a modifiable reference
            -- Can be reseated via assignment
            -- Implicitly converts to T&
            -- get() returns the referenced object
                    arr[1].get() = 5;   // modifies y
                    std::cout << arr[1]; // prints 5 (implicit conversion to int&)
            -- Must use .get() to modify the value:
                    arr[1] = 5; // ❌ ambiguous / invalid
        
        std::ref and std::cref (recommended helpers)
            -- Shortcuts for creating reference wrappers:
                int x{5};
                auto r  = std::ref(x);   // std::reference_wrapper<int>
                auto cr = std::cref(x);  // std::reference_wrapper<const int>
        
            These are:
                -- Shorter
                -- Widely used
                -- Work well with auto
            Even with C++17 CTAD available, std::ref / std::cref remain idiomatic.

        Key takeaway
            -- ❌ Arrays cannot store references directly
            -- ✅ Use std::reference_wrapper to simulate arrays of references
            -- 🟢 Prefer std::ref / std::cref for clean syntax
            -- 📌 This applies to all array types, not just std::array

6 — std::array and enumerations
    -- See examples under relative path : OOPs\17_Fixed-size_arrays_std_array_n_C-style_arrays\17_3_std_array_n_enumerations

7 — Introduction to C-style arrays
    -- Because C-style arrays are the only array type built directly into the language, standard library containers like std::array and 
       std::vector are typically implemented internally using C-style arrays.
    
    Declaring a C-style array
        -- C-style arrays are built into the language and form the basis for std::array and std::vector implementations.
                int arr[30]{};   // 30 elements, value-initialized (zeros)
            -- Uses [] as part of declaration syntax
            -- Length must be:
                -- ≥ 1
                -- A constant expression
            -- Zero-length arrays are illegal in C++ (except some compiler extensions)
            -- C-style arrays dynamically allocated on the heap are allowed to have length 0.

    Indexing
            arr[1] = 7;
        -- Uses operator[]
        -- No bounds checking → out-of-bounds = undefined behavior
        -- Index type can be any integral type (signed, unsigned, or unscoped enum)
        -- No signed/unsigned mismatch issues like std::array
    
    Initialization (aggregate initialization)
            int a[5] { 1, 2, 3, 4, 5 };  // preferred
            int b[5] {};               // all elements zero-initialized
        -- Too many initializers → compile error
        -- Too few initializers → remaining elements value-initialized
        -- No initializer → elements are uninitialized (avoid)
    
    Omitting array length
            int primes[] { 2, 3, 5, 7, 11 }; // compiler deduces size = 5
        -- Allowed only when all elements are initialized
        -- Prevents size/initializer mismatch bugs
        -- Empty initializer {} → illegal (would deduce size 0)
                int badarray[]{};       // compilation error.

        Best practice:
            -- Omit the array length when fully initializing a C-style array.
    
    Type deduction limitations
        -- Element type must be explicit
        -- No CTAD (not a template)
        -- auto does not work
    
    Const and constexpr C-style arrays
        -- C-style arrays can be const or constexpr.
        -- const arrays must be initialized and their elements cannot be modified.
        -- constexpr arrays can be used in compile-time contexts.
                constexpr int squares[5]{ 1, 4, 9, 16, 25 };
                const int primes[5]{ 2, 3, 5, 7, 11 };

    sizeof with C-style arrays
        -- sizeof(array) returns the total size in bytes of the array.
        -- No extra overhead: size = number of elements × size of each element.    
                sizeof(primes); // e.g. 5 * 4 = 20 bytes

    Getting the length of a C-style array
        C++17+
            -- Use std::size() (unsigned) or std::ssize() (signed).
        C++14 and earlier
            -- Use a template that takes the array by reference:
                template <typename T, std::size_t N>
                constexpr std::size_t length(const T(&ref)[N]) { return N; }
        Old idiom (not recommended)
            -- sizeof(arr) / sizeof(arr[0]);
            -- ⚠️ Breaks when arrays decay to pointers.
        
    C-style arrays don’t support assignment
        -- You cannot assign a new initializer list to an existing array.
            int arr[]{1,2,3};
            arr = {4,5,6}; // ❌ compile error
        
        -- Allowed alternatives:
            -- Assign element-by-element
            -- Use std::copy
            -- Prefer std::vector if reassignment is needed
                std::copy(std::begin(src), std::end(src), std::begin(arr));

8 — C-style array decay
    The C-style array passing challenge
        -- Copying arrays is expensive. So, copying array into a function parameter would be slow and impractical.
        -- Functions should accept arrays of any length
            We don’t want separate functions for int[5], int[100], int[1000], etc
        -- The C solution: array-to-pointer decay : 
            When a C-style array is passed to a function, it automatically decays into a pointer to its first element.
                void printElementZero(int arr[1000])
                {
                    std::cout << arr[0];
                }
            -- This function does NOT receive an array. It actually becomes:
                void printElementZero(int* arr)
                {
                    std::cout << arr[0];
                }
            -- Why this works
                int x[7]{5};
                printElementZero(x);
                    -- x → decays to int*
                    -- No array copy is made
                    -- Works with arrays of any length
                ✅ Efficient
                ✅ Length-agnostic
            
            The hidden danger
            -- The function loses array size information
                void printElementZero(int arr[1000])
               
            Despite appearances:
                -- 1000 is ignored
                -- The compiler does not enforce size
                -- arr is just a pointer
            This compiles:
                int small[2]{};
                printElementZero(small); // no error
            ⚠️ Out-of-bounds access = undefined behavior
                void bad(int arr[])
                {
                    arr[999] = 42; // UB if array is smaller
                }
    
    Array to Pointer Conversions (Array Decay)
    Array decay: In most expressions, a C-style array converts to a pointer to its first element.
        -- int arr[5]; auto ptr{arr}; → ptr is int* pointing to &arr[0].
        -- const int arr[5]; → decays to const int*.
    Exceptions (no decay):
        -- Used with sizeof or typeid.
        -- Taking the address of the array (&arr).
        -- Passed as a class member.
        -- Passed by reference.
    Arrays vs pointers:
        -- Array type (e.g. int[5]) includes length info.
        -- Decayed pointer type (int*) loses length info.
        -- Hence, arrays are not pointers, though they often decay into them.
    Key insight:  
        -- Array decay strips away length information—pointers only know the address of the first element, not the array’s size.

        void printFirst(int arr[1000])  // it doesn't mean int arr[1000]; // inside the function
        {
            std::cout << arr[0];
        }

    Subscripting a C-style array actually applies operator[] to the decayed pointer
        -- Array decay: A C-style array evaluates to a pointer to its first element (int arr[] → int*).
        -- Subscripting: arr[i] is actually applied to the decayed pointer, not the raw array type.
                const int arr[]{9,7,5,3,1};
                std::cout << arr[2]; // same as *(arr+2), prints 5
        -- Pointer equivalence:
                const int* ptr{arr}; // arr decays to int*
                std::cout << ptr[2]; // identical result, prints 5
        Key insight: 
            -- operator[] works on pointers; if the pointer holds the address of the first element, subscripting behaves exactly 
               like array indexing.

    Array decay solves our C-style array passing issue
        -- C-style arrays are passed by address, even when it looks like they are passed by value.
        -- Two C-style arrays with the same element type but different lengths will decay into the same pointer type.
    
    C-style array function parameter syntax
        -- Declaring a parameter as int* arr works, but it’s unclear whether it points to a single value or an array.
        -- Using int arr[] is preferred, since it signals the parameter is meant to be a decayed array.
        -- The compiler treats int arr[] and int* arr identically; any length inside [] is ignored.
                void printElementZero(const int arr[]) { // clearer intent
                    std::cout << arr[0];
                }
        Best practice: 
        -- Use int arr[] for parameters expecting a C-style array, but remember it’s still a decayed pointer with no length information.
    
    The problems with array decay
        Loss of length info: When arrays decay to pointers, the size of the array is lost.
            -- sizeof(arr) on an array → total bytes.
            -- sizeof(arr) on a decayed pointer → size of pointer only.
            -- Old hack sizeof(arr)/sizeof(*arr) fails if arr has decayed.
        Safer alternatives:
            -- C++17: std::size(arr) → works only on actual arrays, not pointers.
            -- C++20: std::ssize(arr) → signed version.
        Refactoring issues: 
            -- Code that works with arrays may break or silently misbehave when arrays decay in function calls.
        Undefined behavior risk:
            -- Functions can’t verify array length.
            -- Passing shorter arrays or even single values (via pointer) compiles but crashes at runtime when indexed out of bounds.
        Traversal challenge: Without length info, it’s hard to know when iteration should stop.

        Best practice (modern C++)
        Prefer:
            -- std::array<T, N>& (fixed size, compile-time safe)
            -- std::span<T> (C++20, size-aware view)
            -- std::vector<T>& (dynamic size)
        Avoid raw C-style arrays in function interfaces unless absolutely necessary.
    
    Working around array length issues
        Because C-style arrays lose length info when they decay, programmers historically used two workarounds:
        1. Pass array + length as separate arguments
            void printElement2(const int arr[], int length) {
                assert(length > 2);
                std::cout << arr[2];
            }
            
            -- Caller must ensure length matches the array.
            -- Risk of sign conversion issues (std::size_t vs int).
            -- Only runtime validation (assert), not compile-time.
            -- Doesn’t work for implicit calls (e.g. operators).

        2. Use a sentinel/terminator value
            -- Mark the end of the array with a special invalid value (e.g. -1).
            -- Traverse until sentinel is found.
            -- Example: C-style strings use '\0' as a null terminator.
            int main() { 
                int scores[]{95, 87, 76, 100, -1
            }; // -1 marks end printUntilSentinel(scores); // prints: 95 87 76 100 }

            Issues:
                -- Missing sentinel → undefined behavior.
                -- Functions must handle sentinel specially.
                -- Actual length ≠ number of valid elements.
                -- Requires existence of a semantically invalid value.

        NOTE : Both methods are fragile. Sentinels are error-prone, and passing lengths separately risks mismatches.

    C-style arrays should be avoided in most cases
        Why avoid:
            -- Non-standard semantics (arrays decay → passed by address, not value).
            -- Loss of length info → unsafe, error-prone.
            -- Harder to refactor and maintain.

        Best practice:
            ✅ std::string_view → read-only strings.
            ✅ std::string → modifiable strings.
            ✅ std::array → fixed-size, constexpr arrays (non-global).
            ✅ std::vector → dynamic arrays.
            ⚠️ C-style arrays → only acceptable for global constexpr data.

        Aside:
            Arrays can be passed by reference to avoid decay, but this requires fixed length and templates → better to just 
            use std::array.
    
    Modern usage of C-style arrays:
        -- Global/static constexpr data → simple syntax, avoids decay issues, no sign-conversion quirks.
        -- Functions handling raw C-style strings → avoids unnecessary conversion to std::string_view 
           when performance or compatibility with APIs matters.

9 — Pointer arithmetic and subscripting
    What is pointer arithmetic?
        -- Pointer arithmetic means doing arithmetic on a pointer so that it points to a different element of the same array, not to 
           an arbitrary byte address.
            If you have:
                int arr[] { 9, 7, 5, 3, 1 };
                int* p = arr;   // points to arr[0]

            +------------+-----------------------------------+
            | Expression | Meaning                           |
            +------------+-----------------------------------+
            | p + 1      | address of arr[1]                 |
            | p + 2      | address of arr[2]                 |
            | p - 1      | address of the previous element   |
            | ++p        | move pointer to next element      |
            | --p        | move pointer to previous element  |
            +------------+-----------------------------------+
        Key rule
            -- Pointer arithmetic moves in units of the pointed-to type, not bytes.
        So if int is 4 bytes:
            p + 1  // moves 4 bytes
            p + 2  // moves 8 bytes
        
        Why does arr[i] work?
            -- Because array subscripting is just pointer arithmetic.
                    arr[i]   ≡   *(arr + i)
                    ptr[i]   ≡   *(ptr + i)
            -- Because arr decays to a pointer (int*) in expressions.
        
    Important insight: indices are relative
        -- Indices are not absolute positions in the array.
        
        const int arr[]{9,8,7,6,5};
        const int* p = &arr[3];  // points to element 6

            p[0]  // 6
            p[1]  // 5
            p[-1] // 7
    
        So:
            -- p[1] means “next element after what p points to”
            -- p[-1] means “previous element”
        This works because:
            p[n] == *(p + n)

    Traversing arrays with pointer arithmetic and Relation to range-based for loops
        -- A very common and safe pattern:

            for (auto e : arr)
                std::cout << e;

            const int* begin = arr;
            const int* end   = arr + std::size(arr);

            for (; begin != end; ++begin)
            {
                std::cout << *begin << ' ';
            }

        -- So range-based for over C-style arrays is literally pointer arithmetic.

        Why this is good:
            -- No array decay problems
            -- Length is explicit
            -- Works perfectly when refactoring into functions
            -- Matches how the standard library works
        
        Why pointer arithmetic is restricted
            -- Pointer arithmetic is only defined when:
            -- You stay within the same array
            -- Or go one-past-the-end
            ❌ Undefined behavior:
                int x;
                int* p = &x;
                p + 10;  // UB (not an array)

    Best practices (important)
        ✔ Use subscripting (arr[i]) when indexing from element 0
        ✔ Use pointer arithmetic for relative traversal
        ❌ Avoid negative indices unless you really know what you’re doing
        ❌ Avoid C-style arrays in modern C++ unless necessary
    
10 — C-style strings
    C-Style Strings
        -- C-style strings are char arrays ending with a null-terminator (\0).
                char str1[] = "hello";      // 6 chars including null terminator
                const char str2[] = "world";            
        -- Strings decay to char* or const char*.
        -- Modify individual characters with []:
                char str[] = "string";
                str[1] = 'p'; // "spring"
        -- Length can be obtained with:
                #include <cstring>
                std::strlen(str); // excludes null-terminator
        -- Input safely using std::cin.getline():
                char input[255];
                std::cin.getline(input, std::size(input));
        -- Avoid non-const C-style strings; prefer std::string or std::string_view.

    Best Practices
        -- Avoid raw C-style arrays unless:
        -- Storing constexpr global/local data.
        -- Interfacing with legacy C APIs (e.g., C-style strings).
        +---------------------------+-------------------------------+-------------------------------+
        | Function                  | Purpose                       | Notes / Recommendation        |
        +---------------------------+-------------------------------+-------------------------------+
        | strlen(s)                 | Returns the length of string  | Safe for decayed arrays;      |
        |                           | s (excludes null terminator)  | usable in modern code         |
        +---------------------------+-------------------------------+-------------------------------+
        | strcpy(dest, src)         | Copies C-string from src      | Can overflow; avoid           |
        |                           | to dest                       | in modern code                |
        +---------------------------+-------------------------------+-------------------------------+
        | strncpy(dest, src, n)     | Copies up to n characters     | Safer than strcpy,            |
        |                           | from src to dest              | still error-prone             |
        +---------------------------+-------------------------------+-------------------------------+
        | strcpy_s(dest, size, src) | Safer version                 | Checks buffer size;           |
        |                           | (Microsoft-specific)          | platform-specific             |
        +---------------------------+-------------------------------+-------------------------------+
        | strcat(dest, src)         | Appends src to the end        | Risk of overflow;             |
        |                           | of dest                       | prefer std::string            |
        +---------------------------+-------------------------------+-------------------------------+
        | strncat(dest, src, n)     | Appends up to n characters    | Safer than strcat,            |
        |                           | from src to dest              | still risky                   |
        +---------------------------+-------------------------------+-------------------------------+
        | strcmp(s1, s2)            | Compares two C-strings;       | Simple comparison             |
        |                           | returns 0 if equal            |                               |
        +---------------------------+-------------------------------+-------------------------------+
        | strncmp(s1, s2, n)        | Compares up to n characters   | Safer than strcmp             |
        |                           | of two C-strings              |                               |
        +---------------------------+-------------------------------+-------------------------------+

    Prefer:
        -- std::array for fixed-size arrays.
        -- std::vector for dynamic arrays.
        -- std::string / std::string_view for strings.

11 — C-style string symbolic constants
    Two ways to define:
        -- const char name[]{ "Alex" }; → creates a new const array copy of "Alex".
        -- const char* const color{ "Orange" }; → pointer to string literal in read-only memory.
    
    Memory differences:
        -- Case 1 makes a duplicate copy of the literal.
        -- Case 2 usually points directly to the literal; compiler may consolidate identical literals to save memory.
    
        auto s1{ "Alex" };  // const char*
        auto* s2{ "Alex" }; // const char*
        auto& s3{ "Alex" }; // const char(&)[5]
    
    Output behavior with std::cout:
        -- Non-char pointers → print address.
        -- char* / const char* → print as string (until null terminator).
        Example pitfall:
            char c{'Q'};
            std::cout << &c; // UB: prints garbage after 'Q'
        
        Fix: cast to const void* to print address safely.
    
    Best practice:
        -- Avoid C-style string symbolic constants.
        -- Prefer constexpr std::string_view → safer, consistent, efficient.
    
    Key insight:  
        -- C-style string constants are legacy constructs. Modern C++ favors std::string_view for symbolic constants, 
           avoiding duplication and undefined behavior pitfalls.
    
12 — Multidimensional C-style Arrays
    Motivation: Tic‑Tac‑Toe Example
        -- A Tic‑Tac‑Toe board is a 3×3 grid. While it’s possible to store each cell as a separate variable, arrays are a better fit 
           when working with multiple similar elements.
                int ttt[9]; // 0 = empty, 1 = player 1, 2 = player 2
        -- This is a one‑dimensional (1D) array, where each element is accessed using a single index (e.g., ttt[2]). 
           However, a 1D layout does not naturally represent a 2D board.
    
    One‑Dimensional Arrays (1D)
        -- Require one index to access elements
        -- Elements are stored sequentially in memory
        Example:  int arr[5];
    
    Two‑Dimensional Arrays (2D)
        -- A two‑dimensional array is an array of arrays.
                int a[3][5]; // 3 rows, 5 columns
            
    Conceptual Layout
        -- First index → row
        -- Second index → column
            col 0 col 1 col 2 col 3 col 4
            [0][0] [0][1] [0][2] [0][3] [0][4]
            [1][0] [1][1] [1][2] [1][3] [1][4]
            [2][0] [2][1] [2][2] [2][3] [2][4]
    
    Accessing Elements
            a[2][3] = 7; // row 2, column 3
    
    Multidimensional Arrays
        -- Arrays with more than one dimension are called multidimensional arrays.
            int threedee[4][4][4]; // 3D array
        -- 2D and 3D arrays are common
        -- Higher dimensions are supported but rare
    
    Memory Layout of 2D Arrays
        -- Memory is linear, so multidimensional arrays are stored sequentially.
    
    Row‑Major Order (C++)
        -- C++ stores arrays row by row, left to right, top to bottom.
            [0][0] [0][1] [0][2] [0][3] [0][4]
            [1][0] [1][1] [1][2] [1][3] [1][4]
            [2][0] [2][1] [2][2] [2][3] [2][4]
    
    Column‑Major Order (Other Languages)
        -- Some languages (e.g., Fortran) store arrays column by column.
    
    Initializing 2D Arrays
        Nested Braces (Recommended)
            int array[3][5] {
                { 1, 2, 3, 4, 5 },
                { 6, 7, 8, 9, 10 },
                { 11, 12, 13, 14, 15 }
            };
        
        Partial Initialization
            -- Missing elements are value‑initialized to 0.
            int array[3][5] {
                { 1, 2 },
                { 6, 7, 8 },
                { 11, 12, 13, 14 }
            };

        Omitting Leftmost Dimension
            -- Allowed when initializers are present:
            int array[][5] {
                { 1, 2, 3, 4, 5 },
                { 6, 7, 8, 9, 10 },
                { 11, 12, 13, 14, 15 }
            };
        
        Zero Initialization
            int array[3][5]{};
        
    Iterating Over Arrays
        1D Array
            for (std::size_t i = 0; i < std::size(arr); ++i)
                std::cout << arr[i];

            for (auto e : arr)
                std::cout << e;
        
        2D Array (Row‑Major Friendly)
            for (std::size_t row = 0; row < std::size(arr); ++row)
            {
                for (std::size_t col = 0; col < std::size(arr[0]); ++col)
                    std::cout << arr[row][col] << ' ';
            }

        Range‑Based Loop
            for (const auto& row : arr)
            {
                for (const auto& e : row)
                    std::cout << e << ' ';
            }
        
    Cartesian Coordinates vs Array Indices
    Cartesian System
        -- Coordinates written as {x, y}
        -- x → horizontal (columns)
        -- y → vertical (rows)

        Array Indexing
            -- Indexed as [row][col]
            -- Row corresponds to y
            -- Column corresponds to x
    
        Mapping
            Cartesian {x, y} → Array [y][x]

        Loop Pattern
            for (std::size_t y = 0; y < std::size(arr); ++y)
            {
                for (std::size_t x = 0; x < std::size(arr[0]); ++x)
                std::cout << arr[y][x] << ' ';
            }
        
        -- This ordering matches C++ row‑major memory layout and is cache‑friendly.

        Key Takeaways
            -- Use arrays when managing multiple related elements
            -- 2D arrays model grids naturally
            -- C++ uses row‑major order
            -- Prefer nested braces for initialization
            -- Iterate rows first, columns second for efficiency
            -- Cartesian {x, y} maps to array [y][x]

13 — Multidimensional std::array
    No Standard Multidimensional Array Container
        -- The C++ standard library does not provide a built-in multidimensional array container.
        -- std::array itself is always one-dimensional.
        -- So multidimensional arrays must be built by nesting std::array inside another std::array.
    
    Two-Dimensional std::array
    Canonical Definition
        std::array<std::array<int, 4>, 3> arr {{
            { 1, 2, 3, 4 },
            { 5, 6, 7, 8 },
            { 9, 10, 11, 12 }
        }};
    
    Important Observations
        -- Double braces are required due to brace elision rules
        -- Syntax is verbose and hard to read
        -- Template nesting reverses natural dimensions:    
        -- std::array<std::array<int, 4>, 3> corresponds to [3][4]
        -- Indexing works exactly like C-style arrays:
            arr[1][2]; // row 1, column 2
        
    Passing a 2D std::array to a Function
        template <typename T, std::size_t Row, std::size_t Col>
        void printArray(const std::array<std::array<T, Col>, Row>& arr)
        {
            for (const auto& r : arr)
            {
                for (const auto& e : r)
                    std::cout << e << ' ';
                std::cout << '\n';
            }
        }

        -- This works, but the type is unpleasantly verbose, especially for higher dimensions.
    
    Simplifying with Alias Templates
        Problem with Plain Type Aliases
            using Array2dint34 = std::array<std::array<int, 4>, 3>;
        
        -- Requires a unique alias for every type and dimension combination
        
        Solution: Alias Template
            template <typename T, std::size_t Row, std::size_t Col>
            using Array2d = std::array<std::array<T, Col>, Row>;

            Benefits
                -- Much cleaner syntax
                -- Matches C-style row-first convention
                -- Scales naturally

            Array2d<int, 3, 4> arr {{
                { 1, 2, 3, 4 },
                { 5, 6, 7, 8 },
                { 9, 10, 11, 12 }
            }};
        
        Higher-Dimensional Aliases
            template <typename T, std::size_t Row, std::size_t Col, std::size_t Depth>
            using Array3d = std::array<std::array<std::array<T, Depth>, Col>, Row>;
        
            -- While possible, readability degrades quickly.
        
    Getting Dimension Lengths Safely
        Naive Approach (Dangerous)
            arr.size(); // rows
            arr[0].size(); // cols (UB if rows == 0)
        ❌ Causes undefined behavior if any non-last dimension is zero.
    
        Safe Compile-Time Approach
            template <typename T, std::size_t Row, std::size_t Col>
            constexpr std::size_t rowLength(const Array2d<T, Row, Col>&)
            {
                return Row;
            }


            template <typename T, std::size_t Row, std::size_t Col>
            constexpr std::size_t colLength(const Array2d<T, Row, Col>&)
            {
                return Col;
            }

            ✔ Uses type information, not runtime data ✔ Zero-sized dimensions are safe
        
    Flattening Multidimensional Arrays
    Motivation
    Multidimensional arrays:
        -- Are verbose
        -- Are hard to size generically
        -- Require nested loops
    Flattening reduces storage to one dimension while preserving a multidimensional interface.

    Flat Storage + 2D View
    Flat Storage
        template <typename T, std::size_t Row, std::size_t Col>
        using ArrayFlat2d = std::array<T, Row * Col>;
    
    2D View Class
        -- Non-owning view
        -- Uses std::reference_wrapper to remain assignable
        -- Maps (row, col) → row * Col + col
            T& operator()(int row, int col)
            {
                return m_arr.get()[row * cols() + col];
            }
        
    Notes on Indexing
        -- Pre-C++23: operator[] supports only one index
        -- Use operator() for multidimensional indexing
        -- C++23 allows operator[](row, col)

    Output and Behavior
        -- The flattened view supports:
        -- 1D traversal
        -- 2D traversal
        -- Compile-time dimension queries
    This design avoids complexity while preserving flexibility.

    std::mdspan (C++23)
    What is std::mdspan?
    -- A non-owning, modifiable multidimensional view
    -- Works over contiguous storage (C-array, std::array, std::vector)
        std::mdspan view { arr.data(), 3, 4 };
    
    Key Points
        -- Supports any number of dimensions
        -- Uses data_handle() for 1D access
        -- Uses multidimensional operator[] in C++23:
            view[row, col];
        -- Dimensions are called extents
            view.extents().extent(0);
    
    Future Direction: std::mdarray (C++26)
        -- Owning multidimensional container
        -- Combines std::array + std::mdspan
        -- Eliminates most manual work shown above
    





















