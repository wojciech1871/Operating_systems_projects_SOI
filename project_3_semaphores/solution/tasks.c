#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sem.h"
#include "keys.h"
#include "tasks.h"

void wait_rand_time(int a, int b)
{
  if(b>a) sleep(rand()%(b-a+1)+a);
  return;
}

void enter_item()
{
  int item=rand()%1000;
  buffer[BUFF_END]=item;
  BUFF_END++;
  BUFF_END=BUFF_END%BUFF_SIZE;
  NUM_EL++;
  printf("Producer entered item: %d\n", item);
  printf("\tNumber of elements in buffor: %d\n", NUM_EL);
  return;
}

int remove_item()
{
  int item=buffer[BUFF_BEG];
  buffer[BUFF_BEG] = 0;
  BUFF_BEG++;
  BUFF_BEG=BUFF_BEG%BUFF_SIZE;
  NUM_EL--;
  printf("\tNumber of elements in buffor: %d\n", NUM_EL);
  return item;
}


void producer()
{
  for(int i=0; i<40; i++)
  {
    wait_rand_time(7,8);
    //printf("Prod 1. line\n");
    sem_change(empty_id, -1);
    //printf("Prod 2. line\n");
    binary_lock(mutex_id);
    enter_item(BUFF_END,buffer);
    binary_unlock(mutex_id);
    sem_change(full_id, 1);
  }
}

void consumerA()
{
  while(TRUE)
  {
  	wait_rand_time(3,6);
    //printf("ConsumerA 1. line\n");
    sem_change(full_id, -1);
    //printf("ConsumerA 2. line\n");
  	binary_lock(read_id);
  	if(READ_A == 0 && READ_C==0)
  	{
      binary_lock(mutex_id);
  		printf("ConsumerA read item: %d\n", buffer[BUFF_BEG]);
  		READ_A=1;
      binary_unlock(mutex_id);
  	}
  	binary_unlock(read_id);
    sem_change(full_id, 1);
    //printf("ConsumerA 3. line\n");
  	binary_lock(remove_id);
  	if(FLAG_A != 1)
  	{
      //printf("ConsumerA 4. line\n");
  		binary_lock(read_id);
  		if((READ_A == 1 && READ_B == 1)||(READ_B == 1 && READ_C ==1))
  		{
        //printf("ConsumerA 5. line\n");
        sem_change(full_id, -1);
        //printf("ConsumerA 6. line\n");
  			binary_lock(mutex_id);
  			printf("ConsumerA removed item: %d\n", remove_item());
  			binary_unlock(mutex_id);
  			sem_change(empty_id, 1);
  			READ_A=0;
  			READ_B=0;
  			READ_C=0;
  			binary_unlock(read_id);
  			FLAG_A=1;
  			FLAG_B=0;
  			FLAG_C=0;
  		}
  		else binary_unlock(read_id);
  	}
  	binary_unlock(remove_id);
  }
}

void consumerB()
{
  while(TRUE)
  {
  	wait_rand_time(0,5);
    //printf("ConsumerB 1. line\n");
    sem_change(full_id, -1);
    //printf("ConsumerB 2. line\n");
  	binary_lock(read_id);
  	if(READ_B == 0)
  	{
      binary_lock(mutex_id);
  		printf("ConsumerB read item: %d\n", buffer[BUFF_BEG]);
  		READ_B=1;
      binary_unlock(mutex_id);
  	}
  	binary_unlock(read_id);
    sem_change(full_id, 1);
    //printf("ConsumerB 3. line\n");
  	binary_lock(remove_id);
  	if(FLAG_B != 1)
  	{
      //printf("ConsumerB 4. line\n");
  		binary_lock(read_id);
  		if((READ_A == 1 && READ_B == 1)||(READ_B == 1 && READ_C ==1))
  		{
        //printf("ConsumerB 5. line\n");
        sem_change(full_id, -1);
        //printf("ConsumerB 6. line\n");
  			binary_lock(mutex_id);
  			printf("ConsumerB removed item: %d\n", remove_item());
  			binary_unlock(mutex_id);
  			sem_change(empty_id, 1);
  			READ_A=0;
  			READ_B=0;
  			READ_C=0;
  			binary_unlock(read_id);
  			FLAG_A=0;
  			FLAG_B=1;
  			FLAG_C=0;
  		}
  		else binary_unlock(read_id);
  	}
  	binary_unlock(remove_id);
  }
}

void consumerC()
{
  while(TRUE)
  {
  	wait_rand_time(2,4);
    //printf("ConsumerC 1. line\n");
    sem_change(full_id, -1);
    //printf("ConsumerC 2. line\n");
  	binary_lock(read_id);
  	if(READ_A == 0 && READ_C==0)
  	{
      binary_lock(mutex_id);
  		printf("ConsumerC read item: %d\n", buffer[BUFF_BEG]);
  		READ_C=1;
      binary_unlock(mutex_id);
  	}
  	binary_unlock(read_id);
    sem_change(full_id, 1);
    //printf("ConsumerC 3. line\n");
  	binary_lock(remove_id);
  	if(FLAG_C != 1)
  	{
      //printf("ConsumerC 4. line\n");
  		binary_lock(read_id);
  		if((READ_A == 1 && READ_B == 1)||(READ_B == 1 && READ_C ==1))
  		{
        //printf("ConsumerC 5. line\n");
        sem_change(full_id, -1);
        //printf("ConsumerC 6. line\n");
  			binary_lock(mutex_id);
  			printf("ConsumerC removed item: %d\n", remove_item());
  			binary_unlock(mutex_id);
  			sem_change(empty_id, 1);
  			READ_A=0;
  			READ_B=0;
  			READ_C=0;
  			binary_unlock(read_id);
  			FLAG_A=0;
  			FLAG_B=0;
  			FLAG_C=1;
  		}
  		else binary_unlock(read_id);
  	}
  	binary_unlock(remove_id);
  }
}
