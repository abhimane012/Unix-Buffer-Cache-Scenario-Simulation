# Unix-Buffer-Cache-Scenario-Simulation
Program in C++ for getblk 5 Scenario Simulation

# Buffer Cache

A simple buffer cache implementation for simulation of getblk and brelse algorithms that fulfills allocation and release requests of processes for blocks.

# Scenarios for Retrieval of a Buffer

The algorithms for reading and writing disk blocks use the algorithm getblk to allocate buffers from the pool. There are 5 typical scenarios the kernel may follow in getblk to allocate a buffer for a disk block.

    1. Block is found on its hash queue and its buffer is free.
    2. Block could not be found on the hash queue, so a buffer from the free list is allocated.
    3. Block could not be found on the hash queue, and when allocating a buffer from free list, a buffer marked "delayed write" is allocated. Then the kernel must write the "delayed write" buffer to disk and allocate another buffer.
    4. Block could not be found on the hash queue and the free list of buffers is empty.
    5. Block was found on the hash queue, but its buffer is currently busy.

