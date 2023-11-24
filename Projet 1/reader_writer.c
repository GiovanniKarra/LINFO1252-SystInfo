#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define NB_READS 2560
#define NB_WRITES 640


int VERBOSE;
int NB_READERS;
int NB_WRITERS;
int global_wr = 0;
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
    while (global_wr+1 < NB_WRITES){
        pthread_mutex_lock(&mutex_writecount);
        writecount++;
        if (writecount == 1) {
            if (VERBOSE) printf("le premier écrivain vient d'arriver! Il a le seum parce qu'il attend\n");
            sem_wait(&rsem);
        }
        pthread_mutex_unlock(&mutex_writecount);
        sem_wait(&wsem);
        global_wr++;
        if (VERBOSE) printf("écrivain %d est rentré\n", global_wr);
        for (int i = 0; i < 10000; i++);
        if (VERBOSE) printf("écrivain %d est parti...\n", global_wr);
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
    while (global_rd+1 < NB_READS){
        pthread_mutex_lock(&z);
        sem_wait(&rsem);
        pthread_mutex_lock(&mutex_readcount);
        // exclusion mutuelle, readercount
        readcount++;
        global_rd++;
        if (readcount == 1) {
            // arrivée du premier reader
            sem_wait(&wsem);
        }
        if (VERBOSE) printf("lecteur %d est rentré\n",global_rd);
        pthread_mutex_unlock(&mutex_readcount);
        sem_post(&rsem);
        pthread_mutex_unlock(&z);
        for (int i = 0; i < 10000; i++);
        
        pthread_mutex_lock(&mutex_readcount);
        if (VERBOSE) printf("lecteur %d est parti\n",global_rd);
        // exclusion mutuelle, readcount
        readcount--;
        if (readcount == 0) {
            // départ du dernier reader
            sem_post(&wsem);
        }
        pthread_mutex_unlock(&mutex_readcount);
       
    }
    return NULL;
}

int main(int argc, char const *argv[]) {

    // ARGUMENT PARSING
    VERBOSE = FALSE;
    if (argc != 3) {
        if (argc < 3 || strcmp(argv[1], "-v") != 0) {
            printf("\033[31mERROR : EXPECTED 2 ARGUMENTS, BUT GOT %d !\033[0m\n", argc-1);
            return 1;
        }
        VERBOSE = TRUE;
    }

    NB_READERS = atoi(argv[1+VERBOSE]);
    NB_WRITERS = atoi(argv[2+VERBOSE]);

    if (NB_READERS <= 0 || NB_WRITERS <= 0) {
        printf("\033[31mERROR: EXPECTED NON-NULL POSITIVE INTEGER ARGUMENTS!\033[0m\n");
        return 1;
    }
    // END ARGUMENT PARSING

    sem_init(&wsem, 0, 1);
    sem_init(&rsem, 0, 1);
    
    pthread_t read[NB_READERS], write[NB_WRITERS];
    
    pthread_mutex_init(&mutex_readcount, NULL);
    pthread_mutex_init(&mutex_writecount, NULL);
    pthread_mutex_init(&z, NULL);

    for(int i = 0; i < NB_WRITERS; i++) {
        pthread_create(&read[i], NULL, &reader, NULL);
    }
    for(int i = 0; i < NB_WRITERS; i++) {
        pthread_create(&write[i], NULL, &writer, NULL);
    }

    for(int i = 0; i < NB_READERS; i++) {
        pthread_join(read[i], NULL);
    }   
    for(int i = 0; i < NB_WRITERS; i++) {
        pthread_join(write[i], NULL);
    }

    pthread_mutex_destroy(&mutex_readcount);
    pthread_mutex_destroy(&mutex_writecount);
    pthread_mutex_destroy(&z);
    sem_destroy(&wsem);
    sem_destroy(&rsem);

    return 0;
}