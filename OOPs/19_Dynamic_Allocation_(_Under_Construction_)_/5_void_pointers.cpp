/**
 * @file 5_void_pointers.cpp
 * @brief Demonstrates void pointers, static_cast conversions, and deletion safety
 * 
 * Key Concepts:
 * 1. A void pointer (void*) is a generic pointer that can point to objects of any data type.
 * 2. void* cannot be directly dereferenced; it must first be static_cast back to the appropriate type.
 * 3. Pointer arithmetic is illegal on void* because the size of the pointed-to object is unknown.
 * 4. Deleting a void* results in UNDEFINED BEHAVIOR because the destructor (if any) cannot be called.
 *    Always static_cast back to the original pointer type before deleting!
 * 5. Void references (void&) do not exist in C++.
 */

#include <iostream>

struct Sample
{
    int value{ 100 };
    ~Sample()
    {
        std::cout << "Sample destructor called successfully\n";
    }
};

int main()
{
    // 1. Basic void* usage and typecasting
    int nValue{ 42 };
    void* voidPtr{ &nValue };

    // Direct dereference is illegal:
    // std::cout << *voidPtr; // COMPILE ERROR!

    // Must static_cast back to original type:
    int* intPtr{ static_cast<int*>(voidPtr) };
    std::cout << "Value via casted int pointer: " << *intPtr << "\n\n";

    // 2. Safe deletion of heap memory pointed to by void*
    Sample* s{ new Sample{} };
    void* vPtr{ s };

    // DANGER:
    // delete vPtr; // UNDEFINED BEHAVIOR: compiler does not know the type, so destructor won't be called!

    // SAFE: Cast back to original type before deleting
    Sample* originalPtr{ static_cast<Sample*>(vPtr) };
    delete originalPtr;
    originalPtr = nullptr;

    return 0;
}
