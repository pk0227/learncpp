/*
 * BASIC FUTURE EXAMPLE - ASYNC TASK WITH RESULT
 * 
 * Minimal example showing the fundamentals of std::future.
 * 
 * CORE CONCEPT:
 * - std::future<T>: Placeholder for a value that will be available later
 * - Like a "ticket" you can redeem for the result once it's ready
 * 
 * WORKFLOW:
 * 1. std::async launches function asynchronously (may run on different thread)
 * 2. Returns std::future<int> immediately (function still running)
 * 3. Main thread can do other work while waiting
 * 4. future.get() blocks until result is ready, then returns value
 * 
 * KEY CHARACTERISTICS:
 * - get() can only be called once (value is moved out)
 * - get() propagates exceptions from async function
 * - If future is destroyed before get(), blocks in destructor (!)  
 * 
 * ASYNC LAUNCH POLICIES:
 * - std::launch::async: Guaranteed async execution (new thread/task)
 * - std::launch::deferred: Lazy execution (runs when get() is called)
 * - Default: Implementation chooses (often deferred)
 * 
 * INTERVIEW BASICS:
 * - Difference between future.get() and future.wait()
 * - What happens if you don't call get()?
 * - Can you share futures between threads? (No, use shared_future)
 */

#include <future>
#include <cstdio>
#include <thread>
#include <chrono>

int process_task() {
    printf("Worker is processing tasks...\n");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 42;
}

int main() {
    printf("Manager asks Worker how many tasks have been processed.\n");
    std::future<int> result = std::async(std::launch::async, process_task);
    printf("Manager can do other things while waiting for the result...\n");
    printf("Worker responded with %d tasks completed.\n", result.get());
    return 0;
}
