/**
 * @file 1_unique_ptr_custom_deleters_and_sizeof.cpp
 * @brief Demonstrates std::unique_ptr Internals, Deleters, and Size Overhead:
 *        - Stateless deleter vs EBCO / [[no_unique_address]] (8 bytes).
 *        - Function pointer deleter memory bloat (16 bytes).
 *        - Array specialization mechanics with delete[].
 */

#include <iostream>
#include <memory>
#include <cstdlib>

// 1. Stateless Functor Deleter (Uses EBCO)
struct StatelessDeleter {
    void operator()(int* p) const noexcept {
        std::cout << "  StatelessDeleter: free called.\n";
        std::free(p);
    }
};

// 2. Stateful Deleter (Stores state, cannot be 0-sized)
struct StatefulDeleter {
    int m_logId{42};
    void operator()(int* p) const noexcept {
        std::cout << "  StatefulDeleter [ID=" << m_logId << "]: free called.\n";
        std::free(p);
    }
};

// 3. Free function deleter
void freeFunctionDeleter(int* p) noexcept {
    std::cout << "  freeFunctionDeleter: free called.\n";
    std::free(p);
}

int main() {
    std::cout << "=== std::unique_ptr Size Comparison ===\n";

    // Standard unique_ptr with default_delete
    std::unique_ptr<int> up_default(new int(10));
    std::cout << "sizeof(up_default) [default_delete]: " 
              << sizeof(up_default) << " bytes (Matches raw pointer!)\n";

    // unique_ptr with stateless functor deleter (EBCO)
    std::unique_ptr<int, StatelessDeleter> up_stateless(
        static_cast<int*>(std::malloc(sizeof(int))), StatelessDeleter{}
    );
    std::cout << "sizeof(up_stateless) [StatelessDeleter]: " 
              << sizeof(up_stateless) << " bytes (Zero overhead via EBCO!)\n";

    // unique_ptr with stateful deleter
    std::unique_ptr<int, StatefulDeleter> up_stateful(
        static_cast<int*>(std::malloc(sizeof(int))), StatefulDeleter{101}
    );
    std::cout << "sizeof(up_stateful) [StatefulDeleter]: " 
              << sizeof(up_stateful) << " bytes (Expanded to hold member state!)\n";

    // unique_ptr with function pointer deleter (THE TRAP!)
    std::unique_ptr<int, void(*)(int*)> up_func_ptr(
        static_cast<int*>(std::malloc(sizeof(int))), freeFunctionDeleter
    );
    std::cout << "sizeof(up_func_ptr) [Function Pointer]: " 
              << sizeof(up_func_ptr) << " bytes (16 bytes! Storing raw function pointer!)\n\n";

    std::cout << "=== Array Specialization ===\n";
    std::unique_ptr<int[]> up_array = std::make_unique<int[]>(5);
    up_array[0] = 100;
    up_array[1] = 200;
    std::cout << "up_array[1] = " << up_array[1] << " (Uses delete[] on cleanup)\n";

    return 0;
}
