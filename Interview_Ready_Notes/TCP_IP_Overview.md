# Internetworking with Linux: TCP/IP Detailed Lecture Notes
**Source:** [Lec01 Overview of TCP IP (Arif Butt @ PUCIT)](https://www.youtube.com/watch?v=9nKSShPm3cI)
**Instructor:** Arif Butt
**Course:** Internetworking with Linux

> **Note for the Learner:** These notes are designed to be a complete textual substitute for the lecture. Every concept, example, and explanation provided in the video is preserved here in detail, without summarization.

---

## 1. Introduction and Course Roadmap
*(Transcript 0:00 - 3:20)*

The instructor, Arif Butt, begins by welcoming students, referencing his previous video lectures and website. He expresses thanks to subscribers and students for their feedback.

He introduces this new course, **"Internetworking with Linux,"** as a series of roughly 22 lectures. The target audience includes undergraduate students and anyone with basic networking concepts and a working knowledge of the Linux command-line interface (CLI).

**Today's Goals:** This first session provides a theoretical overview of important internetworking concepts required for the future practical sessions.

**Future Sessions Roadmap:**
In the upcoming lectures, the course will focus on building a lab environment and deploying various Linux services:
1.  **DHCP Server:** For automatically assigning TCP/IP configuration parameters to devices.
2.  **Telnet and SSH (Secure Shell) Servers:** For remote login to Linux machines.
3.  **Apache Web Server:** For deploying websites using HTTP and HTTPS protocols.
4.  **FTP and SFTP Servers:** For secure file transfer between computers.
5.  **NFS Server (Network File System):** For file sharing across the network.
6.  **Squid Proxy Server:** Used for caching web content, performing DNS lookups, and filtering traffic for security.
7.  **DNS (BIND):** The "Berkeley Internet Name Domain" server for name mapping.
8.  **Database Server:** Specifically using MySQL.
9.  **Linux Security:** Using firewalls (`iptables`).

---

## 2. Core Internetworking Concepts
*(Transcript 3:33 - 5:26)*

The specific title for this course is "Internetworking with Linux." To understand that, we define independent networking terms:

### Computer Network
A computer network is defined as a set of computers connected together for the purpose of sharing different resources (like files, apps, or printers).

### The Internet
The Internet is a worldwide system that connects different computer networks, allowing hosts on all those disparate networks to communicate with one another.

### Internet vs. Intranet vs. Extranet
The instructor clarifies these related terms:
1.  **Intranet:** This is a **private** network contained *within* an organization.
    *   Employees can access local computing resources.
    *   They can also access the public Internet, usually via a specific device called a **Gateway Computer**.
2.  **Extranet:** This can be thought of as a part of a company's Intranet that is made available to authorized users *outside* the company.
    *   **Examples:** Suppliers, Vendors, and Customers.

### Protocol Architecture: TCP/IP
*(Transcript 5:38 - 6:13)*
There are many internetworking protocols, but **TCP/IP** (Transmission Control Protocol / Internet Protocol) has become the dominant protocol suite.
*   **Origin:** It is a standard set of protocols developed in the **late 1970s** by **DARPA** (Defense Advanced Research Projects Agency).
*   **Naming:** Because of its funding and origin, the TCP/IP architecture is often referred to as the **"DARPA Model."**

---

## 3. The TCP/IP Protocol Stack (The 4 Layers)
*(Transcript 6:19 - 16:10)*

The instructor draws the model on the board. Unlike the OSI 7-layer model, the TCP/IP (DARPA) stack consists of **four layers**.

### Layer 1: Application Layer (Topmost)
This layer provides applications with the ability to access services of the lower layers.
*   **Interfaces:** It supports programming interfaces like **Sockets**, **Winsock** (on Windows), or **MacTCP** (on Macintosh).
*   **Protocols:** Many common protocols live here, including:
    *   **SSH** (Secure Shell)
    *   **HTTP** (HyperText Transfer Protocol)
    *   **FTP** (File Transfer Protocol)
    *   **DNS** (Domain Name System)
    *   **Telnet**
    *   **SMTP** (Simple Mail Transfer Protocol)
    *   **SNMP** (Simple Network Management Protocol)
*   **Addressing:** We address services here using **Domain Names** (e.g., `google.com`) or **URIs**.

### Layer 2: Transport Layer
This layer is responsible for end-to-end communication. We essentially have two choices of protocols:
1.  **TCP (Transmission Control Protocol):**
    *   It creates a **Reliable** channel (guarantees delivery).
    *   It is **Full-Duplex**.
    *   It is **Stream-oriented** (data flows like a stream).
2.  **UDP (User Datagram Protocol):**
    *   It creates an **Unreliable** channel (no guarantee).
    *   It is **Full-Duplex**.
    *   It is **Packet-oriented**.
*   **Addressing:** This layer uses **Port Numbers** (16-bit) to differentiate applications.

### Layer 3: Internet Layer
This layer handles the logic of finding paths across networks.
*   **Functions:**
    *   **Fragmentation:** Breaking large data packets into smaller fragments.
    *   **Routing:** Discovering paths along which data packets can be sent.
*   **Protocols:**
    *   **IP (Internet Protocol):** The core protocol for addressing, routing, fragmentation, and reassembly.
    *   **ARP (Address Resolution Protocol):** Resolves IP addresses to MAC addresses.
    *   **ICMP (Internet Control Message Protocol):** Providing diagnostic functions (like `ping`) and error reporting (e.g., "Destination Unreachable").
    *   **IGMP (Internet Group Management Protocol):** Management of IP multicast groups.
*   **Addressing:** Uses **IP Addresses** (IPv4 is 32-bit; IPv6 is 128-bit).

### Layer 4: Network Interface Layer (Bottom)
This layer deals with the physical hardware.
*   **Function:** It places TCP/IP packets onto the network medium (sending) and receives them off the network (receiving). The data unit here is called a **Frame**.
*   **Independence:** The architecture is designed to be purely independent of the underlying hardware. It works on:
    *   **Access Methods:** Ethernet, Token Ring, FTDI.
    *   **Media:** Coaxial cable, Twisted pair, Fiber optic, Wireless, Microwave, Bluetooth.
*   **Addressing:** Uses **MAC Addresses** (Media Access Control, 48-bit).

### Data Flow Example
The instructor describes how a message moves:
1.  **Application Layer:** A process (P1) pushes a message down.
2.  **Transport Layer:** Attaches a **Header** (containing Source/Dest Ports).
3.  **Internet Layer:** Attaches an **IP Header** (~20 bytes, containing Source/Dest IPs).
4.  **Network Interface Layer:** Attaches a **Frame Header** (MAC addresses) and a **Trailer** (Footer).
5.  **Transmission:** The complete "Frame" is transmitted over the physical medium.
6.  **Reception:** The destination computer receives the frame. It moves **up** the stack, and each layer **detaches** (removes) its specific header until the raw data reaches the target application.

---

## 4. Addressing in Detail (Exhaustive)

The instructor spends significant time detailing the addressing for each layer.

### A. Application Layer: Domain Names
*(Transcript 16:15 - 20:47)*
*   **DNS:** Humans prefer names (like `arifbutt.me`), but computers need numbers (IPs). The **Domain Name System (DNS)** provides a name mapping service to translate between them.
*   **URI (Uniform Resource Identifier):** A generic string identifying a resource. There are two types:
    1.  **URL (Uniform Resource Locator):** Identifies a resource by its **location**.
        *   Format: `protocol://host:port/path`
        *   Example: `http://arifbutt.me:80/mycv.html` (Port 80 is optional).
        *   Protocols: `http`, `ftp`, `file`, `news`, `telnet`.
    2.  **URN (Uniform Resource Name):** Identifies a resource by its **name**, regardless of location.
        *   Example: The instructor uses the ISBN of a book, *The Last Unicorn* (`urn:isbn:045...`). This number identifies the book no matter which library holds it.

### B. Transport Layer: Port Numbers
*(Transcript 21:02 - 24:25)*
*   **Purpose:** A port number identifies the specific **process** running on a computer. If your computer receives data, the Port Number tells the OS which application (Web Server vs Email Server) should get it.
*   **Size:** 16-bit integer ($0$ to $65,535$).
*   **Categories:**
    1.  **Well-Known Ports (0 - 1,023):** Permanently assigned by IANA.
        *   `80`: HTTP
        *   `443`: HTTPS
        *   `22`: SSH
        *   `23`: Telnet
        *   *Tip:* In Linux, you can view these mappings in the file `/etc/services`.
    2.  **Registered Ports (1,024 - 49,151):** Assigned to specific vendor applications.
        *   `2049`: NFS (Network File System)
        *   `3306`: MySQL Database
        *   *Developer Note:* Avoid using these for your own custom apps to prevent conflicts.
    3.  **Private/Dynamic Ports (49,152 - 65,535):** Also called "Ephemeral ports." These are used temporarily by client applications (like your browser) when they initiate a connection.

### C. Internet Layer: IP Addresses (IPv4)
*(Transcript 24:52 - 37:41)*
*   **Definition:** An IP Address uniquely identifies a **host** on a network.
*   **Structure:** IPv4 uses 32 bits, written as four 8-bit octets (e.g., `192.36.32.4`).
*   **Authority:** **ICANN** manages IP address allocation.

#### Classes of IP Addresses
We categorize IPs into Classes A, B, and C based on network size.

**1. Class A Addresses:**
*   **Structure:** The first octet is the Network part; the remaining three are for Hosts (`N.H.H.H`).
*   **Range:** `1.0.0.0` to `126.0.0.0`.
*   **Capacity:** $2^7 - 2$ Networks (126). Each network supports $2^{24} - 2$ Hosts (approx **16 Million**).
*   **Large Organization Example:** **IBM** owns the explicit Class A block `9.0.0.0`.
*   **Address Properties:**
    *   The **First** address (`1.0.0.0`) is the **Network Address** (identifies the network).
    *   The **Last** address (`1.255.255.255`) is the **Broadcast Address** (sends to all hosts).
    *   Neither is assigned to a single computer.
*   **Default Subnet Mask:** `255.0.0.0` (or /8).

**2. Class B Addresses:**
*   **Structure:** First two octets are Network; last two are for Hosts (`N.N.H.H`).
*   **Range:** `128.0.0.0` to `191.255.0.0`.
*   **Capacity:** $2^{14} - 2$ Networks. Each supports $2^{16} - 2$ Hosts (**~65,534**).
*   **Address Properties:**
    *   Example: `128.0.0.0`.
    *   First address (`128.0.0.0`) is the Network Address.
    *   Last address (`128.0.0.255.255`) is the Broadcast Address.
*   **Default Subnet Mask:** `255.255.0.0` (or /16).

**3. Class C Addresses:**
*   **Structure:** First three octets are Network; last one is for Hosts (`N.N.N.H`).
*   **Range:** `192.0.0.0` to `223.255.255.0`.
*   **Capacity:** $2^{21} - 2$ Networks. Each supports $2^8 - 2$ Hosts (**254**).
*   **Address Properties:**
    *   Example: `192.0.0.0`.
    *   First address (`192.0.0.0`) is the Network Address.
    *   Last address (`192.0.0.255`) is the Broadcast Address.
*   **Default Subnet Mask:** `255.255.255.0` (or /24).

*(Note: `127.x.x.x` is reserved for **Loopback** diagnostic functions, like `127.0.0.1`)*.

### D. Network Interface Layer: MAC Addresses
*(Transcript 52:56 - 54:02)*
*   **Definition:** A hardware address burned into the Network Interface Card (NIC).
*   **Size:** 48 bits (6 Bytes), usually written in Hexadecimal string (e.g., `A1:B2...`).
*   **Structure:**
    *   **First 3 Bytes (Left):** OUI (Organizationally Unique Identifier). This identifies the **Manufacturer** (e.g., Intel, Cisco, Dell).
    *   **Last 3 Bytes (Right):** A unique Serial Number for that specific card.

---

## 5. Subnetting (Detailed Explanation)
*(Transcript 37:53 - 44:28)*

**Definition:** Subnetting is the technique of dividing a single large network into two or more smaller, separate networks.

**Why Subnet? (Advantages):**
1.  **Security:** It physically or logically isolates departments (e.g., Sales cannot access Accounts).
2.  **Efficiency:** It increases routing efficiency by organizing the network.
3.  **Broadcast Control:** It reduces the size of the "Broadcast Domain" (network noise is contained).

**Detailed Example from Lecture:**
Consider an organization with one Class C network: `192.54.6.0` (Default Mask `255.255.255.0`).
*   This single network can hold 254 hosts.
*   **Scenario:** The company has two departments: **Sales** (100 computers) and **Accounts** (100 computers). They want to separate them.
*   **Solution:** We "borrow" bits from the Host part and give them to the Network part.
*   To create **2 subnets**, we need to borrow **1 bit** ($2^1 = 2$).
*   **The New Mask:**
    *   Old Last Octet: `00000000` (0)
    *   New Last Octet: `10000000` (128)
    *   So the mask becomes: `255.255.255.128` (or /25).
    *(Note on Video Discrepancy: The instructor writes .192 at one point but describes the logic for a 1-bit split, which mathematically yields the .128 mask described here as the correct /25 standard).*

**The Resulting Subnets:**
1.  **Sales Subnet:**
    *   Network ID: `192.54.6.0`
    *   Valid IP Range: `192.54.6.1` to `192.54.6.126`
    *   Broadcast: `192.54.6.127`
2.  **Accounts Subnet:**
    *   Network ID: `192.54.6.128`
    *   Valid IP Range: `192.54.6.129` to `192.54.6.254`
    *   Broadcast: `192.54.6.255`

---

## 6. Supernetting (CIDR) and Private IPs
*(Transcript 44:52 - 50:31)*

### CIDR (Classless Inter-Domain Routing)
*   Also known as **Supernetting**.
*   **Purpose:** The rigid Class A, B, and C system wasted too many IP addresses. CIDR allows us to use flexible "slashes" (e.g., `/28`) to assign exactly the number of IPs needed.
*   **Benefit:** This significantly extended the useful life of IPv4.

### Private IP Addresses (RFC 1918)
To address the shortage of public IPs, the IETF defined three ranges of **Private IP Addresses**. These IPs are **non-routable** on the public internet—they only work inside a LAN.
*   **Class A Private Range:** `10.0.0.0` to `10.255.255.255` (/8) — Supports millions of private hosts.
*   **Class B Private Range:** `172.16.0.0` to `172.31.255.255` (/12).
*   **Class C Private Range:** `192.168.0.0` to `192.168.255.255` (/16).

---

## 7. NAT (Network Address Translation)
*(Transcript 50:46 - 52:43)*

**Problem:** If Private IPs (like `192.168.1.1`) cannot connect to the internet, how do we browse the web?

**Solution:** **NAT**.
*   **How it works:** A **Gateway** (Router) sits between the private network and the public internet. This Gateway has **one real Public IP address**.
*   **Process:** When a private computer tries to access a website, the Gateway acts as an **agent**. It takes the request, replaces the Private IP with its own Public IP, sends it, receiving the reply, and forwarding it back to the specific internal computer.
*   **Advantages:**
    1.  **Economy:** One Public IP can represent an entire office.
    2.  **Security:** The outside world sees only the Gateway. Internal computers are hidden behind it.

---

## 8. Packet Flow and ARP
*(Transcript 54:09 - 56:35)*

The instructor concludes by explaining how devices actually find each other.

**Routing Logic:**
1.  **Same Network:** If the Destination IP is on the **same** local network, the OS performs **No Routing**. It delivers the message directly using the hardware (MAC) address.
2.  **Different Network:** If the Destination IP is different, the OS sends the message to the **Gateway** (Router).

**ARP (Address Resolution Protocol):**
Computers talk using MAC addresses (Layer 4), but we usually only know the IP address (Layer 3). **ARP** bridges this gap.
*   **The Problem:** "I have the Destination IP, roughly where is it? But I need the Destination MAC to build the frame."
*   **The Process:**
    1.  **Check Cache:** The computer first checks its own **ARP Cache** (a local list of known devices).
    2.  **Broadcast:** If the MAC is not found, the computer sends a **Broadcast** message (`FF:FF:FF:FF:FF:FF`) to the entire network:
        *   *"Who has IP 192.168.1.5?"*
    3.  **Reply:** Every computer hears the broadcast, but **only** the computer with that IP addresses replies:
        *   *"I have that IP, and here is my MAC address."*
    4.  **Save:** The sender saves this mapping (IP -> MAC) in its ARP Cache so it doesn't have to "shout" next time.

*(The lecture concludes with the instructor mentioning that this theoretical foundation requires a practical lab, which will be the subject of the next session.)*
