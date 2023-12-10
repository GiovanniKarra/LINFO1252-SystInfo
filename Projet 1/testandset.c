#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "spinlock.h"


int VERBOSE;
int cycle_max;
my_mutex_t verrou;

void *thread() {
    int cycle_count = 0;
    while (cycle_count < cycle_max) {
        lock(&verrou);
        if (VERBOSE) printf("%d processing...\n", cycle_count);
        for (int i = 0; i < 10000; i++);
        unlock(&verrou);
        cycle_count++;
    }
    return NULL;
}

// void *testing() {
//     int prev = verrou->lock;
//     while (TRUE) {
//         if (verrou->lock != prev) {
//             printf("value : %d\n", verrou->lock);
//             prev = verrou->lock;
//         }
//     }
//     return NULL;
// }


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
    int N = atoi(argv[1 + VERBOSE]);
    if (N == 0) {
        printf("\033[31;1mERROR : EXPECTED A NON-NULL INTEGER ARGUMENT !\033[0m\n");
        return 1;
    }
    cycle_max = 6400/N;
    // END ARGUMENT PARSING

    my_mutex_init(&verrou, NULL);
    pthread_t threads[N];
    int err;
    for (int i = 0; i < N; i++) {
        err = pthread_create(&threads[i], NULL, &thread, NULL);
        if (err != 0) return 1;
    }

    for (int i = 0; i < N; i++) {
        err = pthread_join(threads[i], NULL);
        if (err != 0) return 1;
    }
    
    my_mutex_destroy(&verrou);

    return 0;
}
