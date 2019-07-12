#include "monitor.h"
#include <time.h>

#define NUM_FLAGS 2

#define CONS_AC   0
#define CONS_B    1

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
    enter();
    if(num_el == buff_size) wait(full);
    enter_item();
    if(num_el > 0)
    {
      if(read[CONS_AC] == 0) signal(emptyAC);
      else if(read[CONS_B] == 0) signal(emptyB);
    }
    leave();
  }

  void consumerAC_task(char proc_id)
  {
    enter();
    if(num_el == 0 || read[CONS_AC]) wait(emptyAC);
    if(num_el != 0)
    {
      if(read[CONS_AC] != 1) cout <<"Consumer " <<proc_id <<" read item " <<read_item(CONS_AC) <<endl;
      if(read[CONS_B])
        {
          if(flag[CONS_AC] != 1)
          {
            cout <<"Consumer " <<proc_id <<" remove item " <<remove_item(CONS_AC) <<endl;
            if(num_el == buff_size - 1) signal(full);
            signal(emptyB);
          }
          else signal(emptyB);
        }
      else signal(emptyB);
    }
    else signal(full);
    leave();
  }

  void consumerB_task()
  {
    enter();
    if(num_el == 0 || read[CONS_B]) wait(emptyB);
    if(num_el != 0)
    {
      if(read[CONS_B] != 1) cout <<"Consumer B read item " <<read_item(CONS_B) <<endl;
      if(read[CONS_AC])
        {
          if(flag[CONS_B] != 1)
          {
            cout <<"Consumer B remove item " <<remove_item(CONS_B) <<endl;
            if(num_el == buff_size - 1) signal(full);
            signal(emptyAC);
          }
          else signal(emptyAC);
        }
      else signal(emptyAC);
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
  Condition full, emptyAC, emptyB;

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
