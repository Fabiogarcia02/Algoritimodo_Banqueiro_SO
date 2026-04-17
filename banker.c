#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

// Verifica estado seguro
int estado_seguro() {
    int work[NUMBER_OF_RESOURCES];
    bool finish[NUMBER_OF_CUSTOMERS] = {false};

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        work[i] = available[i];

    int count = 0;

    while (count < NUMBER_OF_CUSTOMERS) {
        int found = 0;

        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            if (!finish[i]) {
                int possible = 1;

                for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                    if (need[i][j] > work[j]) {
                        possible = 0;
                        break;
                    }
                }

                if (possible) {
                    for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
                        work[j] += allocation[i][j];

                    finish[i] = true;
                    found = 1;
                    count++;
                }
            }
        }

        if (!found) return 0;
    }

    return 1;
}

// Solicitar recursos
void solicitar_recursos(int cliente) {
    int request[NUMBER_OF_RESOURCES];

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        request[i] = rand() % (need[cliente][i] + 1);
    }

    // Verifica disponibilidade
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (request[i] > available[i]) return;
    }

    // Simula alocação
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[cliente][i] += request[i];
        need[cliente][i] -= request[i];
    }

    if (estado_seguro()) {
        printf("Cliente %d: pedido aprovado\n", cliente);
    } else {
        // desfaz
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            available[i] += request[i];
            allocation[cliente][i] -= request[i];
            need[cliente][i] += request[i];
        }

        printf("Cliente %d: pedido NEGADO\n", cliente);
    }
}

// Liberar recursos
void liberar_recursos(int cliente) {
    int release[NUMBER_OF_RESOURCES];

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        release[i] = rand() % (allocation[cliente][i] + 1);

        available[i] += release[i];
        allocation[cliente][i] -= release[i];
        need[cliente][i] += release[i];
    }

    printf("Cliente %d: liberou recursos\n", cliente);
}

int main() {
    srand(time(NULL));

    int init[3] = {10, 5, 7};

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] = init[i];
    }

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            maximum[i][j] = rand() % (available[j] + 1);
            allocation[i][j] = 0;
            need[i][j] = maximum[i][j];
        }
    }

    for (int k = 0; k < 10; k++) {
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            solicitar_recursos(i);
            liberar_recursos(i);
        }
    }

    return 0;
}