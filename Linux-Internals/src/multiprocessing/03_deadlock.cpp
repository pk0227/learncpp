/*
 * INTER-PROCESS DEADLOCK DEMONSTRATION
 * 
 * Classic deadlock scenario using POSIX semaphores between processes.
 * 
 * DEADLOCK CONDITIONS (All 4 must be present):
 * 1. Mutual Exclusion: Resources cannot be shared
 * 2. Hold and Wait: Process holds resource while waiting for another
 * 3. No Preemption: Resources cannot be forcibly taken
 * 4. Circular Wait: Circular chain of processes waiting for resources
 * 
 * THIS EXAMPLE:
 * - Process A: Acquires lock_a, waits for lock_b
 * - Process B: Acquires lock_b, waits for lock_a
 * - Creates circular wait → DEADLOCK!
 * 
 * POSIX SEMAPHORES:
 * - sem_open(): Create/open named semaphore (survives process death)
 * - sem_wait(): Decrement (P operation), blocks if 0
 * - sem_post(): Increment (V operation), wakes waiters
 * - sem_close(): Close semaphore descriptor
 * - sem_unlink(): Remove semaphore from system
 * 
 * DEADLOCK PREVENTION STRATEGIES:
 * 1. Lock ordering: Always acquire locks in same order
 * 2. Timeout: Don't wait indefinitely (sem_timedwait)
 * 3. Deadlock detection: Periodic check and recovery
 * 4. Avoid hold-and-wait: Acquire all locks at once
 * 
 * FIX FOR THIS CODE:
 * - Make both processes acquire locks in same order (e.g., lock_a then lock_b)
 * 
 * INTERVIEW CONCEPTS:
 * - Four conditions for deadlock
 * - Difference between deadlock and livelock
 * - Named vs unnamed semaphores
 * - How to detect and recover from deadlocks
 */

#include <chrono>
#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

void process_a(sem_t *lock_a, sem_t *lock_b) {
  sem_wait(lock_a);
  std::cout << "Process A acquired lock_a" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "Process A trying to acquire lock_b" << std::endl;
  sem_wait(lock_b);
  std::cout << "Process A acquired lock_b" << std::endl;
  sem_post(lock_b);
  sem_post(lock_a);
}

void process_b(sem_t *lock_a, sem_t *lock_b) {
  sem_wait(lock_b);
  std::cout << "Process B acquired lock_b" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "Process B trying to acquire lock_a" << std::endl;
  sem_wait(lock_a);
  std::cout << "Process B acquired lock_a" << std::endl;
  sem_post(lock_a);
  sem_post(lock_b);
}

int main() {
  sem_t *lock_a = sem_open("/lock_a", O_CREAT, 0644, 1);
  sem_t *lock_b = sem_open("/lock_b", O_CREAT, 0644, 1);

  int pid = fork();

  if (pid == 0) { // child process
    process_a(lock_a, lock_b);
  } else { // parent process
    process_b(lock_a, lock_b);
    wait(nullptr);
  }

  sem_close(lock_a);
  sem_close(lock_b);
  sem_unlink("/lock_a");
  sem_unlink("/lock_b");

  std::cout << "Process finished" << std::endl;

  return 0;
}
