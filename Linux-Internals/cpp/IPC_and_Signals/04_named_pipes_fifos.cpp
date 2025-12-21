/*
 * NAMED PIPES (FIFOs) - Inter-Process Communication
 * 
 * FIFO vs UNNAMED PIPE:
 * - FIFO: Has filesystem path, unrelated processes can communicate
 * - Unnamed pipe: No path, only parent-child can communicate
 * - Both: Unidirectional, byte stream, kernel buffered
 * 
 * CHARACTERISTICS:
 * - Appears as special file in filesystem (created with mkfifo())
 * - Same operations as unnamed pipe: read(), write(), close()
 * - Opened like regular file with open()
 * - Deleted with unlink() when no longer needed
 * - Persists until explicitly removed
 * 
 * BLOCKING BEHAVIOR:
 * - open() for reading blocks until writer opens (unless O_NONBLOCK)
 * - open() for writing blocks until reader opens (unless O_NONBLOCK)
 * - This ensures both ends are ready before communication
 * - read() and write() block as with unnamed pipes
 * 
 * CREATION:
 * - mkfifo(path, mode): Create FIFO with permissions
 * - Command line: mkfifo /tmp/myfifo
 * - Returns 0 on success, -1 if exists or error
 * 
 * PERMISSIONS:
 * - Like regular files: owner, group, others
 * - Typical: 0666 (rw-rw-rw-) modified by umask
 * - Must have write permission to write, read permission to read
 * 
 * USE CASES:
 * - Client-server local communication
 * - One-way data pipelines between unrelated processes
 * - Named services (multiple clients, one server)
 * - Shell scripts coordinating programs
 * 
 * LIMITATIONS:
 * - Half-duplex (one direction only, need 2 FIFOs for bidirectional)
 * - Local machine only (not over network)
 * - No message boundaries (byte stream)
 * - Can have only one writer at a time for atomic writes
 * 
 * vs OTHER IPC:
 * - vs Sockets: FIFO simpler but local only, no bidirectional in one file
 * - vs Shared Memory: FIFO slower but simpler, automatic sync
 * - vs Message Queue: FIFO no message boundaries, simpler API
 * - vs Unnamed Pipe: FIFO works with unrelated processes
 * 
 * INTERVIEW TOPICS:
 * - Named pipe vs unnamed pipe?
 * - Why does open() block on FIFOs?
 * - How to implement request-response with FIFOs?
 * - When would you use FIFO vs socket?
 * - Can multiple processes read from one FIFO?
 */

#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <sys/wait.h>
#include <chrono>
#include <thread>

const char* FIFO_PATH = "/tmp/my_fifo_demo";
const char* REQUEST_FIFO = "/tmp/request_fifo";
const char* RESPONSE_FIFO = "/tmp/response_fifo";

/*
 * EXAMPLE 1: Simple Writer-Reader using FIFO
 */
void simple_fifo_example() {
    std::cout << "\n=== SIMPLE FIFO EXAMPLE ===\n";
    
    // Remove FIFO if it exists
    unlink(FIFO_PATH);
    
    // Create FIFO with permissions rw-rw-rw- (modified by umask)
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        perror("mkfifo");
        return;
    }
    
    std::cout << "FIFO created at: " << FIFO_PATH << "\n";
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // CHILD: Reader process
        std::cout << "Reader: Opening FIFO (will block until writer opens)...\n";
        
        int fd = open(FIFO_PATH, O_RDONLY);  // Blocks until writer opens
        if (fd == -1) {
            perror("open read");
            exit(1);
        }
        
        std::cout << "Reader: FIFO opened\n";
        
        char buffer[100];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            std::cout << "Reader: Received: " << buffer << "\n";
        }
        
        close(fd);
        exit(0);
        
    } else {
        // PARENT: Writer process
        sleep(2);  // Let reader open first (for demonstration)
        
        std::cout << "Writer: Opening FIFO...\n";
        
        int fd = open(FIFO_PATH, O_WRONLY);  // Blocks until reader opens
        if (fd == -1) {
            perror("open write");
            return;
        }
        
        std::cout << "Writer: FIFO opened\n";
        
        const char* message = "Hello via FIFO!";
        write(fd, message, strlen(message));
        std::cout << "Writer: Sent: " << message << "\n";
        
        close(fd);
        wait(nullptr);
        
        // Cleanup: Remove FIFO
        unlink(FIFO_PATH);
        std::cout << "FIFO removed\n";
    }
}

/*
 * EXAMPLE 2: Non-blocking FIFO
 */
void nonblocking_fifo_example() {
    std::cout << "\n=== NON-BLOCKING FIFO EXAMPLE ===\n";
    
    unlink(FIFO_PATH);
    mkfifo(FIFO_PATH, 0666);
    
    std::cout << "Opening FIFO in non-blocking mode...\n";
    
    // O_NONBLOCK: Don't block on open or I/O operations
    int fd = open(FIFO_PATH, O_RDONLY | O_NONBLOCK);
    
    if (fd == -1) {
        perror("open");
        unlink(FIFO_PATH);
        return;
    }
    
    std::cout << "FIFO opened (didn't wait for writer)\n";
    
    // Try to read (will return immediately if no data)
    char buffer[100];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
    
    if (bytes_read == -1 && errno == EAGAIN) {
        std::cout << "No data available (EAGAIN) - as expected\n";
    }
    
    close(fd);
    unlink(FIFO_PATH);
}

/*
 * EXAMPLE 3: Bidirectional Request-Response
 * Uses two FIFOs for full-duplex communication
 */
void server_process() {
    std::cout << "Server: Starting...\n";
    
    // Server reads requests and writes responses
    int req_fd = open(REQUEST_FIFO, O_RDONLY);
    int resp_fd = open(RESPONSE_FIFO, O_WRONLY);
    
    if (req_fd == -1 || resp_fd == -1) {
        perror("server open");
        return;
    }
    
    std::cout << "Server: FIFOs opened\n";
    
    // Read request
    char buffer[100];
    ssize_t n = read(req_fd, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0';
        std::cout << "Server: Received request: " << buffer << "\n";
        
        // Process request and send response
        const char* response = "Response: Request processed";
        write(resp_fd, response, strlen(response));
        std::cout << "Server: Sent response\n";
    }
    
    close(req_fd);
    close(resp_fd);
}

void client_process() {
    std::cout << "Client: Starting...\n";
    
    sleep(1);  // Let server start first
    
    // Client writes requests and reads responses
    int req_fd = open(REQUEST_FIFO, O_WRONLY);
    int resp_fd = open(RESPONSE_FIFO, O_RDONLY);
    
    if (req_fd == -1 || resp_fd == -1) {
        perror("client open");
        return;
    }
    
    std::cout << "Client: FIFOs opened\n";
    
    // Send request
    const char* request = "Client request data";
    write(req_fd, request, strlen(request));
    std::cout << "Client: Sent request: " << request << "\n";
    
    // Read response
    char buffer[100];
    ssize_t n = read(resp_fd, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0';
        std::cout << "Client: Received: " << buffer << "\n";
    }
    
    close(req_fd);
    close(resp_fd);
}

void request_response_example() {
    std::cout << "\n=== REQUEST-RESPONSE EXAMPLE ===\n";
    
    // Cleanup old FIFOs
    unlink(REQUEST_FIFO);
    unlink(RESPONSE_FIFO);
    
    // Create two FIFOs for bidirectional communication
    if (mkfifo(REQUEST_FIFO, 0666) == -1 || mkfifo(RESPONSE_FIFO, 0666) == -1) {
        perror("mkfifo");
        return;
    }
    
    std::cout << "FIFOs created:\n";
    std::cout << "  Request:  " << REQUEST_FIFO << "\n";
    std::cout << "  Response: " << RESPONSE_FIFO << "\n";
    
    pid_t pid = fork();
    
    if (pid == 0) {
        client_process();
        exit(0);
    } else {
        server_process();
        wait(nullptr);
    }
    
    // Cleanup
    unlink(REQUEST_FIFO);
    unlink(RESPONSE_FIFO);
    std::cout << "FIFOs removed\n";
}

/*
 * EXAMPLE 4: Multiple Writers (demonstrate race condition)
 */
void multiple_writers_example() {
    std::cout << "\n=== MULTIPLE WRITERS EXAMPLE ===\n";
    
    unlink(FIFO_PATH);
    mkfifo(FIFO_PATH, 0666);
    
    pid_t reader_pid = fork();
    
    if (reader_pid == 0) {
        // READER
        int fd = open(FIFO_PATH, O_RDONLY);
        char buffer[100];
        
        for (int i = 0; i < 3; i++) {
            ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
            if (n > 0) {
                buffer[n] = '\0';
                std::cout << "Reader: " << buffer << "\n";
            }
        }
        
        close(fd);
        exit(0);
    }
    
    sleep(1);
    
    // Create multiple writers
    for (int i = 0; i < 3; i++) {
        pid_t writer_pid = fork();
        
        if (writer_pid == 0) {
            int fd = open(FIFO_PATH, O_WRONLY);
            
            char msg[50];
            snprintf(msg, sizeof(msg), "Message from writer %d", i);
            write(fd, msg, strlen(msg));
            
            close(fd);
            exit(0);
        }
    }
    
    // Wait for all children
    for (int i = 0; i < 4; i++) {
        wait(nullptr);
    }
    
    unlink(FIFO_PATH);
}

int main() {
    std::cout << "Named Pipes (FIFOs) Demo\n";
    std::cout << "========================\n";
    
    simple_fifo_example();
    nonblocking_fifo_example();
    request_response_example();
    multiple_writers_example();
    
    return 0;
}

/*
 * COMPILE: g++ -std=c++17 04_named_pipes_fifos.cpp -o named_pipes
 * 
 * MANUAL TESTING:
 * 
 * Terminal 1 (Reader):
 *   mkfifo /tmp/test_fifo
 *   cat < /tmp/test_fifo
 * 
 * Terminal 2 (Writer):
 *   echo "Hello FIFO" > /tmp/test_fifo
 * 
 * Cleanup:
 *   rm /tmp/test_fifo
 * 
 * INTERVIEW Q&A:
 * 
 * Q: Why does open() block on FIFOs?
 * A: Ensures both reader and writer are ready. Prevents writer from
 *    getting SIGPIPE if no reader, and reader from getting EOF
 *    immediately. Use O_NONBLOCK to avoid blocking.
 * 
 * Q: Can you have multiple readers on one FIFO?
 * A: Yes, but data distribution is undefined (not round-robin).
 *    One reader might get all data. Not recommended.
 * 
 * Q: Named pipe vs Unix domain socket?
 * A: FIFO: Simpler, one direction, byte stream
 *    Socket: Bidirectional, connection-oriented or datagram,
 *    more features (credentials, ancillary data)
 * 
 * Q: How to implement persistent service with FIFOs?
 * A: Server opens FIFO, processes requests in loop. Multiple
 *    clients can connect. Use two FIFOs for request/response,
 *    or embed client PID in request for response routing.
 */
