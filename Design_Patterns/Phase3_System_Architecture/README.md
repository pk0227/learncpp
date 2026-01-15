# 🏗️ System Architecture Patterns

## Important Distinction: Patterns vs Protocols

**This is NOT the Network Protocols Module.**

This directory covers **Architectural Design Patterns** used in high-performance systems.
*   **Here (Design Patterns)**: We study the *structure* of a server (e.g., "How do I handle 10k connections without creating 10k threads?"). The answer is the **Reactor Pattern**.
*   **Next Module (Network Protocols)**: We will study the *implementation* (e.g., "How do I construct a TCP packet? How does `SO_REUSEADDR` work?").

## The Goal
To design systems that are:
1.  **Scalable**: Handle C10k (10,000 concurrent clients).
2.  **Responsive**: Never block the main thread.
3.  **Efficient**: Minimizing Context Switching and CPU Cache invalidation.

## Contents
1.  **Concurrency**:
    *   **Thread Pool**: Managing a fixed set of workers to process tasks.
    *   **Active Object**: Decoupling method execution from method invocation.
2.  **Networking**:
    *   **Reactor**: Non-blocking Synchronous Event Demultiplexing (Linux `epoll`).
    *   **Proactor**: Asynchronous I/O (Windows IOCP, modern Linux `io_uring`).
