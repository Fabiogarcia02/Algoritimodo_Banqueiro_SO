#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

pthread_mutex_t mutex;

/*  Verifica estado seguro */
int safety_check() {
    int work[NUMBER_OF_RESOURCES];
    bool finish[NUMBER_OF_CUSTOMERS] = {false};

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        work[i] = available[i];

    int count = 0;

    while (count < NUMBER_OF_CUSTOMERS) {
        bool found = false;

        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            if (!finish[i]) {
                bool possible = true;

                for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                    if (need[i][j] > work[j]) {
                        possible = false;
                        break;
                    }
                }

                if (possible) {
                    for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
                        work[j] += allocation[i][j];

                    finish[i] = true;
                    found = true;
                    count++;
                }
            }
        }

        if (!found)
            return 0; // inseguro
    }

    return 1; // seguro
}

/* 📥 Solicita recursos */
int request_resources(int customer_num, int request[]) {

    pthread_mutex_lock(&mutex);

    // verifica se request <= need
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (request[i] > need[customer_num][i]) {
            pthread_mutex_unlock(&mutex);
            return -1;
        }
    }

    // verifica se há recursos disponíveis
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (request[i] > available[i]) {
            pthread_mutex_unlock(&mutex);
            return -1;
        }
    }

    // alocação temporária
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
    }

    // verifica segurança
    if (safety_check()) {
        pthread_mutex_unlock(&mutex);
        return 0;
    }

    // rollback (estado inseguro)
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] += request[i];
        allocation[customer_num][i] -= request[i];
        need[customer_num][i] += request[i];
    }

    pthread_mutex_unlock(&mutex);
    return -1;
}

/* Libera recursos */
int release_resources(int customer_num, int release[]) {

    pthread_mutex_lock(&mutex);

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {

        if (release[i] > allocation[customer_num][i]) {
            pthread_mutex_unlock(&mutex);
            return -1;
        }

        available[i] += release[i];
        allocation[customer_num][i] -= release[i];
        need[customer_num][i] += release[i];
    }

    pthread_mutex_unlock(&mutex);
    return 0;
}

/*  Thread do cliente */
void *customer_thread(void *id_ptr) {
    int id = *(int *)id_ptr;

   for (int k = 0; k < 3; k++) {

        int request[NUMBER_OF_RESOURCES];

        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            request[i] = rand() % (need[id][i] + 1);
        }

        if (request_resources(id, request) == 0) {
            printf("Cliente %d: pedido APROVADO\n", id);
        } else {
            printf("Cliente %d: pedido NEGADO\n", id);
        }

        sleep(1);

        int release[NUMBER_OF_RESOURCES];

        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            release[i] = rand() % (allocation[id][i] + 1);
        }

        if (release_resources(id, release) == 0) {
            printf("Cliente %d: liberou recursos\n", id);
        }

        sleep(1);
    }

    return NULL;
}

/*  MAIN */
int main(int argc, char *argv[]) {

    if (argc != NUMBER_OF_RESOURCES + 1) {
        printf("Uso: ./a.out <r1> <r2> <r3>\n");
        return 1;
    }

    pthread_t threads[NUMBER_OF_CUSTOMERS];
    int ids[NUMBER_OF_CUSTOMERS];

    pthread_mutex_init(&mutex, NULL);

    // inicializa available via argv
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] = atoi(argv[i + 1]);
    }

    // inicializa estruturas
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            maximum[i][j] = rand() % (available[j] + 1);
            allocation[i][j] = 0;
            need[i][j] = maximum[i][j];
        }
    }

    // cria threads
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, customer_thread, &ids[i]);
    }

    // mantém programa vivo
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}
