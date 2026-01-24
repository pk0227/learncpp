# Internetworking with Linux: TCP/IP Architecture & The Super Daemon (xinetd)
**Source:** [Lec33 Overview Of TCPIP Architecture and Services (Arif Butt @ PUCIT)](https://www.youtube.com/watch?v=p5SrRob-bWg)
**Instructor:** Arif Butt
**Subject:** System Programming with Linux

> **Note for the Learner:** This document is a complete, textbook-style transcription of the lecture "Overview of TCPIP Architecture and Services". It captures the comprehensive review of TCP/IP (IP Classes, CIDR, Subnetting), the Lab Setup verification (Ping/SSH), and the crucial System Programming setup using `xinetd`.

---

## 1. TCP/IP Architecture Recap
*(Transcript 1:40 - 5:40)*

The instructor reviews the 4-layer TCP/IP model compared to the OSI model.

*   **Application Layer:** Resides in User Space.
    *   **Protocols:** HTTP, FTP, SSH, DNS, DHCP.
    *   **Addresses:** URLs (e.g., `http://www.google.com`), Fully Qualified Domain Names (FQDN).
*   **Transport Layer:** Resides in Kernel Space.
    *   **Protocols:**
        *   **TCP:** Reliable, Connection-Oriented, Stream.
        *   **UDP:** Unreliable, Connection-Less, Datagram.
    *   **Addresses:** **Port Numbers** (16-bit integers).
*   **Internet Layer:** Resides in Kernel Space.
    *   **Responsibilities:** Routing, Fragmentation, Reassembly.
    *   **Protocols:** IP (Internet Protocol), ICMP (Ping/Diagnostics), IGMP (Multicast).
    *   **Addresses:** **IP Addresses** (32-bit for IPv4).
*   **Link Layer:** Resides in Hardware/Driver.
    *   **Addresses:** **MAC Addresses** (48-bit physical address).

---

## 2. IP Addressing & Routing Deep Dive
*(Transcript 9:30 - 14:40)*

The instructor provides a detailed breakdown of IPv4 addressing schematics.

### A. IP Classes (Classful Addressing)
IPv4 addresses are divided into classes based on the leading bits. Each IP has a **Network ID** and a **Host ID**.

| Class | Range | Subnet Mask | Usage |
| :--- | :--- | :--- | :--- |
| **Class A** | `1.0.0.0` - `126.x.x.x` | `255.0.0.0` (/8) | Huge networks (16 Million hosts). |
| **Class B** | `128.0.0.0` - `191.255.x.x` | `255.255.0.0` (/16) | Medium networks (65k hosts). |
| **Class C** | `192.0.0.0` - `223.255.255.x` | `255.255.255.0` (/24) | Small networks (254 hosts). |

*   **Network ID:** Identifies the network. All hosts on the same LAN share this.
*   **Host ID:** Identifies the specific machine.
*   **Subnet Mask:** Used to extract the Network ID via Bitwise AND operation.

### B. CIDR (Classless Inter-Domain Routing)
Because Class A was too big and Class C too small, we ran out of IPv4 addresses.
*   **Solution:** CIDR allows flexible subnet masks (not just /8, /16, /24).
*   **Example:** A `/28` network.
    *   The leftmost 28 bits are the Network ID.
    *   The remaining 4 bits are for Hosts.
    *   Total IPs: $2^4 = 16$ (minus Network & Broadcast = 14 usable hosts).

### C. Private IP Addresses (RFC 1918)
To further save public IPs, IETF defined "Private" ranges that are not routable on the public internet.
1.  `10.0.0.0/8` (Class A Private)
2.  `172.16.0.0/12` (Class B Private)
3.  `192.168.0.0/16` (Class C Private - usually Home Routers)

*   **NAT (Network Address Translation):** A Gateway/Router translates these private IPs to a single Public IP to access the internet.

---

## 3. Lab Environment & Connectivity Testing
*(Transcript 15:20 - 22:00)*

The instructor walks through the specific Virtual Machine setup used for this course.

**The Setup:**
*   **Host Machine:** Mac OS (IP `192.168.1.1` via Home Router).
*   **VM 1 (Kali Linux):** `192.168.1.21` (Client).
*   **VM 2 (Ubuntu Server):** `192.168.1.20` (Server).
*   **VM 3 (Windows 10):** `192.168.1.22` (Client).

**Verification Tools:**
1.  **Checking IP:**
    *   Linux: `ifconfig` (Interface Configuration).
    *   Windows: `ipconfig`.
2.  **Checking Gateway:**
    *   `route -n` (Shows the Default Gateway, e.g., `192.168.1.1`).
3.  **Connectivity:**
    *   `ping <IP>`: Sends ICMP Echo Request.
    *   **Hosts File:** Editing `/etc/hosts` to map names to IPs (e.g., mapping `ubuntu-server` to `.20`).

### D. SSH Demonstration
*(Transcript 23:00 - 28:00)*
The instructor demonstrates **Remote Login** using SSH (Secure Shell), replacing the insecure Telnet.
1.  **Check Service:** `systemctl status ssh` on Ubuntu.
2.  **Check Socket:** `netstat -ntp | grep 22`. Shows listener on port 22.
3.  **Connect from Windows:** Uses **PuTTY** to connect to `192.168.1.20`.
4.  **Connect from Kali:** Uses terminal `ssh root@192.168.1.20`.
5.  **Observation:** Running `netstat` on the server now shows **ESTABLISHED** connections from both the Windows IP and Kali IP, while the original listener on Port 22 remains active.

---

## 4. Linux System Services & xinetd (The Super Daemon)
*(Transcript 28:00 - End)*

This section explains how to manage the services utilized in standard socket programming labs.

### A. What is xinetd?
*   **Daemon:** A background process.
*   **Super Daemon:** `xinetd` (Extended Internet Daemon).
*   **Purpose:** Instead of having 50 different daemons (ftpd, telnetd, tftpd) all running and consuming RAM waiting for connections, `xinetd` listens on *all* their ports.
*   **Mechanism:**
    1.  `xinetd` listens on ports 7 (Echo), 13 (Daytime), 21 (FTP), etc.
    2.  When a request arrives on Port 7, `xinetd` wakes up.
    3.  It spawns the specific server process (e.g., `echo-stream`) to handle the request.
    4.  This saves resources.

### B. Configuration Files
*   **Main Config:** `/etc/xinetd.conf`
*   **Service Configs:** `/etc/xinetd.d/` (Directory containing one file per service).

### C. Enabling Test Services (Echo, Daytime)
To perform specific Socket Programming labs (like writing a client for the Echo server), you must enable these services on your Linux Server (Ubuntu VM).
**Steps Demonstrated:**
1.  **Install:** `sudo apt-get install xinetd`
2.  **Edit Service File:**
    *   File: `/etc/xinetd.d/daytime`
    *   Change: `disable = yes` to `disable = no`
    *   File: `/etc/xinetd.d/echo`
    *   Change: `disable = yes` to `disable = no`
3.  **Restart xinetd:** `systemctl restart xinetd`

### D. The Test Services
The instructor demonstrates connecting to these services using `netcat` (`nc`).
1.  **Daytime (Port 13):**
    *   Command: `nc <IP> 13`
    *   Result: Server sends current date/time string and closes connection.
2.  **Echo (Port 7):**
    *   Command: `nc <IP> 7`
    *   Result: Whatever you type is sent back to you. (Loopback).
3.  **Chargen (Port 19):**
    *   Command: `nc <IP> 19`
    *   Result: Server floods you with characters used for bandwidth testing.
4.  **Discard (Port 9):**
    *   Command: `nc <IP> 9`
    *   Result: Server accepts data and silently throws it away (like `/dev/null`).

---

## 5. /etc/services
*(Transcript 22:26 - 22:55)*

*   **File:** `/etc/services`
*   **Purpose:** The local database mapping Service Names to Port Numbers.
*   **Usage:**
    *   When you write `getservbyname("http", "tcp")` in C code, the OS looks up "http" in this file and returns `80`.
    *   You can view it: `less /etc/services`

---
**Summary:** We have covered the complete architecture of TCP/IP networks, the practical skill of verifying connectivity (Ping/SSH), and the specific configuration of `xinetd` to host the Echo/Daytime services required for future socket programming labs.
