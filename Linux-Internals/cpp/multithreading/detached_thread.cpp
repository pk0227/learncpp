/*
 * DETACHED THREADS - FIRE AND FORGET
 * 
 * Demonstrates detached threads that run independently of parent thread.
 * 
 * THREAD LIFECYCLE:
 * - Joinable (default): Parent must call join() or detach()
 * - Detached: Runs independently, auto-cleaned up on completion
 * 
 * DETACH CHARACTERISTICS:
 * - thread.detach(): Separates thread from thread object
 * - Parent can exit before detached thread finishes
 * - Thread resources auto-reclaimed when thread exits
 * - Cannot join() detached thread
 * - Cannot get return value
 * 
 * WHEN TO DETACH:
 * - Background daemon tasks
 * - Fire-and-forget operations
 * - Don't need return value or status
 * - Thread lifetime independent of parent
 * 
 * WHEN NOT TO DETACH:
 * - Need return value
 * - Must ensure completion before exit
 * - Need exception handling from thread
 * - Coordinated shutdown required
 * 
 * DANGERS:
 * - Accessing destroyed objects (use-after-free)
 * - Program exits before detached thread finishes
 * - No way to wait for completion
 * - Hard to debug (no handle to thread)
 * 
 * SAFETY:
 * - Don't reference parent's stack variables
 * - Use shared_ptr for shared ownership
 * - Detached threads should be self-contained
 * - Consider thread pool instead
 * 
 * DETACH vs JOIN:
 * - join(): Wait for thread, get status, exception-safe
 * - detach(): No waiting, no status, requires careful lifetime management
 * 
 * DESTRUCTOR BEHAVIOR:
 * - Joinable thread destructor: std::terminate() if not joined!
 * - Detached thread destructor: OK to destroy thread object
 * 
 * INTERVIEW POINTS:
 * - joinable() vs detached threads
 * - When to use each
 * - Lifetime management issues
 * - Why std::async might be better for simple cases
 */

#include <thread>
#include <chrono>

void log_monitor() {
    while (true) {
        printf("Alice is monitoring the logs.\n");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    std::thread alice(log_monitor);
    alice.detach();
    for (int i = 0; i < 3; i++) {
        printf("Bob is processing data...\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }
    printf("Bob is done!\n");

    return 0;
}
