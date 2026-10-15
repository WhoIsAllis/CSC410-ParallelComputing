// Implement your own read write lock using mutex and condition variables!

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t readers_ok;
    pthread_cond_t writers_ok;
    int readers;        // active readers
    int writers;        // active writers 
    int waiting_writers; // waiting writers
} rwlock_t;

rwlock_t lock;
int shared_data = 0;

// --- Custom Reader-Writer Lock Functions ---
void rwlock_init(rwlock_t *lock) 
{
    // complete this
}

void rwlock_acquire_read(rwlock_t *lock) 
{
    // complete this
}

void rwlock_release_read(rwlock_t *lock) 
{
    // complete this
}

void rwlock_acquire_write(rwlock_t *lock) 
{
    // complete this
}

void rwlock_release_write(rwlock_t *lock) 
{
    // complete this
}

// ################################################################################################
// ########## Provided to test your read write lock implementation ################################

void* reader(void* arg) 
{
    int id = *(int*)arg;
    for (int i = 0; i < 5; i++) 
    {
        rwlock_acquire_read(&lock);
        printf("Reader %d reads: %d\n", id, shared_data);
        usleep(100000);
        rwlock_release_read(&lock);
        usleep(100000);
    }
    return NULL;
}

void* writer(void* arg) 
{
    int id = *(int*)arg;
    for (int i = 0; i < 5; i++) 
    {
        rwlock_acquire_write(&lock);
        shared_data += 10;
        printf("Writer %d updates value to: %d\n", id, shared_data);
        usleep(150000); 
        rwlock_release_write(&lock);
        usleep(100000); 
    }
    return NULL;
}

int main() 
{
    pthread_t readers[5], writers[2];
    int r_ids[5], w_ids[2];

    rwlock_init(&lock);

    // reader threads
    for (int i = 0; i < 5; i++) 
    {
        r_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &r_ids[i]);
    }

    // writer threads
    for (int i = 0; i < 2; i++) 
    {
        w_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &w_ids[i]);
    }

    for (int i = 0; i < 5; i++)
        pthread_join(readers[i], NULL);
    for (int i = 0; i < 2; i++)
        pthread_join(writers[i], NULL);

    return 0;
}
