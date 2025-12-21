/*
 * BASIC PROCESS CREATION WITH FORK
 * 
 * Demonstrates fundamental process creation in Unix/Linux using fork() system call.
 * 
 * FORK MECHANISM:
 * - fork() creates exact copy of calling process (parent)
 * - Returns twice: once in parent (child's PID), once in child (0)
 * - Child gets copy of parent's memory, file descriptors, etc.
 * - Copy-on-write optimization: memory pages shared until modified
 * 
 * KEY SYSTEM CALLS:
 * - fork(): Create new process
 * - getpid(): Get current process ID
 * - waitpid(): Parent waits for child to terminate
 * 
 * RETURN VALUES:
 * - pid < 0: Fork failed
 * - pid == 0: Code executes in child process
 * - pid > 0: Code executes in parent process (pid is child's PID)
 * 
 * PROCESS vs THREAD:
 * - Process: Separate memory space, heavier, more isolated
 * - Thread: Shared memory space, lighter, faster communication
 * - Process creation ~10-100x slower than thread creation
 * 
 * INTERVIEW CONCEPTS:
 * - Why fork returns twice
 * - Copy-on-write optimization
 * - When to use processes vs threads
 * - What resources are copied vs shared
 * 
 * IMPORTANT:
 * - Always check fork() return value for errors
 * - Parent should wait for child (avoid zombies)
 * - Child and parent execute concurrently after fork
 */

#include <iostream>
#include <mutex>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

void foo() {
  std::cout << "Worker function" << std::endl;
  std::cout << "Worker function process id: " << getpid() << std::endl;
  std::cout << "Worker thread id: " << std::this_thread::get_id() << std::endl;
  std::cout << "Worker function finished" << std::endl;
}

int main() {
  std::cout << "Main function" << std::endl;
  std::cout << "Main function process id: " << getpid() << std::endl;
  std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;

  // fork() returns twice:
  // - In parent: returns child's PID (positive number)
  // - In child: returns 0
  // - On error: returns -1
  pid_t pid = fork();

  if (pid == 0) {
    // Child process executes this block
    // Has different PID than parent, but copy of parent's memory
    foo();
  } else if (pid > 0) {
    // Parent process executes this block
    // pid contains the child's process ID
    waitpid(pid, nullptr, 0);  // Wait for child to finish (reap zombie)
    std::cout << "Main function finished" << std::endl;
  } else {
    // Fork failed (out of resources, too many processes, etc.)
    std::cerr << "Fork failed" << std::endl;
    return 1;
  }

  return 0;
}
