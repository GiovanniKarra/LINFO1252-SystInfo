#ifndef SPINLOCK_H_
#define SPINLOCK_H_

typedef struct mutex {
    int lock;
} my_mutex_t;

void lock(my_mutex_t* verrou);
void unlock(my_mutex_t* verrou);
void my_mutex_init(my_mutex_t *mutex);
void my_mutex_destroy(my_mutex_t *mutex);

#endif
