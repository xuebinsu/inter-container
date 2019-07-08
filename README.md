# inter-container
Utilities for efficient inter-container communication

## Features
- A ring buffer on shared memory with contiguous space allocation and garbage collection.
- Synchronization using the futex(2) syscall on Linux.
- Vectorized memcpy() that achieves comparable perforamce compared to the one in glibc.

All the features above come with some simple test code.
