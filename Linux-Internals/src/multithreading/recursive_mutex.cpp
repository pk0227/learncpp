#include <thread>
/*
 * RECURSIVE MUTEX - ALLOW SAME THREAD TO RELOCK
 * 
 * CONCEPT:
 * - std::recursive_mutex allows same thread to lock multiple times
 * - Must unlock same number of times as locked
 * - Prevents self-deadlock in recursive functions
 * 
 * USE CASES:
 * - Recursive algorithms needing lock
 * - Methods calling other methods that also lock
 * - Legacy code refactoring
 * 
 * REGULAR MUTEX PROBLEM:
 * - Thread locks mutex, calls function that tries to lock again → DEADLOCK!
 * - Recursive mutex solves this
 * 
 * PERFORMANCE:
 * - Slower than regular mutex (tracks ownership and count)
 * - Avoid if possible (often indicates design issue)
 * 
 * INTERVIEW: When would you use recursive_mutex? (Be ready to critique design)
 */

#include <mutex>

unsigned int document_count = 0;
unsigned int report_count = 0;
std::recursive_mutex counter_mutex;

void add_document() {
    counter_mutex.lock();
    document_count++;
    counter_mutex.unlock();
}

void add_report() {
    counter_mutex.lock();
    report_count++;
    add_document();
    counter_mutex.unlock();
}

void worker() {
    for (int i = 0; i < 10000; i++) {
        add_document();
        add_report();
    }
}

int main() {
    std::thread alice(worker);
    std::thread bob(worker);
    alice.join();
    bob.join();
    printf("We processed %u documents.\n", document_count);
    printf("We generated %u reports.\n", report_count);

    return 0;
}
