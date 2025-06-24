#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "merkle_tree.h"
#include <time.h>
#include <stddef.h>  // For size_t

typedef struct {
    int index;
    time_t timestamp;
    char *prev_hash;
    char *block_hash;
    merkle_tree m_tree;
} Block;

typedef struct {
    Block *blocks;
    size_t size;
    size_t capacity;
} Blockchain;

// All functions declared after type definitions
void init_blockchain(Blockchain *bc);
void add_block(Blockchain *bc, char **transactions, size_t num_transactions);
void print_block(const Block *block);
void print_blockchain(const Blockchain *bc);
void free_blockchain(Blockchain *bc);

char *compute_block_hash(const Block *block);

#endif
