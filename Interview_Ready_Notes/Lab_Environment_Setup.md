# Internetworking with Linux: Building the Lab Environment (Static Configuration)
**Source:** [Lec02 Static TCPIP Configurations (Arif Butt @ PUCIT)](https://www.youtube.com/watch?v=hAWgKb_EJLk)
**Instructor:** Arif Butt
**Course:** Internetworking with Linux
**Prerequisite:** View Lecture 1 (TCP/IP Overview) first.

> **Note for the Learner:** This document is a complete textual transcription of the lecture's content, instructions, and demonstrations. It details the setup of a virtualized network lab and the static configuration of TCP/IP parameters on Linux and Windows.

---

## 1. Introduction and Lab Requirements
*(Transcript 0:00 - 1:56)*

The instructor introduces the session, emphasizing that we will set up the lab environment used for all future sessions (DHCP, SSH, Apache, etc.).

**Virtualization Software:**
*   The instructor uses **Oracle VirtualBox**.
*   *Note:* You are free to use VMware or other hypervisors, but the demonstrations use VirtualBox.
*   *Reference:* If you do not know how to create a VM, reference **Lecture 1** of his "Operating Systems with Linux" course on his YouTube channel/website.

**The Lab Topology (4 Machines):**
The instructor's host OS is **Macintosh**. Inside VirtualBox, he runs 4 Virtual Machines (VMs) for this course:
1.  **Ubuntu Server:** This will be the main server we configure services on (SSH, FTP, HTTP, etc.).
2.  **Kali Linux:** Used as a **Client** machine (Alternative: Ubuntu Desktop, CentOS, Red Hat).
3.  **Windows XP:** Used to test connectivity from a legacy Microsoft platform.
4.  **Windows 10:** Used as a modern Microsoft platform client (representing the "other extreme" from XP).

---

## 2. Network Configuration in VirtualBox
*(Transcript 2:35 - 4:05)*

The first step is to network these machines together.
*   **Action:** In VirtualBox Manager, select a machine -> **Settings** -> **Network**.
*   **Adapters:** VirtualBox allows up to 4 network adapters per machine.
*   **Adapter Type Selection:** By default, it might be NAT. The instructor strongly advises using the **Bridged Adapter**.

**Why Bridged Adapter?**
*   **Unique IP:** Every machine gets its own unique IP address from the physical network's router (e.g., your home PTCL router or University DHCP).
*   **Connectivity:** It places the VM on the *same* Local Area Network (LAN) as your Host OS.
*   **Advantages:**
    1.  VMs can access the Internet.
    2.  VMs can communicate with the Host OS.
    3.  VMs can communicate with *other devices* on the physical Wi-Fi/LAN (e.g., your mobile phone can ping the VM).

*   **Status:** The instructor has configured **Bridged Adapter** on all three VMs (Ubuntu, Kali, WinXP).

---

## 3. checking Current Configuration (Discovery)
*(Transcript 4:05 - 5:51)*

The instructor boots all machines and checks their current IPs assigned (likely by his home router's DHCP).

**Tools/Commands Used:**
*   **Linux (Kali/Ubuntu):** `ifconfig`
    *   (Note: `ipconfig` is for Windows, `ifconfig` is for Linux).
    *   Command: `ifconfig` (displays active interfaces).
*   **Windows:** `ipconfig`.

**Current Status Observed:**
1.  **Kali Linux:**
    *   Interface: `eth0`
    *   IP Address: `192.168.1.101`
    *   Netmask: `255.255.255.0`
    *   Broadcast: `192.168.1.255`
    *   MAC Addr: (48-bit address shown).
    *   Loopback: `127.0.0.1`.
2.  **Ubuntu Server:**
    *   Command: `ifconfig` (Requires `root` or `sudo`).
    *   IP Address: `192.168.1.100`.
3.  **Windows XP:**
    *   IP Address: `192.168.1.102`.

---

## 4. Testing Connectivity (Ping)
*(Transcript 5:51 - 9:33)*

**The Ping Command:**
*   Uses **ICMP Echo Request**.
*   Used to check connectivity between network hosts.

**Tests Performed:**
1.  **From Windows:** Pinging Ubuntu (`192.168.1.100`) -> **Success**.
2.  **From Kali:** Pinging Windows (`192.168.1.102`) -> **Success**.
3.  **External Connectivity:** Pinging another device on the physical network (`192.168.1.1`) -> **Success**.
4.  **Internet Connectivity:** `ping google.com` -> **Success**.

**The Gateway Concept:**
*   How can VMs access Google? Because they have a **Gateway**.
*   **Finding Gateway on Linux:** Command `route -n`.
    *   Output shows Gateway `192.168.1.1` (The instructor's home router).
*   **Finding Gateway on Windows:** `ipconfig /all`.
    *   Shows Default Gateway `192.168.1.1`.

---

## 5. Static IP Assignment (The Core Task)
*(Transcript 9:33 - End)*

Currently, IPs were assigned automatically by the home router's **DHCP** (Dynamic Host Configuration Protocol).
**Goal:** Manually (Statically) assign specific IPs to our machines to ensure they don't change.

### A. Assigning IP on Linux (Temporary / Runtime)
**(Demonstrated on Kali Linux)**

1.  **Clear current config:**
    *   `ip address flush dev eth0` (Removes all IPs from interface).
    *   Verify with `ifconfig` (no IP shown).
    *   Verify with `route -n` (no Gateway shown).
2.  **Assign IP:**
    *   Command: `ifconfig eth0 192.168.1.200 netmask 255.255.255.0 up`
    *   *Result:* IP `.200` assigned.
3.  **Add Gateway:**
    *   Command: `route add default gw 192.168.1.1`
    *   *Result:* Internet access restored.
4.  **Test:** `ping 192.168.1.1` -> **Success**.

*   **Problem:** This setting is **temporary**. If you reboot, it is lost.

### B. Permanent Static IP on Linux (Configuration File)
**(Demonstrated on Kali Linux)**

To make settings permanent, we edit the network interfaces file.
*   **File Path:** `/etc/network/interfaces`
*   **Editor:** `vim /etc/network/interfaces`

**Steps:**
1.  Open file. Locate the line for `eth0` (currently likely `iface eth0 inet dhcp`).
2.  Change `dhcp` to `static`.
3.  Add the parameters:
    ```bash
    iface eth0 inet static
        address 192.168.1.101
        netmask 255.255.255.0
        gateway 192.168.1.1
    ```
4.  Save and Exit.
5.  **Apply Changes:**
    *   Restart interface: `ifdown eth0` then `ifup eth0`.
    *   *Troubleshooting:* The instructor faced an issue where the old IP (`.200`) persisted because it was manually assigned in the current session.
    *   *Fix:* He flushed the address again (`ip address flush dev eth0`) and then ran `ifup eth0`.
    *   *Result:* The machine took the IP `192.168.1.101` from the file.

### C. Static IP on Windows (GUI Method)
**(Demonstrated on Windows XP)**

1.  **Navigate:** Start -> Control Panel -> Network Connections -> Local Area Connection.
2.  **Properties:** Right-Click -> Properties.
3.  **Protocol:** Select "Internet Protocol (TCP/IP)" -> Properties.
4.  **Configure:**
    *   Select "Use the following IP address".
    *   **IP Address:** `192.168.1.102`
    *   **Subnet Mask:** `255.255.255.0`
    *   **Default Gateway:** `192.168.1.1`
5.  **Result:** `ipconfig` confirms the static IP `1.102`.

---

## 6. Hostname Resolution (The `/etc/hosts` file)
*(Transcript 20:05 - 24:00)*

**Problem:** We can ping IPs, but we cannot ping names (e.g., `ping ubuntuserver` fails).
**Solution:** We don't have a DNS server yet, so we use the **Local Hosts File** (`/etc/hosts`).

### On Linux (Kali)
1.  **File Config:** Open `/etc/hosts` in an editor.
2.  **Add Mappings:**
    ```text
    192.168.1.100   ubuntuserver
    192.168.1.101   kali
    192.168.1.102   winxp
    ```
3.  **Test:** `ping ubuntuserver` -> **Success**. `ping winxp` -> **Success**.

### On Windows (XP)
1.  **File Location:** `C:\Windows\System32\drivers\etc\hosts`.
    *   *(Note: File has no extension).*
2.  **Config:** Open with Notepad.
3.  **Add Mappings:** Same as above.
4.  **Test:** `ping kali` from Windows Command Prompt -> **Success**.

---

## Summary
In this session, we:
1.  Built a 3-VM lab (Ubuntu, Kali, XP).
2.  Configured **Bridged Networking** in VirtualBox.
3.  Learned temporary (`ifconfig`) and permanent (`/etc/network/interfaces`) static IP assignment on Linux.
4.  configured Static IP on Windows via GUI.
5.  Configured local name resolution using the `/etc/hosts` file (Linux) and `hosts` file (Windows).

**Next Session:** We will learn to automate this using a **DHCP Server**.
