/*
 * POSIX SEMAPHORES - Synchronization Primitive for IPC
 * 
 * SEMAPHORE CONCEPT:
 * - Integer counter with atomic operations
 * - Used for synchronization and mutual exclusion
 * - Two main operations: wait (decrement) and post (increment)
 * - Can be used within process (unnamed) or between processes (named)
 * - Blocks when counter would go negative
 * 
 * TYPES OF SEMAPHORES:
 * 1. Named Semaphores (POSIX):
 *    - Have name like "/sem_name"
 *    - Shared between unrelated processes
 *    - Persist until explicitly removed
 *    - Created with sem_open()
 * 
 * 2. Unnamed Semaphores (POSIX):
 *    - No name, accessed via pointer
 *    - Placed in shared memory for IPC
 *    - Destroyed when not needed
 *    - Created with sem_init()
 * 
 * 3. System V Semaphores (older):
 *    - More complex API
 *    - Use semget, semctl, semop
 *    - Not covered here (POSIX preferred)
 * 
 * SEMAPHORE OPERATIONS:
 * - sem_wait(): Decrement (P operation, lock)
 *   - If value > 0: decrement and continue
 *   - If value = 0: block until > 0
 * 
 * - sem_post(): Increment (V operation, unlock)
 *   - Always succeeds
 *   - Wakes one waiting thread/process
 * 
 * - sem_trywait(): Non-blocking wait
 *   - Returns EAGAIN if would block
 * 
 * - sem_timedwait(): Wait with timeout
 * 
 * NAMED SEMAPHORE FUNCTIONS:
 * - sem_open(): Create/open named semaphore
 * - sem_close(): Close semaphore
 * - sem_unlink(): Remove semaphore name
 * 
 * UNNAMED SEMAPHORE FUNCTIONS:
 * - sem_init(): Initialize semaphore
 * - sem_destroy(): Destroy semaphore
 * 
 * SEMAPHORE VALUES:
 * - Binary semaphore: Initial value 1 (mutex)
 * - Counting semaphore: Initial value N (resource count)
 * - Zero semaphore: Initial value 0 (signaling)
 * 
 * MUTEX vs SEMAPHORE:
 * - Mutex: Binary (0/1), ownership concept, same thread lock/unlock
 * - Semaphore: Can be > 1, no ownership, any thread can post
 * - Mutex: For mutual exclusion only
 * - Semaphore: For counting resources, signaling
 * 
 * USE CASES:
 * - Binary (value=1): Mutual exclusion (like mutex)
 * - Counting: Resource pool (e.g., connection pool)
 * - Signaling: One process signals another (value=0)
 * - Producer-Consumer: Track full/empty slots
 * 
 * COMMON PATTERNS:
 * - Mutex: sem_init(&sem, 0, 1)
 * - Resource pool: sem_init(&sem, 0, N)
 * - Signaling: sem_init(&sem, 0, 0)
 * 
 * PITFALLS:
 * - Forgetting sem_unlink (named semaphores persist)
 * - Not initializing with pshared=1 for IPC
 * - Deadlock from wait without corresponding post
 * - Race conditions if semaphore not in shared memory
 * 
 * INTERVIEW TOPICS:
 * - Semaphore vs mutex?
 * - Named vs unnamed semaphores?
 * - Binary vs counting semaphores?
 * - Producer-consumer implementation?
 * - How to avoid deadlock?
 * 
 * COMPILE: g++ -std=c++17 08_posix_semaphores.cpp -o semaphores -lpthread
 */

#include <iostream>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <cerrno>
#include <ctime>

const char* SEM_NAME = "/my_semaphore";

/*
 * EXAMPLE 1: Named Semaphore (Inter-Process)
 * Semaphore shared between parent and child processes
 */
void named_semaphore_example() {
    std::cout << "\n=== NAMED SEMAPHORE EXAMPLE ===\n";
    
    // Remove if exists
    sem_unlink(SEM_NAME);
    
    // Create named semaphore
    // Initial value = 1 (binary semaphore for mutex)
    sem_t* sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 1);
    
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return;
    }
    
    std::cout << "Created named semaphore: " << SEM_NAME << "\n";
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // CHILD: Access critical section
        for (int i = 0; i < 3; i++) {
            std::cout << "Child: Waiting for semaphore...\n";
            sem_wait(sem);  // Lock
            
            std::cout << "Child: In critical section (iteration " << i << ")\n";
            sleep(1);
            
            std::cout << "Child: Leaving critical section\n";
            sem_post(sem);  // Unlock
            
            usleep(100000);
        }
        
        sem_close(sem);
        exit(0);
        
    } else {
        // PARENT: Access critical section
        for (int i = 0; i < 3; i++) {
            std::cout << "Parent: Waiting for semaphore...\n";
            sem_wait(sem);  // Lock
            
            std::cout << "Parent: In critical section (iteration " << i << ")\n";
            sleep(1);
            
            std::cout << "Parent: Leaving critical section\n";
            sem_post(sem);  // Unlock
            
            usleep(100000);
        }
        
        wait(nullptr);
        
        sem_close(sem);
        sem_unlink(SEM_NAME);
        
        std::cout << "Semaphore removed\n";
    }
}

/*
 * EXAMPLE 2: Unnamed Semaphore in Shared Memory
 * For IPC, unnamed semaphore must be in shared memory
 */
void unnamed_semaphore_ipc() {
    std::cout << "\n=== UNNAMED SEMAPHORE (IPC) ===\n";
    
    // Allocate shared memory for semaphore
    sem_t* sem = static_cast<sem_t*>(
        mmap(nullptr, sizeof(sem_t),
             PROT_READ | PROT_WRITE,
             MAP_SHARED | MAP_ANONYMOUS,
             -1, 0)
    );
    
    if (sem == MAP_FAILED) {
        perror("mmap");
        return;
    }
    
    // Initialize unnamed semaphore
    // pshared=1: Shared between processes
    // value=1: Binary semaphore
    if (sem_init(sem, 1, 1) == -1) {
        perror("sem_init");
        munmap(sem, sizeof(sem_t));
        return;
    }
    
    std::cout << "Initialized unnamed semaphore in shared memory\n";
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // CHILD
        for (int i = 0; i < 3; i++) {
            sem_wait(sem);
            std::cout << "Child: Critical section " << i << "\n";
            usleep(500000);
            sem_post(sem);
            usleep(100000);
        }
        exit(0);
        
    } else {
        // PARENT
        for (int i = 0; i < 3; i++) {
            sem_wait(sem);
            std::cout << "Parent: Critical section " << i << "\n";
            usleep(500000);
            sem_post(sem);
            usleep(100000);
        }
        
        wait(nullptr);
        
        sem_destroy(sem);
        munmap(sem, sizeof(sem_t));
    }
}

/*
 * EXAMPLE 3: Counting Semaphore (Resource Pool)
 * Demonstrates semaphore with value > 1
 */
void counting_semaphore_example() {
    std::cout << "\n=== COUNTING SEMAPHORE (RESOURCE POOL) ===\n";
    
    const char* pool_sem = "/resource_pool";
    sem_unlink(pool_sem);
    
    // Create counting semaphore with 3 resources
    const int NUM_RESOURCES = 3;
    sem_t* sem = sem_open(pool_sem, O_CREAT | O_EXCL, 0666, NUM_RESOURCES);
    
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return;
    }
    
    std::cout << "Resource pool created with " << NUM_RESOURCES << " resources\n";
    
    // Create 5 child processes (more than resources)
    for (int i = 0; i < 5; i++) {
        pid_t pid = fork();
        
        if (pid == 0) {
            // CHILD: Try to acquire resource
            std::cout << "Process " << i << ": Requesting resource...\n";
            
            sem_wait(sem);  // Acquire resource
            
            std::cout << "Process " << i << ": Got resource! Using it...\n";
            sleep(2);  // Use resource
            
            std::cout << "Process " << i << ": Releasing resource\n";
            sem_post(sem);  // Release resource
            
            sem_close(sem);
            exit(0);
        }
    }
    
    // Wait for all children
    for (int i = 0; i < 5; i++) {
        wait(nullptr);
    }
    
    sem_close(sem);
    sem_unlink(pool_sem);
    
    std::cout << "All processes completed\n";
}

/*
 * EXAMPLE 4: Producer-Consumer with Semaphores
 */

struct shared_buffer {
    sem_t mutex;   // Binary semaphore for mutual exclusion
    sem_t full;    // Count of full slots
    sem_t empty;   // Count of empty slots
    int buffer[5];
    int in;
    int out;
};

void producer_consumer_semaphores() {
    std::cout << "\n=== PRODUCER-CONSUMER WITH SEMAPHORES ===\n";
    
    // Allocate shared memory
    shared_buffer* buf = static_cast<shared_buffer*>(
        mmap(nullptr, sizeof(shared_buffer),
             PROT_READ | PROT_WRITE,
             MAP_SHARED | MAP_ANONYMOUS,
             -1, 0)
    );
    
    if (buf == MAP_FAILED) {
        perror("mmap");
        return;
    }
    
    // Initialize semaphores
    sem_init(&buf->mutex, 1, 1);      // Binary mutex
    sem_init(&buf->empty, 1, 5);      // 5 empty slots
    sem_init(&buf->full, 1, 0);       // 0 full slots initially
    
    buf->in = 0;
    buf->out = 0;
    
    std::cout << "Initialized buffer (capacity 5)\n";
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // CHILD: Consumer
        for (int i = 0; i < 8; i++) {
            sem_wait(&buf->full);   // Wait for full slot
            sem_wait(&buf->mutex);  // Enter critical section
            
            int item = buf->buffer[buf->out];
            buf->out = (buf->out + 1) % 5;
            
            sem_post(&buf->mutex);  // Leave critical section
            sem_post(&buf->empty);  // Signal empty slot
            
            std::cout << "Consumer: consumed " << item << "\n";
            usleep(300000);  // 300ms
        }
        
        exit(0);
        
    } else {
        // PARENT: Producer
        for (int i = 0; i < 8; i++) {
            int item = i * 10;
            
            sem_wait(&buf->empty);  // Wait for empty slot
            sem_wait(&buf->mutex);  // Enter critical section
            
            buf->buffer[buf->in] = item;
            buf->in = (buf->in + 1) % 5;
            
            sem_post(&buf->mutex);  // Leave critical section
            sem_post(&buf->full);   // Signal full slot
            
            std::cout << "Producer: produced " << item << "\n";
            usleep(200000);  // 200ms
        }
        
        wait(nullptr);
        
        // Cleanup
        sem_destroy(&buf->mutex);
        sem_destroy(&buf->empty);
        sem_destroy(&buf->full);
        munmap(buf, sizeof(shared_buffer));
    }
}

/*
 * EXAMPLE 5: Non-Blocking and Timed Operations
 */
void nonblocking_timed_example() {
    std::cout << "\n=== NON-BLOCKING AND TIMED OPERATIONS ===\n";
    
    const char* test_sem = "/test_sem";
    sem_unlink(test_sem);
    
    // Create semaphore with value 0
    sem_t* sem = sem_open(test_sem, O_CREAT | O_EXCL, 0666, 0);
    
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return;
    }
    
    // Try non-blocking wait (will fail - value is 0)
    std::cout << "Trying sem_trywait (non-blocking)...\n";
    
    if (sem_trywait(sem) == -1) {
        if (errno == EAGAIN) {
            std::cout << "sem_trywait: Would block (EAGAIN)\n";
        }
    }
    
    // Try timed wait with 2-second timeout
    std::cout << "\nTrying sem_timedwait with 2-second timeout...\n";
    
    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += 2;  // 2 seconds from now
    
    if (sem_timedwait(sem, &timeout) == -1) {
        if (errno == ETIMEDOUT) {
            std::cout << "sem_timedwait: Timed out (ETIMEDOUT)\n";
        }
    }
    
    // Post to semaphore
    std::cout << "\nPosting to semaphore...\n";
    sem_post(sem);
    
    // Now trywait will succeed
    std::cout << "Trying sem_trywait again...\n";
    if (sem_trywait(sem) == 0) {
        std::cout << "sem_trywait: Success!\n";
    }
    
    sem_close(sem);
    sem_unlink(test_sem);
}

/*
 * EXAMPLE 6: Semaphore Value and Status
 */
void semaphore_value_example() {
    std::cout << "\n=== SEMAPHORE VALUE ===\n";
    
    const char* val_sem = "/value_sem";
    sem_unlink(val_sem);
    
    sem_t* sem = sem_open(val_sem, O_CREAT | O_EXCL, 0666, 5);
    
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return;
    }
    
    int value;
    
    // Get current value
    sem_getvalue(sem, &value);
    std::cout << "Initial value: " << value << "\n";
    
    // Wait (decrement)
    sem_wait(sem);
    sem_wait(sem);
    
    sem_getvalue(sem, &value);
    std::cout << "After 2 waits: " << value << "\n";
    
    // Post (increment)
    sem_post(sem);
    
    sem_getvalue(sem, &value);
    std::cout << "After 1 post: " << value << "\n";
    
    sem_close(sem);
    sem_unlink(val_sem);
    
    std::cout << "\nNote: sem_getvalue is mainly for debugging\n";
}

int main() {
    std::cout << "POSIX Semaphores Demo\n";
    std::cout << "=====================\n";
    
    named_semaphore_example();
    unnamed_semaphore_ipc();
    counting_semaphore_example();
    producer_consumer_semaphores();
    nonblocking_timed_example();
    semaphore_value_example();
    
    std::cout << "\n=== SEMAPHORE LOCATIONS ===\n";
    std::cout << "Named semaphores: /dev/shm/sem.* (on Linux)\n";
    std::cout << "List: ls -l /dev/shm/sem.*\n";
    
    return 0;
}

/*
 * INTERVIEW Q&A:
 * 
 * Q: Semaphore vs mutex - when to use which?
 * A: Mutex: Mutual exclusion only, ownership, same thread unlock
 *    Semaphore: Counting resources, signaling, any process can post
 *    Use mutex for simple locks, semaphore for resource pools
 * 
 * Q: Named vs unnamed semaphores?
 * A: Named: Can be used by unrelated processes, persist until unlink
 *    Unnamed: Must be in shared memory for IPC, destroyed explicitly
 *    Named easier for unrelated processes
 * 
 * Q: Binary vs counting semaphore?
 * A: Binary: Value 0 or 1, used like mutex
 *    Counting: Value > 1, tracks multiple resources
 *    Example: Connection pool of size N uses counting semaphore
 * 
 * Q: How does producer-consumer pattern work with semaphores?
 * A: Two counting semaphores: 'empty' (free slots), 'full' (filled slots)
 *    One mutex for critical section
 *    Producer: wait(empty) -> critical -> post(full)
 *    Consumer: wait(full) -> critical -> post(empty)
 * 
 * Q: What happens if you forget sem_unlink?
 * A: Named semaphore persists in /dev/shm/ even after process ends
 *    Can cause issues on next run (already exists error)
 *    Must manually remove or use sem_unlink
 */
