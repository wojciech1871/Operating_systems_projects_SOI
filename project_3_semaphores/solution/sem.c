#include <sys/ipc.h>
#include <sys/sem.h>
#include "sem.h"

int binary_sem_allocate(key_t key, int sem_flags)
{
  return semget(key, 1, sem_flags);
}

int binary_sem_deallocate(int semid)
{
  union semun arg;
  return semctl(semid, 1, IPC_RMID, arg);
}

int binary_sem_init(int semid)
{
  union semun arg;
  unsigned short vals[1];
  vals[0] = 1;
  arg.array = vals;
  return semctl(semid, 0, SETALL, arg);
}

int binary_unlock(int semid)
{
  struct sembuf sem = { 0, +1, SEM_UNDO };
  return semop(semid, &sem, 1);
}

int binary_lock(int semid)
{
  struct sembuf sem = { 0, -1, SEM_UNDO };
  return semop(semid, &sem, 1);
}

int sem_allocate(key_t key, int sem_flags)
{
    return semget(key, 1, sem_flags);
}

int sem_deallocate(int semid)
{
    union semun arg;
    return semctl(semid, 1, IPC_RMID, arg);
}

int sem_init(int semid, int value)
{
    union semun arg;
    unsigned short vals[1];
    vals[0] = value;
    arg.array = vals;
    return semctl(semid, 0, SETALL, arg);
}

int sem_change(int semid, int val)
{
    if(val == 0) return -1;
    if(val < 0 ) val = -1;
    else val = 1;
    struct sembuf sem = { 0 , val, SEM_UNDO };
    return semop(semid, &sem, 1);
}
