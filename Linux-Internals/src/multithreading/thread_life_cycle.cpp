/*
 * THREAD LIFECYCLE AND STATE MANAGEMENT
 * 
 * Comprehensive demonstration of thread states and transitions.
 * 
 * THREAD STATES:
 * 1. Not-a-thread: Default constructed, or moved-from
 * 2. Joinable: Associated with running/finished thread
 * 3. Detached: Running independently, non-joinable
 * 4. Joined: join() called, resources reclaimed
 * 
 * STATE TRANSITIONS:
 * - Construction → Joinable
 * - Joinable + join() → Joined (not-a-thread)
 * - Joinable + detach() → Detached
 * - Move → source becomes not-a-thread
 * 
 * joinable() METHOD:
 * - Returns true if thread is joinable (has active thread)
 * - False if: default constructed, moved-from, joined, or detached
 * - MUST check before calling join() or detach()
 * 
 * CRITICAL RULES:
 * - Must join() OR detach() before thread object destroyed
 * - Can only join() once
 * - Can only detach() once
 * - Destructor of joinable thread calls std::terminate()!
 * 
 * EXCEPTION SAFETY:
 * - If exception thrown before join(), thread not joined
 * - Destructor called during stack unwinding
 * - Joinable thread destructor → std::terminate()!
 * - Solution: RAII wrapper or join in destructor
 * 
 * MOVE SEMANTICS:
 * - std::thread is move-only (not copyable)
 * - Moving transfers ownership
 * - Source thread becomes not-a-thread
 * - Destination must not be joinable (std::terminate if it is!)
 * 
 * BEST PRACTICES:
 * - Use RAII wrapper (std::jthread in C++20)
 * - Check joinable() before operations
 * - Handle exceptions properly
 * - Document whether function joins or caller must join
 * 
 * C++20 IMPROVEMENT:
 * - std::jthread: Auto-joins in destructor
 * - Solves exception-safety problem
 * - Supports stop_token for cancellation
 * 
 * INTERVIEW TOPICS:
 * - Thread state machine
 * - Why destructor terminates for joinable threads
 * - Move semantics for threads
 * - Exception safety with threads
 * - std::jthread vs std::thread
 */

#include <thread>
#include <chrono>
#include <iostream>
#include <atomic>

std::atomic<bool> working(true);

void task_worker(const char* name) {
    unsigned int task_count = 0;
    while (working) {
        task_count++;
    }
    std::cout << name << " completed " << task_count << " tasks.\n";
}

int main() {
    std::thread worker1(task_worker, "Alice");
    std::thread worker2(task_worker, "Bob");
    
    std::cout << "Alice and Bob are performing tasks...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    working = false;
    worker1.join();
    worker2.join();
    
    return 0;
}
