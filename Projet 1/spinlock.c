#include <stdio.h>
#include <stdlib.h>
#include "spinlock.h"


int test_and_set(int *verrou, int value) {
    int to_return = value;
    __asm__(
        "xchgl %0, (%1)\n\t"
        : "+r" (to_return), "+r" (verrou)
    );
    return to_return == value;
}

void lock(my_mutex_t *mutex) {
    while (test_and_set(&(mutex->lock), 1)) {
        #ifdef METH2
        while (mutex->lock);
        #endif
    }
}

void unlock(my_mutex_t *mutex) {
    test_and_set(&(mutex->lock), 0);
}

void my_mutex_init(my_mutex_t *mutex) {
    mutex->lock = 0;
}

void my_mutex_destroy(my_mutex_t *mutex) {
    free(mutex);
}

// int main(int argc, char const *argv[]) {
//     my_mutex_t verrou;
//     my_mutex_init(&verrou);
//     printf("%d\n", verrou.lock);
//     lock(&verrou);
//     printf("%d\n", verrou.lock);
//     unlock(&verrou);
//     printf("%d\n", verrou.lock);
//     return 0;
// }
