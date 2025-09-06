#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "merkle_tree.h"
#include <time.h>
#include <stddef.h>

typedef struct {
    int index;
    time_t timestamp;
    char *prev_hash;    // hex string (owned by block or NULL)
    char *block_hash;   // hex string (owned by block)
    MerkleTree m_tree;  // merkle tree (owns its data)
} Block;

typedef struct {
    Block *blocks;      // dynamic array of blocks
    size_t size;        // number of blocks currently stored
    size_t capacity;    // allocated capacity
} Blockchain;

void init_blockchain(Blockchain *bc);
void add_block(Blockchain *bc, char **transactions, size_t num_transactions);
void print_block(const Block *block);
void print_blockchain(const Blockchain *bc);
void free_blockchain(Blockchain *bc);

// returns a newly malloc'd hex string (32+1). Caller must free.
char *compute_block_hash(const Block *block);

#endif // BLOCKCHAIN_H
