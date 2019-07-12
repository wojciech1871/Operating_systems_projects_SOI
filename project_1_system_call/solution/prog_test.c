#include </usr/include/stdio.h>
#include </usr/include/lib.h>

int getprocnr(int);

int main(int argc, char *argv[])
{
	int proc_nr;
	int i;
	int pid_nr;
	pid_nr=atoi(argv[1]);
	if(argc==1)
	{
		printf("Nastepnym razem podaj numer PID\n");
		return 1;
	}
	for(i=0; i<10; i+=1)
	{
		proc_nr=getprocnr(pid_nr-9+i);
		if(proc_nr!=-1) printf("Indeks procesu o nr PID %d to %d\n", pid_nr, proc_nr);
		else printf("Brak procesu w tablicy procesow. Kod bledu: %d\n", errno);
	}
	return 0;
}

int getprocnr(int pid_number)
{
	int proc_number;
	message mess;
	mess.m1_i1=pid_number;
	proc_number=_syscall(0,78,&mess);
	return(proc_number);
}
