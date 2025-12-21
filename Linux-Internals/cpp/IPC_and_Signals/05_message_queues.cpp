/*
 * POSIX MESSAGE QUEUES - Structured IPC with Priority
 * 
 * MESSAGE QUEUE CONCEPT:
 * - Messages with boundaries (unlike pipes - byte stream)
 * - Priority-based message delivery
 * - Named kernel object (like named pipes)
 * - Messages queued in kernel, not process memory
 * - Can be used by unrelated processes
 * 
 * CHARACTERISTICS:
 * - Each message has priority (0-mq_prio_max, typically 32768)
 * - Messages delivered highest priority first, FIFO within priority
 * - Fixed maximum message size and queue capacity
 * - Atomic message transfer (message boundaries preserved)
 * - Non-blocking and blocking modes
 * - Notification mechanisms (signals, threads)
 * 
 * POSIX vs System V MESSAGE QUEUES:
 * - POSIX: Modern, cleaner API, named like files (/name)
 * - System V: Older, uses numeric keys, more complex API
 * - POSIX: Better notification mechanisms
 * - POSIX: Easier permissions and lifecycle management
 * 
 * KEY FUNCTIONS:
 * - mq_open(): Create/open message queue
 * - mq_send(): Send message with priority (blocks if full)
 * - mq_receive(): Receive highest priority message (blocks if empty)
 * - mq_close(): Close queue descriptor
 * - mq_unlink(): Remove queue from system
 * - mq_getattr/mq_setattr(): Get/set queue attributes
 * - mq_notify(): Request notification when message arrives
 * 
 * QUEUE ATTRIBUTES:
 * - mq_maxmsg: Maximum number of messages in queue
 * - mq_msgsize: Maximum size of each message
 * - mq_curmsgs: Current number of messages
 * - mq_flags: O_NONBLOCK flag
 * 
 * BLOCKING BEHAVIOR:
 * - mq_send() blocks when queue full (unless O_NONBLOCK)
 * - mq_receive() blocks when queue empty (unless O_NONBLOCK)
 * - mq_timedsend/mq_timedreceive for timeout support
 * 
 * PRIORITIES:
 * - Higher number = higher priority
 * - 0 is valid (lowest priority)
 * - Messages with same priority delivered FIFO
 * - Useful for task prioritization, urgent messages
 * 
 * NOTIFICATIONS:
 * - SIGEV_SIGNAL: Send signal when message arrives
 * - SIGEV_THREAD: Create thread when message arrives
 * - SIGEV_NONE: No notification
 * - Only one process can register for notification
 * 
 * USE CASES:
 * - Request queues with priorities
 * - Event notification systems
 * - Task scheduling (high/low priority)
 * - Client-server with structured messages
 * 
 * vs OTHER IPC:
 * - vs Pipe: MQ has message boundaries, priorities
 * - vs Shared Memory: MQ slower but automatic sync, no race conditions
 * - vs Socket: MQ simpler, local only, no connection overhead
 * 
 * INTERVIEW TOPICS:
 * - Message queue vs pipe differences?
 * - How does priority work?
 * - When to use MQ vs shared memory?
 * - What happens if queue is full?
 * - How to implement timeout?
 * 
 * COMPILE: g++ -std=c++17 05_message_queues.cpp -o message_queues -lrt
 * NOTE: -lrt flag links real-time library (required for POSIX MQ)
 */

#include <iostream>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <sys/wait.h>

const char* MQ_NAME = "/my_message_queue";
const int MAX_MSG_SIZE = 256;
const int MAX_MESSAGES = 10;

/*
 * EXAMPLE 1: Basic Send and Receive
 */
void basic_message_queue_example() {
    std::cout << "\n=== BASIC MESSAGE QUEUE EXAMPLE ===\n";
    
    // Remove queue if it exists
    mq_unlink(MQ_NAME);
    
    // Set queue attributes
    struct mq_attr attr;
    attr.mq_flags = 0;                  // Blocking mode
    attr.mq_maxmsg = MAX_MESSAGES;      // Max messages in queue
    attr.mq_msgsize = MAX_MSG_SIZE;     // Max message size
    attr.mq_curmsgs = 0;                // Current messages (ignored for mq_open)
    
    // Create message queue
    // O_CREAT | O_EXCL: Create new, fail if exists
    // 0666: Permissions (modified by umask)
    mqd_t mq = mq_open(MQ_NAME, O_CREAT | O_WRONLY, 0666, &attr);
    
    if (mq == (mqd_t)-1) {
        perror("mq_open sender");
        return;
    }
    
    std::cout << "Message queue created: " << MQ_NAME << "\n";
    std::cout << "  Max messages: " << MAX_MESSAGES << "\n";
    std::cout << "  Max msg size: " << MAX_MSG_SIZE << " bytes\n";
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // CHILD: Receiver
        mq_close(mq);  // Close sender's descriptor
        
        mqd_t mq_recv = mq_open(MQ_NAME, O_RDONLY);
        if (mq_recv == (mqd_t)-1) {
            perror("mq_open receiver");
            exit(1);
        }
        
        char buffer[MAX_MSG_SIZE];
        unsigned int priority;
        
        // Receive message (blocks until message available)
        ssize_t bytes_read = mq_receive(mq_recv, buffer, MAX_MSG_SIZE, &priority);
        
        if (bytes_read >= 0) {
            buffer[bytes_read] = '\0';
            std::cout << "Receiver: Got message (priority " << priority << "): " 
                      << buffer << "\n";
        } else {
            perror("mq_receive");
        }
        
        mq_close(mq_recv);
        exit(0);
        
    } else {
        // PARENT: Sender
        sleep(1);  // Let receiver start
        
        const char* message = "Hello via message queue!";
        unsigned int priority = 5;
        
        // Send message with priority
        if (mq_send(mq, message, strlen(message), priority) == 0) {
            std::cout << "Sender: Sent message (priority " << priority << "): " 
                      << message << "\n";
        } else {
            perror("mq_send");
        }
        
        mq_close(mq);
        wait(nullptr);
        
        // Cleanup
        mq_unlink(MQ_NAME);
        std::cout << "Message queue removed\n";
    }
}

/*
 * EXAMPLE 2: Priority-Based Message Delivery
 */
void priority_queue_example() {
    std::cout << "\n=== PRIORITY QUEUE EXAMPLE ===\n";
    
    mq_unlink(MQ_NAME);
    
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_SIZE;
    
    mqd_t mq = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return;
    }
    
    // Send messages with different priorities
    const char* messages[] = {
        "Low priority task",
        "Medium priority task",
        "HIGH PRIORITY URGENT!",
        "Another low priority"
    };
    
    unsigned int priorities[] = {1, 5, 10, 2};
    
    std::cout << "Sending messages with priorities:\n";
    for (int i = 0; i < 4; i++) {
        mq_send(mq, messages[i], strlen(messages[i]), priorities[i]);
        std::cout << "  Sent (priority " << priorities[i] << "): " 
                  << messages[i] << "\n";
    }
    
    // Receive messages - will come out in priority order
    std::cout << "\nReceiving messages (highest priority first):\n";
    
    char buffer[MAX_MSG_SIZE];
    unsigned int received_priority;
    
    for (int i = 0; i < 4; i++) {
        ssize_t n = mq_receive(mq, buffer, MAX_MSG_SIZE, &received_priority);
        if (n >= 0) {
            buffer[n] = '\0';
            std::cout << "  Received (priority " << received_priority << "): " 
                      << buffer << "\n";
        }
    }
    
    mq_close(mq);
    mq_unlink(MQ_NAME);
}

/*
 * EXAMPLE 3: Non-Blocking Mode and Queue Status
 */
void nonblocking_queue_example() {
    std::cout << "\n=== NON-BLOCKING MODE EXAMPLE ===\n";
    
    mq_unlink(MQ_NAME);
    
    struct mq_attr attr;
    attr.mq_flags = O_NONBLOCK;  // Non-blocking mode
    attr.mq_maxmsg = 3;
    attr.mq_msgsize = MAX_MSG_SIZE;
    
    mqd_t mq = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return;
    }
    
    // Get queue attributes
    struct mq_attr current_attr;
    mq_getattr(mq, &current_attr);
    
    std::cout << "Queue attributes:\n";
    std::cout << "  Max messages: " << current_attr.mq_maxmsg << "\n";
    std::cout << "  Max msg size: " << current_attr.mq_msgsize << "\n";
    std::cout << "  Current msgs: " << current_attr.mq_curmsgs << "\n";
    std::cout << "  Flags: " << (current_attr.mq_flags & O_NONBLOCK ? "O_NONBLOCK" : "Blocking") << "\n";
    
    // Try to receive from empty queue (won't block)
    char buffer[MAX_MSG_SIZE];
    unsigned int priority;
    
    std::cout << "\nTrying to receive from empty queue...\n";
    ssize_t result = mq_receive(mq, buffer, MAX_MSG_SIZE, &priority);
    
    if (result == -1 && errno == EAGAIN) {
        std::cout << "Queue is empty (EAGAIN) - didn't block\n";
    }
    
    // Fill the queue
    std::cout << "\nFilling queue to capacity...\n";
    for (int i = 0; i < current_attr.mq_maxmsg; i++) {
        char msg[50];
        snprintf(msg, sizeof(msg), "Message %d", i);
        if (mq_send(mq, msg, strlen(msg), 0) == 0) {
            std::cout << "  Sent: " << msg << "\n";
        }
    }
    
    // Try to send to full queue (won't block)
    std::cout << "\nTrying to send to full queue...\n";
    result = mq_send(mq, "Extra message", 13, 0);
    
    if (result == -1 && errno == EAGAIN) {
        std::cout << "Queue is full (EAGAIN) - didn't block\n";
    }
    
    mq_close(mq);
    mq_unlink(MQ_NAME);
}

/*
 * EXAMPLE 4: Timeout Operations
 */
void timeout_example() {
    std::cout << "\n=== TIMEOUT EXAMPLE ===\n";
    
    mq_unlink(MQ_NAME);
    
    struct mq_attr attr;
    attr.mq_flags = 0;  // Blocking mode
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_SIZE;
    
    mqd_t mq = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return;
    }
    
    // Set timeout to 2 seconds
    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += 2;  // 2 seconds from now
    
    std::cout << "Trying to receive with 2-second timeout...\n";
    
    char buffer[MAX_MSG_SIZE];
    unsigned int priority;
    
    // This will timeout after 2 seconds (queue is empty)
    ssize_t result = mq_timedreceive(mq, buffer, MAX_MSG_SIZE, &priority, &timeout);
    
    if (result == -1) {
        if (errno == ETIMEDOUT) {
            std::cout << "Operation timed out (ETIMEDOUT)\n";
        } else {
            perror("mq_timedreceive");
        }
    }
    
    mq_close(mq);
    mq_unlink(MQ_NAME);
}

int main() {
    std::cout << "POSIX Message Queues Demo\n";
    std::cout << "=========================\n";
    
    basic_message_queue_example();
    priority_queue_example();
    nonblocking_queue_example();
    timeout_example();
    
    std::cout << "\n=== SYSTEM LIMITS ===\n";
    std::cout << "Check /proc/sys/fs/mqueue/ for system limits\n";
    std::cout << "  msg_max: Maximum messages per queue\n";
    std::cout << "  msgsize_max: Maximum message size\n";
    std::cout << "  queues_max: Maximum number of queues\n";
    
    return 0;
}

/*
 * INTERVIEW Q&A:
 * 
 * Q: Message queue vs pipe - when to use each?
 * A: Pipe: Simple, byte stream, very fast, parent-child
 *    MQ: Message boundaries, priorities, unrelated processes, structured
 * 
 * Q: What happens if queue is full?
 * A: mq_send() blocks (default) or returns EAGAIN (O_NONBLOCK)
 *    Use mq_timedsend() for timeout
 * 
 * Q: How does priority work in message queues?
 * A: Higher number = higher priority. Messages delivered highest
 *    priority first. Within same priority, FIFO order.
 * 
 * Q: Can message queue replace shared memory?
 * A: For small messages, yes. For large data or high frequency,
 *    shared memory is much faster (no copy overhead)
 * 
 * Q: How to handle overflow?
 * A: Use O_NONBLOCK and check EAGAIN, or mq_timedsend with timeout,
 *    or increase queue size (limited by system)
 */
