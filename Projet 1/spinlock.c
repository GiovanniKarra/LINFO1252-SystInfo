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
    //printf("lock\n");
    while (test_and_set(&(mutex->lock), 1)) {
        #ifdef METH2
        while (mutex->lock);
        #endif

        #ifdef BACKOFF
        static int wait_count = 0;
        int wait_time = MINWAIT + rand()*(wait_count/1000);
        if (wait_time > MAXWAIT) wait_time = MAXWAIT;
        for (int i = 0; i < wait_time; i++);
        #endif
    }
    //printf("enter\n");
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
    while (1) {
        while(sem->val <= 0);
        lock(&(sem->mutex));
        if (sem->val > 0) {
            sem->val--;
            unlock(&(sem->mutex));
            break;
        }
        unlock(&(sem->mutex));
    }
}

void my_post(my_sem_t *sem) {
    lock(&(sem->mutex));
    sem->val++;
    unlock(&(sem->mutex));
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
