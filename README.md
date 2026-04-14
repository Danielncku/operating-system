# Operating Systems Laboratory Portfolio

This repository contains my Operating Systems lab work from Lab 1 to Lab 4. The projects were implemented mainly in C on Linux, and they cover several important OS topics such as inter-process communication, shell design, multithreading, synchronization, kernel modules, and file system implementation.



## Overview

These labs helped me understand operating systems from both user space and kernel space. Instead of only learning the theory in class, I was able to implement the core mechanisms by myself and observe how processes, threads, synchronization primitives, and file systems actually work in practice.

## Lab 1: Inter-Process Communication

In Lab 1, I implemented communication between a sender process and a receiver process using two different IPC mechanisms:

- Message queues
- Shared memory
- POSIX semaphores for synchronization
- Timing measurement for performance comparison

Through this lab, I learned how two processes can exchange data in different ways. Message queues made the communication flow more explicit, while shared memory felt faster but required more careful synchronization. Writing both versions helped me understand the trade-off between simplicity and efficiency.

**What I learned from this lab:**

- How Linux IPC mechanisms work in practice
- The difference between message passing and shared memory
- Why synchronization is necessary when processes access shared resources
- How to use `msgget`, `msgsnd`, `msgrcv`, `shmget`, `shmat`, and `sem_open`

**My reflection:**

This was the first lab that made me realize operating systems is not only about concepts like process or communication in a textbook. Once I implemented both mechanisms myself, I could clearly see that correctness and synchronization are just as important as functionality.

## Lab 2: Custom Linux Shell

In Lab 2, I built a small shell that supports command execution, built-in commands, redirection, and pipelines.

**Main features:**

- Built-in commands such as `cd`, `pwd`, `echo`, `record`, and `exit`
- External command execution through `fork` and `execvp`
- Input and output redirection with `<` and `>`
- Piping with `|`
- Basic command parsing and history recording

**What I learned from this lab:**

- How a shell creates child processes to run commands
- How `fork`, `execvp`, and `waitpid` work together
- How file descriptors are redirected using `open` and `dup2`
- How pipelines connect multiple processes
- How command parsing affects the overall shell behavior

**My reflection:**

This lab helped me understand the workflow behind commands that I use every day in Linux. Before this project, I only knew how to use a shell. After implementing one myself, I understood much better how commands are parsed, how child processes are created, and how pipes and redirection are managed internally. It also trained me to think more carefully about edge cases in user input.

## Lab 3: Threads, Synchronization, and Kernel Interaction

Lab 3 focused on concurrency and Linux kernel interaction. The exercises included multithreaded programming, synchronization, matrix multiplication, and communication between user space and kernel space through `/proc`.

**Topics covered:**

- POSIX threads (`pthread`)
- Spin locks and shared-variable protection
- Race condition handling
- Parallel matrix multiplication
- Linux kernel modules
- `/proc` interface design

**What I learned from this lab:**

- Why race conditions happen in multithreaded programs
- How spin locks can be used to protect shared data
- How work can be divided across multiple threads
- How to create a kernel module and expose information through `/proc`
- How user programs and kernel modules can interact

**My reflection:**

This lab was especially important for me because it connected concurrency with real system behavior. I could directly observe that if synchronization is not handled correctly, the output becomes inconsistent. The kernel module part also gave me a clearer idea of the boundary between user space and kernel space, which felt much more concrete after actually reading from and writing to `/proc`.

## Lab 4: Custom In-Memory File System

In Lab 4, I implemented a simple in-memory file system kernel module named `osfs`. This lab brought together many OS concepts in one project, including file system structure, metadata management, and integration with the Linux VFS layer.

**Main components:**

- File system registration and mount handling
- Superblock initialization
- Inode allocation and management
- Directory lookup and iteration
- File read and write operations
- Symbolic link support
- Bitmap-based block and inode management

**What I learned from this lab:**

- How the Linux Virtual File System (VFS) abstracts different file systems
- The role of superblocks, inodes, directory entries, and file operations
- How file metadata and data blocks are managed
- How a custom file system is mounted and connected to kernel interfaces

**My reflection:**

This was the most challenging lab in the series, but also the one I learned the most from. Implementing a file system required me to connect many concepts that had previously seemed separate. I gained a deeper understanding of how the operating system organizes files internally, and I also became more comfortable reading kernel-oriented code and thinking about data structures at the system level.

## Technologies and Skills Learned

Across these four labs, I strengthened my understanding of:

- C systems programming
- Linux process management
- Inter-process communication
- POSIX semaphores and spin locks
- Process creation and synchronization
- File descriptor redirection and pipes
- Pthreads and concurrent programming
- Linux kernel module development
- `/proc` file system interaction
- Virtual File System (VFS) concepts
- In-memory file system design

## Repository Layout

```text
LAB1/   IPC using message queues, shared memory, and semaphores
LAB2/   Custom shell with built-in commands, redirection, and pipes
LAB3/   Threads, synchronization, matrix computation, and kernel modules
Lab4/   In-memory Linux file system implementation
