#include <fcntl.h> /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <semaphore.h>

// • Initialisation • 
pthread_mutex_t mutex_readcount; // protège readcount
pthread_mutex_t mutex_writecount; // protège writecount
pthread_mutex_t z; // un seul reader en attente
sem_t wsem; // acces exclusif a la db
sem_t rsem; // pour bloquer des readers
int readcount=0;
int writecount=0;

sem_init (&wsem, 0, 1);
sem_init (&rsem, 0, 1);

// • Writer • 
while(true){
    think_up_data();

    pthread_mutex_lock(&mutex_writecount);
    // section critique - writecount
    writecount=writecount+1;
    if (writecount==1){
        // premier writer arrive
        sem_wait(&rsem);
    }
    pthread mutex_unlock(&mutex_writecount);

    sem_wait(&wsem);
    // section critique, un seul writer à la fois
    write_database();
    sem_post(&wsem);

    pthread_mutex_lock(&mutex_writecount);
    // section critique - writecount
    writecount = writecount-1;
    if (writecount==0){
        // départ du dernier writer
        sem_post (&rsem);
    }
    pthread_mutex_unlock(&mutex_writecount);
}

// • Reader • 
while(true)
{
    pthread_mutex_lock(&z);
    // exclusion mutuelle, un seul reader en attente sur rsem
    sem_wait(&rsem);

    pthread_mutex_lock(&mutex_readcount);
    // exclusion mutuelle, readercount
    readcount=readcount+1;
    if (readcount==1){
    // arrivée du premier reader
        sem_wait(&wsem);
    }
    pthread_mutex_unlock(&mutex_readcount);
    sem_post(&rsem); // liberation prochain reader/writer
    pthread_mutex_unlock(&z);

    read_database();
    pthread_mutex_lock(&mutex_readcount) ;
    // exclusion mutuelle, readcount
    readcount=readcount-1;
    if(readcount==0){
    // départ du dernier reader
    sem_post(&wsem);
    }
    pthread_mutex_unlock(&mutex_readcount);
    use_data_read();
}   