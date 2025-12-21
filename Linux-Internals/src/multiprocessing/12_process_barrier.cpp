/*
 * BARRIER IMPLEMENTATION USING POSIX SEMAPHORES (PROCESSES)
 * 
 * Demonstrates synchronization point where all processes must arrive before any proceed.
 * 
 * BARRIER CONCEPT:
 * - N processes must all reach barrier before any can continue
 * - Like a gate that opens only when everyone arrives
 * - Useful for phased parallel algorithms
 * 
 * **NOTE: This implementation is simplified and has issues:**
 * - Semaphore initialized to num_workers
 * - Each process does sem_wait() then sem_post()
 * - This doesn't implement true barrier semantics!
 * - Processes may pass through at different times
 * 
 * PROPER BARRIER ALGORITHM:
 * 1. Counter: Track how many arrived
 * 2. Mutex: Protect counter
 * 3. When Nth arrives: Reset counter, release all
 * 4. Others wait on condition/semaphore
 * 
 * WHAT THIS CODE ACTUALLY DOES:
 * - Semaphore acts like ticket system
 * - Each process takes ticket (wait) and returns it (post)
 * - Doesn't guarantee all wait until N arrive
 * - Processes may proceed as soon as one slot available
 * 
 * CORRECT BARRIER NEEDS:
 * - Arrival counter in shared memory
 * - Mutex protecting counter
 * - Release semaphore for waiting processes
 * - Two-phase protocol (gather phase, release phase)
 * 
 * BETTER APPROACHES:
 * - Use library implementation (Boost.Interprocess)
 * - Implement proper sense-reversing barrier
 * - Use MPI_Barrier for distributed computing
 * 
 * WHY BARRIERS IMPORTANT:
 * - Parallel matrix operations (phases)
 * - Iterative algorithms (synchronize iterations)
 * - Map-reduce patterns (sync between map and reduce)
 * - Parallel simulations (time-step synchronization)
 * 
 * INTERVIEW CONCEPTS:
 * - How to implement correct barrier
 * - Difference between barrier and latch
 * - Why simple semaphore doesn't work for barrier
 * - Barrier reusability requirements
 * 
 * NOTE: For production, use proper barrier library or std::barrier (C++20) for threads
 */

#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
#include <sys/wait.h>
#include <unistd.h>

void worker(sem_t *barrier, int worker_id) {
  // Simulate some work
  int sleep_time = rand() % 3 + 1;
  sleep(sleep_time);
  std::cout << "Worker " << worker_id
            << " finished initial work and is waiting at the barrier...\n";

  // Wait at the barrier
  sem_wait(barrier);
  sem_post(barrier);

  std::cout << "Worker " << worker_id
            << " passed the barrier and is continuing execution...\n";
}

int main() {
  srand(time(0));
  const int num_workers = 5;

  // Create a barrier with num_workers as the number of participants
  sem_t *barrier = sem_open("/barrier", O_CREAT | O_EXCL, 0644, num_workers);
  if (barrier == SEM_FAILED) {
    perror("sem_open");
    return 1;
  }

  // Start worker processes
  for (int i = 0; i < num_workers; i++) {
    pid_t pid = fork();
    if (pid == 0) { // Child process
      worker(barrier, i);
      return 0;
    } else if (pid < 0) { // Fork failed
      perror("fork");
      return 1;
    }
  }

  // Wait for worker processes to finish
  for (int i = 0; i < num_workers; i++) {
    wait(NULL);
  }

  // Cleanup
  sem_close(barrier);
  sem_unlink("/barrier");

  return 0;
}
