#include "monitor.h"
#include <time.h>

#define NUM_FLAGS 3

#define CONS_A    0
#define CONS_B    1
#define CONS_C    2

using namespace std;

class MyMonitor
    : public Monitor
{
public:
  MyMonitor(int n)
  {
    srand(time(NULL));
    buff_size = n;
    buffer = new int[buff_size];
    buff_beg = 0;
    buff_end = 0;
    num_el = 0;
    for(int i=0; i<NUM_FLAGS; i++)
    {
      read[i] = 0;
      flag[i] = 0;
    }
  }
  ~MyMonitor()
  {
    delete buffer;
  }

  void producer_task()
  {
    wait_rand_time(0,1);
    enter();
    cout <<"Producer 1. line" <<endl;
    if(num_el == buff_size) wait(full);
    cout <<"Producer 2. line" <<endl;
    enter_item();
    if(num_el == 1) signal(emptyB);
    leave();
  }

  void consumerA_task()
  {
    wait_rand_time(2,3);
    enter();
    //cout <<"Consumer A 1. line" <<endl;
    if(num_el == 0 || read[CONS_A]) wait(emptyA);
    //cout <<"Consumer A 2. line" <<endl;
    if(num_el != 0)
    {
      if(read[CONS_A] != 1 && read[CONS_C] != 1) cout <<"Consumer A read item " <<read_item(CONS_A) <<endl;
      if(read[CONS_B])
        {
          if(flag[CONS_A] != 1)
          {
            cout <<"Consumer A remove item " <<remove_item(CONS_A) <<endl;
            if(num_el == buff_size - 1) signal(full);
            //signal(emptyC);
          }
          else
          {
            signal(emptyC);
            //signal(emptyB);
          }
        }
      else signal(emptyB);
    }
    else signal(full);
    leave();
  }

  void consumerB_task()
  {
    wait_rand_time(3,4);
    enter();
    cout <<"Consumer B 1. line" <<endl;
    if(num_el == 0 || read[CONS_B]) wait(emptyB);
    cout <<"Consumer B 2. line" <<endl;
    if(num_el != 0)
    {
      if(read[CONS_B] != 1) cout <<"Consumer B read item " <<read_item(CONS_B) <<endl;
      if(read[CONS_A] || read[CONS_C])
        {
          if(flag[CONS_B] != 1)
          {
            cout <<"Consumer B remove item " <<remove_item(CONS_B) <<endl;
            if(num_el == buff_size - 1) signal(full);
          }
          else
          {
            signal(emptyA);
            //signal(emptyC);
          }
        }
      else
      {
        //signal(emptyA);
        signal(emptyC);
      }
    }
    else signal(full);
    leave();
  }

  void consumerC_task()
  {
    wait_rand_time(3,4);
    enter();
    cout <<"Consumer C 1. line" <<endl;
    if(num_el == 0 || read[CONS_C]) wait(emptyC);
    cout <<"Consumer C 2. line" <<endl;
    if(num_el != 0)
    {
      if(read[CONS_A] != 1 && read[CONS_C] != 1) cout <<"Consumer C read item " <<read_item(CONS_C) <<endl;
      if(read[CONS_B])
        {
          if(flag[CONS_C] != 1)
          {
            cout <<"Consumer C remove item " <<remove_item(CONS_C) <<endl;
            if(num_el == buff_size - 1) signal(full);
            //signal(emptyA);
          }
          else
          {
            signal(emptyA);
            //signal(emptyB);
          }
        }
      else signal(emptyB);
    }
    else signal(full);
    leave();
  }

private:
  int *buffer;
  int buff_beg, buff_end, buff_size;
  int num_el;
  unsigned char read[NUM_FLAGS];
  unsigned char flag[NUM_FLAGS];
  Condition full, emptyA, emptyB, emptyC;

  void enter_item()
  {
    int item=rand()%1000;
    buffer[buff_end]=item;
    buff_end++;
    buff_end=buff_end%buff_size;
    num_el++;
    cout <<"\t\tProducer entered item: " <<item <<endl;
    cout <<"\tNumber of elements in buffor: " <<num_el <<endl;
  }

  int read_item(unsigned char proc_id)
  {
    int item = buffer[buff_beg];
    read[proc_id] = 1;
    return item;
  }

  int remove_item(unsigned char proc_id)
  {
    int item=buffer[buff_beg];
    buffer[buff_beg] = 0;
    buff_beg++;
    buff_beg=buff_beg%buff_size;
    num_el--;
    cout <<"\tNumber of elements in buffor: " <<num_el <<endl;
    for(int i=0; i<NUM_FLAGS; i++)
    {
      flag[i] = 0;
      read[i] = 0;
    }
    flag[proc_id] = 1;
    return item;
  }
  void wait_rand_time(int a, int b)
  {
    if(b>a) sleep(rand()%(b-a+1)+a);
  }
};
