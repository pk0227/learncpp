/*
 * SIGNAL BLOCKING AND MASKING - Advanced Signal Control
 * 
 * SIGNAL MASK CONCEPT:
 * - Each process has signal mask (set of blocked signals)
 * - Blocked signals are held pending until unblocked
 * - Critical sections can block signals temporarily
 * - Prevents race conditions in signal handlers
 * 
 * SIGNAL STATES:
 * - Generated: Signal created (kill, raise, hardware exception)
 * - Pending: Generated but not yet delivered
 * - Blocked: Delivery suspended by signal mask
 * - Delivered: Handler executes (or default action)
 * 
 * SIGNAL SET OPERATIONS:
 * - sigemptyset(): Initialize empty set
 * - sigfillset(): Initialize full set (all signals)
 * - sigaddset(): Add signal to set
 * - sigdelset(): Remove signal from set
 * - sigismember(): Check if signal in set
 * 
 * BLOCKING OPERATIONS:
 * - sigprocmask(): Change signal mask (process-wide in single-threaded)
 * - pthread_sigmask(): Change signal mask (thread-specific)
 * - SIG_BLOCK: Add signals to mask
 * - SIG_UNBLOCK: Remove signals from mask
 * - SIG_SETMASK: Replace entire mask
 * 
 * USE CASES:
 * - Protect critical sections from signal interruption
 * - Ensure atomic operations complete
 * - Coordinate signal handling between threads
 * - Implement signal-safe cleanup code
 * 
 * PENDING SIGNALS:
 * - sigpending(): Get set of pending signals
 * - Signals delivered when unblocked
 * - Multiple instances of same signal = one delivery (merged)
 * - Real-time signals queue (not merged)
 * 
 * INTERVIEW TOPICS:
 * - How to protect critical section from signals?
 * - Difference between blocking and ignoring?
 * - Can blocked signals be lost?
 * - sigprocmask vs pthread_sigmask?
 */

#include <iostream>
#include <csignal>
#include <unistd.h>
#include <cstring>

volatile sig_atomic_t signal_count = 0;
volatile sig_atomic_t critical_section_interrupted = 0;

void signal_handler(int signum) {
    signal_count++;
    const char msg[] = "Signal received!\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

void critical_section_handler(int signum) {
    critical_section_interrupted = 1;
    const char msg[] = "CRITICAL SECTION INTERRUPTED!\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

/*
 * Demonstrates signal blocking to protect critical section
 */
void demonstrate_signal_blocking() {
    std::cout << "\n=== SIGNAL BLOCKING DEMO ===\n";
    std::cout << "PID: " << getpid() << "\n";
    
    // Install signal handler
    signal(SIGINT, critical_section_handler);
    
    // Prepare signal set for SIGINT
    sigset_t block_set, old_set, pending_set;
    sigemptyset(&block_set);        // Start with empty set
    sigaddset(&block_set, SIGINT);  // Add SIGINT to set
    
    std::cout << "\nEntering critical section (SIGINT blocked)...\n";
    std::cout << "Try pressing Ctrl+C now - signal will be pending\n";
    
    // BLOCK SIGINT (add to signal mask)
    // SIG_BLOCK: Add signals in block_set to current mask
    // old_set: Saves previous mask for restoration
    if (sigprocmask(SIG_BLOCK, &block_set, &old_set) == -1) {
        perror("sigprocmask - block");
        return;
    }
    
    // CRITICAL SECTION - protected from SIGINT
    for (int i = 0; i < 5; i++) {
        std::cout << "Critical work " << i + 1 << "/5...\n";
        sleep(1);
        
        // Check if SIGINT is pending
        if (sigpending(&pending_set) == -1) {
            perror("sigpending");
            continue;
        }
        
        if (sigismember(&pending_set, SIGINT)) {
            std::cout << "  (SIGINT is pending - will deliver after unblock)\n";
        }
    }
    
    std::cout << "\nCritical section complete. Unblocking SIGINT...\n";
    
    // UNBLOCK SIGINT (restore old mask)
    // Pending SIGINT will be delivered NOW
    if (sigprocmask(SIG_SETMASK, &old_set, nullptr) == -1) {
        perror("sigprocmask - restore");
        return;
    }
    
    sleep(1);  // Give time for signal delivery
    
    if (critical_section_interrupted) {
        std::cout << "Critical section WAS interrupted after unblocking\n";
    } else {
        std::cout << "Critical section completed without interruption\n";
    }
}

/*
 * Demonstrates signal pending queue
 */
void demonstrate_pending_signals() {
    std::cout << "\n=== PENDING SIGNALS DEMO ===\n";
    
    sigset_t block_all, old_set, pending_set;
    sigfillset(&block_all);  // Create set with ALL signals
    
    std::cout << "Blocking ALL signals...\n";
    sigprocmask(SIG_SETMASK, &block_all, &old_set);
    
    std::cout << "Now send signals with: kill -SIGUSR1 " << getpid() << "\n";
    std::cout << "And: kill -SIGUSR2 " << getpid() << "\n";
    std::cout << "Waiting 5 seconds...\n";
    
    sleep(5);
    
    // Check which signals are pending
    if (sigpending(&pending_set) == 0) {
        std::cout << "\nPending signals:\n";
        
        // Check common signals
        int signals[] = {SIGINT, SIGTERM, SIGUSR1, SIGUSR2, SIGALRM};
        const char* names[] = {"SIGINT", "SIGTERM", "SIGUSR1", "SIGUSR2", "SIGALRM"};
        
        for (int i = 0; i < 5; i++) {
            if (sigismember(&pending_set, signals[i])) {
                std::cout << "  " << names[i] << " is pending\n";
            }
        }
    }
    
    std::cout << "\nRestoring signal mask (pending signals will be delivered)...\n";
    sigprocmask(SIG_SETMASK, &old_set, nullptr);
    
    sleep(1);
}

/*
 * Demonstrates temporary signal blocking pattern
 */
void atomic_operation_with_signal_blocking() {
    std::cout << "\n=== ATOMIC OPERATION PATTERN ===\n";
    
    sigset_t block_set, old_set;
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGINT);
    sigaddset(&block_set, SIGTERM);
    
    // PATTERN: Block signals before critical operation
    sigprocmask(SIG_BLOCK, &block_set, &old_set);
    
    // Perform atomic operation (won't be interrupted)
    std::cout << "Performing atomic operation...\n";
    // ... critical code ...
    
    // PATTERN: Restore previous mask
    sigprocmask(SIG_SETMASK, &old_set, nullptr);
    
    std::cout << "Operation complete\n";
}

int main() {
    std::cout << "Signal Blocking and Masking Demo\n";
    std::cout << "=================================\n";
    
    // Install handlers for user signals
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);
    
    // Demo 1: Protect critical section
    demonstrate_signal_blocking();
    
    // Reset flag
    critical_section_interrupted = 0;
    
    // Demo 2: Check pending signals
    demonstrate_pending_signals();
    
    // Demo 3: Atomic operation pattern
    atomic_operation_with_signal_blocking();
    
    std::cout << "\nTotal signals received: " << signal_count << "\n";
    std::cout << "\nProgram complete\n";
    
    return 0;
}

/*
 * KEY INTERVIEW POINTS:
 * 
 * BLOCKING vs IGNORING:
 * - Blocking: Signal held pending, delivered later
 * - Ignoring: Signal discarded, never delivered
 * - Use SIG_IGN to ignore: signal(SIGINT, SIG_IGN)
 * 
 * SIGNAL MERGING:
 * - Standard signals: Multiple instances → one delivery
 * - Real-time signals (SIGRTMIN-SIGRTMAX): Queued, all delivered
 * - Can lose signals if sent repeatedly while blocked!
 * 
 * RACE CONDITIONS:
 * - Signal can arrive between check and action
 * - Solution: Block signals, then check and act, then unblock
 * - Or use signalfd() for synchronous signal handling
 * 
 * THREAD SAFETY:
 * - Each thread has own signal mask
 * - Use pthread_sigmask() in multi-threaded programs
 * - sigprocmask() behavior undefined in multi-threaded
 * 
 * BEST PRACTICES:
 * - Keep handlers simple (set flag only)
 * - Use sigprocmask to protect critical sections
 * - Don't assume signals are delivered immediately
 * - Remember: same signal can be merged/lost
 * 
 * COMPILE: g++ -std=c++17 02_signal_blocking_masking.cpp -o signal_blocking
 */
