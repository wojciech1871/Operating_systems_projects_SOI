#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <signal.h>
#include "sem.h"
#include "keys.h"
#include "tasks.h"


int main(int argc, char* argv[])
{
  pid_t pidA, pidB, pidC;
  srand(time(NULL));

  flags_id = shmget(KEY_FLAGS, 9*sizeof(int), IPC_CREAT | 0666);
  buffer_id = shmget(KEY_BUFF, BUFF_SIZE*sizeof(int), IPC_CREAT | 0666 );

  flags = (int*)shmat(flags_id, 0, 0);
  buffer = (int*)shmat(buffer_id, 0, 0);

  mutex_id = binary_sem_allocate(KEY_MUTEX, IPC_CREAT | 0666);
  read_id = binary_sem_allocate(KEY_READ, IPC_CREAT | 0666);
  remove_id = binary_sem_allocate(KEY_REMOVE, IPC_CREAT | 0666);

  empty_id = sem_allocate(KEY_EMPTY, IPC_CREAT | 0666);
  full_id = sem_allocate(KEY_FULL, IPC_CREAT | 0666);

  binary_sem_init(mutex_id);
  binary_sem_init(read_id);
  binary_sem_init(remove_id);

  sem_init(empty_id, BUFF_SIZE);
  sem_init(full_id, 0);

  for(int i=0; i<9; i+=1) flags[i]=0;
  for(int i=0; i<BUFF_SIZE; i+=1) buffer[i]=0;

  printf("ID's of shm segments and semaphores (if some of them 0 - sth going wrong): \n");
  printf("%d\n%d\n%d\n%d\n%d\n%d\n%d\n\n", flags_id, buffer_id, mutex_id, read_id, remove_id, empty_id, full_id);
  sleep(10);
  // Creating new tasks
  pidA=fork();
  if(pidA==0)
  {
    consumerA();
    //NOTREACHAED
  }
  pidB=fork();
  if(pidB==0)
  {
    consumerB();
    //NOTREACHAED
  }
  pidC=fork();
  if(pidC==0)
  {
    consumerC();
    //NOTREACHAED
  }
  producer();
  // Producer - stop working and kill customers
  kill(pidA, SIGKILL);
  kill(pidB, SIGKILL);
  kill(pidC, SIGKILL);

  binary_sem_deallocate(mutex_id);
  binary_sem_deallocate(read_id);
  binary_sem_deallocate(remove_id);

  sem_deallocate(empty_id);
  sem_deallocate(full_id);

  shmdt(flags);
  shmdt(buffer);

  shmctl(flags_id, IPC_RMID, NULL);
  shmctl(buffer_id, IPC_RMID, NULL);

  printf("\nProducer killed consumers, dealloacated semaphoresfores and shared memory\n");
  return(0);
}
