/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

#define MAX_TRANSACTIONS 100
#define BLOCK_SIZE 1024

int main() {
    Blockchain bc;
    init_blockchain(&bc);

    int n;
    printf("Enter number of transactions for block: ");
    scanf("%d", &n);
    getchar();

    if (n <= 0 || n > MAX_TRANSACTIONS) {
        printf("Invalid number of transactions.\n");
        return 1;
    }

    char **transactions = (char **)malloc(sizeof(char *) * n);
    for (int i = 0; i < n; i++) {
        transactions[i] = (char *)malloc(BLOCK_SIZE);
        printf("Transaction %d: ", i + 1);
        scanf(" %[^\n]", transactions[i]);
    }

    add_block(&bc, transactions, n);
    printf("\nBlock added to blockchain.\n");

    print_blockchain(&bc);

    for (int i = 0; i < n; i++)
        free(transactions[i]);
    free(transactions);

    free_blockchain(&bc);
    return 0;
}
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

#define MAX_TRANSACTIONS 100
#define BLOCK_SIZE 1024

// Function to input transactions from user
void input_transactions(char ***transactions, int *n) {
    printf("Enter number of transactions for block: ");
    scanf("%d", n);
    getchar();  // consume newline

    if (*n <= 0 || *n > MAX_TRANSACTIONS) {
        printf("Invalid number of transactions.\n");
        *transactions = NULL;
        return;
    }

    *transactions = (char **)malloc(sizeof(char *) * (*n));
    if (!*transactions) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < *n; i++) {
        (*transactions)[i] = (char *)malloc(BLOCK_SIZE);
        if (!(*transactions)[i]) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        printf("Transaction %d: ", i + 1);
        scanf(" %[^\n]", (*transactions)[i]);
    }
}

// Function to free the dynamically allocated transactions array
void free_transactions(char **transactions, int n) {
    if (!transactions) return;
    for (int i = 0; i < n; i++)
        free(transactions[i]);
    free(transactions);
}

// Function to tamper with a block and replace the first transaction
void tamper_transaction(Blockchain *bc) {
    int block_num;
    printf("Enter block number to tamper: ");
    scanf("%d", &block_num);

    if (block_num < 0 || block_num >= (int)bc->size) {
        printf("Invalid block number.\n");
        return;
    }

    char buffer[BLOCK_SIZE];
    printf("Enter new fake transaction to replace the first one:\n> ");
    scanf(" %[^\n]", buffer);

    // Replace first transaction
    free(bc->blocks[block_num].m_tree.transactions[0]);
    bc->blocks[block_num].m_tree.transactions[0] = strdup(buffer);

    // Rebuild Merkle tree and update block hash
    free_merkle_tree(&(bc->blocks[block_num].m_tree));
    build_merkle_tree(bc->blocks[block_num].m_tree.transactions,
                      bc->blocks[block_num].m_tree.num_transactions,
                      &(bc->blocks[block_num].m_tree));

    free(bc->blocks[block_num].block_hash);
    bc->blocks[block_num].block_hash = compute_block_hash(&bc->blocks[block_num]);

    printf("\nTampering complete. Block %d has been modified.\n", block_num);
}

int main() {
    Blockchain bc;
    init_blockchain(&bc);

    int choice;
    while (1) {
        printf("\n====== BLOCKCHAIN MENU ======\n");
        printf("1. Add Block\n");
        printf("2. Print Blockchain\n");
        printf("3. Tamper Transaction\n");
        printf("4. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        if (choice == 1) {
            char **transactions = NULL;
            int num;
            input_transactions(&transactions, &num);
            if (transactions)
                add_block(&bc, transactions, num);
            free_transactions(transactions, num);
        }
        else if (choice == 2) {
            print_blockchain(&bc);
        }
        else if (choice == 3) {
            tamper_transaction(&bc);
        }
        else if (choice == 4) {
            free_blockchain(&bc);
            printf("Exiting. Blockchain memory cleaned.\n");
            break;
        }
        else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
