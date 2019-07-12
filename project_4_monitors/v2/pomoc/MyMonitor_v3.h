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
    //wait_rand_time(4,5);
    enter();
    if(num_el == buff_size) wait(full);
    enter_item();
    signal(empty);
    leave();
  }

  void consumerA_task()
  {
    enter();
    if(num_el == 0) wait(empty);
    if(read[CONS_A]) wait(goA);
    if(num_el == 0) wait(empty);
    if(read[CONS_A] != 1) cout <<"Consumer A read item " <<read_item(CONS_A) <<endl;
    if(read[CONS_B])
      {
        if(flag[CONS_A] != 1){
        cout <<"Consumer A remove item " <<remove_item(CONS_A) <<endl;
        signal(full);
        }
        else signal(goB);
      }
    else signal(goB);
    leave();
  }

  void consumerB_task()
  {
    enter();
    if(num_el == 0) wait(empty);
    if(read[CONS_B]) wait(goB);
    if(num_el == 0) wait(empty);
    if(read[CONS_B] != 1) cout <<"Consumer B read item " <<read_item(CONS_B) <<endl;
    if(read[CONS_A])
      {
        if(flag[CONS_B] != 1) {
        cout <<"Consumer B remove item " <<remove_item(CONS_B) <<endl;
        signal(full);
        }
        else signal(goA);
      }
    else signal(goA);
    leave();
  }

  void consumerC_task()
  {

  }

private:
  int *buffer;
  int buff_beg, buff_end, buff_size;
  int num_el;
  unsigned char read[3];
  unsigned char flag[3];
  Condition full, empty, goA, goB;

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
    for(int i=0; i<3; i++)
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
