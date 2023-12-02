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

int my_mutex_init(my_mutex_t *mutex, void *attr) {
    mutex->lock = 0;
    return 0;
}

int my_mutex_destroy(my_mutex_t *mutex) {
    // DESTROY
    return 0;
}

void my_wait(my_sem_t *sem) {
    if (sem->val <= 1) lock(&(sem->mutex));
    sem->val--;
    if (sem->val > 0 && sem->mutex.lock) unlock(&(sem->mutex));
}

void my_post(my_sem_t *sem) {
    sem->val++;
    if (sem->val > 0 && sem->mutex.lock) unlock(&(sem->mutex));
}

int my_sem_init(my_sem_t *sem, int pshared, int value) {
    my_mutex_init(&(sem->mutex), NULL);
    sem->val = value;
    return 0;
}

int my_sem_destroy(my_sem_t *sem) {
    my_mutex_destroy(&(sem->mutex));
    return 0;
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
