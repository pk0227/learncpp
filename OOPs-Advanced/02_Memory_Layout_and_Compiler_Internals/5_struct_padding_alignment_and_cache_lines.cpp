/**
 * @file 5_struct_padding_alignment_and_cache_lines.cpp
 * @brief Demonstrates Struct Padding, Alignment, Cache Lines, and False Sharing:
 *        - Alignment requirements and compiler padding rules.
 *        - Reordering struct members to reduce memory footprint.
 *        - Cache line alignment (64 bytes) to eliminate false sharing.
 */

#include <iostream>
#include <cstdint>
#include <new>

// 1. Naive member ordering: Interleaving small and large members
struct NaivePadding {
    char c1;      // 1 byte  (+3 bytes padding to align next int)
    int i;        // 4 bytes
    char c2;      // 1 byte  (+7 bytes padding to align next double)
    double d;     // 8 bytes
}; // Total: 24 bytes (10 bytes data + 14 bytes padding!)

// 2. Optimized member ordering: Ordered from largest to smallest alignment
struct OptimizedPadding {
    double d;     // 8 bytes
    int i;        // 4 bytes
    char c1;      // 1 byte
    char c2;      // 1 byte  (+2 bytes padding to round struct size to multiple of 8)
}; // Total: 16 bytes (10 bytes data + 2 bytes padding!)

// 3. Cache-line alignment to prevent false sharing in multithreaded systems
// A typical CPU cache line is 64 bytes.
struct alignas(64) ThreadData {
    uint64_t counter{0}; // Occupies its own dedicated 64-byte cache line
};

int main() {
    std::cout << "=== Primitive Alignments ===\n";
    std::cout << "alignof(char):   " << alignof(char) << " byte\n";
    std::cout << "alignof(int):    " << alignof(int) << " bytes\n";
    std::cout << "alignof(double): " << alignof(double) << " bytes\n\n";

    std::cout << "=== Member Reordering Optimization ===\n";
    std::cout << "sizeof(NaivePadding):     " << sizeof(NaivePadding) 
              << " bytes (50% wasted on padding!)\n";
    std::cout << "sizeof(OptimizedPadding): " << sizeof(OptimizedPadding) 
              << " bytes (Reduced from 24 to 16 bytes! 33% memory reduction!)\n\n";

    std::cout << "=== Cache Line Alignment (False Sharing Prevention) ===\n";
    std::cout << "alignof(ThreadData): " << alignof(ThreadData) << " bytes\n";
    std::cout << "sizeof(ThreadData):  " << sizeof(ThreadData) << " bytes\n";

    ThreadData t1, t2;
    uintptr_t addr1 = reinterpret_cast<uintptr_t>(&t1);
    uintptr_t addr2 = reinterpret_cast<uintptr_t>(&t2);
    std::cout << "Address of t1: 0x" << std::hex << addr1 << std::dec << "\n";
    std::cout << "Address of t2: 0x" << std::hex << addr2 << std::dec << "\n";
    std::cout << "Distance between t1 and t2: " << (addr2 > addr1 ? addr2 - addr1 : addr1 - addr2) 
              << " bytes (Guaranteed >= 64 bytes to eliminate false sharing across CPU cores!)\n";

    return 0;
}
