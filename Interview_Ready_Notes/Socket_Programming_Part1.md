# Internetworking with Linux: Socket Programming Part 1 (TCP/IP)
**Source:** [Lec34 Socket Programming Part-I (Arif Butt @ PUCIT)](https://www.youtube.com/watch?v=p5SrRob-bWg)
**Instructor:** Arif Butt
**Subject:** System Programming with Linux

> **Note for the Learner:** This document is a complete, textbook-style transcription of the lecture "Socket Programming Part 1". It covers the fundamental concepts, specific API calls, the detailed **Proof of Concept** using `strace` and `wireshark`, and **Full Source Code** examples for the TCP Echo Client and Server demonstrated in the video.

---

## 1. Introduction and Recap
*(Transcript 0:00 - 1:05)*

The instructor welcomes students to the **System Programming with Linux** series.
*   **Topic:** Socket Programming Part 1.
*   **Goal:** Programming Client and Server processes using **Internet Domain TCP Sockets**.
*   **Prerequisite:** Lecture 33 (TCP/IP Architecture review).

**Client-Server Paradigm Recap:**
*   **Socket:** An IPC (Inter-Process Communication) tool used to communicate between processes on the *same* machine or *different* machines.
*   **Server Process:** An application that waits for incoming communication requests. (e.g., Web Server).
*   **Client Process:** An application that initiates the request. (e.g., Web Browser).
*   **Connection Types:**
    *   **Short Connection:** One request, one response, close (e.g., TIME, ECHO services).
    *   **Long Connection:** Interaction continues until explicit quit (e.g., SSH, Telnet, FTP).
*   **State:**
    *   **Stateful:** Server remembers client data/state between requests (simpler client logic).
    *   **Stateless:** Server keeps no state (Reliable, but slower if requests get long).

---

## 2. What is a Socket?
*(Transcript 4:31 - 5:29)*

**Definition:** A communication endpoint to which an application can write data and from which it can read data.
**API Flavors:**
1.  **Berkeley Sockets (BSD):** Derived from UC Berkeley (used in Linux/Unix). **We will use this.**
2.  **TLI (Transport Layer Interface):** Derived from AT&T System V Unix.

**Socket Types:**
1.  **SOCK_STREAM (TCP):** Internet Domain Stream Sockets. (Focus of this lecture).
2.  **SOCK_DGRAM (UDP):** Datagram Sockets. (Future lecture).

---

## 3. Stream Sockets (TCP) Characteristics
*(Transcript 6:00 - 8:08)*

A Stream Socket provides a **Reliable, Full-Duplex, Stream-Oriented** channel.
*   **Connection-Oriented:** A logical connection is established before data exchange.
*   **No Message Boundaries:** Data flows like a stream (byte stream), not discrete packets. You might send 100 bytes in two 50-byte chunks, but the receiver might read them as four 25-byte chunks.
*   **Reliability Mechanisms:**
    *   **Sequencing:** Packets are numbered to handle out-of-order delivery.
    *   **Acknowledgments (ACK):** Sender knows if data arrived.
    *   **Checksums:** For error detection.
    *   **Flow Control:** Sliding Window mechanism prevents overwhelming the receiver.
    *   **Congestion Control:** Prevents overwhelming the network (Slow Start, Fast Retransmit).

> **Note:** For deep theory, read `man 7 tcp` and RFC 793 (TCP).

---

## 4. The System Call Sequence (The Telephone Analogy)
*(Transcript 9:37 - 12:29)*

The instructor explains the flow using a "Telephone System" analogy.

### The Server's Sequence
1.  **`socket()`**: Installing the telephone set. Creates the endpoint.
2.  **`bind()`**: Assigning a phone number. Binds an IP and Port to the socket.
3.  **`listen()`**: Waiting for the phone to ring. Notifies the kernel we are willing to accept connections.
4.  **`accept()`**: Picking up the receiver. Blocks until a call arrives. Returns a **new** socket for the conversation.

### The Client's Sequence
1.  **`socket()`**: Installing the phone.
2.  **`connect()`**: Dialing the number. Initiates the **3-Way Handshake**.

### Data Transfer
Once connected, both sides use `read()`/`write()` (or `send()`/`recv()`) to talk.
When done, they call `close()` (hanging up).

### Important Concept: Master vs. Data Socket
*(Transcript 12:43 - 13:57)*
On the Server side, there are **two** types of sockets:
1.  **Master Socket (Listening Socket):** Created via `socket()`, bound, and listening. It *only* handles connection requests.
2.  **Data Socket (Connected Socket):** Created by `accept()` when a client connects. This is used for the actual conversation.
    *   *Why?* This allows the Master socket to stay free to listen for *new* callers while the Data socket handles the current caller.

---

## 5. The TCP State Machine (Handshake & Termination)
*(Transcript 14:14 - 16:32)*

### Connection Establishment (3-Way Handshake)
1.  **Client:** Calls `connect()`. Sends **SYN** (Synchronize) packet.
    *   *Client State:* `SYN_SENT`
2.  **Server:** Receives SYN. Sends **SYN-ACK**.
    *   *Server State:* `SYN_RCVD`
3.  **Client:** Receives SYN-ACK. Sends **ACK**.
    *   *Client State:* `ESTABLISHED`
4.  **Server:** Receives ACK. `accept()` returns the new socket.
    *   *Server State:* `ESTABLISHED`

### Connection Termination (4-Way Handshake)
1.  **Active Close (Client):** Calls `close()`. Sends **FIN**.
2.  **Passive Close (Server):** Kernel receives FIN. Sends **ACK**. Application sees EOF (read returns 0).
3.  **Server Closes:** Server calls `close()`. Sends its own **FIN**.
4.  **Client:** Receives FIN. Sends **ACK**.
    *   *Result:* Connection closed.

---

## 6. Proof of Concept: Debugging & Analysis (Detailed Demo)
*(Transcript 16:38 - 29:29)*

The instructor moves to the Kali Linux terminal to **prove** the concepts using standard tools.
**Setup:**
*   **Kali (Client):** `192.168.1.101`
*   **Ubuntu (Server):** `192.168.1.20` running `inetd` (services like Echo on port 7).

### A. Netstat Analysis
1.  **Client Side:** While connected to the server via `netcat` (`nc 192.168.1.20 7`), running `netstat -ntp` shows a connection in **ESTABLISHED** state.
    *   Local Port: Random High Port (e.g., 37616).
    *   Remote Port: 7.
2.  **Server Side:** Shows **two** sockets:
    *   One in `LISTEN` (Master Socket).
    *   One in `ESTABLISHED` (Data Socket connecting to Kali).
3.  **TIME_WAIT:** When the client closes the connection, the socket doesn't vanish immediately. It enters `TIME_WAIT` state for 1-5 minutes to ensure all stray packets are handled.

### B. Tracing System Calls with `strace`
*(Transcript 21:12)*
The instructor runs `strace nc ...` to see the actual system calls behind the scenes.
**Observed Sequence:**
1.  **`socket(...) = 3`**: Creates the socket. Returns File Descriptor (FD) 3.
2.  **`connect(3, ...)`**: Initiates connection. Returns 0 (Success).
3.  **`select(...)`**: The program essentially waits ("blocks") for input. It monitors FDs for readiness.
4.  **`read(0, ...)`**: When user types "Learning is Fun", it reads from FD 0 (STDIN).
5.  **`write(3, ...)`**: Writes that data to FD 3 (The Network Socket).
6.  **`select(...)`**: Waits again.
7.  **`read(3, ...)`**: The server echoes back; client reads from FD 3.
8.  **`write(1, ...)`**: Client writes the echo to FD 1 (STDOUT).

### C. Packet Analysis with Wireshark
*(Transcript 24:02)*
The instructor captures the traffic to visualize the headers.
1.  **The Handshake:**
    *   Packet 1: **SYN** (Seq=0).
    *   Packet 2: **SYN, ACK** (Seq=0, Ack=1).
    *   Packet 3: **ACK** (Seq=1, Ack=1).
2.  **Data Transfer:**
    *   He highlights the **Push (PSH)** flag used when data is sent.
    *   He notes that the packet size changes based on data length.
3.  **Fragmentation Lesson:**
    *   He sends a *large* file via `netcat`.
    *   **Observation:** The data is split into multiple packets.
    *   **Critical Programming Lesson:** "The second packet is sent only after the server has received acknowledgment."
    *   **Data Read Rule:** When reading from a TCP socket, **ALWAYS read in a loop**. You might receive 128 bytes as two 64-byte chunks or one 128-byte chunk. Never assume `read()` gives you everything in one go.

---

## 7. Socket Programming APIs (Detailed Arguments)
*(Transcript 29:36 - 32:29)*

### 1. `socket(domain, type, protocol)`
*   **Domain:** `AF_INET` (IPv4) or `AF_INET6` (IPv6).
*   **Type:** `SOCK_STREAM` (TCP) or `SOCK_DGRAM` (UDP).
*   **Protocol:** `0` (Let OS choose default, usually TCP for Stream).
*   **Return:** File Descriptor (int) or -1 on error.

### 2. `connect(sockfd, addr, addrlen)`
*   **sockfd:** The socket file descriptor.
*   **addr:** Pointer to `struct sockaddr` containing Server IP/Port.
*   **addrlen:** Size of the address structure.
*   **Tasks:**
    *   Ensures socket is valid.
    *   Populates remote endpoint info.
    *   Initiates 3-Way Handshake.

### The Address Structure (`struct sockaddr_in`)
*(Transcript 32:58 - 36:00)*
For IPv4, we use specific C structs:
```c
struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(1234)
    struct in_addr   sin_addr;     // IP Address
    char             sin_zero[8];  // Padding
};
```
**Network Byte Order:**
Machine memory is usually **Little-Endian**. Network protocols require **Big-Endian**.
We MUST convert using helper functions:
*   `htons()`: Host to Network Short (for Ports).
*   `htonl()`: Host to Network Long (for IPs).
*   `ntohs()`: Network to Host Short.
*   `inet_aton()`: Converts string IP ("192.168.1.1") to binary format.

---

## 8. Practical Example: TCP Echo Client
*(Transcript 39:28 - 42:27)*

This code connects to a standard Echo Server (running on port 7).

```c
/* tcp_echo_client.c */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main() {
    int sockfd;
    struct sockaddr_in saddr;
    char buf[1024];
    int n;

    // 1. Create Socket (IPv4, TCP, Default Protocol)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // 2. Prepare Address Structure
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(7);      // Echo Protocol Port
    inet_aton("192.168.1.20", &saddr.sin_addr); // Server IP

    // 3. Connect to Server
    if (connect(sockfd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
        perror("Connect failed");
        exit(1);
    }
    printf("Connected to Echo Server...\n");

    // 4. Data Exchange Loop
    while(1) {
        printf("Enter message: ");
        fgets(buf, 1024, stdin); // Read from keyboard

        write(sockfd, buf, strlen(buf)); // Send to server

        n = read(sockfd, buf, 1024); // Read reply
        write(1, buf, n);            // Write to stdout (FD 1)
    }

    close(sockfd);
    return 0;
}
```

---

## 9. Practical Example: TCP Echo Server
*(Transcript 46:19 - 48:35)*

This code creates a server that listens on port 54154 and echoes back any data it receives.

```c
/* tcp_echo_server.c */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main() {
    int master_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char buf[1024];
    int n;

    // 1. Create Master Socket
    master_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (master_sock < 0) {
        perror("Socket failed");
        exit(1);
    }

    // 2. Bind to IP/Port
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(54154); // Ephemeral Port
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces

    if (bind(master_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    // 3. Listen
    listen(master_sock, 10); // Backlog queue size = 10
    printf("Server waiting for connections on port 54154...\n");

    // 4. Accept Connection (Blocking Call)
    addr_len = sizeof(client_addr);
    client_sock = accept(master_sock, (struct sockaddr*)&client_addr, &addr_len);
    if (client_sock < 0) {
        perror("Accept failed");
        exit(1);
    }
    printf("Client connected!\n");

    // 5. Data Loop (Reading from Data Socket)
    while((n = read(client_sock, buf, 1024)) > 0) {
        write(client_sock, buf, n); // Echo back
    }

    close(client_sock);
    close(master_sock);
    return 0;
}
```

---

## 10. DNS and Service Lookups
*(Transcript 51:25 - 56:39)*

Clients usually know names (like `google.com`), not IPs.
*   **`gethostbyname(char *name)`**:
    *   Takes a domain name string.
    *   Returns a `struct hostent*`.
    *   This struct contains the list of **IP Addresses** associated with that name.
    *   *Demo:* A program `gethostname.c` resolves `google.com` to its IPs.

Clients usually know Service Names (like "http"), not Port Numbers (80).
*   **`getservbyname(char *name, char *proto)`**:
    *   Takes "ftp", "ssh", etc.
    *   Returns `struct servent*` containing the port number.
    *   *Source:* it reads from `/etc/services`.

---

## 11. Web Server Architecture (Assignment)
*(Transcript 57:02 - End)*

The instructor concludes by explaining HTTP to set up the assignment: **Write a TCP Web Server.**

**HTTP Request Format:**
1.  **Request Line:** `GET /index.html HTTP/1.1`
2.  **Headers:** Host, User-Agent, etc.
3.  **Blank Line**
4.  **Body** (Optional)

**HTTP Response Format:**
1.  **Status Line:** `HTTP/1.1 200 OK`
2.  **Headers:** Content-Type: text/html, etc.
3.  **Blank Line**
4.  **Body:** The actual HTML file.

**Assignment Logic:**
1.  Server accepts connection.
2.  Reads the Request String ("GET /file...").
3.  Parses the filename.
4.  Opens the file locally.
5.  Writes the HTTP Headers + File Content back to the socket.
6.  Closes the socket.

---
**Summary:** We have covered the complete lifecycle of a TCP connection from both Client and Server perspectives, the specific system calls involved, and the standard formats for addressing. Next session: **UDP Sockets**.
