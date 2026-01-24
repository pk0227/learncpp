# Internetworking with Linux: Socket Programming Part 3 (UNIX Domain)
**Source:** [Lec36 Socket Programming Part-III (Arif Butt @ PUCIT)](https://www.youtube.com/watch?v=TDRIweWXHe4)
**Instructor:** Arif Butt
**Subject:** System Programming with Linux

> **Note for the Learner:** This document is the **Zero-Loss** companion to Lecture 36. It covers the shift from *Internet Domain* to *UNIX Domain* sockets, which are used for high-performance IPC (Inter-Process Communication) on the **same machine**. It includes full C code for both Stream (TCP-like) and Datagram (UDP-like) UNIX sockets.

---

## 1. Introduction: Internet vs UNIX Domain
*(Transcript 1:40 - 2:20)*

The instructor explains the fundamental difference between the domains.
*   **Internet Domain (`AF_INET`):** For communicating between processes on **Different Machines** (or same machines via loopback stack). Identifiers: IP Address + Port.
*   **UNIX Domain (`AF_UNIX` / `AF_LOCAL`):** For communicating between processes on the **Same Machine**.
    *   **Speed:** Twice as fast as TCP/IP loopback because it avoids the overhead of the network stack (Checksums, headers, routing logic).
    *   **Identifier:** A **File Path** (e.g., `/tmp/mysocket`) in the file system.
    *   **Security:** Leverages standard Linux file permissions (Read/Write) to control who can connect.

---

## 2. UNIX Domain Architecture & System Calls
*(Transcript 3:20 - 5:00)*

### Creating the Socket
The system calls are identical to Internet sockets, but the arguments change.
*   **Socket Call:** `socket(AF_UNIX, SOCK_STREAM, 0)` or `socket(AF_UNIX, SOCK_DGRAM, 0)`.
*   **Address Structure:**
    Instead of `sockaddr_in`, we use `sockaddr_un`.
    ```c
    #include <sys/un.h>
    struct sockaddr_un {
        sa_family_t sun_family; // Always AF_UNIX
        char sun_path[108];     // Path name (e.g., "./socket_file")
    };
    ```

### The "File" Side Effect
When you call `bind()` on a UNIX socket:
1.  A special file (Type `s` in `ls -l`) is created on the disk at `sun_path`.
2.  **Critical Rule:** This file must **NOT** exist before calling `bind()`, or bind will fail with "Address already in use".
3.  **Cleanup:** You must manually `unlink()` (delete) this file when the server exits.

---

## 3. Comparison with Named Pipes (FIFO)
*(Transcript 5:46 - 7:00)*

Since both use "files" for IPC, why not just use Named Pipes?

| Feature | UNIX Domain Sockets | Named Pipes (FIFO) |
| :--- | :--- | :--- |
| **Communication** | Bi-Directional (Stream) or Uni-Directional (Dgram) | Uni-Directional Only |
| **Concurrency** | **Distinguishes Clients.** `accept()` returns a unique FD for *each* client connection. | **Mixed Data.** Server reads from one FD. If multiple clients write, data intermingles. |
| **Connection** | Connection-Oriented (Stream) | Connection-less (Open/Read/Write) |

**Conclusion:** Use UNIX Domain Sockets when you have a Server handling **multiple independent clients**.

---

## 4. Lab 1: UNIX Domain Stream (TCP-like) 
*(Transcript 8:00 - 12:15)*

This setup mimics the TCP Echo Server but uses local files.

### A. The Server (Stream)
Note the use of `unlink()` before binding.

```c
/* unix_stream_server.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "./demo_socket"

int main() {
    int master_sock, client_sock;
    struct sockaddr_un server_addr, client_addr;
    socklen_t len;
    char buf[1024];
    int n;

    // 1. Create Socket (AF_UNIX)
    master_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (master_sock < 0) { perror("socket"); exit(1); }

    // 2. Prepare Address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // 3. Unlink (Delete) existing file to prevent Bind Error
    unlink(SOCKET_PATH);

    // 4. Bind
    if (bind(master_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind"); exit(1);
    }

    // 5. Listen & Accept
    listen(master_sock, 10);
    printf("Server listening on %s...\n", SOCKET_PATH);

    while(1) {
        len = sizeof(client_addr);
        client_sock = accept(master_sock, (struct sockaddr*)&client_addr, &len);
        printf("Client Connected!\n");

        while((n = read(client_sock, buf, sizeof(buf))) > 0) {
             write(client_sock, buf, n); // Echo back
        }
        close(client_sock);
        printf("Client Disconnected.\n");
    }
    
    // Unreachable in this loop, but good practice
    close(master_sock);
    unlink(SOCKET_PATH);
    return 0;
}
```

### B. The Client (Stream)

```c
/* unix_stream_client.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "./demo_socket"

int main() {
    int sockfd;
    struct sockaddr_un addr;
    char buf[1024];
    int n;

    // 1. Create Socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    // 2. Connect
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect"); exit(1);
    }
    printf("Connected to Server.\n");

    // 3. Echo Loop
    while(1) {
        printf("Enter message: ");
        fgets(buf, 1024, stdin);
        write(sockfd, buf, strlen(buf));
        
        n = read(sockfd, buf, 1024);
        buf[n] = '\0';
        printf("Echo: %s", buf);
    }
    close(sockfd);
    return 0;
}
```

---

## 5. Lab 2: UNIX Domain Datagram (UDP-like)
*(Transcript 12:40 - 15:30)*

**Important Constraint:** As mentioned in the intro, UNIX Datagram sockets are reliable (unlike Inet UDP), but they are typically **Uni-Directional** in simple setups unless both sides bind. The instructor demonstrates a **Sender -> Receiver** flow.

### A. The Receiver (Server)

```c
/* unix_dgram_receiver.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "./dgram_socket"

int main() {
    int sockfd;
    struct sockaddr_un addr;
    char buf[1024];
    int n;

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    unlink(SOCKET_PATH); // Vital!
    bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));

    printf("Waiting for datagrams on %s...\n", SOCKET_PATH);
    while(1) {
        // recvfrom (we don't care about sender address here)
        n = recvfrom(sockfd, buf, sizeof(buf), 0, NULL, NULL);
        buf[n] = '\0';
        printf("Received: %s", buf);
    }
    return 0;
}
```

### B. The Sender (Client)

```c
/* unix_dgram_sender.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "./dgram_socket"

int main() {
    int sockfd;
    struct sockaddr_un addr;
    char buf[1024];

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);

    // Setup Destination Address
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    printf("Enter message to send: ");
    fgets(buf, 1024, stdin);

    // Just Send. No Bind needed for sender.
    sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&addr, sizeof(addr));

    printf("Message Sent.\n");
    close(sockfd);
    return 0;
}
```

---

## 6. Verification
*(Transcript 11:20)*
When the server is running, if you open a new terminal and type `ls -l`, you will see:
```bash
srwxr-xr-x 1 user group 0 Jan 24 12:00 demo_socket
```
The leading **`s`** indicates it is a **Socket File**.

**Next Session:** Socket Programming Part 4 (Concurrent Servers).
