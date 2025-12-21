/*
 * INTER-PROCESS COMMUNICATION USING UNNAMED PIPES
 * 
 * Demonstrates unidirectional data flow between parent and child processes.
 * 
 * PIPE FUNDAMENTALS:
 * - Unidirectional: Data flows one way (write end → read end)
 * - FIFO: First In, First Out queue
 * - Byte stream: No message boundaries
 * - Unnamed pipe: Only works between related processes (parent-child)
 * 
 * PIPE CREATION:
 * - pipe(int fd[2]): Creates pipe with two file descriptors
 * - fd[0]: Read end (consumer reads from here)
 * - fd[1]: Write end (producer writes to here)
 * 
 * CRITICAL PATTERN:
 * 1. Parent creates pipe before fork
 * 2. After fork, each process closes unused end
 * 3. Producer closes read end, Consumer closes write end
 * 4. Prevents deadlock and enables EOF detection
 * 
 * WHY CLOSE UNUSED ENDS?
 * - Reader detects EOF only when ALL write ends closed
 * - If process keeps both ends open, reader waits forever
 * - Good practice: close what you don't use
 * 
 * READ/WRITE SEMANTICS:
 * - write(): Blocks if pipe buffer full
 * - read(): Blocks if pipe empty, returns 0 on EOF
 * - read() returns when: data available OR all write ends closed
 * 
 * PIPE vs MESSAGE QUEUE:
 * - Pipe: Byte stream, no boundaries, parent-child only
 * - Message Queue: Messages with boundaries, unrelated processes OK
 * 
 * NAMED PIPES (FIFOs):
 * - Exist in filesystem, unrelated processes can use
 * - Created with mkfifo()
 * 
 * INTERVIEW CONCEPTS:
 * - Why close unused pipe ends
 * - Difference between pipes and sockets
 * - How to achieve bidirectional communication (need 2 pipes)
 * - Pipe buffer size and blocking behavior
 */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

void producer(int pipe_fd) {
  srand(time(NULL));
  for (int i = 0; i < 5; ++i) {
    int item = rand() % 10 + 1;
    std::cout << "Producer: Adding " << item << " to the pipe" << std::endl;
    write(pipe_fd, &item, sizeof(item));
    usleep((rand() % 500000) + 500000);
  }
  close(pipe_fd);
}

void consumer(int pipe_fd) {
  int item;
  ssize_t bytesRead;
  while ((bytesRead = read(pipe_fd, &item, sizeof(item))) > 0) {
    std::cout << "Consumer: Processing " << item << std::endl;
    usleep((rand() % 500000) + 500000);
  }
  close(pipe_fd);
}

int main() {
  // Create pipe before fork (both processes will inherit it)
  // fd[0] = read end, fd[1] = write end
  int pipe_fd[2];

  if (pipe(pipe_fd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    // Child process (Consumer)
    close(pipe_fd[1]);  // Close write end (consumer only reads)
    consumer(pipe_fd[0]);
    exit(EXIT_SUCCESS);
  } else {
    // Parent process (Producer)
    close(pipe_fd[0]);  // Close read end (producer only writes)
    producer(pipe_fd[1]);

    // Wait for the child process to finish
    wait(NULL);  // Reap child to avoid zombie
  }

  return 0;
}
