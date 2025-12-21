/*
 * NAMED SEMAPHORE FOR RESOURCE LIMITING (PROCESS VERSION)
 * 
 * Controls concurrent access to shared resource across multiple processes.
 * 
 * SEMAPHORE FOR PROCESSES:
 * - Named semaphore: Identified by filesystem path (/semaphore_example)
 * - Exists in kernel, survives process termination
 * - Accessible by any process knowing the name
 * - Automatically process-shared (unlike pthread mutex)
 * 
 * RESOURCE LIMITING PATTERN:
 * - 10 processes created
 * - Semaphore initialized to 3
 * - Maximum 3 processes can access resource simultaneously
 * - Other 7 must wait for a slot
 * 
 * SEMAPHORE OPERATIONS:
 * - sem_open(): Create/open named semaphore
 * - sem_wait(): Decrement (blocks if 0), acquire permit
 * - sem_post(): Increment, release permit
 * - sem_close(): Close descriptor (doesn't delete semaphore!)
 * - sem_unlink(): Delete semaphore from system
 * 
 * SEMAPHORE LIFECYCLE:
 * 1. Parent creates semaphore with initial count 3
 * 2. Parent forks 10 children
 * 3. Each child calls sem_wait() (blocks if count is 0)
 * 4. First 3 children proceed immediately
 * 5. When child calls sem_post(), count increases
 * 6. One waiting child can now proceed
 * 7. Parent waits for all children
 * 8. Parent cleans up semaphore
 * 
 * NAMED vs UNNAMED SEMAPHORE:
 * - Named: Filesystem path, any process can access
 * - Unnamed: Memory address, only shared-memory processes
 * 
 * COMMON USE CASES:
 * - Database connection pools
 * - Thread/process pool management  
 * - Rate limiting (API requests)
 * - Resource quotas
 * 
 * CLEANUP IMPORTANCE:
 * - sem_unlink() removes from filesystem
 * - Without unlink, semaphore persists after program exits
 * - Can cause issues when re-running program
 * - Check /dev/shm/ on Linux to see leftover semaphores
 * 
 * INTERVIEW POINTS:
 * - Counting semaphore vs binary semaphore
 * - Why use semaphore instead of N mutexes
 * - Named vs unnamed semaphores
 * - How to implement barrier with semaphores
 */

#include <chrono>
#include <fcntl.h>
#include <iostream>
#include <random>
#include <semaphore.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>

// The shared resource is printing to the console
void shared_resource(int process_id) {
  std::cout << "Process " << process_id << " is using the shared resource"
            << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 500 + 500));
  std::cout << "Process " << process_id << " is done using the shared resource"
            << std::endl;
}

void worker(sem_t *semaphore, int process_id) {
  std::cout << "Process " << process_id << " is waiting for the semaphore"
            << std::endl;
  sem_wait(semaphore);
  std::cout << "Process " << process_id << " acquired the semaphore"
            << std::endl;
  shared_resource(process_id);
  std::cout << "Process " << process_id << " released the semaphore"
            << std::endl;
  sem_post(semaphore);
}

int main() {
  const int num_processes = 10;
  const int max_concurrent = 3;

  sem_t *semaphore = sem_open("/semaphore_example", O_CREAT, S_IRUSR | S_IWUSR,
                              max_concurrent);

  std::vector<pid_t> children;

  for (int i = 0; i < num_processes; ++i) {
    pid_t pid = fork();

    if (pid == 0) { // child process
      worker(semaphore, i);
      exit(0);
    } else if (pid > 0) { // parent process
      children.push_back(pid);
    } else {
      std::cerr << "Fork failed" << std::endl;
      return 1;
    }
  }

  // Wait for all child processes to finish
  for (const auto &child_pid : children) {
    waitpid(child_pid, nullptr, 0);
  }

  sem_close(semaphore);
  sem_unlink("/semaphore_example");

  return 0;
}
