#include <sys/types.h>
#include <errno.h>

union semun {
    int val;
    struct semid_ds *buf;
    ushort* array;
};

/*allocate a new binary semaphore*/
int binary_sem_allocate(key_t, int);

/*deallocate the binary semaphore*/
int binary_sem_deallocate(int);

/*initialize the binary semaphore with the value of 1*/
int binary_sem_init(int);

/*dencrement the semaphore value by 1*/
int binary_unlock(int);

/*dencrement the semaphore value by 1*/
int binary_lock(int);

/*create a semaphore with multiple values*/
int sem_allocate(key_t, int);

/*deallocate a semaphore*/
int sem_deallocate(int);

/*initialize the semaphore with a given value*/
int sem_init(int, int);

/*change the value of the semaphore by 1 or -1*/
int sem_change(int, int);
