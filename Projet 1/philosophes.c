#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

// Le paramètre MY_MUTEX indique qu'il faut utiliser les primitives de synchronisation
// par attente active au lieu de celles de POSIX
#ifdef MY_MUTEX
#include "intercept.h"
#endif

#define N_CYCLES 1000000


int PHILOSOPHES;
int VERBOSE;
pthread_mutex_t *baguette;


// action de manger
void mange(int id) {
    if (VERBOSE) printf("Philosophe %d mange\n", id);
}


void* philosophe(void* arg) {
    int *id = (int*)arg; // identificateur du thread
    int left = *id;
    int right = (left + 1) % PHILOSOPHES;
    for (int i = 0; i < N_CYCLES; i++) {
        if(left < right) {
            pthread_mutex_lock(&baguette[left]);
            pthread_mutex_lock(&baguette[right]);
        }
        else {
            pthread_mutex_lock(&baguette[right]);
            pthread_mutex_lock(&baguette[left]);
        }
        mange(*id);
        pthread_mutex_unlock(&baguette[left]);
        pthread_mutex_unlock(&baguette[right]);
    }
    return NULL;
}


int main(int argc, char const *argv[]) {
    // ARGUMENT PARSING
    VERBOSE = false;
    if (argc != 2) {
        if (argc < 2 || strcmp(argv[1], "-v") != 0) {
            printf("\033[31;1mERROR : EXPECTED 1 ARGUMENT, BUT GOT %d !\033[0m\n", argc-1);
            return 1;
        }
        VERBOSE = true;
    }

    PHILOSOPHES = atoi(argv[1 + VERBOSE]);
    if (PHILOSOPHES == 0) {
        printf("\033[31;1mERROR : EXPECTED A NON-NULL INTEGER ARGUMENT !\033[0m\n");
        return 1;
    }
    // END ARGUMENT PARSING

    // SETUP
    pthread_t philosophes[PHILOSOPHES];
    baguette = (pthread_mutex_t*)malloc(PHILOSOPHES*sizeof(pthread_mutex_t));
    if (baguette == NULL) return 1;
    
    int err;
    for (int i = 0; i < PHILOSOPHES; i++) {
        err = pthread_mutex_init(&baguette[i], NULL);
        if (err != 0) return 1;
    }

    // START THREADS
    int arg[PHILOSOPHES];
    for (int i = 0; i < PHILOSOPHES; i++) {
        arg[i] = i;
        err = pthread_create(&philosophes[i], NULL, &philosophe, (void*)&arg[i]);
        if (err != 0) return 1;
    }

    // WAIT FOR THREADS TO FINISH
    for(int i = 0; i < PHILOSOPHES; i++) {
        err=pthread_join(philosophes[i], NULL);
        if(err!=0) return 1;
    }

    // DESTROY MUTEX
    for (int i = 0; i < PHILOSOPHES; i++) {
        err = pthread_mutex_destroy(&baguette[i]);
        if (err != 0) return 1;
    }

    // FREE RESSOURCES
    free(baguette);

    return 0;
}