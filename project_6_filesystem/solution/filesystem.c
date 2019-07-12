#include "filesystem.h"

int createDisc(char *name, int size)
{
	FILE *disc;
	char buffer[1];
	struct	disc_info info;
	int sizeBytes = size*BLOCK;
	disc = fopen(name, "r");		//check whether such disc exists
	if(size < 3)
	{
		printf("Capacity to small. Cannot create new disc.\n");
		return -1;
	}
	if(disc != NULL)
	{
		printf("Disc %s already exists. Give another name.\n", name);
		fclose(disc);
		return -1;
	}

	disc = fopen(name, "wb+");		//if name is new - create new disc file and open in binary mode
	fseek(disc, sizeBytes-1, SEEK_SET);
	buffer[0] = '0';
	fwrite(buffer, sizeof(char), 1, disc);
	fseek(disc, 0, SEEK_SET);

	info.no_of_descriptor_blocks = 2;
	info.size = size;
	info.free = size-2;
	info.f_count = 0;

	fwrite(&info, sizeof(disc_info), 1, disc);

	fclose(disc);
	return 0;
}

int	deleteDisc(char* name)
{
	if(unlink(name) != 0)			//function unlink defined in unistd.h (POSIX)
	{
		printf("Cannot remove disc %s.\n", name);
		return -1;
	}
	printf("Disc %s was removed.\n", name);
	return 0;
}

int	cpy2v(char* disc_name, char* file_name)		//copy file to disc
{
	FILE *disc, *file;
	long int size_in_bytes, size, position;
	int free_bytes;
	struct disc_info info;
	struct descriptor current;
	char *buffer;

	disc = fopen(disc_name, "r+b");
	if(disc == NULL)
	{
		printf("Disc %s doesn't exist.\n", disc_name);
		return -1;
	}
	file = fopen(file_name, "r");
	if(file == NULL)
	{
		printf("File %s doesn't exists.\n", file_name);
		fclose(disc);
		return -1;
	}
	if(MAX_LENGTH < strlen(file_name))
	{
		printf("Name of file %s too long. Maximum size is 19 characters.\n", file_name);
		fclose(disc);
		fclose(file);
		return -1;
	}
	fseek(file, 0, SEEK_END);
	size_in_bytes = ftell(file);
	size = (size_in_bytes + BLOCK -1) / BLOCK;	//let's calculate number of blocks necessary for write file
	free_bytes = size * BLOCK - size_in_bytes;	//let's calculate internal fragmentation
	buffer = malloc(sizeof(char) * size_in_bytes);
	fread(&info, sizeof(disc_info), 1, disc);

	if(size > info.free)			//check whether is enough space for file
	{
		printf("Lack of free space on disc %s.\n", disc_name);
		fclose(disc);
		fclose(file);
		free(buffer);
		return -1;
	}

	for(int i = 0; i < info.f_count; i++)		//check whether file <file_name> exists on disc
	{
		fread(&current, sizeof(descriptor), 1, disc);
		if( strcmp(current.name, file_name) == 0)
		{
			printf("File %s is already saved on disc %s.\n", file_name, disc_name);
			fclose(disc);
			fclose(file);
			free(buffer);
			return -1;
		}
	}

	if(info.f_count == 0)
	{
		fseek(file, 0, SEEK_SET);
		fread(buffer, sizeof(char), size_in_bytes, file);
		fseek(disc, info.no_of_descriptor_blocks * BLOCK, SEEK_SET);
		fwrite(buffer, sizeof(char), size_in_bytes, disc);
		current.adress = info.no_of_descriptor_blocks;		//save information for first file
		current.size = size;
		current.free_bytes = free_bytes;
		strcpy(current.name, file_name);
		info.f_count++;
		info.free-=current.size;
		fseek(disc, 0, SEEK_SET);
		fwrite(&info, sizeof(disc_info), 1, disc);				//update information in descriptors' segment
		fwrite(&current, sizeof(descriptor), 1, disc);
		fclose(disc);
		fclose(file);
		free(buffer);
		return 0;
	}
					//check whether is enough space for new file descriptor
	if(info.no_of_descriptor_blocks *BLOCK - info.f_count * sizeof(info) - sizeof(disc_info) - sizeof(info) <0)
	{
		if(add_descriptor_block(disc) == -1)
		{
			printf("Lack of free space on disc %s for file descriptor.\n", disc_name);
			fclose(disc);
			fclose(file);
			free(buffer);
			return -1;
		}
		else
		{
			fseek(disc, 0, SEEK_SET);
			info.free--;
			fread(&info, sizeof(disc_info), 1, disc);
		}
	}

	fseek(disc, sizeof(disc_info) + (info.f_count - 1)*sizeof(descriptor), SEEK_SET);
	fread(&current, sizeof(descriptor), 1, disc);								//load descriptor of last file
	if((info.size - (current.adress + current.size)) > size)		//check whether is enough continuous space for file
	{																														//if not - it's necessary to shift data
		position = ftell(disc);
		fseek(file, 0, SEEK_SET);
		fread(buffer, sizeof(char), size_in_bytes, file);
		fseek(disc, (current.adress + current.size) * BLOCK, SEEK_SET);
		fwrite(buffer, sizeof(char), size_in_bytes, disc);
		fseek(disc, position, SEEK_SET);
		current.adress = current.adress + current.size;		//save information for file
		current.size = size;
		strcpy(current.name, file_name);
		current.free_bytes = free_bytes;
		fwrite(&current, sizeof(descriptor), 1, disc);		//update information for file in descriptors' segment
		info.f_count++;
		info.free-=current.size;
		fseek(disc, 0, SEEK_SET);
		fwrite(&info, sizeof(disc_info), 1, disc);
		fclose(disc);
		fclose(file);
		free(buffer);
		return 0;
	}
	if(reallocate(disc) == 0)			//earlier checked whether is enough space - now try to reallocate
	{
		fseek(disc, 0, SEEK_SET);
		fread(&info, sizeof(disc_info), 1, disc);
		fseek(disc, sizeof(disc_info) + (info.f_count - 1) * sizeof(descriptor), SEEK_SET);
		fread(&current, sizeof(descriptor), 1, disc);		//read descriptor of last file
		position = ftell(disc);
		fseek(file, 0, SEEK_SET);
		fread(buffer, sizeof(char), size_in_bytes, file);
		fseek(disc, (current.adress + current.size) * BLOCK, SEEK_SET);
		fwrite(buffer, sizeof(char), size_in_bytes, disc);
		fseek(disc, position, SEEK_SET);
		current.adress = current.adress + current.size;		//save information for file
		current.size = size;
		strcpy(current.name, file_name);
		current.free_bytes = free_bytes;
		fwrite(&current, sizeof(descriptor), 1, disc);		//update information for file in descriptors' segment
		info.f_count++;
		info.free-=current.size;
		fseek(disc, 0, SEEK_SET);
		fwrite(&info, sizeof(disc_info), 1, disc);
		fclose(disc);
		fclose(file);
		free(buffer);
		return 0;
	}
	fclose(disc);
	fclose(file);
	free(buffer);
	printf("Error occurs. Try again.\n");
	return -1;
}

int cpy2l(char* disc_name, char* file_name)
{
	FILE *disc, *file;
	struct disc_info info;
	struct descriptor current;
	char* buffer;
	int i;
	disc = fopen(disc_name, "r+b");

	file = fopen(file_name, "r");		//check whether such file exists in local directory
	if(file != NULL)
	{
		printf("File %s already exists in local directory.\n", file_name);
		fclose(disc);
		return -1;
	}
	if(disc == NULL)
	{
		printf("Disc %s doesn't exist.\n", disc_name);
		return -1;
	}
	fread(&info, sizeof(disc_info), 1, disc);
	for(i = 0; i < info.f_count; i++)
	{
		fread(&current, sizeof(descriptor), 1, disc);
		if( strcmp(current.name, file_name) == 0)
		{
			buffer = malloc (sizeof(char) * current.size * BLOCK - current.free_bytes);
			fseek(disc, current.adress *BLOCK, SEEK_SET);
			fread(buffer, sizeof(char), current.size *BLOCK - current.free_bytes, disc);
			file = fopen(file_name, "wb");
			fwrite(buffer, sizeof(char), current.size *BLOCK - current.free_bytes, file);
			fclose(file);
			fclose(disc);
			return 0;
		}
	}
	printf("File %s doesn't exist on disc %s.\n", file_name, disc_name);
	fclose(disc);
	return 1;
}

int deleteFile(char* disc_name, char* file_name)
{
	FILE* disc;
	struct disc_info info;
	struct descriptor current;
	int i;
	disc = fopen(disc_name, "r+b");
	if(disc == NULL)
	{
		printf("Disc %s doesn't exist.\n", disc_name);
		return -1;
	}
	fread(&info, sizeof(disc_info), 1, disc);
	fread(&current, sizeof(descriptor), 1, disc);
	if( strcmp( current.name, file_name) == 0)		//check first file
	{
		shift_right(disc, sizeof(descriptor), sizeof(disc_info) + sizeof(descriptor), (info.f_count - 1) * sizeof(descriptor));
		info.f_count--;
		info.free+=current.size;
		fseek(disc, 0, SEEK_SET);
		fwrite(&info, sizeof(disc_info), 1, disc);
		fclose(disc);
		return 0;
	}
	for(i = 1; i < info.f_count; i++)
	{
		fread(&current, sizeof(descriptor), 1, disc);
		if( strcmp( current.name, file_name) == 0)
		{
			shift_right(disc, sizeof(descriptor), sizeof(disc_info) + (i+1) * sizeof(descriptor), (info.f_count - i - 1) * sizeof(descriptor));
			info.f_count--;
			info.free+=current.size;
			fseek(disc, 0, SEEK_SET);
			fwrite(&info, sizeof(disc_info), 1, disc);
			fclose(disc);
			return 0;
		}
	}
	printf("File %s doesn't exist on disc %s.\n", file_name, disc_name);
	fclose(disc);
	return -1;
}

int ls(char* disc_name)
{
	FILE *disc;
	int i;
	struct disc_info info;
	struct descriptor current;
	disc = fopen(disc_name, "r+b");
	if(disc == NULL)
	{
		printf("Disc %s doesn't exist.\n", disc_name);
		return -1;
	}
	fread(&info, sizeof(disc_info), 1, disc);
	for(i = 0; i < info.f_count; i++)
	{
		fread(&current, sizeof(descriptor), 1, disc);
		printf("%d. file's name:\t%s\t", (i+1), current.name);
		printf("Size in blocks:\t%d\tSize in bytes:\t%d\n", current.size, (current.size)*BLOCK-current.free_bytes);
	}
	printf("\n");
	fclose(disc);
	return 0;
}

int info(char* disc_name)
{
	FILE* disc;
	struct disc_info info;
	struct descriptor current;
	disc = fopen(disc_name, "r+b");
	if(disc == NULL){
		printf("Disc %s doesn't exist.\n", disc_name);
		return -1;
	}
	fread(&info, sizeof(disc_info), 1, disc);
	printf("Descriptors\tAddress: 0\tSize: %d\n", info.no_of_descriptor_blocks);
	fread(&current, sizeof(descriptor), 1, disc);
	printf("Number of files saved on disc:\t%d\n", info.f_count);
	printf("Free space on disc in blocks:\t%d\n", info.free);
	printf("Free space on disc in bytes:\t%d\n", info.free*BLOCK);
	printf("\n");
	fclose(disc);
	return 0;
}

void help()
{
	printf("First argument is always name of disc. All available commands below:\n");
	printf("c \t<arg1> <arg2>\tCreate disc <arg1> of size <arg2>\n");
	printf("d \t<arg1>\t\tDelete disc <arg1> \n");
	printf("v \t<arg1> <arg2>\tCopy file <arg2> to disc <arg1>\n");
	printf("u \t<arg1> <arg2>\tCopy file <arg2> from disc <arg2> to local user's folder\n");
	printf("f \t<arg1> <arg2>\tDelete file <arg2> from disc <arg1>\n");
	printf("l \t<arg1>\t\tShow list of files saved on disc <arg1>\n");
	printf("i \t<arg1>\t\tCheck disc <arg1> condition\n");
	printf("h \t\t\tDisplay help\n");
	printf("q \t\t\tFinish work with program\n");
	printf("\n");
	return;
}


int reallocate(FILE* f)
{
	int nochange = 1;		//a priori let's assume that no changes was made
	int shift;
	int position;
	fseek(f, 0, SEEK_SET);
	struct disc_info info;
	struct descriptor current, prev;

	fread(&info, sizeof(disc_info), 1, f);
	fread(&current, sizeof(descriptor), 1, f);
	shift = current.adress - info.no_of_descriptor_blocks;		//shift first file
	if(shift != 0)
	{
		shift_right(f, shift * BLOCK, current.adress * BLOCK, current.size * BLOCK);
		current.adress = current.adress - shift;
		fseek(f, sizeof(disc_info), SEEK_SET);
		fwrite(&current, sizeof(descriptor), 1, f);
		nochange =0;
	}
	prev = current;
	for(int i = 1; i < info.f_count; i++)
	{
		fread(&current, sizeof(descriptor), 1, f);
		shift = current.adress - (prev.adress + prev.size);
		if(shift != 0)
		{
			position = ftell(f);			//remember position in descriptor segmnent before shifting
			shift_right(f, shift * BLOCK, current.adress * BLOCK, current.size * BLOCK);
			fseek(f, position, SEEK_SET);			//restore position after shifting
			current.adress = current.adress - shift;
			fwrite(&current, sizeof(descriptor), 1, f);
			nochange = 0;
		}
		prev = current;
	}
	return nochange;
}

int add_descriptor_block(FILE* f)		//add new descriptor block if necessary
{
	struct disc_info info;
	struct descriptor desc;

	fseek(f, 0, SEEK_SET);
	fread(&info, sizeof(disc_info), 1, f);
	fseek(f, (info.f_count - 1) * sizeof(descriptor), SEEK_CUR);
	fread(&desc, sizeof(descriptor), 1, f);			//read descriptor of last file on disc
	if(desc.adress + desc.size == info.size)		//no enough place for new descriptor block - try reallocate data
	{
		if(reallocate(f) == 1)			//if no change was made - we don't have space for new descriptor
			return -1;
	}
	shift_left(f, BLOCK, info.no_of_descriptor_blocks *BLOCK, (info.size - info.no_of_descriptor_blocks -1) * BLOCK);
	info.no_of_descriptor_blocks++;
	return 0;
}

int shift_right(FILE* f, int shift, int offset, int size)		//shift - distance in bytes to shift;
{																														//offset - position of bytes to shift
	if(size == 0) return 1;																		//size - number of bytes to shift
	if(shift == 0) return 1;
	char *buffer;
	buffer = malloc(sizeof(char)*size);
	fseek(f, offset, SEEK_SET);
	fread(buffer, sizeof(char), size, f);
	fseek(f, offset - shift, SEEK_SET);
	fwrite(buffer, sizeof(char), size, f);
	free(buffer);
	return 0;
}

int shift_left(FILE* f, int shift, int offset, int size)
{
	return shift_right(f, -shift, offset, size);
}
