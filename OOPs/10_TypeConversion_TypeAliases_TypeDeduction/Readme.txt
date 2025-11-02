The process of converting data from one type to another type is called “type conversion”.
Implicit type conversion is performed automatically by the compiler when one data type is required, but a different data type is supplied.
Conversions do not change the data being converted. Instead, the conversion process uses that data as input, and produces the converted result.
When converting a value to another type of value, the conversion process produces a temporary object of the target type that holds the result of the conversion.

Type Conversions
    -- type conversion of certain narrower numeric types (such as a char) to certain wider numeric types (typically int or double) that can be processed efficiently.
    -- Two types.
        -- Numeric Promotion / safe conversion / always value-preserving conversion
        -- Numeric Conversion / unsafe conversion / Data can be lost

1.Numeric promotion categories
    -- integral promotions
    -- floating point promotions

    Integral promotions
        -- bool, char, signed char, unsigned char, signed short, and unsigned short all get promoted to int.
           if int cant hold entire range of the type, those get promoted to unsigned int.

NOTES : 
    1.  while integral promotion is value-preserving, it does not necessarily preserve the signedness (signed/unsigned) of the type.
    2.  Some widening type conversions (such as char to short, or int to long) are not considered to be numeric promotions.
        they are numeric conversions.


2.Numeric Conversions categories
    -- Value-preserving (safe)
    -- Reinterpretive   (unsafe but no data loss)
    -- Lossy            (unsafe and data may be lost)

3. Narrowing conversions
    -- potentially unsafe numeric conversion.
    -- may not be able to hold all the values of the source type.

    The following conversions are defined to be narrowing:
+-------------------------------+-------------------------------+---------------------------+------------------------------+---------
| Conversion Type                   | Risk Description              | Allowed with constexpr?       | Example                       |
+-------------------------------+-------------------------------+---------------------------+------------------------------+---------
| Int   <- Float                    | Fractional part lost          | ❌                            | int x = 3.14;                 |
| Float <- Double                   | Precision loss                | ✅ if value fits              | float f{3.0}; // OK           |
| Float <- Int                      | Large int may round           | ✅ if exactly representable   | float f{42}; // OK            |
| Smaller Int / Sign Change <- Int  | Overflow or reinterpretation  | ✅ if value fits exactly      | unsigned char c{100}; // OK   |
+-------------------------------+-------------------------------+---------------------------+------------------------------+---------

Best practice : Because they can be unsafe and are a source of errors, 
                avoid narrowing conversions whenever possible.

                If you need to perform a narrowing conversion, 
                use static_cast to convert it into an explicit conversion.

                Brace initialization{} disallows narrowing conversions
                When list-initializing ({}), narrowing conversions are checked only for compile-time constants.
                -- If the source value is constexpr, and the destination cannot represent it exactly, → error.
                -- If the source value is NOT constexpr (like a runtime variable), → no compile-time check — the compiler lets it through, possibly with a warning.
                
Use the typeid operator (included in the <typeinfo> header), to show the resulting type of an expression.

C++ supports 5 different types of casts: static_cast, dynamic_cast, const_cast, reinterpret_cast, and C-style casts. 
The first four are sometimes referred to as named casts.

|-----------------------------------------------------------------------------------------------------------------------------------|
| Cast             | Description                                                                            | Safe?                 |
|------------------|----------------------------------------------------------------------------------------|-----------------------|
| static_cast      | Performs compile-time type conversions between related types.                          | Yes                   |
| dynamic_cast     | Performs runtime type conversions on pointers or references in a polymorphic hierarchy.| Yes                   |
| const_cast       | Adds or removes const.                                                                 | Only for adding const |
| reinterpret_cast | Reinterprets the bit-level representation of one type as if it were another type.      | No                    |
| C-style casts    | Performs some combination of static_cast, const_cast, or reinterpret_cast.             | No                    |
|-----------------------------------------------------------------------------------------------------------------------------------|

=> const_cast and reinterpret_cast should generally be avoided because they are only useful in rare cases and can be harmful if used incorrectly.

C-style cast / C-style function-style cast:
    --  std::cout << (double)x / y << '\n'; // C-style cast of x to double
    --  std::cout << double(x) / y << '\n'; // function-style cast of x to double
    
    Avoid using C-style casts.
    --  it can actually perform a variety of different conversions depending on how it is used. 
        This can include a static cast, a const cast, or a reinterpret cast (the latter two of which we mentioned above you should avoid). 
        A C-style cast does not make it clear which cast(s) will actual be performed, which not only makes your code that much harder to understand, 
        but also opens the door for inadvertent misuse (where you think you’re implementing a simple cast and you end up doing something dangerous instead). 
        Often this will end up producing an error that isn’t discovered until runtime.

    One thing you can do with a C-style cast that you can’t do with C++ casts: 
    --  C-style casts can convert a derived object to a base class that is inaccessible (e.g. because it was privately inherited).

static_cast:
int x { 10 };
std::cout << static_cast<double>(x) / y << '\n';

--  static_cast<double>(x) returns a temporary double object containing the converted value 10.0. 
--  First, static_cast provides compile-time type checking. If we try to convert a value to a type and the compiler doesn’t know how to perform 
    that conversion, we will get a compilation error.
    int x { static_cast<int>("Hello") }; // invalid: will produce compilation error
--  Second, static_cast is (intentionally) less powerful than a C-style cast, as it will prevent certain kinds of dangerous conversions 
    (such as those that require reinterpretation or discarding const).
--  Since static_cast uses direct initialization, any explicit constructors of the target class type 
    will be considered when initializing the temporary object to be returned.

Casting vs initializing a temporary object:
--  static_cast<int>(x), which returns a temporary int object direct-initialized with x.
--  int { x }, which creates a temporary int object direct-list-initialized with x.

    Three notable differences between the static_cast and the direct-list-initialized temporary:
    --  int { x } uses list initialization, which disallows narrowing conversions. 
        This is great when initializing a variable, because we rarely intend to lose data in such cases. 
        But when using a cast, it is presumed we know what we’re doing, and if we want to do a cast that might lose some data, we should be able to do that. 
        The narrowing conversion restriction can be an impediment in this case.

        On a 32-bit architecture, this will work fine (because a double can represent all the values that can be stored 
        in a 32-bit int, so it isn’t a narrowing conversion). But on a 64-bit architecture, this is not the case, 
        so converting a 64-bit int to a double is a narrowing conversion. And since list initialization disallows narrowing conversions, 
        this won’t compile on architectures where int is 64-bits.

    --  static_cast makes it clearer that we are intending to perform a conversion. 
        Although the static_cast is more verbose than the direct-list-initialized alternative, 
        in this case, that’s a good thing, as it makes the conversion easier to spot and search for. 
        That ultimately makes your code safer and easier to understand.

    --  Direct-list-initializion of a temporary only allows single-word type names. 
        Due to a weird syntax quirk, there are several places within C++ where only single-word type names are allowed 
        (the C++ standard calls these names “simple type specifiers”). So while int { x } is a valid conversion syntax, 
        unsigned int { x } is not.

Prefer static_cast over initializing a temporary object when a conversion is desired.

