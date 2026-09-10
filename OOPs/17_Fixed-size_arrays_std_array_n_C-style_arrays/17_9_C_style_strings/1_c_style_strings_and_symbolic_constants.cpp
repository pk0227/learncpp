/**
 * @file 1_c_style_strings_and_symbolic_constants.cpp
 * @brief Demonstrates C-style strings, null termination, std::cout behavior, and pitfalls
 * 
 * Key Concepts:
 * 1. C-style strings are char arrays ending with a null-terminator ('\0').
 * 2. std::strlen excludes the null terminator, whereas sizeof includes it.
 * 3. const char str[] creates an independent array copy; const char* points to a string literal in static memory.
 * 4. std::cout treats char* and const char* as C-strings, printing characters until it hits '\0'.
 * 5. Printing &c (where c is a single char) invokes undefined behavior because there is no null-terminator!
 *    Fix: cast to static_cast<const void*>(&c) to print the pointer address.
 * 6. In modern C++, prefer constexpr std::string_view over C-style strings.
 */

#include <iostream>
#include <cstring>
#include <string_view>

int main()
{
    // 1. C-style string declaration and sizing
    char word[]{ "hello" }; // 5 letters + 1 null terminator '\0' = 6 chars
    std::cout << "word:         " << word << '\n';
    std::cout << "std::strlen:  " << std::strlen(word) << " (excludes null terminator)\n";
    std::cout << "sizeof(word): " << sizeof(word) << " bytes (includes null terminator)\n\n";

    // 2. Symbolic constants: array copy vs pointer to string literal
    const char arrName[]{ "Alex" };         // Independent array of 5 chars
    const char* const ptrName{ "Orange" };  // Pointer to string literal in read-only memory
    std::cout << "arrName: " << arrName << ", ptrName: " << ptrName << "\n\n";

    // 3. std::cout printing behavior & single char address pitfall
    char singleChar{ 'Q' };
    std::cout << "singleChar value: " << singleChar << '\n';

    // DANGER: std::cout << &singleChar prints characters starting at &singleChar until
    // it encounters a random '\0' in memory (Undefined Behavior / garbage output)!
    // Safe way to print the address of a char: cast to const void*
    std::cout << "singleChar address (safe): " << static_cast<const void*>(&singleChar) << "\n\n";

    // 4. Modern C++ preference: std::string_view
    constexpr std::string_view modernStr{ "Prefer std::string_view in modern C++" };
    std::cout << modernStr << '\n';

    return 0;
}
