#ifndef INTERCEPT_H_
#define INTERCEPT_H_

// Ce fichier est inclu lorsqu'il faut utiliser les primitives de synchronisation
// par attente active au lieu de celles de POSIX, dans quel cas nous changeons les
// définitions des fonctions pour appeler les nôtres à la place

#include <pthread.h>
#include <semaphore.h>
#include "spinlock.h"

#define pthread_mutex_t my_mutex_t
#define pthread_mutex_init my_mutex_init
#define pthread_mutex_destroy my_mutex_destroy
#define pthread_mutex_lock lock
#define pthread_mutex_unlock unlock

#define sem_t my_sem_t
#define sem_init my_sem_init
#define sem_destroy my_sem_destroy
#define sem_wait my_wait
#define sem_post my_post

#endif
