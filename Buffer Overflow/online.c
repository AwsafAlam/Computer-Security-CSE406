#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void matchPassword(FILE* f)
{
    char str[400];

    fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

    fread(str, sizeof(char), fsize, f);
	if(strcmp(str,"password")){
		printf("Password Matched\n");
	}
	else{
		printf("Wrong password\n");
	}
}


int main(int argc, char **argv)
{
    FILE *badfile;
	badfile = fopen("input","r");

	matchPassword(badfile);
    printf("You shall not pass\n");
    return 1;
}