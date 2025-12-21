/*
 * CREATING MULTIPLE CHILD PROCESSES
 * 
 * Demonstrates creating multiple concurrent processes using sequential forks.
 * 
 * PROCESS TREE:
 * - Parent process (main)
 *   ├─ Child 1 (runs foo)
 *   └─ Child 2 (runs bar)
 * 
 * EXECUTION FLOW:
 * 1. Parent forks first child
 * 2. Parent forks second child
 * 3. Both children run concurrently
 * 4. Parent waits for both to finish
 * 
 * KEY POINTS:
 * - Each fork creates exactly ONE new process
 * - Children don't fork (if-else prevents it)
 * - Parent must wait for ALL children to avoid zombies
 * - Children may finish in any order (concurrent execution)
 * 
 * WAIT SEMANTICS:
 * - waitpid(pid, &status, 0): Waits for specific child
 * - status: Contains exit code and termination info
 * - WIFEXITED(status): Check if normal termination
 * - WEXITSTATUS(status): Extract exit code
 * 
 * COMMON PITFALLS:
 * - Forgetting to wait for children (creates zombies)
 * - Accidental fork bombs (child processes also forking)
 * - Not checking fork() return values
 * 
 * INTERVIEW QUESTIONS:
 * - How many processes created by N fork calls?
 * - What if you removed the if-else structure?
 * - How to create N processes in a loop?
 */

#include <chrono>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

void foo() {
  std::cout << "Function foo starts" << std::endl;
  std::cout << "Worker function process id: " << getpid() << std::endl;
  std::cout << "Worker thread id: " << pthread_self() << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "Function foo finishes" << std::endl;
}

void bar() {
  std::cout << "Function bar starts" << std::endl;
  std::cout << "Worker function process id: " << getpid() << std::endl;
  std::cout << "Worker thread id: " << pthread_self() << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  std::cout << "Function bar finishes" << std::endl;
}

int main() {
  std::cout << "Main function" << std::endl;
  std::cout << "Main function process id: " << getpid() << std::endl;
  std::cout << "Main thread id: " << pthread_self() << std::endl;

  pid_t pid = fork();

  if (pid == 0) { // Child process
    foo();
  } else if (pid > 0) { // Parent process
    pid_t pid2 = fork();
    if (pid2 == 0) { // Second child process
      bar();
    } else if (pid2 > 0) { // Parent process
      int status;
      waitpid(pid, &status, 0);
      waitpid(pid2, &status, 0);
      std::cout << "Main function finished" << std::endl;
    } else {
      std::cerr << "Error: Failed to create the second child process"
                << std::endl;
      return 1;
    }
  } else {
    std::cerr << "Error: Failed to create the first child process" << std::endl;
    return 1;
  }

  return 0;
}
