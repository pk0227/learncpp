
🧠 HOW PAGE CACHE INTERACTS WITH fork() AND COPY-ON-WRITE (COW)

-------------------------------------------------------------------------------
✨ 1. CORE IDEA (ONE-LINER)
-------------------------------------------------------------------------------
Page cache is shared kernel memory and is **NEVER duplicated** by fork().

Copy-On-Write (COW) depends entirely on **HOW pages are MAPPED**:
- 🔒 Private mappings  → COW
- 🔓 Shared mappings   → No COW

COW is **NOT** a property of the page cache itself.

-------------------------------------------------------------------------------
🧩 2. THREE KEY CONCEPTS TO REMEMBER
-------------------------------------------------------------------------------

### 1) PAGE CACHE 📦
- Kernel memory that caches **disk-backed file data**
- Shared globally by **all processes**
- Indexed by `(inode, file offset)`
- Not owned by any single process
- Never duplicated on fork()

### 2) fork() 🧬
- Creates a **child process**
- Duplicates the parent's **virtual memory mappings (VMAs)**
- Copies **page tables**, not physical memory
- Very cheap operation

### 3) COPY-ON-WRITE (COW) 🪄
- Pages are **shared initially**
- Pages are marked **read-only**
- Actual copy happens **ONLY on write**
- Implemented via:
  - Page fault
  - New page allocation
  - Data copy

-------------------------------------------------------------------------------
🧪 3. fork() + ANONYMOUS MEMORY (CLASSIC COW)
-------------------------------------------------------------------------------

### Memory types involved
- Stack
- Heap
- Global / BSS / Data segment

### Before fork()

```
Parent Process
Virtual Address  -->  Physical Page P1 (Read-Write)
```

### After fork()

```
Parent Process
Virtual Address  -->  Physical Page P1 (Read-Only + COW)

Child Process
Virtual Address  -->  Physical Page P1 (Read-Only + COW)
```

- Same physical page
- Page tables duplicated
- Physical memory NOT copied

### On write by ONE process

```
1) Page fault occurs
2) Kernel allocates new physical page
3) Old data is copied
4) Writing process gets private page
```

This is **CLASSIC COW**

Key insight:
Classic fork() memory behaves like an **IMPLICIT MAP_PRIVATE mapping**.

-------------------------------------------------------------------------------
📂 4. fork() + PAGE CACHE (READ-ONLY FILE ACCESS)
-------------------------------------------------------------------------------

### Scenario

```
open("file")
read(fd, buf, 4096)
fork()
```

### What happens internally
- File data already exists in **page cache**
- Page cache is **kernel-owned**
- fork() does **NOT** duplicate page cache

### After fork()

```
Parent Process  -->  Page Cache Page X
Child Process   -->  Page Cache Page X
```

### Reads by both processes
- Served from same page cache page
- No page fault
- No COW
- Extremely fast

Golden rule:
**Page cache pages are ALWAYS shared**

-------------------------------------------------------------------------------
✍️ 5. fork() + write() TO A FILE (IMPORTANT)
-------------------------------------------------------------------------------

### If parent or child does

```
write(fd, data, size)
```

### Then
- Write modifies **page cache**
- Page becomes dirty
- Eventually flushed to disk
- Change is visible to **all processes**

This is **NOT COW**

Reason:
- File semantics require shared visibility
- Page cache consistency must be preserved

-------------------------------------------------------------------------------
🗺 6. mmap() + fork() + MAP_PRIVATE (COW WITH FILES)
-------------------------------------------------------------------------------

### Code flow

```
mmap(file, MAP_PRIVATE)
fork()
```

### Initial state

```
Parent VA  -->  Page Cache Page X (Read-Only)
Child  VA  -->  Page Cache Page X (Read-Only)
```

### On write

```
1) Page fault
2) Kernel allocates anonymous page
3) Copies data from page cache
4) Writing process uses private page
```

### Final state

```
Writer Process --> Anonymous Private Page
Other Process  --> Page Cache Page X
```

This **IS COW**

Important:
COW happens because the mapping is **PRIVATE**,  
NOT because page cache itself supports COW.

-------------------------------------------------------------------------------
🤝 7. mmap() + MAP_SHARED (NO COW)
-------------------------------------------------------------------------------

### Code

```
mmap(file, MAP_SHARED)
```

### Write behavior
- Writes go directly to page cache
- Visible to all processes
- No private copy created

Used for:
- IPC
- Shared memory via files
- Databases

-------------------------------------------------------------------------------
❗ 8. VERY IMPORTANT CLARIFICATION
-------------------------------------------------------------------------------

**Question**  
"In classic COW, is the page mmaped private internally?"

**Answer**
- YES — semantically
- NO  — syscall-wise

**Explanation**
- Kernel does NOT call mmap()
- Kernel marks existing private VMAs as COW
- Behavior is IDENTICAL to MAP_PRIVATE

Correct wording:
> "The kernel converts existing private mappings into copy-on-write mappings during fork()."

-------------------------------------------------------------------------------
📊 9. SUMMARY TABLE (MEMORIZE THIS)
-------------------------------------------------------------------------------

```
Case                              | Shared Initially | COW on Write
----------------------------------|------------------|-------------
fork() + heap/stack               | Yes              | Yes
fork() + read() from file         | Yes              | No
fork() + write() to file          | Yes              | No
mmap(MAP_PRIVATE) + write         | Yes              | Yes
mmap(MAP_SHARED) + write          | Yes              | No
Page cache itself                 | Yes              | Never
```

-------------------------------------------------------------------------------
🧠 10. FINAL MENTAL MODEL (GOLD)
-------------------------------------------------------------------------------

```
Disk
 |
 v
Page Cache  (shared kernel memory)
 |
 +-----------------------------------+
 | read()                -> no COW   |
 | mmap(MAP_SHARED)      -> no COW   |
 | mmap(MAP_PRIVATE)     -> COW      |
 | fork() private memory -> COW      |
 +-----------------------------------+
```

-------------------------------------------------------------------------------
🎯 11. ONE-LINE INTERVIEW ANSWER
-------------------------------------------------------------------------------

Copy-On-Write is not a property of the page cache itself; it is a property of how
a page is mapped (private vs shared), and fork() enforces COW only for private
anonymous or private file-backed mappings.


🧪 PROVING COPY-ON-WRITE (COW) USING A C++ PROGRAM

### Goal of this demo
- Prove fork() does NOT copy memory immediately
- Prove parent & child initially SHARE the same physical page
- Prove WRITE triggers the copy (COW)
- No mmap(), no files — pure classic fork() COW

### C++ Program (Classic COW Proof)

```cpp
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    std::cout << "Parent PID: " << getpid() << "\n";

    int* shared = new int{42};

    std::cout << "Before fork():\n";
    std::cout << "Value = " << *shared << "\n";
    std::cout << "VA    = " << shared << "\n\n";

    pid_t pid = fork();

    if (pid == 0)
    {
        std::cout << "Child PID: " << getpid() << "\n";
        std::cout << "Child BEFORE write:\n";
        std::cout << "Value = " << *shared << "\n";
        std::cout << "VA    = " << shared << "\n\n";

        sleep(2);

        *shared = 99;

        std::cout << "Child AFTER write:\n";
        std::cout << "Value = " << *shared << "\n";
        std::cout << "VA    = " << shared << "\n";

        _exit(0);
    }
    else
    {
        sleep(1);

        std::cout << "Parent BEFORE child write:\n";
        std::cout << "Value = " << *shared << "\n";
        std::cout << "VA    = " << shared << "\n\n";

        wait(nullptr);

        std::cout << "Parent AFTER child write:\n";
        std::cout << "Value = " << *shared << "\n";
        std::cout << "VA    = " << shared << "\n";
    }

    delete shared;
    return 0;
}
```

-------------------------------------------------------------------------------
🔬 WHAT THIS PROGRAM PROVES
-------------------------------------------------------------------------------
- Same virtual address in parent & child
- Different values after write
- Physical memory copied ONLY on write

**Same VA does NOT mean same physical page**


🔍 PROVING COW USING /proc/<pid>/smaps

### Why smaps matters
- Shows private vs shared memory
- Shows real kernel accounting
- Definitive proof of COW

### Steps

```
ps -ef | grep your_program
cat /proc/<parent_pid>/smaps | grep -E "Private|Shared" -A 5
cat /proc/<child_pid>/smaps  | grep -E "Private|Shared" -A 5
```

### Expected observation

Before write:
- Shared_Clean: large
- Private_Dirty: small

After write (child):
- Private_Dirty increases
- Shared_Clean decreases

This confirms **Copy-On-Write**


📐 VISUAL DIAGRAMS (MENTAL MODELS)


### Classic fork() COW (anonymous memory)

```
Before fork():
Parent VA --> Physical Page P1 (RW)

After fork():
Parent VA --+
             +--> Physical Page P1 (RO + COW)
Child  VA --+

After child write:
Parent VA --> Physical Page P1
Child  VA --> Physical Page P2 (private copy)
```

### Page cache sharing (NO COW)

```
Disk
 |
 v
Page Cache Page X
 |             |
Parent read()  Child read()
```

### mmap(MAP_PRIVATE) COW with file

```
Initial:
Parent VA --+
             +--> Page Cache Page X
Child  VA --+

After write in child:
Parent VA --> Page Cache Page X
Child  VA --> Anonymous Page P2
```


🎓 INTERVIEW NOTES (CRISP & SENIOR-LEVEL)


Q1: Why does fork() not copy memory immediately?
- fork() uses Copy-On-Write to avoid unnecessary memory duplication.
- Only page tables are copied initially.

Q2: Does page cache participate in COW?
- No.
- Page cache is shared kernel memory.
- COW applies only to PRIVATE mappings.

Q3: Why does mmap(MAP_PRIVATE) cause COW but read() does not?
- mmap(MAP_PRIVATE) creates a private mapping.
- read() copies data into a private user buffer.

Q4: How do you prove COW practically?
- Use fork()
- Trigger write in child
- Inspect `/proc/<pid>/smaps`
- Observe increase in `Private_Dirty`

Q5: One killer sentence for interviews?
- “COW is a property of page mappings, not of the page cache or memory itself.”

================================================================================
