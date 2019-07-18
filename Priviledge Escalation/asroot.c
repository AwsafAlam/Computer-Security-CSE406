#include <stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
int main()
{
    
    setuid(geteuid());
    system("cat /etc/shadow");
    return 0;
}