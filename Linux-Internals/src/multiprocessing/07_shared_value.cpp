/**
 * SHARED MEMORY WITH PROCESS SYNCHRONIZATION
 * 
 * Demonstrates inter-process communication using POSIX shared memory and semaphores.
 * 
 * SHARED MEMORY CONCEPT:
 * - Different processes normally have isolated memory spaces
 * - Shared memory: Region of memory accessible by multiple processes
 * - Fastest IPC mechanism (no kernel copying, direct memory access)
 * - Requires synchronization to prevent race conditions
 * 
 * POSIX SHARED MEMORY API:
 * - shm_open(): Create/open named shared memory object
 * - ftruncate(): Set size of shared memory
 * - mmap(): Map shared memory into process address space
 * - munmap(): Unmap shared memory
 * - shm_unlink(): Delete shared memory object
 * 
 * MEMORY MAPPING:
 * - mmap() returns pointer to shared memory region
 * - Multiple processes map same object → see same physical memory
 * - Changes by one process immediately visible to others
 * 
 * SYNCHRONIZATION NECESSITY:
 * - Without sync: Race conditions (lost updates, inconsistent reads)
 * - Semaphore protects critical section (incrementer/decrementer)
 * - Binary semaphore (initialized to 1) acts like mutex
 * 
 * SEMAPHORE OPERATIONS:
 * - sem_wait(): Decrement, blocks if 0 (lock)
 * - sem_post(): Increment, wakes waiters (unlock)
 * - Named semaphore: Survives process termination
 * 
 * RACE CONDITION WITHOUT SEMAPHORE:
 * 1. Process A reads value (10)
 * 2. Process B reads value (10)
 * 3. Process A increments and writes (11)
 * 4. Process B decrements and writes (9)
 * 5. Result: 9, but should be 10 (one operation lost!)
 * 
 * CLEANUP:
 * - sem_close(): Close semaphore descriptor
 * - sem_unlink(): Remove semaphore from system
 * - shm_unlink(): Remove shared memory from system
 * - Important: Prevents resource leaks
 * 
 * INTERVIEW TOPICS:
 * - Why shared memory fastest IPC
 * - Difference between threads (implicit sharing) and processes (explicit sharing)
 * - When to use shared memory vs message passing
 * - Memory consistency and cache coherence issues
 */

#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
constexpr char SHM_NAME[] = "/shared_memory";
constexpr char SEM_NAME[] = "/shared_semaphore";

void incrementer() {
  // Open shared memory and semaphore
  int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
  sem_t *sem = sem_open(SEM_NAME, 0);

  // Map shared memory to the address space
  int *shared_value = static_cast<int *>(
      mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));

  // Perform increments
  for (int i = 0; i < 10; ++i) {
    sem_wait(sem);
    ++(*shared_value);
    std::cout << "Incrementer: " << *shared_value << std::endl;
    sem_post(sem);

    sleep(1);
  }

  // Clean up
  munmap(shared_value, sizeof(int));
  close(shm_fd);
  sem_close(sem);
}

void decrementer() {
  // Open shared memory and semaphore
  int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
  sem_t *sem = sem_open(SEM_NAME, 0);

  // Map shared memory to the address space
  int *shared_value = static_cast<int *>(
      mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));

  // Perform decrements
  for (int i = 0; i < 10; ++i) {
    sem_wait(sem);
    --(*shared_value);
    std::cout << "Decrementer: " << *shared_value << std::endl;
    sem_post(sem);

    sleep(1);
  }

  // Clean up
  munmap(shared_value, sizeof(int));
  close(shm_fd);
  sem_close(sem);
}

int main() {
  // Create shared memory and semaphore
  int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
  ftruncate(shm_fd, sizeof(int));
  sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);

  // Fork a new process
  pid_t pid = fork();

  if (pid == 0) {
    incrementer();
  } else {
    decrementer();

    // Wait for the child process to finish
    waitpid(pid, nullptr, 0);

    // Clean up
    sem_close(sem);
    sem_unlink(SEM_NAME);
    shm_unlink(SHM_NAME);
  }

  return 0;
}
