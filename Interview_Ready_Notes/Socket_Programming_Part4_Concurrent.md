# Internetworking with Linux: Socket Programming Part 4 (Concurrent Servers)
**Source:** [Lec37 Socket Programming Part-IV (Arif Butt @ PUCIT)](https://www.youtube.com/watch?v=irRkNrruwxc)
**Instructor:** Arif Butt
**Subject:** System Programming with Linux

> **Note for the Learner:** This is the **Final** and most advanced part of the Socket Programming series. It covers the transition from "Iterative" servers (which block efficiently) to "Concurrent" servers using **Fork**, **Threads**, and **I/O Multiplexing (Select)**. All code examples are fully reconstructed below.

---

## 1. The Limitation of Iterative Servers
*(Transcript 1:00 - 9:00)*

An **Iterative Server** handles one client at a time.
*   **The Problem:** If Client A connects and transmits data for 5 minutes, Client B, C, and D are stuck in the **Listen Queue**.
*   **Demonstration:**
    *   The instructor runs the `tcp_echo_server` (Iterative).
    *   Client 1 (`hello world`) works.
    *   Client 2 connects... but **hangs**. It sends no response.
    *   Client 3 connects... hangs.
    *   Client 1 terminates (Ctrl+C). Client 2 *immediately* wakes up and gets served.
*   **Conclusion:** Iterative servers are useless for long-running sessions (SSH, Telnet, FTP).

---

## 2. Concurrency Model 1: Multi-Process (Fork)
*(Transcript 12:40 - 23:20)*

The server `fork()`s a child process for every incoming connection.
*   **Parent:** Closes the *Data Socket*. Continues `accept()`ing new clients.
*   **Child:** Closes the *Master Socket*. Handles the client communication. Exits when done.

### The Zombie Process Issue
When a child process finishes, it becomes a **Zombie (Defunct)** until the parent calls `wait()`.
*   **Instructor Demo:** He runs the forked server, connects 4 clients, and disconnects them. `ps` shows 4 `<defunct>` processes.
*   **Silver Bullet Failure:** Even `kill -9` cannot kill a Zombie. You must kill the parent to clean them up.
*   **Solution:** Use a Signal Handler for `SIGCHLD` to reap zombies asynchronously.

### Code: Concurrent Echo Server (Fork + Signal Handling)

```c
/* tcp_server_fork.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>

void reaper(int sig) {
    // Wait for any child, do not block (WNOHANG)
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
    int master_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    char buf[1024];
    int n;

    // 1. Register Signal Handler for Zombies
    signal(SIGCHLD, reaper);

    // 2. Create Socket
    master_sock = socket(AF_INET, SOCK_STREAM, 0);
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(54154);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(master_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(master_sock, 10);

    while(1) {
        len = sizeof(client_addr);
        client_sock = accept(master_sock, (struct sockaddr*)&client_addr, &len);
        
        // 3. Fork
        if (fork() == 0) {
            // == CHILD PROCESS ==
            close(master_sock); // Child doesn't need listener

            while((n = read(client_sock, buf, 1024)) > 0) {
                write(client_sock, buf, n); // Echo
            }
            close(client_sock);
            exit(0); // Child Must Exit!
        }
        else {
            // == PARENT PROCESS ==
            close(client_sock); // Parent doesn't need data socket
        }
    }
    return 0;
}
```

---

## 3. Concurrency Model 2: Multi-Threaded
*(Transcript 23:30 - 27:50)*

Uses `pthread_create` instead of `fork`.
*   **Pros:** Lighter weight than processes. Shared memory.
*   **Cons:** One thread crashing can take down the whole server.
*   **Requirement:** Must use **Detached Threads** (`pthread_detach`) so their resources are auto-released on exit (no "joining" needed).
*   **Compile:** `gcc server.c -o server -lpthread`

### Code: Concurrent Echo Server (Threaded)

```c
/* tcp_server_thread.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

void* echo_handler(void* arg) {
    int sock = *(int*)arg;
    free(arg); // Free the malloc'd pointer
    char buf[1024];
    int n;

    pthread_detach(pthread_self()); // Auto-reap on exit

    while((n = read(sock, buf, 1024)) > 0) {
        write(sock, buf, n);
    }
    close(sock);
    return NULL;
}

int main() {
    int master_sock, *new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    
    master_sock = socket(AF_INET, SOCK_STREAM, 0);
    // ... bind and listen code same as above ...
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(54154);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bind(master_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(master_sock, 10);

    while(1) {
        len = sizeof(client_addr);
        int client = accept(master_sock, (struct sockaddr*)&client_addr, &len);
        
        pthread_t tid;
        new_sock = malloc(sizeof(int)); // Heap alloc to avoid race condition
        *new_sock = client;
        
        pthread_create(&tid, NULL, echo_handler, (void*)new_sock);
    }
    return 0;
}
```

---

## 4. Concurrency Model 3: I/O Multiplexing (Select)
*(Transcript 28:00 - 37:50)*

Handling multiple clients effectively in a **Single Process** by using `select()`.
*   **Problem:** If you `read()` from Client A, you block. If Client B sends data meanwhile, you miss it.
*   **Solution:** `select()` monitors a *set* of File Descriptors (FDs) and wakes you up only when one is **Ready** (has data).

**The FD Macros:**
*   `FD_ZERO(&set)`: Clear set.
*   `FD_SET(fd, &set)`: Add FD to set.
*   `FD_CLR(fd, &set)`: Remove FD.
*   `FD_ISSET(fd, &set)`: Check if FD is ready.

### Code: Concurrent Echo Server (Select)

```c
/* tcp_server_select.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int master_sock, client_sock, max_fd, i;
    struct sockaddr_in server_addr, client_addr;
    fd_set read_fds, active_fds; // The sets of FDs
    socklen_t len;
    char buf[1024];

    master_sock = socket(AF_INET, SOCK_STREAM, 0);
    // ... bind/listen ...
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(54154);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bind(master_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(master_sock, 10);

    FD_ZERO(&active_fds);
    FD_SET(master_sock, &active_fds); // Add listener to set
    max_fd = master_sock;

    while(1) {
        read_fds = active_fds; // Copy because select is destructive

        // Block until SOMETHING happens
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("select"); exit(1);
        }

        // Loop through all possible FDs
        for (i = 0; i <= max_fd; i++) {
            if (FD_ISSET(i, &read_fds)) {
                
                // Case 1: Master Socket is Ready -> New Connection
                if (i == master_sock) {
                    len = sizeof(client_addr);
                    client_sock = accept(master_sock, (struct sockaddr*)&client_addr, &len);
                    FD_SET(client_sock, &active_fds); // Add new client to set
                    if (client_sock > max_fd) max_fd = client_sock;
                    printf("New Client: %d\n", client_sock);
                }
                
                // Case 2: Data Socket is Ready -> Data Arrival
                else {
                    int n = read(i, buf, 1024);
                    if (n <= 0) {
                        close(i);
                        FD_CLR(i, &active_fds); // Remove from set
                    } else {
                        write(i, buf, n); // Echo
                    }
                }
            }
        }
    }
    return 0;
}
```

---

## 5. Summary and Assignment
*(Transcript 39:00 - End)*

*   **Concurrency is mandatory** for real-world servers (SSH, Web, etc).
*   **Assignment:** Enhance your `netcat` clone (from Lec 35) to be concurrent (using `select` is best for clients that need to listen to both Keyboard (Stdin) and Socket simultaneously).
*   **Next Topic:** Buffer Overflow Exploits (Security).

**End of Socket Programming Series.**
