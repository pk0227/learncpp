/*
 * MEMORY-MAPPED FILES - File I/O via Memory Access
 * 
 * MEMORY MAPPING CONCEPT:
 * - Maps file content directly into process address space
 * - Access file as if it's memory (no read/write system calls)
 * - Kernel handles paging file content in/out
 * - Changes to memory automatically written to file
 * - Multiple processes can map same file for IPC
 * 
 * CHARACTERISTICS:
 * - Fast file I/O (especially random access)
 * - Lazy loading - pages loaded on demand
 * - Kernel manages file caching
 * - Can be used for IPC between processes
 * - Works with regular files (unlike shm_open)
 * - File persists on disk (unlike shared memory)
 * 
 * KEY FUNCTIONS:
 * - mmap(): Map file into memory
 * - munmap(): Unmap file from memory
 * - msync(): Force write of changes to disk
 * - mprotect(): Change protection of mapped region
 * - madvise(): Give kernel advice about usage pattern
 * 
 * mmap() PARAMETERS:
 * - addr: Preferred address (usually nullptr)
 * - length: Number of bytes to map
 * - prot: PROT_READ, PROT_WRITE, PROT_EXEC
 * - flags: MAP_SHARED, MAP_PRIVATE, MAP_ANONYMOUS, etc.
 * - fd: File descriptor
 * - offset: Offset in file (must be page-aligned)
 * 
 * MAPPING TYPES:
 * - MAP_SHARED: Changes visible to other processes, written to file
 * - MAP_PRIVATE: Copy-on-write, changes not written to file
 * - MAP_ANONYMOUS: No file backing, just memory (like malloc)
 * 
 * SYNCHRONIZATION:
 * - msync(MS_SYNC): Synchronous write, blocks until complete
 * - msync(MS_ASYNC): Asynchronous write, returns immediately
 * - msync(MS_INVALIDATE): Invalidate cached copies
 * 
 * PROTECTION:
 * - PROT_READ: Can read
 * - PROT_WRITE: Can write
 * - PROT_EXEC: Can execute
 * - PROT_NONE: No access
 * - Can change with mprotect()
 * 
 * ADVICE (madvise):
 * - MADV_NORMAL: Default behavior
 * - MADV_SEQUENTIAL: Will be accessed sequentially
 * - MADV_RANDOM: Random access pattern
 * - MADV_WILLNEED: Will need soon (prefetch)
 * - MADV_DONTNEED: Won't need anymore (free pages)
 * 
 * USE CASES:
 * - Large file processing
 * - Database files (direct page access)
 * - Shared libraries (code sharing)
 * - IPC via files
 * - Log file analysis
 * - Memory-efficient file I/O
 * 
 * vs REGULAR I/O:
 * - mmap: Better for random access, large files
 * - read/write: Better for sequential, small files
 * - mmap: Simpler code (no buffer management)
 * - read/write: More explicit control
 * 
 * PITFALLS:
 * - File size changes (truncate while mapped)
 * - SIGBUS on I/O errors or accessing beyond file
 * - Not freeing with munmap (resource leak)
 * - Offset not page-aligned
 * - Concurrent writers without sync
 * 
 * INTERVIEW TOPICS:
 * - mmap vs read/write?
 * - MAP_SHARED vs MAP_PRIVATE?
 * - When to use msync?
 * - What causes SIGBUS?
 * - Memory-mapped file benefits?
 * 
 * COMPILE: g++ -std=c++17 07_memory_mapped_files.cpp -o mapped_files
 */

#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <sys/wait.h>

/*
 * EXAMPLE 1: Basic Memory-Mapped File I/O
 */
void basic_mmap_example() {
    std::cout << "\n=== BASIC MEMORY-MAPPED FILE ===\n";
    
    const char* filename = "test_mmap.txt";
    const char* text = "Hello, memory-mapped file!\nThis is line 2.\n";
    
    // Create and write to file first
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("open");
        return;
    }
    
    write(fd, text, strlen(text));
    
    // Get file size
    struct stat sb;
    fstat(fd, &sb);
    off_t file_size = sb.st_size;
    
    std::cout << "Created file: " << filename << " (size: " << file_size << ")\n";
    
    // Map file into memory
    char* mapped = static_cast<char*>(
        mmap(nullptr,           // Let kernel choose address
             file_size,         // Map entire file
             PROT_READ | PROT_WRITE,  // Read and write access
             MAP_SHARED,        // Changes written to file
             fd,
             0)                 // Offset 0 (start of file)
    );
    
    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }
    
    std::cout << "File mapped at: " << static_cast<void*>(mapped) << "\n";
    
    // Read from memory (actually reading file)
    std::cout << "\nReading via memory:\n";
    std::cout.write(mapped, file_size);
    
    // Modify memory (modifies file)
    std::cout << "\nModifying first word...\n";
    memcpy(mapped, "HELLO", 5);
    
    // Force write to disk
    msync(mapped, file_size, MS_SYNC);
    
    // Unmap
    munmap(mapped, file_size);
    close(fd);
    
    // Verify modification by reading file again
    std::cout << "\nVerifying file contents:\n";
    fd = open(filename, O_RDONLY);
    char buffer[256];
    ssize_t n = read(fd, buffer, sizeof(buffer));
    std::cout.write(buffer, n);
    close(fd);
    
    unlink(filename);
}

/*
 * EXAMPLE 2: IPC via Memory-Mapped File
 * Parent and child communicate through mapped file
 */
void mmap_ipc_example() {
    std::cout << "\n=== IPC VIA MEMORY-MAPPED FILE ===\n";
    
    const char* filename = "ipc_mmap.dat";
    
    // Create file
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("open");
        return;
    }
    
    // Set file size
    const size_t size = 4096;
    ftruncate(fd, size);
    
    // Map file with MAP_SHARED (changes visible to other processes)
    char* mapped = static_cast<char*>(
        mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)
    );
    
    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }
    
    std::cout << "Created shared file mapping\n";
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // CHILD: Read data written by parent
        sleep(1);  // Wait for parent to write
        
        std::cout << "Child reads: " << mapped << "\n";
        
        // Write response
        strcpy(mapped + 100, "Response from child");
        msync(mapped + 100, 100, MS_SYNC);
        
        munmap(mapped, size);
        close(fd);
        exit(0);
        
    } else {
        // PARENT: Write data
        strcpy(mapped, "Message from parent");
        msync(mapped, strlen(mapped), MS_SYNC);
        
        std::cout << "Parent wrote: " << mapped << "\n";
        
        wait(nullptr);
        
        // Read child's response
        std::cout << "Parent reads: " << (mapped + 100) << "\n";
        
        munmap(mapped, size);
        close(fd);
        unlink(filename);
    }
}

/*
 * EXAMPLE 3: Large File Processing with mmap
 * Demonstrates advantages for random access
 */
void large_file_processing() {
    std::cout << "\n=== LARGE FILE PROCESSING ===\n";
    
    const char* filename = "large_data.bin";
    const size_t size = 1024 * 1024;  // 1MB
    
    // Create file with data
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("open");
        return;
    }
    
    ftruncate(fd, size);
    
    // Map file
    int* mapped = static_cast<int*>(
        mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)
    );
    
    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }
    
    size_t num_ints = size / sizeof(int);
    
    // Write data (sequential)
    std::cout << "Writing " << num_ints << " integers...\n";
    for (size_t i = 0; i < num_ints; i++) {
        mapped[i] = i;
    }
    
    // Random access - much faster with mmap than read/lseek
    std::cout << "Random access reads:\n";
    size_t positions[] = {1000, 50000, 100000, 200000};
    
    for (size_t pos : positions) {
        std::cout << "  Position " << pos << ": " << mapped[pos] << "\n";
    }
    
    // Memory advice - sequential access pattern
    madvise(mapped, size, MADV_SEQUENTIAL);
    
    // Sum all values (sequential scan)
    long long sum = 0;
    for (size_t i = 0; i < num_ints; i++) {
        sum += mapped[i];
    }
    
    std::cout << "Sum of all values: " << sum << "\n";
    
    munmap(mapped, size);
    close(fd);
    unlink(filename);
}

/*
 * EXAMPLE 4: MAP_PRIVATE vs MAP_SHARED
 */
void private_vs_shared_mapping() {
    std::cout << "\n=== MAP_PRIVATE vs MAP_SHARED ===\n";
    
    const char* filename = "test_mapping.txt";
    const char* original = "Original content";
    
    // Create file
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
    write(fd, original, strlen(original));
    
    struct stat sb;
    fstat(fd, &sb);
    size_t size = sb.st_size;
    
    // MAP_SHARED mapping
    char* shared = static_cast<char*>(
        mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)
    );
    
    // MAP_PRIVATE mapping
    char* private_map = static_cast<char*>(
        mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)
    );
    
    std::cout << "Original file: " << original << "\n";
    
    // Modify shared mapping
    memcpy(shared, "SHARED", 6);
    msync(shared, size, MS_SYNC);
    
    std::cout << "After shared modification:\n";
    std::cout << "  Shared map: " << shared << "\n";
    std::cout << "  Private map: " << private_map << "\n";
    
    // Verify file was modified
    char buffer[256];
    lseek(fd, 0, SEEK_SET);
    read(fd, buffer, size);
    buffer[size] = '\0';
    std::cout << "  File content: " << buffer << "\n";
    
    // Modify private mapping
    memcpy(private_map, "PRIVATE", 7);
    
    std::cout << "\nAfter private modification:\n";
    std::cout << "  Shared map: " << shared << "\n";
    std::cout << "  Private map: " << private_map << "\n";
    
    // Verify file NOT modified by private mapping
    lseek(fd, 0, SEEK_SET);
    read(fd, buffer, size);
    buffer[size] = '\0';
    std::cout << "  File content: " << buffer << " (unchanged)\n";
    
    munmap(shared, size);
    munmap(private_map, size);
    close(fd);
    unlink(filename);
}

/*
 * EXAMPLE 5: Memory Advice with madvise
 */
void madvise_example() {
    std::cout << "\n=== MEMORY ADVICE (madvise) ===\n";
    
    const char* filename = "advice_test.dat";
    const size_t size = 1024 * 1024;  // 1MB
    
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
    ftruncate(fd, size);
    
    char* mapped = static_cast<char*>(
        mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)
    );
    
    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }
    
    // Tell kernel we'll access sequentially
    std::cout << "Advising kernel: MADV_SEQUENTIAL\n";
    madvise(mapped, size, MADV_SEQUENTIAL);
    
    // Sequential write
    for (size_t i = 0; i < size; i++) {
        mapped[i] = i % 256;
    }
    
    // Tell kernel we'll need this data soon
    std::cout << "Advising kernel: MADV_WILLNEED (prefetch)\n";
    madvise(mapped, size, MADV_WILLNEED);
    
    // Now tell kernel we don't need it anymore
    std::cout << "Advising kernel: MADV_DONTNEED (can free pages)\n";
    madvise(mapped, size, MADV_DONTNEED);
    
    munmap(mapped, size);
    close(fd);
    unlink(filename);
    
    std::cout << "Note: madvise is advisory - kernel may ignore it\n";
}

/*
 * EXAMPLE 6: Anonymous Mapping (no file backing)
 */
void anonymous_mapping() {
    std::cout << "\n=== ANONYMOUS MAPPING ===\n";
    
    const size_t size = 4096;
    
    // MAP_ANONYMOUS: No file backing, just memory
    // Similar to malloc but uses page-sized blocks
    int* mapped = static_cast<int*>(
        mmap(nullptr, size,
             PROT_READ | PROT_WRITE,
             MAP_ANONYMOUS | MAP_PRIVATE,  // No file, private
             -1,  // No file descriptor
             0)
    );
    
    if (mapped == MAP_FAILED) {
        perror("mmap");
        return;
    }
    
    std::cout << "Anonymous memory allocated at: " << mapped << "\n";
    std::cout << "Size: " << size << " bytes\n";
    
    // Use like regular memory
    mapped[0] = 42;
    mapped[1] = 100;
    
    std::cout << "Values: " << mapped[0] << ", " << mapped[1] << "\n";
    
    munmap(mapped, size);
    
    std::cout << "Note: Anonymous mmap often used for large allocations\n";
}

int main() {
    std::cout << "Memory-Mapped Files Demo\n";
    std::cout << "========================\n";
    
    basic_mmap_example();
    mmap_ipc_example();
    large_file_processing();
    private_vs_shared_mapping();
    madvise_example();
    anonymous_mapping();
    
    std::cout << "\n=== PAGE SIZE ===\n";
    std::cout << "System page size: " << sysconf(_SC_PAGESIZE) << " bytes\n";
    std::cout << "Note: mmap offset must be page-aligned\n";
    
    return 0;
}

/*
 * INTERVIEW Q&A:
 * 
 * Q: When to use mmap vs read/write?
 * A: mmap: Large files, random access, multiple processes sharing
 *    read/write: Sequential access, small files, more control
 * 
 * Q: What's the difference between MAP_SHARED and MAP_PRIVATE?
 * A: MAP_SHARED: Changes visible to others and written to file
 *    MAP_PRIVATE: Copy-on-write, changes local only, not written
 * 
 * Q: When do you need to call msync?
 * A: When you need guaranteed write to disk. Kernel may delay writes
 *    for performance. msync(MS_SYNC) forces immediate write.
 * 
 * Q: What causes SIGBUS when using mmap?
 * A: Accessing beyond file size, I/O error reading/writing file,
 *    file was truncated while mapped
 * 
 * Q: How does mmap improve performance?
 * A: Zero-copy I/O (no buffer copying), lazy loading (pages on demand),
 *    kernel manages caching, simple random access (no lseek)
 */
