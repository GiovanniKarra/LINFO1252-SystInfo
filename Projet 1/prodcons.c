#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdbool.h>

// Le paramètre MY_MUTEX indique qu'il faut utiliser les primitives de synchronisation
// par attente active au lieu de celles de POSIX
#ifdef MY_MUTEX
#include "intercept.h"
#endif

// Nous augmentons RAND_MAX pour pouvoir avoir une valeur aléatoire allant
// de -2147483647 à +2149483647 en offsetant le résultat
#define RAND_MAX 2147483647*2

#define N_CONSPRODS 8192
#define BUFFER_SIZE 8


int VERBOSE;
int CONS;
int PROD;

int buffer[BUFFER_SIZE];

pthread_mutex_t buffer_mutex;
sem_t empty;
sem_t full;

int prod_count;
int cons_count;


void *prod() {
    while(true) {
        // PROCESS
        if (VERBOSE) printf("processing...\n");
        for (int i = 0; i < 10000; i++);

        sem_wait(&empty);
        pthread_mutex_lock(&buffer_mutex);
        if (prod_count >= N_CONSPRODS) {
            pthread_mutex_unlock(&buffer_mutex);
            sem_post(&full);
            return NULL;
        }
        // insert in buffer
        if (VERBOSE) printf("inserting in buffer...\n");
        buffer[prod_count%BUFFER_SIZE] = rand()-RAND_MAX/2;
        (prod_count)++;
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&full);
    }

    return NULL;
}

void *cons() {
    while(true) {
        sem_wait(&full);
        pthread_mutex_lock(&buffer_mutex);
        if (cons_count >= N_CONSPRODS) {
            pthread_mutex_unlock(&buffer_mutex);
            sem_post(&empty);
            return NULL;
        }
        // take from buffer
        if (VERBOSE) printf("reading from buffer...\n");
        int var = buffer[cons_count%BUFFER_SIZE];
        (cons_count)++;
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&empty);

        // PROCESS
        if (VERBOSE) printf("processing...\n");
        for (int i = 0; i < 10000; i++);
    }

    return NULL;
}


int main(int argc, char const *argv[]) {
    // ARGUMENT PARSING
    VERBOSE = false;
    if (argc != 3) {
        if (argc < 3 || strcmp(argv[1], "-v") != 0) {
            printf("\033[31mERROR : EXPECTED 2 ARGUMENTS, BUT GOT %d !\033[0m\n", argc-1);
            return 1;
        }
        VERBOSE = true;
    }

    PROD = atoi(argv[1 + VERBOSE]);
    CONS = atoi(argv[2 + VERBOSE]);
    if (CONS == 0 || PROD == 0) {
        printf("\033[31mERROR : EXPECTED A NON-NULL INTEGER ARGUMENT !\033[0m\n");
        return 1;
    }
    if (VERBOSE) printf("prod: %d, cons : %d\n", PROD, CONS);
    // END ARGUMENT PARSING

    // SETUP
    int err;
    err = pthread_mutex_init(&buffer_mutex, NULL);
    err += sem_init(&empty, 0, BUFFER_SIZE);
    err += sem_init(&full, 0, 0);
    if (err != 0) return 1;

    pthread_t threads[CONS+PROD];
    
    // START THREADS
    for (int i = 0; i < PROD; i++) {
        err = pthread_create(&threads[i], NULL, &prod, NULL);
        if (err != 0) return 1;
    }
    for (int i = PROD; i < PROD+CONS; i++) {
        err = pthread_create(&threads[i], NULL, &cons, NULL);
        if (err != 0) return 1;
    }
    // WAIT FOR THREADS
    for (int i = 0; i < PROD+CONS; i++) {
        err = pthread_join(threads[i], NULL);
        if (err != 0) return 1;
    }

    // DESTROY MUTEX & SEMAPHORE
    err = pthread_mutex_destroy(&buffer_mutex);
    err += sem_destroy(&empty);
    err += sem_destroy(&full);
    if (err != 0) return 1;

    return 0;
}
