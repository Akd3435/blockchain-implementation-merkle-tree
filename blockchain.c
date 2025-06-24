#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "blockchain.h"
#include "md5.h"
#include<math.h>
// Generate block hash: MD5(MerkleRoot + prev_hash + index + timestamp)
char* compute_block_hash(const Block *block) {
    unsigned char *buffer = (unsigned char *)malloc(MD5_DIGEST_LENGTH * 2 + 100);
    char time_str[30];
    sprintf(time_str, "%ld", block->timestamp);
    
    memcpy(buffer, block->m_tree.nodes[1].hash, MD5_DIGEST_LENGTH);
    if (block->prev_hash)
        memcpy(buffer + MD5_DIGEST_LENGTH, block->prev_hash, MD5_DIGEST_LENGTH);
    memcpy(buffer + 2 * MD5_DIGEST_LENGTH, time_str, strlen(time_str));

    char *digest = (char *)malloc(MD5_DIGEST_LENGTH);
    MD5One(buffer, MD5_DIGEST_LENGTH * 2 + strlen(time_str), (unsigned char *)digest);
    free(buffer);
    return digest;
}

void init_blockchain(Blockchain *bc) {
    bc->size = 0;
    bc->capacity = 10;
    bc->blocks = (Block *)malloc(sizeof(Block) * bc->capacity);
}

void add_block(Blockchain *bc, char **transactions, size_t num_transactions) {
    if (bc->size >= bc->capacity) {
        bc->capacity *= 2;
        bc->blocks = realloc(bc->blocks, sizeof(Block) * bc->capacity);
    }

    Block *new_block = &bc->blocks[bc->size];
    new_block->index = bc->size;
    new_block->timestamp = time(NULL);
    new_block->prev_hash = NULL;
    new_block->block_hash = NULL;

    if (bc->size > 0) {
        Block *prev = &bc->blocks[bc->size - 1];
        new_block->prev_hash = (char *)malloc(MD5_DIGEST_LENGTH);
        memcpy(new_block->prev_hash, prev->block_hash, MD5_DIGEST_LENGTH);
    }

    size_t height = (size_t)ceil(log2(num_transactions)) + 1;
    new_block->m_tree = (merkle_tree){0, height, MD5_DIGEST_LENGTH, 1024, num_transactions, MD5One, NULL};
    make_tree(&new_block->m_tree, transactions);

    new_block->block_hash = compute_block_hash(new_block);
    bc->size++;
}

void print_block(const Block *block) {
    printf("\n-------------------------\n");
    printf("Block #%d\n", block->index);
    printf("Timestamp: %s", ctime(&block->timestamp));
    printf("Merkle Root: ");
    MD5Print(block->m_tree.nodes[1].hash);
    printf("Previous Hash: ");
    if (block->prev_hash)
        MD5Print(block->prev_hash);
    else
        printf("NULL\n");
    printf("Block Hash: ");
    MD5Print(block->block_hash);
    printf("-------------------------\n");
}

void print_blockchain(const Blockchain *bc) {
    for (size_t i = 0; i < bc->size; i++)
        print_block(&bc->blocks[i]);
}

void free_blockchain(Blockchain *bc) {
    for (size_t i = 0; i < bc->size; i++) {
        Block *blk = &bc->blocks[i];
        free_Tree_space(&blk->m_tree);
        if (blk->prev_hash) free(blk->prev_hash);
        if (blk->block_hash) free(blk->block_hash);
    }
    free(bc->blocks);
}
