/**
 * @file 5_custom_operator_new_delete_memory_pool.cpp
 * @brief Demonstrates Class-Specific Memory Allocation Operators and Placement New:
 *        - Overloading class-specific operator new and operator delete for fixed-size pools.
 *        - C++14 sized deallocation (operator delete(void*, size_t)).
 *        - Placement new mechanics and mandatory manual destructor invocation.
 */

#include <iostream>
#include <new>
#include <vector>

class PooledParticle {
public:
    float x{0.0f}, y{0.0f}, z{0.0f};

    PooledParticle(float px, float py, float pz) : x(px), y(py), z(pz) {
        std::cout << "  PooledParticle constructed at: " << this << "\n";
    }

    ~PooledParticle() {
        std::cout << "  PooledParticle destructed at:  " << this << "\n";
    }

    // Static memory arena for fast allocation
    alignas(alignof(float)) static inline char s_pool[sizeof(float) * 3 * 10];
    static inline bool s_occupied[10]{false};

    // 1. Class-Level Overloaded operator new
    static void* operator new(std::size_t size) {
        std::cout << "  [Custom operator new] Allocating " << size << " bytes from memory pool...\n";
        for (int i = 0; i < 10; ++i) {
            if (!s_occupied[i]) {
                s_occupied[i] = true;
                return &s_pool[i * sizeof(PooledParticle)];
            }
        }
        throw std::bad_alloc(); // Pool exhausted!
    }

    // 2. Class-Level Sized Deallocation (C++14)
    static void operator delete(void* ptr, std::size_t size) noexcept {
        std::cout << "  [Custom operator delete] Freeing " << size << " bytes back to memory pool...\n";
        char* byte_ptr = static_cast<char*>(ptr);
        std::size_t offset = byte_ptr - s_pool;
        std::size_t index = offset / sizeof(PooledParticle);
        if (index < 10) {
            s_occupied[index] = false;
        }
    }
};

int main() {
    std::cout << "=== 1. Class-Specific Pool Allocation ===\n";
    PooledParticle* p1 = new PooledParticle(1.0f, 2.0f, 3.0f);
    PooledParticle* p2 = new PooledParticle(4.0f, 5.0f, 6.0f);

    delete p1;
    delete p2;

    std::cout << "\n=== 2. Placement New and Manual Destruction ===\n";
    alignas(PooledParticle) char stackBuffer[sizeof(PooledParticle)];
    std::cout << "Stack buffer address: " << static_cast<void*>(stackBuffer) << "\n";

    // Placement new: constructs object in pre-allocated buffer using global ::new!
    PooledParticle* p_stack = ::new (stackBuffer) PooledParticle(7.0f, 8.0f, 9.0f);

    // CRITICAL: Memory was NOT allocated via heap new, so calling 'delete p_stack' is UNDEFINED BEHAVIOR!
    // Must invoke destructor MANUALLY:
    p_stack->~PooledParticle();

    return 0;
}
