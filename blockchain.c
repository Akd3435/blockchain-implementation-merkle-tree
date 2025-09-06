#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "blockchain.h"
#include "md5.h"   // MD5One and MD5_DIGEST_LENGTH

// Compute block hash: MD5( merkle_root + prev_hash(if any) + index + timestamp )
// Returns malloc'd hex string (32 + 1)
char *compute_block_hash(const Block *block) {
    if (!block) return NULL;

    const char *root = block->m_tree.root_hash ? block->m_tree.root_hash : "";
    const char *prev = block->prev_hash ? block->prev_hash : "";

    char meta[64];
    int meta_len = snprintf(meta, sizeof(meta), "%d%ld", block->index, (long)block->timestamp);

    size_t total = strlen(root) + strlen(prev) + (size_t)meta_len;
    char *buf = malloc(total + 1);
    if (!buf) return NULL;
    buf[0] = '\0';
    strcat(buf, root);
    strcat(buf, prev);
    strcat(buf, meta);

    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5One((unsigned char *)buf, strlen(buf), digest);
    free(buf);

    char *hex = malloc(33);
    if (!hex) return NULL;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
        sprintf(hex + i * 2, "%02x", digest[i]);
    hex[32] = '\0';
    return hex;
}

void init_blockchain(Blockchain *bc) {
    if (!bc) return;
    bc->size = 0;
    bc->capacity = 8;
    bc->blocks = malloc(sizeof(Block) * bc->capacity);
    if (!bc->blocks) { perror("malloc"); exit(EXIT_FAILURE); }

    // initialize fields to safe defaults
    for (size_t i = 0; i < bc->capacity; ++i) {
        bc->blocks[i].index = -1;
        bc->blocks[i].timestamp = 0;
        bc->blocks[i].prev_hash = NULL;
        bc->blocks[i].block_hash = NULL;
        bc->blocks[i].m_tree.transactions = NULL;
        bc->blocks[i].m_tree.hashes = NULL;
        bc->blocks[i].m_tree.root_hash = NULL;
        bc->blocks[i].m_tree.num_transactions = 0;
    }
}

void add_block(Blockchain *bc, char **transactions, size_t num_transactions) {
    if (!bc) return;

    if (bc->size >= bc->capacity) {
        bc->capacity *= 2;
        Block *tmp = realloc(bc->blocks, sizeof(Block) * bc->capacity);
        if (!tmp) { perror("realloc"); exit(EXIT_FAILURE); }
        bc->blocks = tmp;
        // initialize new slots
        for (size_t i = bc->size; i < bc->capacity; ++i) {
            bc->blocks[i].index = -1;
            bc->blocks[i].timestamp = 0;
            bc->blocks[i].prev_hash = NULL;
            bc->blocks[i].block_hash = NULL;
            bc->blocks[i].m_tree.transactions = NULL;
            bc->blocks[i].m_tree.hashes = NULL;
            bc->blocks[i].m_tree.root_hash = NULL;
            bc->blocks[i].m_tree.num_transactions = 0;
        }
    }

    Block *new_block = &bc->blocks[bc->size];
    new_block->index = (int)bc->size;
    new_block->timestamp = time(NULL);
    new_block->prev_hash = NULL;
    new_block->block_hash = NULL;
    // Build Merkle tree (this duplicates transactions into the MerkleTree)
    build_merkle_tree(transactions, num_transactions, &new_block->m_tree);

    // copy previous block hash if exists
    if (bc->size > 0) {
        Block *prev = &bc->blocks[bc->size - 1];
        if (prev->block_hash) new_block->prev_hash = strdup(prev->block_hash);
    }

    // compute this block's hash
    new_block->block_hash = compute_block_hash(new_block);
    bc->size++;
}

void print_block(const Block *block) {
    if (!block) return;
    printf("\n-------------------------\n");
    printf("Block #%d\n", block->index);
    printf("Timestamp: %s", ctime(&block->timestamp));
    printf("Merkle Root: %s\n", block->m_tree.root_hash ? block->m_tree.root_hash : "NULL");
    printf("Previous Hash: %s\n", block->prev_hash ? block->prev_hash : "NULL");
    printf("Block Hash: %s\n", block->block_hash ? block->block_hash : "NULL");
    printf("-------------------------\n");
}

void print_blockchain(const Blockchain *bc) {
    if (!bc) return;
    for (size_t i = 0; i < bc->size; ++i) print_block(&bc->blocks[i]);
}

void free_blockchain(Blockchain *bc) {
    if (!bc) return;
    for (size_t i = 0; i < bc->size; ++i) {
        Block *blk = &bc->blocks[i];
        free_merkle_tree(&blk->m_tree);
        if (blk->prev_hash) { free(blk->prev_hash); blk->prev_hash = NULL; }
        if (blk->block_hash) { free(blk->block_hash); blk->block_hash = NULL; }
    }
    free(bc->blocks);
    bc->blocks = NULL;
    bc->size = bc->capacity = 0;
}
