/*
 * SHARED ARRAY BETWEEN PROCESSES (HAS RACE CONDITIONS!)
 * 
 * Demonstrates anonymous shared memory using mmap() - BUT HAS BUGS!
 * 
 * ANONYMOUS SHARED MEMORY:
 * - MAP_ANONYMOUS: No backing file, memory only in RAM
 * - MAP_SHARED: Changes visible across all processes
 * - Faster setup than named shared memory (no shm_open needed)
 * - Only works between related processes (parent-child)
 * 
 * MMAP FLAGS:
 * - PROT_READ | PROT_WRITE: Memory can be read and written
 * - MAP_SHARED: Share modifications between processes
 * - MAP_ANONYMOUS: Not backed by file (use -1 for fd)
 * 
 * **CRITICAL BUG: NO SYNCHRONIZATION!**
 * - Two processes modify array concurrently
 * - No mutex/semaphore protection
 * - RACE CONDITION: Results unpredictable
 * - Array values will be inconsistent
 * 
 * RACE CONDITION EXAMPLE:
 * 1. Process A reads arr[0] = 5
 * 2. Process B reads arr[0] = 5  
 * 3. Process A increments: 5 + 1 = 6
 * 4. Process B decrements: 5 - 1 = 4
 * 5. Process A writes arr[0] = 6
 * 6. Process B writes arr[0] = 4
 * 7. Result: 4 (but increment was lost!)
 * 
 * PROPER FIX NEEDED:
 * - Add named semaphore or mutex (see 07_shared_value.cpp)
 * - Protect critical section (array modification)
 * - This is example of what NOT to do!
 * 
 * INTERVIEW POINTS:
 * - Identify race condition in code
 * - Explain why synchronization needed
 * - Anonymous vs named shared memory
 * - When mmap appropriate vs other IPC
 * 
 * NOTE: This code has intentional bugs for demonstration purposes!
 */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

constexpr size_t ARRAY_SIZE = 3;

void increment_array(int *shared_array) {
  while (true) {
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
      shared_array[i] += 1;
    }
    std::cout << "Array incremented: ";
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
      std::cout << shared_array[i] << ' ';
    }
    std::cout << std::endl;
    sleep(1);
  }
}

void decrement_array(int *shared_array) {
  while (true) {
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
      shared_array[i] -= 1;
    }
    std::cout << "Array decremented: ";
    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
      std::cout << shared_array[i] << ' ';
    }
    std::cout << std::endl;
    sleep(1);
  }
}

int main() {
  int *shared_array = static_cast<int *>(
      mmap(NULL, sizeof(int) * ARRAY_SIZE, PROT_READ | PROT_WRITE,
           MAP_SHARED | MAP_ANONYMOUS, -1, 0));

  for (size_t i = 0; i < ARRAY_SIZE; ++i) {
    shared_array[i] = 0;
  }

  pid_t pid1 = fork();
  if (pid1 == 0) {
    increment_array(shared_array);
    exit(0);
  }

  pid_t pid2 = fork();
  if (pid2 == 0) {
    decrement_array(shared_array);
    exit(0);
  }

  int status;
  waitpid(pid1, &status, 0);
  waitpid(pid2, &status, 0);

  munmap(shared_array, sizeof(int) * ARRAY_SIZE);

  return 0;
}
