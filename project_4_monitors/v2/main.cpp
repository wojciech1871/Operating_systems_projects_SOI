#include <iostream>
#include <time.h>
#include "MyMonitor.h"

#define BUFF_SIZE 20
#define CYCLES  60

using namespace std;


MyMonitor buffer(BUFF_SIZE);


void *consumerA(void *arg)
{
  for(;;)
  {
    wait_rand_time(2,3);
    buffer.consumerA_task();
  }
  //NOTREACHED
}

void *consumerB(void *arg)
{
  for(;;)
  {
    wait_rand_time(3,4);
    buffer.consumerB_task();
  }
  //NOTREACHED
}

void *consumerC(void *arg)
{
  for(;;)
  {
    wait_rand_time(3,4);
    buffer.consumerC_task();
  }
  //NOTREACHED
}

void producer(int n)
{
  for(int i=0; i<n; i++)
  {
    wait_rand_time(1,2);
    buffer.producer_task();
  }
  return;
}

int main()
{
  int rc;
  pthread_t thread_id[3];

  cout <<"Program started. In 5 sec it will start new 3 threads" <<endl <<endl;
  sleep(5);
  rc = pthread_create(&thread_id[0], NULL, &consumerA, NULL);
  if(rc != 0)
  {
    cout <<"Cannot create new thread" <<endl;
    return(-1);
  }
  rc = pthread_create(&thread_id[1], NULL, &consumerB, NULL);
  if(rc != 0)
  {
    cout <<"Cannot create new thread" <<endl;
    return(-1);
  }
  rc = pthread_create(&thread_id[2], NULL, &consumerC, NULL);
  if(rc != 0)
  {
    cout <<"Cannot create new thread" <<endl;
    return(-1);
  }

  producer(CYCLES);

  sleep(5);
  cout <<"Producer finished job" <<endl;
  return(0);
}
