/*
 * POSIX MESSAGE QUEUES FOR INTER-PROCESS COMMUNICATION
 * 
 * Demonstrates structured message passing between processes using POSIX mqueues.
 * 
 * MESSAGE QUEUE ADVANTAGES:
 * - Message boundaries preserved (unlike pipes)
 * - FIFO ordering guaranteed
 * - Can be accessed by unrelated processes (named resource)
 * - Priority-based message delivery supported
 * - Non-blocking and blocking modes available
 * 
 * POSIX MESSAGE QUEUE API:
 * - mq_open(): Create/open message queue
 * - mq_send(): Send message (blocks if queue full)
 * - mq_receive(): Receive message (blocks if queue empty)
 * - mq_close(): Close queue descriptor
 * - mq_unlink(): Remove queue from system
 * 
 * MESSAGE QUEUE vs PIPE:
 * - Message Queue: Preserved boundaries, structured, priority support
 * - Pipe: Byte stream, no boundaries, sequential only
 * - Message Queue: Can overflow (limited size)
 * - Pipe: Kernel buffering, simpler
 * 
 * PRODUCER-CONSUMER PATTERN:
 * - Producer: Generates data, sends to queue
 * - Consumer: Receives from queue, processes data
 * - Decoupled: Can run at different rates
 * - Buffering smooths out rate differences
 * 
 * INTERVIEW POINTS:
 * - When to use message queue vs shared memory
 * - Message queue vs socket differences
 * - How to handle queue overflow
 * - Priority-based message queues
 * 
 * COMPILE: May need to link with -lrt on some systems
 */

#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

const char *MQ_NAME = "/my_message_queue";

void producer() {
  srand(time(NULL));

  mqd_t mq = mq_open(MQ_NAME, O_WRONLY | O_CREAT, 0666, nullptr);
  if (mq == (mqd_t)-1) {
    perror("mq_open");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < 5; ++i) {
    int item = rand() % 10 + 1;
    std::cout << "Producer: Adding " << item << " to the message queue"
              << std::endl;
    mq_send(mq, reinterpret_cast<char *>(&item), sizeof(item), 0);
    usleep((rand() % 500000) + 500000);
  }

  mq_close(mq);
}

void consumer() {
  mqd_t mq = mq_open(MQ_NAME, O_RDONLY);
  if (mq == (mqd_t)-1) {
    perror("mq_open");
    exit(EXIT_FAILURE);
  }

  int item;
  ssize_t bytesRead;
  while ((bytesRead = mq_receive(mq, reinterpret_cast<char *>(&item),
                                 sizeof(item), nullptr)) > 0) {
    std::cout << "Consumer: Processing " << item << std::endl;
    usleep((rand() % 500000) + 500000);
  }

  mq_close(mq);
}

int main() {
  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    // Child process (Consumer)
    consumer();
    exit(EXIT_SUCCESS);
  } else {
    // Parent process (Producer)
    producer();

    // Wait for the child process to finish
    wait(NULL);
  }

  // Cleanup
  mq_unlink(MQ_NAME);

  return 0;
}

// g++ -o main main.cpp -lrt
