#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/times.h>
#include <unistd.h>
#include <lib.h>
#include <minix/type.h>
#include <time.h>
#include "priority.h"

int main(int argc, char* argv[])
{
	int i;
        int j;
        int result;
	int pid=getpid();
	struct tms time_buff;
	result = set_priority(3,10,pid);
	result = set_priority(4,10,pid);
	result = set_priority(5,0,pid);
	printf("\nA1 pid = %d\n", pid);
	sleep(1);
	for(i = 0; i < 10000; i ++)
		{
			for(j = 0; j < 10000; j++) 
			{
				result=(i-j)*(j-i)*i;
				result--;
				result=result-i*j;
				result=result-i;
				result++;
			}
		}
	printf("\nResult A1 = %d\n", result);
	printf("\nA1 base priority = %d\n", get_priority(4,pid)) ;
	times(&time_buff);
	printf("Process A1 user time: = %d\n", time_buff.tms_utime);
	printf("Process A1 sys time: = %d\n", time_buff.tms_stime);
	printf("Process A1 finished work\n");
	return 0;
}
