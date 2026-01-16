# 🧠 UNIX Domain Sockets (IPC)
> **Deep, Practical, Senior-Level Understanding**  
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. What is a UNIX Domain Socket? (Real Meaning)

**Standard Definition:**
"A data communications endpoint for exchanging data between processes executing on the same host operating system."

**Senior Definition:**
*   ✅ **"Network Sockets without the Network Overhead"**
*   ✅ **"Pipe on Steroids"**

It uses the standard Socket API (`socket`, `bind`, `connect`), but addresses are **Filesystem Paths** (e.g., `/tmp/mysql.sock`) instead of IPs. It bypasses the entire TCP/IP Protocol Stack (No Routing, No TCP Headers, No Checksums, No Fragmentation).

---

## 🔹 2. Reasonable Code Snippet (Server Setup)

```cpp
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

void createIPCSocket() {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    
    // The "Address" is a file path
    strncpy(addr.sun_path, "/tmp/demo.sock", sizeof(addr.sun_path) - 1);

    // Unlink (delete) old socket file if it exists, otherwise bind fails
    unlink("/tmp/demo.sock");

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("Bind failed");
        return;
    }
    
    // Now listen() and accept() work exactly like TCP!
    std::cout << "Listening on /tmp/demo.sock\n";
    close(fd);
}
```

---

## 🔹 3. Deep Pros & Cons (vs Loopback TCP)

### ✅ Pro #1: Performance (Zero Protocol Overhead)
*   **The Benefit**: Copying data from Process A to Process B happens purely in memory. No TCP sequence numbers, no ACKs, no sliding windows calculation.
*   **Impact**: ~30-50% lower latency and higher throughput compared to `127.0.0.1`.

### ✅ Pro #2: Security (File Permissions)
*   **The Benefit**: You can secure a UDS using standard Linux file permissions (`chmod 700`). Only the user owning the file can connect.
*   **Impact**: Much simpler Access Control than configuring `iptables` for localhost ports.

### ✅ Pro #3: Credential Passing (`SCM_CREDENTIALS`)
*   **The Benefit**: The server can ask the kernel: "Who is the User ID (UID) of the process talking to me?". The Kernel guarantees the answer is true. You cannot spoof your UID over UDS.

### ❌ Con #1: No Network
*   **The Issue**: If you decide to move the Database to a separate machine, you MUST rewrite code to use TCP/IP.
*   **Impact**: Loss of deployment flexibility.

---

## 🔹 4. When to Use / Not Use

### ❌ Do NOT use UNIX Sockets when:
*   You expect the architecture to scale horizontally (Process A and B might move to different servers).
*   You need to support Windows heavily (Windows has Named Pipes, but UDS support is recent/quirky).

### ✅ Acceptable use cases:
*   **Database Connections**: Connecting to MySQL/Postgres on localhost.
*   **Container Sidecars**: Docker Daemon API, Kubernetes Pod-to-Sidecar comms.
*   **Logging Agents**: App sending logs to a local Logstash implementation.

---

## 🔹 5. Relevant Linux Tools

### 🛠️ nc (Netcat with -U)
Test your IPC server.
```bash
nc -U /tmp/demo.sock
```

### 🛠️ sockstat
View open UNIX sockets.
```bash
ss -x -a
# Netid  State   Recv-Q  Send-Q  Local Address:Port
# u_str  LISTEN  0       128     /tmp/demo.sock 12345
```

### 🛠️ ls -l (File Type)
Check if a file is a socket.
```bash
ls -l /tmp/demo.sock
# s rwxr-xr-x 1 user group ... (First char 's' means Socket)
```

---

## 🔹 6. Senior-Level Interview Answer

> "I strictly use UNIX Domain Sockets for inter-process communication on the same node. It allows me to bypass the TCP stack overhead (checksums, routing) for better latency. A critical feature I leverage is **File Descriptor Passing** (SCM_RIGHTS)—I can open a file in a privileged process and 'pass' the open FD to an unprivileged worker process via the UDS. This enables robust privilege separation architectures."

---

## 🔹 7. Interview Question Bank

### Q1. Can UDP be used with UNIX Sockets?
**Answer**: "Yes! `SOCK_DGRAM` with `AF_UNIX`. It provides **reliable**, ordered message boundaries (unlike Network UDP which is unreliable). It is essentially a message queue."

### Q2. What happens if the server crashes but leaves the socket file?
**Answer**: "The client gets 'Connection Refused'. However, when the server restarts, `bind()` will fail with 'Address already in use' because the file exists. The server code MUST call `unlink()` on the path before binding."

### Q3. What is Abstract Namespace?
**Answer**: "Linux-specific feature. If the path starts with a NULL byte (`\0`), the socket is not created on the filesystem. It creates a hidden kernel identifier. Benefit: Auto-cleanup when the socket closes (no need to `unlink`)."
