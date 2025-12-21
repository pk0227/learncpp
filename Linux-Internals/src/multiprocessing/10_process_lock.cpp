/*
 * PTHREAD MUTEX FOR INTER-PROCESS SYNCHRONIZATION
 * 
 * Demonstrates using pthread mutex in shared memory to synchronize processes.
 * 
 * KEY CONCEPT:
 * - pthread mutex can work across processes (not just threads!)
 * - Requires PTHREAD_PROCESS_SHARED attribute
 * - Must reside in shared memory (not process-private memory)
 * 
 * SETUP STEPS:
 * 1. Create shared memory region
 * 2. Place mutex in shared memory
 * 3. Set mutex attribute to PTHREAD_PROCESS_SHARED
 * 4. Initialize mutex with shared attribute
 * 5. All processes access same physical mutex
 * 
 * PTHREAD MUTEX ATTRIBUTES:
 * - PTHREAD_PROCESS_PRIVATE: Default, only same process (different threads)
 * - PTHREAD_PROCESS_SHARED: Can synchronize different processes
 * 
 * WHY THIS WORKS:
 * - Mutex data structure in shared memory
 * - All processes map same physical memory
 * - Kernel handles mutex operations atomically
 * - Process-shared attribute tells kernel to handle cross-process case
 * 
 * CRITICAL SECTION:
 * - Counter increment protected by mutex
 * - Only one process can execute increment at a time
 * - Prevents race conditions on shared counter
 * 
 * MUTEX vs SEMAPHORE (for processes):
 * - Mutex: Ownership concept, same process must lock/unlock
 * - Semaphore: No ownership, any process can post
 * - Mutex: POSIX requires process-shared attribute
 * - Semaphore: Named semaphore automatically process-shared
 * 
 * CLEANUP:
 * - pthread_mutex_destroy(): Destroy mutex
 * - pthread_mutexattr_destroy(): Destroy attribute
 * - munmap(): Unmap shared memory
 * - Important: Clean up in correct order
 * 
 * INTERVIEW TOPICS:
 * - Difference between thread mutex and process mutex
 * - Why mutex must be in shared memory
 * - Alternative: named POSIX semaphores
 * - Performance comparison of IPC sync primitives
 */

#include <iostream>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

struct SharedData {
  pthread_mutex_t mutex;
  int counter;
};

void worker(SharedData *data) {
  pthread_mutex_lock(&data->mutex);
  std::cout << "Process " << getpid() << " is starting..." << std::endl;
  sleep(1); // Simulate some work
  data->counter += 1;
  std::cout << "Process " << getpid()
            << " is finished. Counter: " << data->counter << std::endl;
  pthread_mutex_unlock(&data->mutex);
}

int main() {
  const int num_processes = 4;

  // Create shared memory
  SharedData *data = static_cast<SharedData *>(
      mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE,
           MAP_SHARED | MAP_ANONYMOUS, -1, 0));

  // Initialize the mutex and the counter
  pthread_mutexattr_t mutex_attr;
  pthread_mutexattr_init(&mutex_attr);
  pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
  pthread_mutex_init(&data->mutex, &mutex_attr);
  data->counter = 0;

  for (int i = 0; i < num_processes; ++i) {
    pid_t pid = fork();
    if (pid == 0) {
      // Child process
      worker(data);
      exit(0);
    } else if (pid > 0) {
      // Parent process
    } else {
      std::cerr << "Fork failed." << std::endl;
      exit(1);
    }
  }

  // Wait for all child processes to finish
  for (int i = 0; i < num_processes; ++i) {
    wait(NULL);
  }

  std::cout << "Final counter value: " << data->counter << std::endl;

  // Cleanup
  pthread_mutex_destroy(&data->mutex);
  pthread_mutexattr_destroy(&mutex_attr);
  munmap(data, sizeof(SharedData));

  return 0;
}
