/*
 * ZOMBIE PROCESS DEMONSTRATION (ANTI-PATTERN)
 * 
 * Shows the problem of zombie processes and why parent must reap children.
 * 
 * ZOMBIE PROCESS DEFINED:
 * - Terminated child process that hasn't been reaped by parent
 * - Process is dead but entry remains in process table
 * - Wastes system resources (limited process table entries)
 * - Shows as "<defunct>" or state "Z+" in ps/top
 * 
 * WHY ZOMBIES EXIST:
 * - OS keeps terminated child's exit status
 * - Parent may want to check why/how child died (wait/waitpid)
 * - Can't free process table entry until parent collects status
 * - If parent never calls wait(), child stays zombie forever
 * 
 * LIFECYCLE:
 * 1. Child executes and calls exit(0)
 * 2. Kernel marks process as zombie, sends SIGCHLD to parent
 * 3. Parent ignores signal, doesn't call wait()
 * 4. Child remains zombie until parent dies
 * 5. When parent dies, init adopts zombie and reaps it
 * 
 * PROCESS TABLE ENTRY CONTAINS:
 * - Process ID (PID)
 * - Exit status code
 * - Resource usage statistics
 * - Minimal memory (most resources freed)
 * 
 * VERIFICATION:
 * - Run program: ./zombie_demo
 * - In another terminal: ps aux | grep Z
 * - You'll see child process with "Z+" state
 * - Note <defunct> in process name
 * 
 * PROPER FIX:
 * - Parent should call wait() or waitpid() to reap child
 * - Or install SIGCHLD handler
 * - Or use double-fork technique for fire-and-forget children
 * 
 * SYSTEM IMPACT:
 * - Each zombie wastes one process table entry
 * - System has limited PIDs (32768 by default on Linux)
 * - Many zombies can exhaust PID space
 * - New processes fail to spawn (fork returns -1)
 * 
 * INTERVIEW QUESTIONS:
 * - What is a zombie process?
 * - How to prevent/cleanup zombies?
 * - Difference between zombie and orphan?
 * - Can you kill a zombie with kill -9? (No! Already dead)
 * - How to remove zombie? (Kill parent or wait in parent)
 * 
 * WARNING: This intentionally creates a zombie. Kill parent process after testing.
 */

#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();

  if (pid == 0) {
    // Child process
    std::cout << "Child process (PID: " << getpid() << ") started."
              << std::endl;
    sleep(2);
    std::cout << "Child process (PID: " << getpid() << ") finished."
              << std::endl;
    exit(0);
  } else {
    // Parent process
    std::cout << "Parent process (PID: " << getpid() << ") started."
              << std::endl;
    std::cout << "Parent process (PID: " << getpid()
              << ") is NOT waiting for the child process to terminate."
              << std::endl;
    std::cout << "Child process (PID: " << pid
              << ") will become a zombie after termination." << std::endl;
    sleep(10);
    std::cout << "Parent process (PID: " << getpid() << ") finished."
              << std::endl;
  }

  return 0;
}
