/**
 * @file 4_ebco_and_no_unique_address.cpp
 * @brief Demonstrates Empty Base Class Optimization (EBCO) and C++20 [[no_unique_address]]:
 *        - The 1-byte rule for empty types.
 *        - Padding overhead of empty member variables.
 *        - Eliminating overhead via EBCO inheritance.
 *        - Modern zero-overhead member layout via C++20 [[no_unique_address]].
 */

#include <iostream>

// An empty class (e.g., stateless custom deleter or allocator)
struct EmptyDeleter {
    void operator()(void* p) const noexcept {
        // Stateless custom cleanup logic
    }
};

// 1. Naive member composition (suffers from padding!)
struct NaiveContainer {
    int* ptr{nullptr};          // 8 bytes (on 64-bit)
    EmptyDeleter deleter;       // 1 byte + 7 bytes padding!
};

// 2. Pre-C++20 Solution: Empty Base Class Optimization (EBCO)
struct EBCOContainer : private EmptyDeleter {
    int* ptr{nullptr};          // 8 bytes (EmptyDeleter takes 0 bytes as base!)
};

// 3. Modern C++20 Solution: [[no_unique_address]] attribute
struct ModernContainer {
    int* ptr{nullptr};          // 8 bytes
    [[no_unique_address]] EmptyDeleter deleter; // 0 bytes allocated!
};

int main() {
    std::cout << "=== The 1-Byte Rule ===\n";
    std::cout << "sizeof(EmptyDeleter): " << sizeof(EmptyDeleter) 
              << " byte (Standard mandates >= 1 for pointer identity)\n\n";

    std::cout << "=== Memory Footprint Comparison ===\n";
    std::cout << "sizeof(NaiveContainer):  " << sizeof(NaiveContainer) 
              << " bytes (8 ptr + 1 deleter + 7 bytes padding = 50% wasted space!)\n";
    std::cout << "sizeof(EBCOContainer):   " << sizeof(EBCOContainer) 
              << " bytes (EBCO: Empty base sub-object takes 0 bytes!)\n";
    std::cout << "sizeof(ModernContainer): " << sizeof(ModernContainer) 
              << " bytes (C++20 [[no_unique_address]]: 0-overhead member without inheritance!)\n\n";

    ModernContainer mc;
    std::cout << "Address of mc.ptr:     " << static_cast<void*>(&mc.ptr) << "\n";
    std::cout << "Address of mc.deleter: " << static_cast<void*>(&mc.deleter) 
              << " (Overlaps with ptr address safely!)\n";

    return 0;
}
