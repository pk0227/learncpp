/*
 * BASIC SIGNAL HANDLING IN LINUX
 * 
 * SIGNALS OVERVIEW:
 * - Software interrupts sent to processes
 * - Asynchronous events (can arrive at any time)
 * - Used for: notifications, errors, user input (Ctrl+C), timers
 * - Each signal has default action (terminate, ignore, core dump, stop)
 * 
 * COMMON SIGNALS:
 * - SIGINT (2): Interrupt from keyboard (Ctrl+C)
 * - SIGTERM (15): Termination signal (graceful shutdown)
 * - SIGKILL (9): Force kill (cannot be caught or ignored!)
 * - SIGSEGV (11): Segmentation fault (invalid memory access)
 * - SIGALRM (14): Alarm clock (timer expiration)
 * - SIGCHLD (17): Child process terminated
 * - SIGUSR1/SIGUSR2: User-defined signals
 * 
 * SIGNAL HANDLING:
 * - signal(): Old, unreliable API (avoid in new code)
 * - sigaction(): Modern, portable, reliable API
 * - Handler runs asynchronously (interrupt current execution)
 * - Only async-signal-safe functions allowed in handler!
 * 
 * ASYNC-SIGNAL-SAFE FUNCTIONS:
 * - write() is safe, printf() is NOT safe
 * - Most library functions are NOT safe
 * - Global variables need 'volatile sig_atomic_t' type
 * - Keep handlers simple: set flag, write to pipe, return
 * 
 * SIGNAL DELIVERY:
 * - Signal pending: Sent but not yet delivered
 * - Signal blocked: Delivery temporarily suspended
 * - Signal mask: Set of blocked signals
 * - Signals can be lost if same signal sent multiple times
 * 
 * DEFAULT ACTIONS:
 * - Term: Terminate process
 * - Ign: Ignore signal
 * - Core: Terminate and dump core
 * - Stop: Stop (suspend) process
 * - Cont: Continue if stopped
 * 
 * INTERVIEW TOPICS:
 * - What happens when you press Ctrl+C?
 * - Difference between SIGTERM and SIGKILL?
 * - Why can't you catch SIGKILL or SIGSTOP?
 * - What is async-signal-safe?
 * - Race conditions in signal handlers
 * 
 * COMPILE: g++ -std=c++17 01_basic_signals.cpp -o basic_signals
 * RUN: ./basic_signals
 * TEST: Press Ctrl+C (sends SIGINT) or use: kill -SIGTERM <pid>
 */

#include <iostream>
#include <csignal>
#include <unistd.h>
#include <cstring>

// Global flag - must be volatile sig_atomic_t for signal safety
// volatile: Prevents compiler optimizations (variable can change unexpectedly)
// sig_atomic_t: Guaranteed atomic read/write (no race conditions)
volatile sig_atomic_t signal_received = 0;

/*
 * SIGNAL HANDLER FUNCTION
 * 
 * CONSTRAINTS:
 * - Must be async-signal-safe (limited set of functions allowed)
 * - Cannot use printf, malloc, or most library functions
 * - Should be as short as possible
 * - Use write() instead of cout/printf
 * 
 * PARAMETERS:
 * - signum: Signal number (e.g., SIGINT = 2)
 */
void signal_handler(int signum) {
    // write() is async-signal-safe, printf() is NOT!
    const char msg[] = "\nSignal caught! Signum: ";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    
    // Convert signal number to character (simple, safe)
    char signum_char = '0' + signum;
    if (signum < 10) {
        write(STDOUT_FILENO, &signum_char, 1);
    }
    write(STDOUT_FILENO, "\n", 1);
    
    // Set flag for main program to check
    signal_received = signum;
    
    // DON'T DO THIS in handlers (not async-signal-safe):
    // std::cout << "Signal: " << signum << std::endl;  // BAD!
    // printf("Signal: %d\n", signum);                  // BAD!
    // malloc/new                                       // BAD!
    // Most library functions                          // BAD!
}

/*
 * IMPROVED SIGNAL HANDLER using sigaction
 * Provides more control and portability
 */
void advanced_signal_handler(int signum, siginfo_t *info, void *context) {
    const char msg[] = "\nAdvanced handler - Signal: ";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    
    // info provides additional signal information:
    // - info->si_pid: PID of sending process
    // - info->si_uid: UID of sending process
    // - info->si_value: Data from sender (sigqueue)
    
    signal_received = signum;
}

int main() {
    std::cout << "Signal Handling Demo (PID: " << getpid() << ")\n";
    std::cout << "Press Ctrl+C to send SIGINT\n";
    std::cout << "Or use: kill -SIGTERM " << getpid() << "\n\n";
    
    // METHOD 1: Using signal() - old, simple but unreliable
    // Avoid in production code (behavior varies across systems)
    std::signal(SIGINT, signal_handler);   // Catch Ctrl+C
    std::signal(SIGTERM, signal_handler);  // Catch termination
    
    // METHOD 2: Using sigaction() - modern, reliable, portable
    struct sigaction sa;
    std::memset(&sa, 0, sizeof(sa));  // Zero-initialize structure
    
    // Set handler function
    sa.sa_sigaction = advanced_signal_handler;
    
    // Flags for handler behavior:
    // SA_SIGINFO: Use sa_sigaction instead of sa_handler (more info)
    // SA_RESTART: Restart interrupted system calls (convenience)
    // SA_NODEFER: Don't block this signal while handling it
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    
    // Block all signals during handler execution (safer)
    sigfillset(&sa.sa_mask);
    
    // Install handler for SIGUSR1 (user-defined signal)
    if (sigaction(SIGUSR1, &sa, nullptr) == -1) {
        perror("sigaction");
        return 1;
    }
    
    std::cout << "Handlers installed. Waiting for signals...\n";
    std::cout << "Test commands:\n";
    std::cout << "  kill -SIGUSR1 " << getpid() << "\n";
    std::cout << "  kill -SIGTERM " << getpid() << "\n\n";
    
    // Main loop - check flag periodically
    int count = 0;
    while (signal_received == 0 && count < 30) {
        std::cout << "Working... (iteration " << ++count << ")\n";
        sleep(1);  // Sleep can be interrupted by signals
    }
    
    if (signal_received) {
        std::cout << "\nExiting due to signal: " << signal_received << "\n";
        std::cout << "Common signals:\n";
        std::cout << "  SIGINT  = 2  (Ctrl+C)\n";
        std::cout << "  SIGTERM = 15 (Termination)\n";
        std::cout << "  SIGUSR1 = 10 (User-defined)\n";
    } else {
        std::cout << "\nTimeout - no signal received\n";
    }
    
    return 0;
}

/*
 * TESTING GUIDE:
 * 
 * 1. Compile and run:
 *    g++ -std=c++17 01_basic_signals.cpp -o basic_signals
 *    ./basic_signals &
 * 
 * 2. Send signals from another terminal:
 *    kill -SIGINT <pid>   # Same as Ctrl+C
 *    kill -SIGTERM <pid>  # Graceful termination
 *    kill -SIGUSR1 <pid>  # Custom signal
 * 
 * 3. Try signals that CAN'T be caught:
 *    kill -SIGKILL <pid>  # Force kill (no handler runs!)
 *    kill -SIGSTOP <pid>  # Force stop (no handler runs!)
 * 
 * INTERVIEW QUESTIONS:
 * 
 * Q: Why use sigaction() instead of signal()?
 * A: sigaction() is standardized, more portable, and provides better control
 *    (signal() behavior varies across Unix systems)
 * 
 * Q: What happens if signal arrives during handler execution?
 * A: Depends on sa_mask and flags. By default, same signal is blocked.
 *    Other signals can arrive unless explicitly blocked.
 * 
 * Q: Can you catch SIGKILL?
 * A: No! SIGKILL and SIGSTOP cannot be caught, blocked, or ignored.
 *    They ensure admin can always kill/stop processes.
 * 
 * Q: What is a signal mask?
 * A: Set of signals currently blocked for the process.
 *    Blocked signals remain pending until unblocked.
 */
