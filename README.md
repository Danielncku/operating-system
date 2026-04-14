# Operating Systems Laboratory 

Four operating systems labs implemented in C and Linux kernel space. Together, the labs cover core OS topics including inter-process communication, shell design, multithreading, synchronization, kernel modules, and file system implementation.

`

## Overview

This project was developed as a hands-on operating systems practice repository. Each lab focuses on a different layer of the OS stack, from user-space process communication to kernel-space extensions and a custom in-memory file system.

## Lab Structure

### Lab 1: Inter-Process Communication

Lab 1 compares two IPC mechanisms for sender-receiver communication:

- Message queues
- Shared memory
- POSIX semaphores for synchronization
- Execution time measurement for communication overhead

**Key learning outcomes:**

- Designing mailbox-based communication between processes
- Coordinating producer-consumer style workflows
- Comparing message-passing and shared-memory trade-offs
- Using Linux IPC APIs such as `msgget`, `msgsnd`, `msgrcv`, `shmget`, `shmat`, and `sem_open`

### Lab 2: Custom Linux Shell

Lab 2 implements a miniature shell that supports both built-in and external commands.

**Core features:**

- Built-in commands such as `cd`, `pwd`, `echo`, `record`, and `exit`
- External command execution using `fork` and `execvp`
- Input and output redirection with `<` and `>`
- Pipelining with `|`
- Command parsing and command history management

**Key learning outcomes:**

- Process creation and replacement
- Parent-child process control with `waitpid`
- File descriptor manipulation using `open`, `dup2`, and `close`
- Shell parsing and command execution flow

### Lab 3: Threads, Synchronization, and Kernel Interaction

Lab 3 explores multithreaded programming and Linux kernel interaction through several exercises.

**Covered topics:**

- POSIX threads (`pthread`)
- Spin locks and synchronization
- Race condition control in shared-variable updates
- Parallel matrix multiplication
- Linux kernel modules
- `/proc` file system interfaces for user-kernel communication

**Key learning outcomes:**

- Protecting shared data in concurrent programs
- Implementing spin-lock based synchronization
- Dividing workloads across threads for parallel computation
- Exposing kernel data to user space through `/proc`
- Observing thread identity, scheduling-related information, and process metadata

### Lab 4: Custom In-Memory File System

Lab 4 implements a simple Linux in-memory file system kernel module named `osfs`.

**Main components:**

- File system registration and mount lifecycle
- Superblock initialization
- Inode allocation and management
- Directory lookup and iteration
- Regular file read and write operations
- Symbolic link support
- Block and inode bitmap management

**Key learning outcomes:**

- Understanding the Linux Virtual File System (VFS) model
- Implementing superblock, inode, file, and directory operations
- Managing file system metadata and data blocks
- Mapping custom in-memory structures to kernel VFS objects

## Technologies and Concepts Learned

Across Labs 1 to 4, this repository demonstrates practical experience with:

- C systems programming
- Linux process and thread management
- Inter-process communication
- POSIX semaphores and spin locks
- File descriptors and redirection
- Shell architecture
- Pthreads-based concurrency
- Linux kernel module development
- `/proc` file system programming
- Virtual File System internals
- In-memory file system design

## Repository Layout

```text
LAB1/   IPC with message queues, shared memory, and semaphores
LAB2/   Custom shell with parsing, redirection, and pipes
LAB3/   Threads, synchronization, matrix computation, and kernel modules
Lab4/   In-memory Linux file system implementation
