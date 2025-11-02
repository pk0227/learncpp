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





