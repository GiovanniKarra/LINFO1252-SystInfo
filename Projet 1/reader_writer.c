#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <strings.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define NB_READERS 16
#define NB_WRITERS 4


int VERBOSE;
int total_readers = 0;
int total_writers = 0;
int max_readers;
int max_writers;
int global_wr = 1;
int global_rd = 0;

pthread_mutex_t mutex_readcount; // protège readcount
pthread_mutex_t mutex_writecount; // protège writecount
pthread_mutex_t z; // un seul reader en attente
sem_t wsem; // acces exclusif a la db
sem_t rsem; // pour bloquer des readers
int readcount = 0;
int writecount = 0;

//variable pour faciliter le debug
int current_reader;
int current_writer;

void *writer(void *arg) {
    while (global_wr < NB_WRITERS){
        pthread_mutex_lock(&mutex_writecount);
        writecount++;
        if (writecount == 1) {
            printf("le premier écrivain vient d'arriver! Il a le seum parce qu'il attend\n");
            sem_wait(&rsem);
        }       
        pthread_mutex_unlock(&mutex_writecount);
        printf("écrivain %d essaye de rentrer\n", global_wr);
        sem_wait(&wsem);
        printf("écrivain %d  est rentré\n", global_wr);
        sleep(1);
        global_wr ++;
        printf("écrivain %d est parti...\n", global_wr-1);
        sem_post(&wsem);
        pthread_mutex_lock(&mutex_writecount);
        writecount--;
        if (writecount == 0) {
            sem_post(&rsem);
        }
        pthread_mutex_unlock(&mutex_writecount);
    }
    return NULL;
}

void *reader(void *arg) {
    while (global_rd < NB_READERS){
        pthread_mutex_lock(&z);
        sem_wait(&rsem);
        pthread_mutex_lock(&mutex_readcount);
        // exclusion mutuelle, readercount
        readcount++;
        if (readcount == 1) {
            // arrivée du premier reader
            sem_wait(&wsem);
        }
        pthread_mutex_unlock(&mutex_readcount);
        sem_post(&rsem);
        pthread_mutex_unlock(&z);
        //action("reader");
        global_rd ++;
        printf("lecteur %d est rentré\n",global_rd);
        sleep(1);
        pthread_mutex_lock(&mutex_readcount);
        // exclusion mutuelle, readcount
        readcount--;
        if (readcount == 0) {
            // départ du dernier reader
            sem_post(&wsem);
        }
        pthread_mutex_unlock(&mutex_readcount);
        printf("lecteur %d est parti\n",global_rd);
    }
    return NULL;
}

int main(int argc, char const *argv[]) {

    // ARGUMENT PARSING
    if (argc != 3) {
        printf("\033[31mERROR: EXPECTED 2 ARGUMENTS, BUT GOT %d!\033[0m\n", argc - 1);
        return 1;
    }

    max_readers = atoi(argv[1]);
    max_writers = atoi(argv[2]);

    if (max_readers <= 0 || max_writers <= 0) {
        printf("\033[31mERROR: EXPECTED NON-NULL POSITIVE INTEGER ARGUMENTS!\033[0m\n");
        return 1;
    }
    // END ARGUMENT PARSING

    VERBOSE = FALSE; 

    sem_init(&wsem, 0, 1);
    sem_init(&rsem, 0, 1);
    
    pthread_t read[max_readers],write[max_writers];
    
    pthread_mutex_init(&mutex_readcount, NULL);
    pthread_mutex_init(&mutex_writecount, NULL);
    pthread_mutex_init(&z, NULL);

    int rd[max_readers];
    int wr[max_writers];

    for(int i = 0; i < max_writers; i++) {
        pthread_create(&read[i], NULL, (void *)reader, (void *)&rd[i]);
    }
    for(int i = 0; i < max_writers; i++) {
        pthread_create(&write[i], NULL, (void *)writer, (void *)&wr[i]);
    }

    for(int i = 0; i < max_readers; i++) {
        pthread_join(read[i], NULL);
    }   
    for(int i = 0; i < max_writers; i++) {
        pthread_join(write[i], NULL);
    }

    pthread_mutex_destroy(&mutex_readcount);
    pthread_mutex_destroy(&mutex_writecount);
    pthread_mutex_destroy(&z);
    sem_destroy(&wsem);
    sem_destroy(&rsem);

    return 0;
}