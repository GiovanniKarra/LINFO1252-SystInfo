#ifndef SPINLOCK_H_
#define SPINLOCK_H_

#ifdef BACKOFF
#define MINWAIT 1000
#define MAXWAIT 100000000
#endif

typedef struct mutex {
    int lock;
} my_mutex_t;

typedef struct sem {
    my_mutex_t mutex;
    int val;
} my_sem_t;

void lock(my_mutex_t* verrou);
void unlock(my_mutex_t* verrou);
int my_mutex_init(my_mutex_t *mutex, void* attr);
int my_mutex_destroy(my_mutex_t *mutex);

void my_wait(my_sem_t *sem);
void my_post(my_sem_t *sem);
int my_sem_init(my_sem_t *sem, int pshared, int value);
int my_sem_destroy(my_sem_t *sem);

#endif
