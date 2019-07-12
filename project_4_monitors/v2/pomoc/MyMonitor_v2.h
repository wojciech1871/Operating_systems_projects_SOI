#include "monitor.h"
#include <time.h>

#define CONS_A  0
#define CONS_B  1
#define CONS_C  2

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
    for(int i=0; i<3; i++)
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
    wait_rand_time(4,5);
    enter();
    if(num_el == buff_size) wait(full);
    enter_item();
    if(num_el == 1)
    {
      signal(empty);
    }
    leave();
  }

  void consumerA_task()
  {
    wait_rand_time(1,2);
    enter();
    cout <<"Consumer A 1. line" <<endl;
    if(num_el == 0) wait(empty);
    cout <<"Consumer A 2. line" <<endl;
    if(read[CONS_A] == 0 && read[CONS_C] == 0)
    {
      cout <<"Consumer A read item " <<read_item(CONS_A) <<endl;
      if(flag[CONS_A] == 0)
      {
        if((read[CONS_A] == 1 && read[CONS_B] == 1)||(read[CONS_B] == 1 && read[CONS_C] == 1))
        {
          cout <<"Consumer A remove item " <<remove_item(CONS_A) <<endl;
          if(num_el == buff_size-1) signal(full);
        }
      }
    }
    cout <<"Consumer A 3. line" <<endl;
    leave();
  }

  void consumerB_task()
  {
    wait_rand_time(1,2);
    enter();
    cout <<"Consumer B 1. line" <<endl;
    if(num_el == 0) wait(empty);
    cout <<"Consumer B 2. line" <<endl;
    if(read[CONS_B] == 0)
    {
      cout <<"Consumer B read item " <<read_item(CONS_B) <<endl;
      if(flag[CONS_B] == 0)
      {
        if((read[CONS_A] == 1 && read[CONS_B] == 1)||(read[CONS_B] == 1 && read[CONS_C] == 1))
        {
          cout <<"Consumer B remove item " <<remove_item(CONS_B) <<endl;
          if(num_el == buff_size-1) signal(full);
        }
      }
    }
    cout <<"Consumer B 3. line" <<endl;
    leave();
  }

  void consumerC_task()
  {
    wait_rand_time(1,2);
    enter();
    cout <<"Consumer C 1. line" <<endl;
    if(num_el == 0) wait(empty);
    cout <<"Consumer C 2. line" <<endl;
    if(read[CONS_C] == 0 && read[CONS_A] == 0)
    {
      cout <<"Consumer C read item " <<read_item(CONS_C) <<endl;
      if(flag[CONS_C] == 0)
      {
        if((read[CONS_A] == 1 && read[CONS_B] == 1)||(read[CONS_B] == 1 && read[CONS_C] == 1))
        {
          cout <<"Consumer C remove item " <<remove_item(CONS_C) <<endl;
          if(num_el == buff_size-1) signal(full);
        }
      }
    }
    cout <<"Consumer C 3. line" <<endl;
    leave();
  }

private:
  int *buffer;
  int buff_beg, buff_end, buff_size;
  int num_el;
  unsigned char read[3];
  unsigned char flag[3];
  Condition full, empty;

  void enter_item()
  {
    int item=rand()%1000;
    buffer[buff_end]=item;
    buff_end++;
    buff_end=buff_end%buff_size;
    num_el++;
    cout <<"Producer entered item: "<<item <<endl;
    cout <<"\tNumber of elements in buffor: "<<num_el <<endl;
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
    for(int i=0; i<3; i++) read[i] = 0;   //reset flags
    flag[proc_id] = 1;
    return item;
  }
  void wait_rand_time(int a, int b)
  {
    if(b>a) sleep(rand()%(b-a+1)+a);
  }
};
