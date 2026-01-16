# 🎤 Top 50 Linux Network Programming Questions
> **Strictly for Senior (L5/L6) Roles**

## 🟢 Basics (The Filter)

1.  **What is a File Descriptor?** (Index into kernel open file table).
2.  **Difference between Process and Thread?** (Shared memory vs Isolated memory).
3.  **What is a Zombie Process?** (Child exited, Parent didn't `wait()`).
4.  **How does `fork()` work?** (Copy-on-Write pages).
5.  **TCP vs UDP header size?** (TCP=20 bytes min, UDP=8 bytes).
6.  **Does partial read happen on UDP?** (No, it's message based. It's all or nothing).
7.  **What is LO (Loopback)?** (Virtual interface `127.0.0.1`, bypasses NIC hardware).
8.  **What is Little Endian?** (LSB at lowest address. Common in x86).
9.  **What is ARP?** (Maps IP to MAC).
10. **Difference between `close()` and `shutdown()`?** (RefCount-- vs sending FIN).

## 🟡 Internals (The Standard)

11. **Explain the 3-Way Handshake.** (SYN, SYN-ACK, ACK).
12. **Why is the Sequence Number Random?** (Security + Confusion with old packets).
13. **What is `TIME_WAIT`?** (Waiting for delayed packets to die).
14. **What is `CLOSE_WAIT`?** (App forgot to close socket after receiving FIN).
15. **What is Nagle's Algorithm?** (Coalescing small packets. Bad for gaming).
16. **What is Delayed ACK?** (Waiting 40/500ms to piggyback ACK on data).
17. **Why Nagle + Delayed ACK = Bad?** (Write-Write-Read interaction latency spike).
18. **What is MTU?** (1500 bytes standard Ethernet).
19. **What happens if packet > MTU?** (IP Fragmentation).
20. **Why is Fragmentation bad?** (Loss of 1 fragment = Loss of entire packet).

## 🔴 High Performance (The Senior)

21. **Select vs Epoll?** (O(N) vs O(1)).
22. **Level Triggered vs Edge Triggered?** (Notify always vs Notify on change).
23. **What is the C10k problem?** (Thread-per-client scaling limit).
24. **How to tune Linux for 1M connections?** (`ulimit -n`, `fs.file-max`, Ephemeral ports).
25. **What is Zero Copy?** (`sendfile`, `splice`).
26. **What is `SO_REUSEADDR`?** (Bind to port in `TIME_WAIT` state).
27. **What is `SO_REUSEPORT`?** (Multiple threads bind same port for load balancing).
28. **What is `TCP_NODELAY`?** (Disables Nagle).
29. **What is `TCP_CORK`?** (Aggressive buffering, opposite of NODELAY).
30. **What is `SO_KEEPALIVE` vs App Heartbeat?** (TCP Keepalive is too slow/dumb. App heartbeat works better).

## 🟣 Advanced / Kernel (The Staff)

31. **What is a Raw Socket?** (Bypasses Transport Layer).
32. **What is Promiscuous Mode?** (NIC passes all traffic to CPU).
33. **What is `BPF` (Berkeley Packet Filter)?** (In-kernel VM for filtering packets efficiently).
34. **What is `XDP` (eXpress Data Path)?** (Process packets *before* sk_buff allocation).
35. **What is `recvmsg` vs `recv`?** (Access to ancillary data/control messages).
36. **How does `getaddrinfo` block?** (DNS resolution issues).
37. **What is `epoll` Thundering Herd?** (Multiple waiters waking up).
38. **What is `eventfd`?** (Lightweight counter for inter-thread notification in epoll).
39. **What is `signalfd`?** (Handling signals via file descriptor).
40. **What is `timerfd`?** (Handling timers via file descriptor).

## ⚪ Scenario Based (The Architect)

41. **Design a Load Balancer.** (NAT vs DSR (Direct Server Return)).
42. **Debug high latency on a specific server.** (`tcpdump`, `strace`, `perf`).
43. **Handle a DDoS attack.** (Syn Cookies, Rate Limiting).
44. **Why is my throughput stuck at 1Gbps on a 10Gbps card?** (CPU affinity, interrupts).
45. **How to secure a socket?** (SSL/TLS integration).
46. **What is Head-of-Line Blocking in HTTP/1.1?** (One slow request blocks the connection).
47. **How does UDP solve Head-of-Line Blocking (QUIC)?** (Independent streams).
48. **When would you choose busy-waiting over sleeping?** (Microsecond latency needed).
49. **How to pass a File Descriptor between processes?** (UNIX Domain Socket + `SCM_RIGHTS`).
50. **What is `defer_accept`?** (Wake up listener only when data arrives, not just handshake).
