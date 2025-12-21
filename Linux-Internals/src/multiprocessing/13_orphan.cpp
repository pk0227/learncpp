/*
 * ORPHAN PROCESS DEMONSTRATION
 * 
 * Shows what happens when parent process terminates before child process.
 * 
 * ORPHAN PROCESS DEFINED:
 * - Child process whose parent has terminated
 * - Child continues running even though parent is gone
 * - Automatically re-parented by init/systemd (PID 1)
 * 
 * LIFECYCLE:
 * 1. Parent creates child
 * 2. Parent exits immediately
 * 3. Child continues running (sleeps for 10 seconds)
 * 4. Child becomes orphan, adopted by init (PID 1)
 * 5. When child exits, init reaps it (collects exit status)
 * 
 * WHY RE-PARENT TO INIT?
 * - Every process must have a parent
 * - Init is "ultimate parent" that never dies
 * - Init automatically reaps all orphaned children
 * - Prevents zombies from accumulating
 * 
 * VERIFICATION:
 * - Run: ps -ef | grep <child_pid>
 * - PPID (parent PID) changes from original parent to 1 (init)
 * - Child continues running normally
 * 
 * PRACTICAL SCENARIOS:
 * - Daemon processes (intentionally orphaned background services)
 * - Parent crashes but child should continue
 * - Long-running background tasks
 * 
 * ORPHAN vs ZOMBIE:
 * - Orphan: Running child with dead parent (adopted by init)
 * - Zombie: Dead child whose parent hasn't reaped it (wastes resources)
 * - Orphan is NOT a zombie (child still running)
 * 
 * DAEMON CREATION:
 * - Double fork technique creates intentional orphan
 * - Parent exits, grandchild orphaned and adopted by init
 * - Grandchild becomes daemon (no terminal, runs in background)
 * 
 * INTERVIEW POINTS:
 * - What happens to orphan processes?
 * - Role of init/systemd in process management
 * - Difference between orphan and zombie
 * - How to create daemon processes
 */

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
    sleep(10);
    std::cout << "Child process (PID: " << getpid() << ") finished."
              << std::endl;
  } else {
    // Parent process
    std::cout << "Parent process (PID: " << getpid() << ") started."
              << std::endl;
    std::cout << "Parent process (PID: " << getpid()
              << ") is exiting, leaving the child process orphaned."
              << std::endl;
  }

  return 0;
}
