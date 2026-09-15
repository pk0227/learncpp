/**
 * @file 1_exception_in_constructor_destructor_trap.cpp
 * @brief Demonstrates the Constructor Exception Trap:
 *        - An object's destructor is NEVER invoked if its constructor throws.
 *        - Raw owning pointers leak memory permanently.
 *        - RAII sub-objects (like std::unique_ptr) are cleanly destroyed in reverse order.
 */

#include <iostream>
#include <memory>
#include <stdexcept>

// Global allocation tracker for demonstration
static int g_allocations = 0;

void* operator new[](std::size_t size) {
    ++g_allocations;
    return std::malloc(size);
}

void operator delete[](void* ptr) noexcept {
    if (ptr) {
        --g_allocations;
        std::free(ptr);
    }
}

// -----------------------------------------------------------------------------
// HAZARD: Class with raw owning pointers
// -----------------------------------------------------------------------------
class DangerousClass {
private:
    int* m_buffer1{nullptr};
    int* m_buffer2{nullptr};

public:
    DangerousClass(bool throwOnSecond) {
        std::cout << "DangerousClass: Allocating buffer 1...\n";
        m_buffer1 = new int[100]; // Allocation 1

        if (throwOnSecond) {
            std::cout << "DangerousClass: Simulating allocation failure on buffer 2!\n";
            throw std::runtime_error("Buffer 2 allocation failed!");
        }

        m_buffer2 = new int[200]; // Allocation 2
        std::cout << "DangerousClass: Fully constructed!\n";
    }

    ~DangerousClass() {
        std::cout << "DangerousClass: Destructor called (cleaning up buffers)!\n";
        delete[] m_buffer1;
        delete[] m_buffer2;
    }
};

// -----------------------------------------------------------------------------
// SOLUTION: Class using RAII sub-objects
// -----------------------------------------------------------------------------
class SafeClass {
private:
    std::unique_ptr<int[]> m_buffer1;
    std::unique_ptr<int[]> m_buffer2;

public:
    SafeClass(bool throwOnSecond) 
        : m_buffer1(std::make_unique<int[]>(100)) // Fully constructed sub-object
    {
        std::cout << "SafeClass: Buffer 1 initialized via std::unique_ptr.\n";
        if (throwOnSecond) {
            std::cout << "SafeClass: Simulating failure during constructor body!\n";
            throw std::runtime_error("SafeClass construction aborted!");
        }
        m_buffer2 = std::make_unique<int[]>(200);
        std::cout << "SafeClass: Fully constructed!\n";
    }

    ~SafeClass() {
        std::cout << "SafeClass: Destructor called.\n";
    }
};

int main() {
    std::cout << "=== Test 1: DangerousClass with Exception in Constructor ===\n";
    int initial_allocs = g_allocations;
    try {
        DangerousClass obj(true);
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }
    std::cout << "Active allocations after DangerousClass failure: " 
              << (g_allocations - initial_allocs) << " (MEMORY LEAKED! Destructor never ran!)\n\n";

    std::cout << "=== Test 2: SafeClass with Exception in Constructor ===\n";
    initial_allocs = g_allocations;
    try {
        SafeClass obj(true);
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }
    std::cout << "Active allocations after SafeClass failure: " 
              << (g_allocations - initial_allocs) << " (NO LEAK! RAII sub-object cleaned up automatically!)\n";

    return 0;
}
