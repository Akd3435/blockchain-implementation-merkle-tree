#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merkle_tree.h"
#include "md5.h"

void build_merkle_tree(char **transactions, size_t num_transactions, MerkleTree *mt) {
    mt->num_transactions = num_transactions;
    mt->transactions = transactions;

    // Allocate space for transaction hashes
    mt->hashes = (char **)malloc(sizeof(char *) * num_transactions);
    for (size_t i = 0; i < num_transactions; i++) {
        unsigned char digest[16];
        MD5One((unsigned char *)transactions[i], strlen(transactions[i]), digest);
        mt->hashes[i] = (char *)malloc(33);  // 32 hex digits + null terminator

        for (int j = 0; j < 16; j++)
            sprintf(&mt->hashes[i][j * 2], "%02x", digest[j]);
        mt->hashes[i][32] = '\0';
    }

    // Construct Merkle Tree upward
    size_t level_size = num_transactions;
    char **current_level = mt->hashes;

    while (level_size > 1) {
        size_t next_level_size = (level_size + 1) / 2;
        char **next_level = (char **)malloc(sizeof(char *) * next_level_size);

        for (size_t i = 0; i < next_level_size; i++) {
            char combined[65];  // two 32-char hashes + '\0'

            if (2 * i + 1 < level_size) {
                snprintf(combined, sizeof(combined), "%s%s", current_level[2 * i], current_level[2 * i + 1]);
            } else {
                snprintf(combined, sizeof(combined), "%s%s", current_level[2 * i], current_level[2 * i]);
            }

            unsigned char digest[16];
            MD5One((unsigned char *)combined, strlen(combined), digest);
            next_level[i] = (char *)malloc(33);
            for (int j = 0; j < 16; j++)
                sprintf(&next_level[i][j * 2], "%02x", digest[j]);
            next_level[i][32] = '\0';
        }

        if (current_level != mt->hashes) {
            for (size_t i = 0; i < level_size; i++) {
                free(current_level[i]);
            }
            free(current_level);
        }

        current_level = next_level;
        level_size = next_level_size;
    }

    mt->root_hash = strdup(current_level[0]);
    free(current_level);
}

void free_merkle_tree(MerkleTree *mt) {
    for (size_t i = 0; i < mt->num_transactions; i++) {
        free(mt->hashes[i]);
    }
    free(mt->hashes);
    free(mt->root_hash);
}
