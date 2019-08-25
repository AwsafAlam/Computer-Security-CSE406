#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int foo(FILE *fn)
{
	char str[100];
	fread(str, sizeof(char), 300, fn);
	// strcpy(buffer, str);
	return 1;
}

int main(int argc, char **argv)
{
	char str[400];
	FILE *badfile;
	
	badfile = fopen("badfile","r");
	foo(badfile);

	printf("Returned Properly\n");
	return 1;
}

