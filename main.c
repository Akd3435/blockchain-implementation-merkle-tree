#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

#define MAX_TRANSACTIONS 100
#define BLOCK_SIZE 1024

// Input transactions
void input_transactions(char ***transactions, int *n) {
    printf("Enter number of transactions for block: ");
    scanf("%d", n);
    getchar();

    if (*n <= 0 || *n > MAX_TRANSACTIONS) {
        printf("Invalid number of transactions.\n");
        *transactions = NULL;
        return;
    }

    *transactions = (char **)malloc(sizeof(char *) * (*n));
    for (int i = 0; i < *n; i++) {
        (*transactions)[i] = (char *)malloc(BLOCK_SIZE);
        printf("Transaction %d: ", i + 1);
        scanf(" %[^\n]", (*transactions)[i]);
    }
}

// Free transactions
void free_transactions(char **transactions, int n) {
    if (!transactions) return;
    for (int i = 0; i < n; i++)
        free(transactions[i]);
    free(transactions);
}

int main() {
    Blockchain bc;
    init_blockchain(&bc);

    int choice;
    while (1) {
        printf("\n====== BLOCKCHAIN MENU ======\n");
        printf("1. Add Block\n");
        printf("2. Print Blockchain\n");
        printf("3. Exit\n");
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
