/**
 * @file 3_make_shared_vs_new_weak_ptr_retention.cpp
 * @brief Demonstrates the std::make_shared vs new Memory Retention Tradeoff:
 *        - Single allocation vs two separate heap allocations.
 *        - The weak_ptr memory retention trap: heap block cannot be freed while weak_ptr lives.
 *        - Separate allocation allowing large object memory to be deallocated early.
 */

#include <iostream>
#include <memory>

static int g_activeHeapChunks = 0;

void* operator new(std::size_t size) {
    ++g_activeHeapChunks;
    std::cout << "    [Allocator] malloc(" << size << " bytes) -> Total active chunks: " 
              << g_activeHeapChunks << "\n";
    return std::malloc(size);
}

void operator delete(void* ptr) noexcept {
    if (ptr) {
        --g_activeHeapChunks;
        std::cout << "    [Allocator] free() -> Total active chunks remaining: " 
                  << g_activeHeapChunks << "\n";
        std::free(ptr);
    }
}

struct BigObject {
    char data[1024]; // Simulates a large buffer

    BigObject() { std::cout << "  BigObject constructor called.\n"; }
    ~BigObject() { std::cout << "  BigObject destructor called!\n"; }
};

int main() {
    std::cout << "=== Case 1: std::make_shared and Weak Pointer Retention ===\n";
    std::weak_ptr<BigObject> wp_make_shared;
    {
        std::cout << "Calling std::make_shared<BigObject>():\n";
        auto sp = std::make_shared<BigObject>(); // Exactly ONE combined allocation!
        wp_make_shared = sp;
        std::cout << "Destroying sp (strong count drops to 0)...\n";
    }
    std::cout << "At this point: BigObject destructor HAS run,\n"
              << "BUT notice active heap chunks: " << g_activeHeapChunks 
              << " (Memory CANNOT be freed because weak_ptr still observes the control block!)\n\n";

    std::cout << "Now resetting wp_make_shared:\n";
    wp_make_shared.reset(); // Underlying heap memory is freed HERE!
    std::cout << "Active heap chunks after weak_ptr reset: " << g_activeHeapChunks << "\n\n";

    std::cout << "=== Case 2: std::shared_ptr<T>(new T) Early Deallocation ===\n";
    std::weak_ptr<BigObject> wp_separate;
    {
        std::cout << "Calling std::shared_ptr<BigObject>(new BigObject):\n";
        // TWO separate allocations: 1 for BigObject, 1 for Control Block!
        std::shared_ptr<BigObject> sp(new BigObject);
        wp_separate = sp;
        std::cout << "Destroying sp (strong count drops to 0)...\n";
    }
    std::cout << "Notice: The large BigObject memory chunk WAS FREED IMMEDIATELY!\n"
              << "Only the tiny control block chunk remains: " << g_activeHeapChunks << " chunk active.\n\n";

    std::cout << "Resetting wp_separate:\n";
    wp_separate.reset(); // Tiny control block chunk freed HERE!
    std::cout << "Active heap chunks remaining: " << g_activeHeapChunks << "\n";

    return 0;
}
