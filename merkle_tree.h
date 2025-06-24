#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include <stdlib.h>

#define MAX_TRANSACTIONS 100

typedef struct {
    char **transactions;    // Original transaction strings
    int num_transactions;   // Number of transactions
    char **hashes;          // Array of transaction hashes (leaf hashes)
    char *root_hash;        // Final Merkle root hash
} MerkleTree;

// Build a Merkle tree from the transactions
void build_merkle_tree(char **transactions, size_t num_transactions, MerkleTree *mt);

// Free all memory allocated inside the Merkle tree
void free_merkle_tree(MerkleTree *mt);

#endif
