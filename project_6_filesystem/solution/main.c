/* Interface for user */

#include <stdio.h>
#include "filesystem.h"
int main(int argc, char* argv[])
{
    char command, temp;
    char arg1[50], arg2[50];
    help();
    while(1)
    {
    	fflush(stdout);
    	printf("$ ");
    	command = getchar();
    	switch( command )
      {
    		case 'c':
    			scanf("%s", arg1);
    			scanf("%s", arg2);
    			createDisc(arg1, atoi(arg2));
    			break;
    		case 'd':
    			scanf("%s", arg1);
    			deleteDisc(arg1);
    			break;
    		case 'f':
    			scanf("%s", arg1);
    			scanf("%s", arg2);
    			deleteFile(arg1, arg2);
    			break;
    		case 'l':
    			scanf("%s", arg1);
    			ls(arg1);
    			break;
    		case 'i':
    			scanf("%s", arg1);
    			info(arg1);
    			break;
    		case 'v':
    			scanf("%s", arg1);
    			scanf("%s", arg2);
    			cpy2v(arg1, arg2);
    			break;
    		case 'h':
    			help();
    			break;
    		case 'q':
    			return 0;
    		case 'u':
    			scanf("%s", arg1);
    			scanf("%s", arg2);
    			cpy2l(arg1, arg2);
    			break;
    		default:
    			printf("Command wasn't recognized. Press \"h\" for more information\n");
    			break;
    	}
    	while ((temp = getchar()) != '\n' && temp != EOF) { }
    }
}
