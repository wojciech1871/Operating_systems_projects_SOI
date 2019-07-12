#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#define BLOCK 256
#define MAX_LENGTH 20
struct disc_info
{
	int no_of_descriptor_blocks;
	int	size;
	int	free;
	int	f_count;
} disc_info;

struct descriptor
{
	int adress;
	int size;
	int free_bytes;
	char name[MAX_LENGTH];
} descriptor;

int createDisc(char *name, int size);
int deleteDisc(char *name);
int	cpy2v(char* disc_name, char* file_name);
int cpy2l(char* disc_name, char* file_name);
int deleteFile(char* disc_name, char* file_name);
int	ls(char *disc_name);
int info(char* disc_name);
void help();

int reallocate(FILE* f);
int add_descriptor_block(FILE* f);
int shift_right(FILE* f, int shift, int offset, int size);
int shift_left(FILE* f, int shift, int offset, int size);
