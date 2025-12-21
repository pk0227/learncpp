/*
 * POSIX SHARED MEMORY - Fast IPC via Direct Memory Access
 * 
 * SHARED MEMORY CONCEPT:
 * - Multiple processes map same physical memory into their address space
 * - Fastest IPC method - no kernel involvement after setup
 * - Data written by one process immediately visible to others
 * - NO automatic synchronization - must use mutexes/semaphores
 * - Persists until explicitly removed or system reboot
 * 
 * CHARACTERISTICS:
 * - Zero-copy communication (unlike pipes/MQ)
 * - Direct memory access speed
 * - Requires manual synchronization
 * - Can share complex data structures
 * - Named like files (/name format)
 * - Size set at creation, can be changed with ftruncate
 * 
 * POSIX vs System V SHARED MEMORY:
 * - POSIX: Modern, cleaner API, file-like interface
 * - POSIX: Uses shm_open (similar to open) + mmap
 * - System V: Uses shmget, shmat, shmdt (older, more complex)
 * - POSIX preferred for new code
 * 
 * KEY FUNCTIONS:
 * - shm_open(): Create/open shared memory object
 * - ftruncate(): Set size of shared memory
 * - mmap(): Map shared memory into process address space
 * - munmap(): Unmap shared memory
 * - shm_unlink(): Remove shared memory object
 * - close(): Close file descriptor (doesn't remove shm)
 * 
 * WORKFLOW:
 * 1. shm_open() - get file descriptor
 * 2. ftruncate() - set size (creator only)
 * 3. mmap() - map to process address space
 * 4. Use memory (read/write)
 * 5. munmap() - unmap
 * 6. close() - close descriptor
 * 7. shm_unlink() - remove (last process)
 * 
 * SYNCHRONIZATION:
 * - Shared memory has NO built-in sync
 * - MUST use: semaphores, mutexes, or atomic ops
 * - Race conditions if no sync
 * - Can share mutex in shared memory (process-shared)
 * 
 * PROCESS-SHARED MUTEX:
 * - Regular mutex only works within one process
 * - PTHREAD_PROCESS_SHARED attribute makes it work across processes
 * - Must be in shared memory location
 * - Both processes must access same mutex instance
 * 
 * MEMORY MAPPING (mmap):
 * - PROT_READ | PROT_WRITE: Read and write access
 * - MAP_SHARED: Changes visible to other processes
 * - MAP_PRIVATE: Private copy-on-write (not for IPC)
 * - Returns pointer to mapped memory
 * 
 * USE CASES:
 * - High-performance IPC (databases, HPC)
 * - Large data sharing (images, datasets)
 * - Shared state (game state, caches)
 * - Memory pools between processes
 * 
 * vs OTHER IPC:
 * - vs Pipe: Much faster, but needs manual sync
 * - vs MQ: Faster for large data, no message boundaries
 * - vs Socket: Local only, much faster
 * 
 * PITFALLS:
 * - Forgetting synchronization -> race conditions
 * - Not unlinking -> memory leak (persists after crash)
 * - Size mismatch between processes
 * - Pointer sharing (only offsets, not pointers!)
 * 
 * INTERVIEW TOPICS:
 * - Why is shared memory fastest IPC?
 * - How to synchronize shared memory access?
 * - Shared memory vs message passing?
 * - Process-shared mutex setup?
 * - Cleanup on crash?
 * 
 * COMPILE: g++ -std=c++17 06_shared_memory.cpp -o shared_memory -lrt -lpthread
 * NOTE: -lrt for shm_open, -lpthread for pthread mutexes
 */

#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <pthread.h>
#include <sys/wait.h>
#include <semaphore.h>

const char* SHM_NAME = "/my_shared_memory";
const int SHM_SIZE = 4096;

/*
 * EXAMPLE 1: Basic Shared Memory - No Synchronization
 * Demonstrates basic setup but shows race condition problem
 */
void basic_shared_memory() {
    std::cout << "\n=== BASIC SHARED MEMORY (UNSAFE) ===\n";
    
    // Remove if exists
    shm_unlink(SHM_NAME);
    
    // Create shared memory object
    // O_CREAT | O_RDWR: Create if doesn't exist, read-write
    // 0666: Permissions
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    
    if (shm_fd == -1) {
        perror("shm_open");
        return;
    }
    
    // Set size of shared memory
    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        close(shm_fd);
        return;
    }
    
    std::cout << "Shared memory created: " << SHM_NAME << " (size: " << SHM_SIZE << ")\n";
    
    // Map shared memory into process address space
    // Returns pointer to mapped memory
    void* ptr = mmap(nullptr, SHM_SIZE, 
                     PROT_READ | PROT_WRITE,  // Read-write access
                     MAP_SHARED,               // Shared with other processes
                     shm_fd, 0);
    
    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return;
    }
    
    std::cout << "Memory mapped at address: " << ptr << "\n";
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // CHILD: Read from shared memory
        sleep(1);  // Wait for parent to write
        
        char* data = static_cast<char*>(ptr);
        std::cout << "Child reads: " << data << "\n";
        
        munmap(ptr, SHM_SIZE);
        close(shm_fd);
        exit(0);
        
    } else {
        // PARENT: Write to shared memory
        char* data = static_cast<char*>(ptr);
        strcpy(data, "Hello from parent via shared memory!");
        
        std::cout << "Parent wrote: " << data << "\n";
        
        wait(nullptr);
        
        munmap(ptr, SHM_SIZE);
        close(shm_fd);
        shm_unlink(SHM_NAME);
        
        std::cout << "Shared memory removed\n";
    }
}

/*
 * EXAMPLE 2: Shared Memory with Process-Shared Mutex
 * Demonstrates proper synchronization using pthread mutex
 */

struct shared_data {
    pthread_mutex_t mutex;  // Process-shared mutex
    int counter;
    char message[256];
};

void shared_memory_with_mutex() {
    std::cout << "\n=== SHARED MEMORY WITH MUTEX ===\n";
    
    shm_unlink(SHM_NAME);
    
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return;
    }
    
    ftruncate(shm_fd, sizeof(shared_data));
    
    shared_data* data = static_cast<shared_data*>(
        mmap(nullptr, sizeof(shared_data),
             PROT_READ | PROT_WRITE, MAP_SHARED,
             shm_fd, 0)
    );
    
    if (data == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return;
    }
    
    // Initialize process-shared mutex
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    
    // CRITICAL: Set mutex to be shared across processes
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    
    pthread_mutex_init(&data->mutex, &attr);
    pthread_mutexattr_destroy(&attr);
    
    data->counter = 0;
    strcpy(data->message, "Initial message");
    
    std::cout << "Initialized shared data with process-shared mutex\n";
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // CHILD: Increment counter safely
        for (int i = 0; i < 5; i++) {
            pthread_mutex_lock(&data->mutex);
            
            data->counter++;
            snprintf(data->message, sizeof(data->message), 
                     "Child updated: iteration %d", i);
            
            std::cout << "Child: counter = " << data->counter 
                      << ", msg = " << data->message << "\n";
            
            pthread_mutex_unlock(&data->mutex);
            
            usleep(100000);  // 100ms
        }
        
        munmap(data, sizeof(shared_data));
        close(shm_fd);
        exit(0);
        
    } else {
        // PARENT: Increment counter safely
        for (int i = 0; i < 5; i++) {
            pthread_mutex_lock(&data->mutex);
            
            data->counter++;
            snprintf(data->message, sizeof(data->message), 
                     "Parent updated: iteration %d", i);
            
            std::cout << "Parent: counter = " << data->counter 
                      << ", msg = " << data->message << "\n";
            
            pthread_mutex_unlock(&data->mutex);
            
            usleep(100000);  // 100ms
        }
        
        wait(nullptr);
        
        std::cout << "Final counter value: " << data->counter 
                  << " (should be 10)\n";
        
        pthread_mutex_destroy(&data->mutex);
        munmap(data, sizeof(shared_data));
        close(shm_fd);
        shm_unlink(SHM_NAME);
    }
}

/*
 * EXAMPLE 3: Producer-Consumer with Shared Memory and Semaphores
 */

struct buffer_data {
    sem_t full;     // Count of full slots
    sem_t empty;    // Count of empty slots
    sem_t mutex;    // Binary semaphore for mutual exclusion
    int buffer[10];
    int in;         // Producer index
    int out;        // Consumer index
};

void producer_consumer_shared_memory() {
    std::cout << "\n=== PRODUCER-CONSUMER WITH SHARED MEMORY ===\n";
    
    shm_unlink(SHM_NAME);
    
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return;
    }
    
    ftruncate(shm_fd, sizeof(buffer_data));
    
    buffer_data* data = static_cast<buffer_data*>(
        mmap(nullptr, sizeof(buffer_data),
             PROT_READ | PROT_WRITE, MAP_SHARED,
             shm_fd, 0)
    );
    
    if (data == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return;
    }
    
    // Initialize semaphores (process-shared)
    // pshared=1 means shared between processes
    sem_init(&data->empty, 1, 10);  // 10 empty slots
    sem_init(&data->full, 1, 0);    // 0 full slots
    sem_init(&data->mutex, 1, 1);   // Binary mutex
    
    data->in = 0;
    data->out = 0;
    
    std::cout << "Initialized shared buffer (capacity 10)\n";
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // CHILD: Consumer
        for (int i = 0; i < 5; i++) {
            sem_wait(&data->full);   // Wait for item
            sem_wait(&data->mutex);  // Enter critical section
            
            // Consume item
            int item = data->buffer[data->out];
            data->out = (data->out + 1) % 10;
            
            sem_post(&data->mutex);  // Leave critical section
            sem_post(&data->empty);  // Signal empty slot
            
            std::cout << "Consumer: consumed " << item << "\n";
            usleep(200000);  // 200ms
        }
        
        munmap(data, sizeof(buffer_data));
        close(shm_fd);
        exit(0);
        
    } else {
        // PARENT: Producer
        for (int i = 0; i < 5; i++) {
            int item = i * 10;
            
            sem_wait(&data->empty);  // Wait for empty slot
            sem_wait(&data->mutex);  // Enter critical section
            
            // Produce item
            data->buffer[data->in] = item;
            data->in = (data->in + 1) % 10;
            
            sem_post(&data->mutex);  // Leave critical section
            sem_post(&data->full);   // Signal full slot
            
            std::cout << "Producer: produced " << item << "\n";
            usleep(150000);  // 150ms
        }
        
        wait(nullptr);
        
        // Cleanup
        sem_destroy(&data->empty);
        sem_destroy(&data->full);
        sem_destroy(&data->mutex);
        
        munmap(data, sizeof(buffer_data));
        close(shm_fd);
        shm_unlink(SHM_NAME);
    }
}

/*
 * EXAMPLE 4: Checking Shared Memory Info
 */
void shared_memory_info() {
    std::cout << "\n=== SHARED MEMORY INFO ===\n";
    
    shm_unlink(SHM_NAME);
    
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return;
    }
    
    ftruncate(shm_fd, SHM_SIZE);
    
    // Get file status
    struct stat sb;
    if (fstat(shm_fd, &sb) == 0) {
        std::cout << "Shared memory object info:\n";
        std::cout << "  Size: " << sb.st_size << " bytes\n";
        std::cout << "  Mode: " << std::oct << (sb.st_mode & 0777) << std::dec << "\n";
        std::cout << "  UID: " << sb.st_uid << "\n";
        std::cout << "  GID: " << sb.st_gid << "\n";
    }
    
    close(shm_fd);
    shm_unlink(SHM_NAME);
    
    std::cout << "\nView shared memory: ls -l /dev/shm/\n";
    std::cout << "List all: ls /dev/shm/\n";
}

int main() {
    std::cout << "POSIX Shared Memory Demo\n";
    std::cout << "========================\n";
    
    basic_shared_memory();
    shared_memory_with_mutex();
    producer_consumer_shared_memory();
    shared_memory_info();
    
    return 0;
}

/*
 * INTERVIEW Q&A:
 * 
 * Q: Why is shared memory the fastest IPC?
 * A: Zero-copy - data is not copied between processes. Both processes
 *    directly access same physical memory. No kernel involvement after
 *    initial setup (mmap).
 * 
 * Q: How to synchronize shared memory access?
 * A: Use process-shared mutexes (PTHREAD_PROCESS_SHARED), semaphores
 *    (sem_init with pshared=1), or atomic operations. Regular mutexes
 *    don't work across processes.
 * 
 * Q: What happens if process crashes without shm_unlink?
 * A: Shared memory persists! Creates memory leak. Check /dev/shm/.
 *    Must manually remove or reboot to clean up.
 * 
 * Q: Can you share pointers in shared memory?
 * A: NO! Pointers are virtual addresses, different per process.
 *    Use offsets instead: (char*)base + offset
 * 
 * Q: Shared memory vs message passing trade-offs?
 * A: Shared memory: Faster, requires sync, harder to debug
 *    Message passing: Safer, automatic sync, slower (copy overhead)
 */
