# blockchain-implementation-merkle-tree
# Merkle Tree Implementation in C

This project implements a **Merkle Tree** in the C programming language, using a standalone **MD5 hashing algorithm**. The tree is used to compute a single hash (Merkle Root) that represents a list of transactions. This structure is a core component of many cryptographic systems such as blockchain technology.

Features

-  Hashes transactions using **MD5**
-  Builds Merkle Tree from bottom-up
-  Handles odd number of transactions by duplicating the last node
-  Efficient memory management with proper cleanup functions
-  Designed to integrate directly with a blockchain system

 Technologies Used

- C Programming Language
- MD5 Hash Function (custom standalone implementation)
- Merkle Tree Data Structure
- Dynamic Memory Allocation ('malloc', 'free')
- Modular Design with separate '.c' and '.h' files


