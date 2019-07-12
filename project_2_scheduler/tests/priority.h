int set_priority(int number, int value, int pid)
{
	message m;
	m.m1_i1 = number;
	m.m1_i2 = value;
	m.m1_i3 = pid;
	return(_syscall(MM, SETPRI, &m));
}

int get_priority(int number, int pid)
{
	message m;
	m.m1_i1 = number;
	m.m1_i3 = pid;
	return(_syscall(MM, GETPRI, &m));
}

