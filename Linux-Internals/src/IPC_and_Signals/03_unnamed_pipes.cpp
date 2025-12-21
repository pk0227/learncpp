/*
 * UNNAMED PIPES (ANONYMOUS PIPES) - Basic IPC
 * 
 * PIPE FUNDAMENTALS:
 * - Oldest and simplest form of IPC in Unix
 * - Unidirectional: Data flows one way (read end ← write end)
 * - FIFO: First In, First Out byte stream
 * - Kernel buffered: Typical size 64KB (check with ulimit -p)
 * - Created in kernel, no filesystem presence
 * 
 * CHARACTERISTICS:
 * - Half-duplex: One direction only (need 2 pipes for bidirectional)
 * - No message boundaries: Byte stream, not message stream
 * - Blocking I/O: read() blocks if empty, write() blocks if full
 * - Atomic writes: Up to PIPE_BUF bytes (usually 4KB) are atomic
 * - Close-on-exec: Can prevent inheritance across exec()
 * 
 * CREATION:
 * - pipe(int fd[2]): Creates pipe, returns two file descriptors
 * - fd[0]: Read end (pull data from here)
 * - fd[1]: Write end (push data to here)
 * 
 * TYPICAL PATTERN (Parent-Child):
 * 1. Parent calls pipe() before fork()
 * 2. Parent calls fork() - child inherits both ends
 * 3. Close unused ends in each process:
 *    - Writer closes read end (fd[0])
 *    - Reader closes write end (fd[1])
 * 4. Communicate via pipe
 * 5. Both close their ends when done
 * 
 * WHY CLOSE UNUSED ENDS?
 * - read() returns 0 (EOF) only when ALL write ends closed
 * - If process keeps write end open, reader waits forever
 * - Prevents deadlock and enables EOF detection
 * - Good practice: close what you don't use
 * 
 * PIPE CAPACITY:
 * - Limited buffer size (check: cat /proc/sys/fs/pipe-max-size)
 * - Writer blocks when pipe full
 * - Reader blocks when pipe empty
 * - Can set non-blocking mode with fcntl()
 * 
 * ATOMIC WRITES:
 * - Writes ≤ PIPE_BUF bytes are atomic (not interleaved)
 * - PIPE_BUF typically 4096 bytes
 * - Larger writes may be split, interleaved with other writers
 * - Important for multiple writers to same pipe
 * 
 * BROKEN PIPE:
 * - Writing to pipe with no readers → SIGPIPE signal
 * - Default action: Terminate process
 * - write() returns -1, errno = EPIPE
 * - Ignore SIGPIPE or handle to prevent crash
 * 
 * LIMITATIONS:
 * - Only related processes (parent-child, siblings)
 * - One direction only (need 2 for bidirectional)
 * - No message boundaries (byte stream)
 * - Limited buffer (can fill up)
 * 
 * WHEN TO USE:
 * - Parent-child communication
 * - Shell pipelines (cmd1 | cmd2)
 * - Simple producer-consumer
 * - Redirecting stdout/stderr
 * 
 * INTERVIEW TOPICS:
 * - How does shell pipeline work? (fork + pipe + dup2)
 * - Why close unused pipe ends?
 * - What happens on broken pipe?
 * - Pipe vs named pipe vs socket?
 * - How to make bidirectional communication?
 */

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <cerrno>

/*
 * EXAMPLE 1: Simple Parent-Child Communication
 */
void simple_pipe_example() {
    std::cout << "\n=== SIMPLE PIPE EXAMPLE ===\n";
    
    int pipefd[2];  // File descriptors: [0] = read, [1] = write
    
    // Create pipe BEFORE fork (both processes will inherit it)
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }
    
    std::cout << "Pipe created: read_fd=" << pipefd[0] 
              << ", write_fd=" << pipefd[1] << "\n";
    
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        // CHILD PROCESS (Reader)
        close(pipefd[1]);  // Close write end (child only reads)
        
        char buffer[100];
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
        
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';  // Null-terminate
            std::cout << "Child received: " << buffer << "\n";
            std::cout << "Child received " << bytes_read << " bytes\n";
        } else if (bytes_read == 0) {
            std::cout << "Child: EOF (all write ends closed)\n";
        } else {
            perror("read");
        }
        
        close(pipefd[0]);  // Close read end
        exit(0);
        
    } else {
        // PARENT PROCESS (Writer)
        close(pipefd[0]);  // Close read end (parent only writes)
        
        const char* message = "Hello from parent via pipe!";
        ssize_t bytes_written = write(pipefd[1], message, strlen(message));
        
        if (bytes_written > 0) {
            std::cout << "Parent sent: " << message << "\n";
            std::cout << "Parent sent " << bytes_written << " bytes\n";
        } else {
            perror("write");
        }
        
        close(pipefd[1]);  // Close write end (triggers EOF for reader)
        
        // Wait for child to finish
        wait(nullptr);
        std::cout << "Parent: Child finished\n";
    }
}

/*
 * EXAMPLE 2: Bidirectional Communication (Two Pipes)
 */
void bidirectional_pipe_example() {
    std::cout << "\n=== BIDIRECTIONAL PIPE EXAMPLE ===\n";
    
    int pipe_parent_to_child[2];  // Parent writes, child reads
    int pipe_child_to_parent[2];  // Child writes, parent reads
    
    if (pipe(pipe_parent_to_child) == -1 || pipe(pipe_child_to_parent) == -1) {
        perror("pipe");
        return;
    }
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // CHILD PROCESS
        // Close unused ends
        close(pipe_parent_to_child[1]);  // Won't write to parent→child pipe
        close(pipe_child_to_parent[0]);  // Won't read from child→parent pipe
        
        // Read from parent
        char buffer[100];
        ssize_t n = read(pipe_parent_to_child[0], buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            std::cout << "Child received: " << buffer << "\n";
        }
        
        // Send response to parent
        const char* response = "ACK from child";
        write(pipe_child_to_parent[1], response, strlen(response));
        
        // Cleanup
        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);
        exit(0);
        
    } else {
        // PARENT PROCESS
        // Close unused ends
        close(pipe_parent_to_child[0]);  // Won't read from parent→child pipe
        close(pipe_child_to_parent[1]);  // Won't write to child→parent pipe
        
        // Send to child
        const char* request = "Request from parent";
        write(pipe_parent_to_child[1], request, strlen(request));
        std::cout << "Parent sent: " << request << "\n";
        
        // Read response from child
        char buffer[100];
        ssize_t n = read(pipe_child_to_parent[0], buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            std::cout << "Parent received: " << buffer << "\n";
        }
        
        // Cleanup
        close(pipe_parent_to_child[1]);
        close(pipe_child_to_parent[0]);
        
        wait(nullptr);
    }
}

/*
 * EXAMPLE 3: Redirecting Output (Implementing Simple Shell Pipeline)
 */
void pipe_redirect_example() {
    std::cout << "\n=== PIPE REDIRECT EXAMPLE (ls | wc) ===\n";
    
    int pipefd[2];
    pipe(pipefd);
    
    pid_t pid1 = fork();
    
    if (pid1 == 0) {
        // FIRST CHILD: Execute 'ls' with stdout redirected to pipe
        close(pipefd[0]);              // Close read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe write end
        close(pipefd[1]);              // Close original fd (now have stdout)
        
        execlp("ls", "ls", "-l", nullptr);
        perror("execlp ls");
        exit(1);
    }
    
    pid_t pid2 = fork();
    
    if (pid2 == 0) {
        // SECOND CHILD: Execute 'wc' with stdin from pipe
        close(pipefd[1]);             // Close write end
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe read end
        close(pipefd[0]);             // Close original fd (now have stdin)
        
        execlp("wc", "wc", "-l", nullptr);
        perror("execlp wc");
        exit(1);
    }
    
    // PARENT: Close both ends and wait for children
    close(pipefd[0]);
    close(pipefd[1]);
    
    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
}

/*
 * EXAMPLE 4: Handling SIGPIPE (Broken Pipe)
 */
void broken_pipe_example() {
    std::cout << "\n=== BROKEN PIPE HANDLING ===\n";
    
    // Ignore SIGPIPE (otherwise process terminates on broken pipe)
    signal(SIGPIPE, SIG_IGN);
    
    int pipefd[2];
    pipe(pipefd);
    
    // Close read end immediately (no readers!)
    close(pipefd[0]);
    
    // Try to write (will fail with EPIPE)
    const char* msg = "This will fail";
    ssize_t result = write(pipefd[1], msg, strlen(msg));
    
    if (result == -1) {
        if (errno == EPIPE) {
            std::cout << "Caught broken pipe (EPIPE) - no readers\n";
        } else {
            perror("write");
        }
    }
    
    close(pipefd[1]);
}

int main() {
    std::cout << "Unnamed Pipes (Anonymous Pipes) Demo\n";
    std::cout << "====================================\n";
    
    simple_pipe_example();
    bidirectional_pipe_example();
    pipe_redirect_example();
    broken_pipe_example();
    
    std::cout << "\n=== PIPE PROPERTIES ===\n";
    std::cout << "PIPE_BUF (atomic write size): " << PIPE_BUF << " bytes\n";
    
    return 0;
}

/*
 * COMPILE: g++ -std=c++17 03_unnamed_pipes.cpp -o unnamed_pipes
 * 
 * INTERVIEW Q&A:
 * 
 * Q: How does "ls | grep txt" work in shell?
 * A: Shell creates pipe, forks two children. First child redirects
 *    stdout to pipe write end and execs ls. Second child redirects
 *    stdin to pipe read end and execs grep.
 * 
 * Q: What happens if you forget to close write end in parent?
 * A: Child's read() never returns 0 (EOF), waits forever.
 *    All write ends must be closed for reader to see EOF.
 * 
 * Q: Can multiple processes write to same pipe?
 * A: Yes, but writes > PIPE_BUF may interleave. Keep writes small
 *    (≤ PIPE_BUF) for atomicity.
 * 
 * Q: Pipe vs socket for IPC?
 * A: Pipe: Simpler, parent-child only, one direction
 *    Socket: More complex, any processes, bidirectional, network-capable
 */
