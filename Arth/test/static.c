#include<stdio.h>

static int i =10;
//int *ptr=&i;
int main()
{
static int i =5;
printf("value of i= %d\n",i);

return 0;
}
