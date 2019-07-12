#include "monitor.h"

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
    enter();
    if(num_el == buff_size) wait(full);
    enter_item();
    if(num_el == 1) signal(empty);
    leave();
  }

  void consumerA_task()
  {
    enter();
    if(num_el == 0) wait(empty);
    if(read[CONS_A] == 0 && read[CONS_C] == 0)
    {
      cout <<"Consumer A read item " <<read_item(CONS_A) <<endl;
      if(flag[CONS_A] != 1)
      {
        if((read[CONS_A] == 1 && read[CONS_B] == 1)||(read[CONS_B] == 1 && read[CONS_C] == 1))
        {
          cout <<"Consumer A remove item " <<remove_item() <<endl;
          if(num_el == buff_size-1) signal(full);
        }
      }
    }
    leave();
  }

  void consumerB_task()
  {
    enter();
    if(num_el == 0) wait(empty);
    if(read[CONS_B] == 0)
    {
      cout <<"Consumer B read item " <<read_item(CONS_B) <<endl;
      if(flag[CONS_B] != 1)
      {
        if((read[CONS_A] == 1 && read[CONS_B] == 1)||(read[CONS_B] == 1 && read[CONS_C] == 1))
        {
          cout <<"Consumer B remove item " <<remove_item() <<endl;
          if(num_el == buff_size-1) signal(full);
        }
      }
    }
    leave();
  }

  void consumerC_task()
  {
    enter();
    if(num_el == 0) wait(empty);
    if(read[CONS_C] == 0 && read[CONS_A] == 0)
    {
      cout <<"Consumer C read item " <<read_item(CONS_C) <<endl;
      if(flag[CONS_C] != 1)
      {
        if((read[CONS_A] == 1 && read[CONS_B] == 1)||(read[CONS_B] == 1 && read[CONS_C] == 1))
        {
          cout <<"Consumer C remove item " <<remove_item() <<endl;
          if(num_el == buff_size-1) signal(full);
        }
      }
    }
    leave();
  }

private:
  int *buffer;
  int buff_beg, buff_end, buff_size;
  int num_el;
  char read[3];
  char flag[3];
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

  int remove_item()
  {
    int item=buffer[buff_beg];
    buffer[buff_beg] = 0;
    buff_beg++;
    buff_beg=buff_beg%buff_size;
    num_el--;
    return item;
  }
};
