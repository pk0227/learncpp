# Linux C++ Concurrency & Systems Programming Master Study Guide & Roadmap

> **Target Audience:** Systems Software Engineers, High-Frequency Trading (HFT) Engineers, and Senior/Staff Infrastructure Engineers preparing for advanced C++ and Linux systems programming interviews.

---

## 📑 Table of Contents

1. [Operating System Architecture & Platform Guidelines](#1-operating-system-architecture--platform-guidelines)
2. [Master 5-Phase Learning Roadmap](#2-master-5-phase-learning-roadmap)
   - [Phase 1: Shared-Memory Multithreading (38 Files)](#phase-1-shared-memory-multithreading-38-files)
   - [Phase 2: Multiprocessing & Process Lifecycle (14 Files)](#phase-2-multiprocessing--process-lifecycle-14-files)
   - [Phase 3: Deep Dive: Inter-Process Communication (IPC) & Signals (8 Files)](#phase-3-deep-dive-inter-process-communication-ipc--signals-8-files)
   - [Phase 4: Modern C++ Asynchrony & Event-Driven Patterns (16 Files)](#phase-4-modern-c-asynchrony--event-driven-patterns-16-files)
   - [Phase 5: Distributed-Memory Parallelism with MPI (11 Files)](#phase-5-distributed-memory-parallelism-with-mpi-11-files)
3. [Concurrency vs Multiprocessing Architecture Matrix](#3-concurrency-vs-multiprocessing-architecture-matrix)
4. [Linux IPC Mechanism Performance & Latency Hierarchy](#4-linux-ipc-mechanism-performance--latency-hierarchy)
5. [Senior / Staff Interview Preparation Framework](#5-senior--staff-interview-preparation-framework)
   - [High-Frequency Architectural Questions](#high-frequency-architectural-questions)
   - [Common Production Traps & Pitfalls](#common-production-traps--pitfalls)
6. [Executive Cheat Sheet: When to Use What](#6-executive-cheat-sheet-when-to-use-what)
7. [Verification & Compilation Instructions](#7-verification--compilation-instructions)

---

## 1. Operating System Architecture & Platform Guidelines

This curriculum teaches **native Linux/POSIX systems programming and modern C++ (C++17/C++20/C++23)**.

### Target Environment
- **Native Linux (Ubuntu 20.04+, Debian, Fedora, RHEL)**: The primary deployment environment.
- **Windows / WSL2**: Developers on Windows should use **WSL2 (Windows Subsystem for Linux)** to execute POSIX system calls (`fork`, `pipe`, `shm_open`, `sigaction`, etc.).

### Build Dependencies
All 87 `.cpp` files in this repository compile cleanly under `g++ -std=c++20`:
```bash
# Compiler & build tools
sudo apt update && sudo apt install -y build-essential

# Optional real-time & threading flags
g++ -std=c++20 -pthread -lrt example.cpp -o example
```

---

## 2. Master 5-Phase Learning Roadmap

### Phase 1: Shared-Memory Multithreading (38 Files)

> Focuses on single-process concurrency, thread synchronization, mutex lock hierarchies, race conditions, memory visibility, and thread pool architectures.

| File Name & Link | Core Concept & Mechanics |
|---|---|
| [`01_single_worker_thread.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/01_single_worker_thread.cpp) | Basic `std::thread` spawning, callable execution, and `.join()`. |
| [`01a_raw_pthread.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/01a_raw_pthread.cpp) | Low-level POSIX `pthread_create()`, `pthread_join()`, and thread attributes. |
| [`02_thread_subclass.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/02_thread_subclass.cpp) | Encapsulating thread execution within an object-oriented class wrapper. |
| [`03_multiple_worker_threads.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/03_multiple_worker_threads.cpp) | Managing collections of worker threads with `std::vector<std::thread>`. |
| [`04_race_condition.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/04_race_condition.cpp) | Demonstrates data corruption caused by unsynchronized concurrent increments. |
| [`05_mutex.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/05_mutex.cpp) | Protecting critical sections using `std::mutex` and RAII `std::lock_guard`. |
| [`06_semaphore.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/06_semaphore.cpp) | Controlling bounded concurrent access using semaphore patterns. |
| [`07_producer_consumer.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/07_producer_consumer.cpp) | Classic synchronized queue using `std::mutex` and `std::condition_variable`. |
| [`08_fetch_parallel.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/08_fetch_parallel.cpp) | Concurrent HTTP I/O fetching with thread-level parallelization. |
| [`09_merge_sort.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/09_merge_sort.cpp) | Divide-and-conquer parallel sorting using recursive thread spawning. |
| [`10_schedule_every_n_sec.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/10_schedule_every_n_sec.cpp) | Periodic task scheduler with interval timing and thread synchronization. |
| [`11_barrier.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/11_barrier.cpp) | Multi-phase cyclic synchronization barrier for multi-stage computation. |
| [`12_thread_local_storage.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/12_thread_local_storage.cpp) | Per-thread private state using `thread_local` storage duration. |
| [`13_thread_pool.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/13_thread_pool.cpp) | Fixed worker pool reusing threads via a synchronized work queue. |
| [`14_reader_writer_lock.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/14_reader_writer_lock.cpp) | Multiple concurrent readers with exclusive writer using shared locks. |
| [`atomic.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/atomic.cpp) | Lock-free synchronization with `std::atomic<T>` and atomic operations. |
| [`barrier.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/barrier.cpp) | Phase-based thread synchronization using POSIX barriers. |
| [`condition_variable.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/condition_variable.cpp) | Thread suspension and notification with `wait()`, `notify_one()`, and predicate. |
| [`deadlock_philosophers.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/deadlock_philosophers.cpp) | The Dining Philosophers problem demonstrating circular wait deadlock. |
| [`deadlock_philosophers_solved.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/deadlock_philosophers_solved.cpp) | Deadlock resolution using hierarchical resource ordering and `std::scoped_lock`. |
| [`detached_thread.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/detached_thread.cpp) | Background daemon threads using `std::thread::detach()` and lifecycle caveats. |
| [`download_images.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/download_images.cpp) | I/O-bound parallel downloading comparing sequential vs concurrent runtimes. |
| [`is_joinable.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/is_joinable.cpp) | Safely inspecting `.joinable()` state before joining or detaching. |
| [`latch.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/latch.cpp) | Single-use thread countdown latch for multi-worker startup synchronization. |
| [`livelock.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/livelock.cpp) | Livelock demonstration where threads actively yield without making progress. |
| [`matrix_multiply.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/matrix_multiply.cpp) | Multithreaded parallel matrix multiplication partitioning rows across cores. |
| [`merge_sort.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/merge_sort.cpp) | Production recursive parallel merge sort bounded by hardware concurrency. |
| [`producer_consumer.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/producer_consumer.cpp) | Classic producer-consumer queue with condition variables and bounded buffer. |
| [`race_condition.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/race_condition.cpp) | Deep dive into data race undefined behavior and atomic remedies. |
| [`recursive_mutex.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/recursive_mutex.cpp) | Re-entrant locking using `std::recursive_mutex` in recursive algorithms. |
| [`semaphore.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/semaphore.cpp) | Counting semaphore implementation for bounded resource pools. |
| [`shared_mutex.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/shared_mutex.cpp) | High-performance C++17 `std::shared_mutex` for read-heavy cache architectures. |
| [`simple_data_race.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/simple_data_race.cpp) | Minimal reproduction of data races on shared primitive integers. |
| [`simple_mutex.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/simple_mutex.cpp) | Mutex locking and unlocking mechanics with RAII wrappers. |
| [`starvation.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/starvation.cpp) | Demonstrates thread starvation caused by greedy priority locks. |
| [`thread_life_cycle.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/thread_life_cycle.cpp) | Complete state diagram: New $\to$ Ready $\to$ Running $\to$ Blocked $\to$ Terminated. |
| [`thread_pool.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/thread_pool.cpp) | Scalable production thread pool supporting asynchronous task submission. |
| [`try_lock.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multithreading/try_lock.cpp) | Non-blocking lock acquisition using `.try_lock()` to avoid deadlock. |

---

### Phase 2: Multiprocessing & Process Lifecycle (14 Files)

> Explores POSIX process creation, isolated memory spaces, Copy-on-Write (COW), process supervision, and zombie/orphan lifecycle management.

| File Name & Link | Core Concept & Mechanics |
|---|---|
| [`01_basic_process.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multiprocessing/01_basic_process.cpp) | The `fork()` system call, dual return values, and parent-child execution paths. |
| [`02_multiple_processes.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multiprocessing/02_multiple_processes.cpp) | Spawning and managing fan-outs of multiple child worker processes. |
| [`03_deadlock.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multiprocessing/03_deadlock.cpp) | Cross-process deadlock when competing for multiple inter-process resources. |
| [`04_process_pool.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multiprocessing/04_process_pool.cpp) | Supervising a pool of long-lived worker processes over IPC channels. |
| [`05_queue_communication.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multiprocessing/05_queue_communication.cpp) | Inter-process task dispatch using POSIX message queues. |
| [`06_pipe_communication.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multiprocessing/06_pipe_communication.cpp) | Parent-to-child data streaming using kernel anonymous pipes (`pipe()`). |
| [`07_shared_value.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multiprocessing/07_shared_value.cpp) | Sharing primitive values across process boundaries via anonymous `mmap()`. |
| [`08_shared_array.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multiprocessing/08_shared_array.cpp) | Managing shared structured buffers across isolated processes. |
| [`09_manager.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multiprocessing/09_manager.cpp) | Process supervisor pattern managing worker health, restarts, and task routing. |
| [`10_process_lock.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multiprocessing/10_process_lock.cpp) | Mutual exclusion across processes using process-shared mutexes. |
| [`11_process_semaphore.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multiprocessing/11_process_semaphore.cpp) | Process-shared semaphores (`pshared=1`) for multi-process resource pools. |
| [`12_process_barrier.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multiprocessing/12_process_barrier.cpp) | Cross-process execution synchronization barrier via shared memory. |
| [`13_orphan.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multiprocessing/13_orphan.cpp) | Orphan process lifecycle, parent termination, and re-parenting to PID 1 / subreaper. |
| [`14_zombie.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/multiprocessing/14_zombie.cpp) | Zombie process creation (`TASK_DEAD`), process table slot exhaustion, and `waitpid()` reaping. |

---

### Phase 3: Deep Dive: Inter-Process Communication (IPC) & Signals (8 Files)

> Production-grade Linux IPC implementations with system call ergonomics, error handling, atomicity rules, and signal architectures.

| File Name & Link | Core Concept & Mechanics |
|---|---|
| [`01_basic_signals.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/IPC_and_Signals/01_basic_signals.cpp) | Signal registration with `sigaction()`, async-signal safety, and safe shutdown. |
| [`02_signal_blocking_masking.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/IPC_and_Signals/02_signal_blocking_masking.cpp) | Signal masks, `sigprocmask()`, pending signal inspection, and `sigsuspend()`. |
| [`03_unnamed_pipes.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/IPC_and_Signals/03_unnamed_pipes.cpp) | Anonymous pipes (`pipe()`), `PIPE_BUF` atomic write limits, and `SIGPIPE` handling. |
| [`04_named_pipes_fifos.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/IPC_and_Signals/04_named_pipes_fifos.cpp) | Filesystem FIFOs (`mkfifo()`) enabling IPC between unrelated processes. |
| [`05_message_queues.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/IPC_and_Signals/05_message_queues.cpp) | POSIX Message Queues (`mq_open()`, `mq_send()`, `mq_receive()`) with priority ordering. |
| [`06_shared_memory.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/IPC_and_Signals/06_shared_memory.cpp) | POSIX shared memory objects (`shm_open()`, `ftruncate()`, `mmap()`) for zero-copy IPC. |
| [`07_memory_mapped_files.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/IPC_and_Signals/07_memory_mapped_files.cpp) | File-backed memory mapping (`MAP_SHARED`), page dirtying, and `msync()` persistence. |
| [`08_posix_semaphores.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/IPC_and_Signals/08_posix_semaphores.cpp) | Named (`sem_open()`) and unnamed (`sem_init()`) POSIX counting semaphores. |

---

### Phase 4: Modern C++ Asynchrony & Event-Driven Patterns (16 Files)

> Asynchronous task coordination using `std::async`, futures, promises, packaged tasks, and coroutines.

| File Name & Link | Core Concept & Mechanics |
|---|---|
| [`01_basic_async.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/asynchrony/01_basic_async.cpp) | Launch policies (`std::launch::async` vs `deferred`) and non-blocking execution. |
| [`02_future_create_task.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/asynchrony/02_future_create_task.cpp) | Creating deferred asynchronous tasks and extracting values via `std::future`. |
| [`03_future_callback.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/asynchrony/03_future_callback.cpp) | Simulating future completion notification and callback execution. |
| [`04_pause_resume.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/asynchrony/04_pause_resume.cpp) | Asynchronous task suspension and resumption flows. |
| [`05_run_heavy_functions.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/asynchrony/05_run_heavy_functions.cpp) | Offloading heavy CPU-bound computation without blocking caller threads. |
| [`06_data_sharing_queue.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/asynchrony/06_data_sharing_queue.cpp) | Thread-safe asynchronous message queue coordinating producer and consumer tasks. |
| [`07_semaphore.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/asynchrony/07_semaphore.cpp) | Bounding concurrent asynchronous operations using semaphores. |
| [`08_producer_consumer.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/asynchrony/08_producer_consumer.cpp) | Asynchronous pipeline pattern using promises and futures. |
| [`09_fetch_parallel.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/asynchrony/09_fetch_parallel.cpp) | Concurrent I/O fetching comparing sequential time ($O(\sum T_i)$) vs async time ($O(\max T_i)$). |
| [`10_mutex.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/asynchrony/10_mutex.cpp) | Protecting shared state accessed by multiple asynchronous futures. |
| [`11_barrier.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/asynchrony/11_barrier.cpp) | Multi-task rendezvous barrier coordinating async future stages. |
| [`12_async_generator.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/asynchrony/12_async_generator.cpp) | Modern asynchronous generator patterns for stream processing. |
| [`13_async_server.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/asynchrony/13_async_server.cpp) | Non-blocking concurrent server architecture handling multiple client streams. |
| [`14_distributed_computing.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/asynchrony/14_distributed_computing.cpp) | Distributed task decomposition across simulated worker nodes. |
| [`future.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/asynchrony/future.cpp) | Direct usage of `std::promise` and `std::future` for one-shot channel communication. |
| [`recursive_sum.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/asynchrony/recursive_sum.cpp) | Parallel divide-and-conquer array summation using recursive `std::async`. |

---

### Phase 5: Distributed-Memory Parallelism with MPI (11 Files)

> Message Passing Interface (MPI) standards for distributed-memory supercomputing, cluster computing, and HPC applications where memory is physically partitioned across multiple nodes.

| File Name & Link | Core Concept & Mechanics |
|---|---|
| [`hello_world_mpi.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/mpi/hello_world_mpi.cpp) | MPI environment lifecycle: `MPI_Init`, `MPI_Finalize`, `MPI_Comm_rank`, and `MPI_Comm_size`. |
| [`point_to_point_communication.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/mpi/point_to_point_communication.cpp) | Blocking peer-to-peer messaging using `MPI_Send` and `MPI_Recv`. |
| [`Broadcast Communication.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/mpi/Broadcast%20Communication.cpp) | One-to-all collective communication using `MPI_Bcast`. |
| [`Reduce Operation.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/mpi/Reduce%20Operation.cpp) | Distributed reduction (sum, min, max) using `MPI_Reduce`. |
| [`Non-Blocking Communication.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/mpi/Non-Blocking%20Communication.cpp) | Overlapping computation with communication via `MPI_Isend`, `MPI_Irecv`, and `MPI_Wait`. |
| [`Derived Data Types.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/mpi/Derived%20Data%20Types.cpp) | Custom struct serialization with `MPI_Type_create_struct` and `MPI_Type_commit`. |
| [`Communicators and Groups.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/mpi/Communicators%20and%20Groups.cpp) | Subdividing process spaces with `MPI_Comm_split` into custom communicators. |
| [`Cartesian Topology.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/mpi/Cartesian%20Topology.cpp) | Grid and torus process topologies using `MPI_Dims_create` and `MPI_Cart_create`. |
| [`Asynchronous Communication.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/mpi/Asynchronous%20Communication.cpp) | Dynamic message size probing using `MPI_Probe` and `MPI_Get_count`. |
| [`Parallel Matrix Multiplication.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/mpi/Parallel%20Matrix%20Multiplication.cpp) | Distributed matrix multiplication using `MPI_Scatter`, `MPI_Bcast`, and `MPI_Gather`. |
| [`Parallel Computation of Pi.cpp`](file:///home/prashanth/Learnings/learncpp/Linux-Internals/src/mpi/Parallel%20Computation%20of%20Pi.cpp) | Numerical integration for $\pi$ with distributed partial sums and `MPI_Reduce`. |

---

## 3. Concurrency vs Multiprocessing Architecture Matrix

| Dimension | Multithreading (`std::thread`) | Multiprocessing (`fork()`) | Distributed Computing (`MPI`) |
|---|---|---|---|
| **Memory Model** | **Shared Memory Space** (same page tables) | **Isolated Memory Spaces** (Copy-on-Write) | **Physically Disjoint Memory** (network / fabric) |
| **Communication** | Direct memory reads/writes + synchronization | IPC (Shared Memory, Sockets, Pipes, Queues) | Message passing over interconnect (Infiniband, RoCE) |
| **Fault Isolation** | **Poor**: A segfault in one thread crashes the whole process | **Strong**: A crashed child leaves the parent intact | **Strong**: Node failures isolated by cluster orchestrator |
| **Creation Overhead** | Very Low (~ 10–50 $\mu$s, 8KB–2MB stack) | Moderate (~ 500–1500 $\mu$s for page table clone) | High (process launch across nodes) |
| **Scalability Limit** | Bounded by single machine CPU cores & RAM bus | Bounded by single machine OS PID limit & RAM | Scalable to tens of thousands of cluster nodes |
| **Synchronization Primitives** | `std::mutex`, `std::atomic`, condition variables | POSIX semaphores, file locks, socket handshakes | MPI barriers, collective reductions, non-blocking requests |

---

## 4. Linux IPC Mechanism Performance & Latency Hierarchy

```mermaid
graph TD
    A[Linux IPC Options] --> B[Shared Memory / mmap]
    A --> C[Unix Domain Sockets]
    A --> D[Anonymous / Named Pipes]
    A --> E[POSIX Message Queues]
    A --> F[TCP Loopback]
    
    B --- B1[Throughput: >10 GB/s | Latency: Sub-microsecond | Zero-Copy]
    C --- C1[Throughput: 2-4 GB/s | Latency: 2-5 us | SCM_RIGHTS FD Passing]
    D --- D1[Throughput: 1-3 GB/s | Latency: 3-8 us | Byte Stream]
    E --- E1[Throughput: ~500 MB/s | Latency: 5-10 us | Prioritized Messages]
    F --- F1[Throughput: 500 MB-1.5 GB/s | Latency: 15-30 us | Network Stack Overhead]
```

---

## 5. Senior / Staff Interview Preparation Framework

### High-Frequency Architectural Questions

#### Q1: "Why does calling `fork()` inside a multithreaded application cause deadlocks?"
**Answer:** Under POSIX, `fork()` creates a child process containing **only the calling thread**. All other threads vanish without running cleanup or unlocking mutexes. If any other thread was inside a critical section (e.g. glibc `malloc()` holding the heap arena lock), that mutex is copied into the child in a locked state. Any subsequent allocation in the child deadlocks forever. After `fork()` in a multithreaded application, the child must **strictly call only async-signal-safe functions or immediately invoke `execve()`**.

#### Q2: "What is the `std::future` destructor trap when using `std::async`?"
**Answer:** The destructor of an `std::future` returned by `std::async(std::launch::async, ...)` **blocks until the asynchronous task completes**. If you discard the return value (`std::async(std::launch::async, f); std::async(std::launch::async, g);`), the temporary future is destroyed at the semicolon, forcing the tasks to run **completely sequentially**!

#### Q3: "What are zombie processes, and how do you properly reap them in Linux?"
**Answer:** A zombie (`TASK_DEAD`, status `Z`) has exited and freed its memory, but retains an entry in the kernel process table so the parent can inspect its exit status via `waitpid()`. If not reaped, zombies exhaust the system PID space (`/proc/sys/kernel/pid_max`). Because standard signals are not queued, a single `SIGCHLD` can represent multiple dead children. The only correct way to reap them is a non-blocking loop inside the `SIGCHLD` handler:
```cpp
while (waitpid(-1, &status, WNOHANG) > 0) {}
```

#### Q4: "Explain the difference between `std::memory_order_relaxed`, `acquire`, `release`, and `seq_cst`."
**Answer:**
- `relaxed`: Guarantees atomicity of the single operation only. No reordering fences.
- `acquire`: Guarantees that subsequent memory reads/writes cannot be reordered before this load. Synchronizes with a `release` store.
- `release`: Guarantees that prior memory reads/writes cannot be reordered after this store.
- `seq_cst`: Full sequential consistency with a globally agreed total order across all CPU cores.

---

## 6. Executive Cheat Sheet: When to Use What

| Requirement | Optimal Choice | Justification |
|---|---|---|
| **Ultra-low latency inter-thread communication** | Lock-free ring buffer (`std::atomic`) | Zero kernel context switch, cache-coherent memory access. |
| **Protecting shared data structures in threads** | `std::mutex` + `std::unique_lock` / `std::scoped_lock` | RAII prevents deadlocks; supports condition variables. |
| **Read-heavy, rarely modified in-memory cache** | `std::shared_mutex` with `std::shared_lock` | Allows infinite parallel concurrent readers; exclusive writer lock. |
| **High-frequency cross-process data sharing** | POSIX Shared Memory (`shm_open` + `mmap`) | Zero-copy user-space memory access; fastest IPC possible. |
| **Bidirectional microservice messaging on same machine** | Unix Domain Sockets (`AF_UNIX`, `SOCK_STREAM`) | High throughput; can pass open file descriptors via `SCM_RIGHTS`. |
| **Simple parent-child data streaming** | Unnamed Pipes (`pipe()`) | Lightweight FIFO byte stream buffered in kernel (64KB). |
| **Thread-safe signal handling in multithreaded servers** | Dedicated signal thread running `sigwait()` | Replaces unsafe asynchronous handlers with clean synchronous execution. |
| **Submitting thousands of asynchronous I/O requests** | Linux `io_uring` (or Boost.Asio / epoll) | Shared ring buffers between kernel and user space; zero syscall overhead. |
| **Supercomputing / Cluster distributed computing** | MPI (`MPI_Send`, `MPI_Recv`, `MPI_Reduce`) | High-performance interconnects across physically distributed machines. |

---

## 7. Verification & Compilation Instructions

To verify that all 87 source files in `src/` compile cleanly on your Linux machine:

```bash
# Verify all multithreading examples
for f in src/multithreading/*.cpp; do g++ -std=c++20 -fsyntax-only -pthread "$f"; done

# Verify all multiprocessing examples
for f in src/multiprocessing/*.cpp; do g++ -std=c++20 -fsyntax-only -pthread "$f"; done

# Verify all IPC & Signals examples
for f in src/IPC_and_Signals/*.cpp; do g++ -std=c++20 -fsyntax-only -pthread -lrt "$f"; done

# Verify all asynchrony examples
for f in src/asynchrony/*.cpp; do g++ -std=c++20 -fsyntax-only -pthread "$f"; done

# Verify all MPI examples (using self-contained fallback or mpic++)
for f in src/mpi/*.cpp; do g++ -std=c++20 -fsyntax-only -I src/mpi "$f"; done
```

> [!NOTE]
> All 87 files in `src/` have been verified to compile with **0 errors and 0 warnings** under `g++ -std=c++20`.
