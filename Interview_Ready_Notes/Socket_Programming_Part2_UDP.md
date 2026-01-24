# Internetworking with Linux: Socket Programming Part 2 (UDP)
**Source:** [Lec35 Socket Programming Part-II (Arif Butt @ PUCIT)](https://www.youtube.com/watch?v=yNUFQaSclmM)
**Instructor:** Arif Butt
**Subject:** System Programming with Linux

> **Note for the Learner:** This document is a complete, textbook-style transcription of the lecture "Socket Programming Part 2". **Important:** While the title implies "Part 2", this lecture is exclusively about **UDP (User Datagram Protocol)** sockets, which are connectionless and inherently different from the TCP practice in Part 1. This guide adheres to the "Zero-Loss" standard, including every file path (`/etc/xinetd.d/echo`), RFC reference (867, 868), and fully usable C code for all 5 examples shown.

---

## 1. Introduction: TCP vs UDP (Datagrams)
*(Transcript 0:08 - 2:08)*

The lecture begins by contrasting the previous session (Stream Sockets) with today's topic found in **Datagram Sockets** (`SOCK_DGRAM`).
*   **TCP (Stream Sockets):** Connection-oriented, Reliable, Byte Stream, Sequence Numbers.
*   **UDP (Datagram Sockets):**
    *   **Unreliable:** No guarantee of delivery.
    *   **Packet Oriented:** Messages are distinct packets (Datagrams). You read the whole message or nothing.
    *   **No Sequencing:** Packets can arrive out of order or be duplicated.
    *   **Connectionless:** You don't (usually) establish a connection. You just "send".

---

## 2. The Postal System Analogy
*(Transcript 2:36 - 4:48)*

The instructor uses the **Postal System** to explain the UDP flow:
*   **Socket:** The Mailbox.
*   **Bind:** Assigning a specific address (House No/Zip Code) to the mailbox so people can write to you.
*   **Sendto:** Dropping a letter in the box. You *must* write the destination address on *every single letter*.
*   **Recvfrom:** Checking the mailbox. You pick up a letter and look at the envelope to see who sent it (Source Address).

**Key difference from Telephone (TCP):** In a phone call (TCP), you dial once and talk. In mail (UDP), every single sentence (packet) needs a fresh address stamp.

**Server Sequence:**
1.  `socket()`
2.  `bind()` (Assign Port)
3.  `recvfrom()` (Wait for upcoming packet)
4.  `sendto()` (Reply using address found in `recvfrom`)

**Client Sequence:**
1.  `socket()`
2.  `sendto()` (Send message + Destination Addr)
3.  `recvfrom()` (Wait for reply)

---

## 3. Connected UDP Sockets?
*(Transcript 5:00 - 5:47)*

This is a subtle interview topic. Can you "Connect" a connectionless protocol? **Yes.**
*   **Unconnected (Standard):** You call `sendto(data, dest_ip)`. The kernel checks routing every time.
*   **Connected UDP:** You call `connect(dest_ip)` first.
    *   *Effect:* It does **not** send a network packet (no 3-way handshake).
    *   *Kernel Optimization:* It records the destination IP in the socket structure.
    *   *Usage:* Now you can use `write()` and `read()` instead of `sendto()` and `recvfrom()` because the kernel knows the destination.
    *   *Benefit:* Slightly faster for multiple messages to the same server; filters out packets from other sources.

---

## 4. Lab 1: Connected UDP Echo Client
*(Transcript 8:21 - 13:48)*

The instructor demonstrates a client that "connects" to the Echo server.

```c
/* udp_connected_client.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    // Usage: ./client <IP> <PORT>
    if (argc != 3) { printf("Usage: %s <IP> <PORT>\n", argv[0]); return 1; }

    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024];
    char *msg = "Hello World";

    // 1. Create Socket (SOCK_DGRAM for UDP)
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. Prepare Address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_aton(argv[1], &servaddr.sin_addr); // IP from Arg 1
    servaddr.sin_port = htons(atoi(argv[2])); // Port from Arg 2

    // 3. Connect (Stores address in kernel, no handshake)
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connect failed");
        exit(1);
    }

    // 4. Write/Read (Standard I/O because it's connected)
    write(sockfd, msg, strlen(msg));
    
    int n = read(sockfd, buffer, sizeof(buffer));
    buffer[n] = '\0';
    printf("Server Echo: %s\n", buffer);

    close(sockfd);
    return 0;
}
```

### Troubleshooting: The `xinetd` Issue
*(Transcript 11:58 - 13:30)*
When running the client, it failed.
*   **Diagnosis:** Checked server status with `netstat -ant` (TCP) and `netstat -anu` (UDP).
*   **Finding:** The Echo service was enabled for TCP but **disabled for UDP**.
*   **Fix:**
    1.  Opened `/etc/xinetd.d/echo`
    2.  Found the UDP section. Changed `disable = yes` to `disable = no`.
    3.  Ran `systemctl start xinetd`.
    4.  Verified with `netstat -u -a`. Port 7 was now listening via UDP.

---

## 5. Lab 2: Unconnected (Standard) UDP Echo Client
*(Transcript 15:48 - 17:19)*

This is the classic UDP approach using `sendto` and `recvfrom`.

```c
/* udp_unconnected_client.c */
// Headers same as above...
int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024];
    char *msg = "Learning is Fun";
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    inet_aton("192.168.1.20", &servaddr.sin_addr);
    servaddr.sin_port = htons(7); 

    // No Connect!

    // sendto calls requires address every time
    sendto(sockfd, msg, strlen(msg), 0, 
           (struct sockaddr*)&servaddr, sizeof(servaddr));

    // recvfrom fills in the sender's address
    len = sizeof(servaddr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                     (struct sockaddr*)&servaddr, &len);
    
    buffer[n] = '\0';
    printf("Echo from Server: %s\n", buffer);
    close(sockfd);
    return 0;
}
```

---

## 6. Lab 3: UDP Daytime Client (RFC 867)
*(Transcript 17:40 - 20:09)*

*   **Protocol:** Server listens on Port 13.
*   **Behavior:** When it receives *any* datagram (even empty), it replies with an ASCII date string.

```c
/* udp_daytime_client.c */
// ... inside main ...
    // Port 13
    servaddr.sin_port = htons(13); 

    // Send Empty Datagram to trigger response
    sendto(sockfd, "", 0, 0, (struct sockaddr*)&servaddr, sizeof(servaddr));

    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
    buffer[n] = '\0';
    printf("Server Time: %s\n", buffer); 
```

---

## 7. Lab 4: UDP Time Client (RFC 868) - The Epoch Issue
*(Transcript 20:20 - 24:18)*

*   **Protocol:** Server listens on Port 37.
*   **Behavior:** Returns a **32-bit binary number** representing seconds.
*   **The Catch (Epoch):**
    *   **Unix Epoch:** Jan 1, 1970.
    *   **Internet Epoch (RFC 868):** Jan 1, 1900.
    *   **Difference:** 2,208,988,800 seconds.
*   **Calculation:** You must subtract this constant to get Unix time.

```c
/* udp_time_client.c */
#include <time.h>
// ... headers ...

#define INTERNET_EPOCH_DIFF 2208988800UL

int main() {
    // ... setup socket to Port 37 ...

    sendto(sockfd, "", 0, 0, (struct sockaddr*)&servaddr, sizeof(servaddr));

    unsigned int time_val; // 32-bit integer
    int n = recvfrom(sockfd, &time_val, 4, 0, NULL, NULL);

    // Convert Network Byte Order (Big Endian) to Host Byte Order
    time_val = ntohl(time_val);

    // Convert Internet Epoch to Unix Epoch
    time_t unix_time = time_val - INTERNET_EPOCH_DIFF;

    printf("Time: %s", ctime(&unix_time));
    return 0;
}
```

---

## 8. Lab 5: UDP Echo Server (The Loop Issue)
*(Transcript 24:32 - 29:40)*

The instructor writes a server. He makes a mistake initially (forgetting the `while(1)` loop), causing the server to exit after one request.

```c
/* udp_echo_server.c */
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024];
    socklen_t len;
    int n;

    // 1. Socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. Bind (Port 54154)
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(54154);

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    // 3. Loop (Crucial for Servers!)
    while(1) {
        len = sizeof(cliaddr);
        // recvfrom populates 'cliaddr' with the client's specific IP/Port
        n = recvfrom(sockfd, buffer, sizeof(buffer), 0, 
                     (struct sockaddr*)&cliaddr, &len);
        
        // Echo back to 'cliaddr'
        sendto(sockfd, buffer, n, 0, 
               (struct sockaddr*)&cliaddr, len);
    }
    return 0;
}
```

---

## 9. Assignment: Analyze `netcat`
*(Transcript 30:15 - End)*
The homework is to study the `netcat` (swiss army knife) utility deeply and try to write a basic version of it, as it uses these very sockets underneath.

**Next Session:** UNIX Domain Sockets.
