#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define NUM_CLIENTES 5
#define NUM_RECURSOS 3

int disponivel[NUM_RECURSOS];
int maximo[NUM_CLIENTES][NUM_RECURSOS];
int alocacao[NUM_CLIENTES][NUM_RECURSOS];
int necessidade[NUM_CLIENTES][NUM_RECURSOS];

pthread_mutex_t mutex;


void mostrar_estado() {
    printf("\n========== ESTADO DO SISTEMA ==========\n");

    printf("Disponivel: ");
    for (int i = 0; i < NUM_RECURSOS; i++)
        printf("%d ", disponivel[i]);
    printf("\n\n");

    printf("Alocacao:\n");
    for (int i = 0; i < NUM_CLIENTES; i++) {
        printf("Cliente %d: ", i);
        for (int j = 0; j < NUM_RECURSOS; j++)
            printf("%d ", alocacao[i][j]);
        printf("\n");
    }

    printf("\nNecessidade:\n");
    for (int i = 0; i < NUM_CLIENTES; i++) {
        printf("Cliente %d: ", i);
        for (int j = 0; j < NUM_RECURSOS; j++)
            printf("%d ", necessidade[i][j]);
        printf("\n");
    }

    printf("=======================================\n");
}


int verificar_seguranca() {
    int trabalho[NUM_RECURSOS];
    bool finalizado[NUM_CLIENTES] = {false};

    for (int i = 0; i < NUM_RECURSOS; i++)
        trabalho[i] = disponivel[i];

    printf("\n[VERIFICANDO SEGURANÇA]\n");

    int count = 0;

    while (count < NUM_CLIENTES) {
        bool encontrou = false;

        for (int i = 0; i < NUM_CLIENTES; i++) {
            if (!finalizado[i]) {
                bool pode_executar = true;

                for (int j = 0; j < NUM_RECURSOS; j++) {
                    if (necessidade[i][j] > trabalho[j]) {
                        pode_executar = false;
                        break;
                    }
                }

                if (pode_executar) {
                    printf(" Cliente %d pode finalizar\n", i);

                    for (int j = 0; j < NUM_RECURSOS; j++)
                        trabalho[j] += alocacao[i][j];

                    finalizado[i] = true;
                    encontrou = true;
                    count++;
                }
            }
        }

        if (!encontrou) {
            printf(" Sistema em estado INSEGURO\n");
            return 0;
        }
    }

    printf(" Sistema em estado SEGURO\n");
    return 1;
}



int solicitar_recursos(int cliente, int pedido[]) {

    pthread_mutex_lock(&mutex);

    printf("\n Cliente %d solicitando: ", cliente);
    for (int i = 0; i < NUM_RECURSOS; i++)
        printf("%d ", pedido[i]);
    printf("\n");

    // Verifica se excede a necessidade
    for (int i = 0; i < NUM_RECURSOS; i++) {
        if (pedido[i] > necessidade[cliente][i]) {
            printf(" NEGADO (excede necessidade)\n");
            pthread_mutex_unlock(&mutex);
            return -1;
        }
    }

    // Verifica se há recursos disponíveis
    for (int i = 0; i < NUM_RECURSOS; i++) {
        if (pedido[i] > disponivel[i]) {
            printf(" NEGADO (recursos insuficientes)\n");
            pthread_mutex_unlock(&mutex);
            return -1;
        }
    }

    // Alocação temporária
    for (int i = 0; i < NUM_RECURSOS; i++) {
        disponivel[i] -= pedido[i];
        alocacao[cliente][i] += pedido[i];
        necessidade[cliente][i] -= pedido[i];
    }

    // Verifica segurança
    if (verificar_seguranca()) {
        printf(" Pedido APROVADO\n");
        mostrar_estado();
        pthread_mutex_unlock(&mutex);
        return 0;
    }

   
    printf("desfaz uma alocação\n");// quando a verificação de segurança
    //  indica que o sistema entraria em um estado inseguro

    for (int i = 0; i < NUM_RECURSOS; i++) {
        disponivel[i] += pedido[i];
        alocacao[cliente][i] -= pedido[i];
        necessidade[cliente][i] += pedido[i];
    }

    mostrar_estado();

    pthread_mutex_unlock(&mutex);
    return -1;
}


int liberar_recursos(int cliente, int liberar[]) {

    pthread_mutex_lock(&mutex);

    printf("\n Cliente %d liberando: ", cliente);
    for (int i = 0; i < NUM_RECURSOS; i++)
        printf("%d ", liberar[i]);
    printf("\n");

    for (int i = 0; i < NUM_RECURSOS; i++) {

        if (liberar[i] > alocacao[cliente][i]) {
            printf(" ERRO: liberando mais do que possui\n");
            pthread_mutex_unlock(&mutex);
            return -1;
        }

        disponivel[i] += liberar[i];
        alocacao[cliente][i] -= liberar[i];
        necessidade[cliente][i] += liberar[i];
    }

    mostrar_estado();

    pthread_mutex_unlock(&mutex);
    return 0;
}


void *thread_cliente(void *arg) {
    int id = *(int *)arg;

    while (1) {

        int pedido[NUM_RECURSOS];

        for (int i = 0; i < NUM_RECURSOS; i++) {
            pedido[i] = rand() % (necessidade[id][i] + 1);
        }

        solicitar_recursos(id, pedido);

        sleep(1);

        int liberar[NUM_RECURSOS];

        for (int i = 0; i < NUM_RECURSOS; i++) {
            liberar[i] = rand() % (alocacao[id][i] + 1);
        }

        liberar_recursos(id, liberar);

        sleep(1);
    }

    return NULL;
}


int main(int argc, char *argv[]) {

    if (argc != NUM_RECURSOS + 1) {
        printf("Uso: ./a.out <r1> <r2> <r3>\n");
        return 1;
    }

    srand(time(NULL));

    pthread_t threads[NUM_CLIENTES];
    int ids[NUM_CLIENTES];

    pthread_mutex_init(&mutex, NULL);

    // Inicializa recursos disponíveis
    for (int i = 0; i < NUM_RECURSOS; i++) {
        disponivel[i] = atoi(argv[i + 1]);
    }

    // Inicializa estruturas
    for (int i = 0; i < NUM_CLIENTES; i++) {
        for (int j = 0; j < NUM_RECURSOS; j++) {
            maximo[i][j] = rand() % (disponivel[j] + 1);
            alocacao[i][j] = 0;
            necessidade[i][j] = maximo[i][j];
        }
    }

    mostrar_estado();

    // Cria threads
    for (int i = 0; i < NUM_CLIENTES; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, thread_cliente, &ids[i]);
    }

    // Aguarda threads
    for (int i = 0; i < NUM_CLIENTES; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}