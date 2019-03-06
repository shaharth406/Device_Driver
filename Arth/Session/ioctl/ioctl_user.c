#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>
 
#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)
 
int main()
{
	int fd;
	struct temp
	{
	int value1;
	char str[20];
	int value2;

	};

	struct temp rd,wr;
	struct temp *p=&rd;
	struct temp *q=&wr;

        int32_t value, number;
        printf("*********************************\n");
 
        printf("\nOpening Driver\n");
        fd = open("/dev/my_device", O_RDWR);
        if(fd < 0)
	{
                printf("Cannot open device file...\n");
                return 0;
        }
 
        printf("Enter the Value to send\n");
        scanf("%d",&p->value1);

        
        printf("Enter the string\n");
        scanf("%s",p->str);


        printf("Enter the Value to send\n");
        scanf("%d",&p->value2);

       
	printf("Writing Value to Driver\n");
        ioctl(fd, WR_VALUE, (struct temp*)p); 


        printf("Reading Value from Driver\n");
        ioctl(fd, RD_VALUE, (struct temp*)q);

        printf("Value_1 is %d\n",q->value1); 
	printf("String is %s\n",q->str);
	printf("Value_2 is %d\n",q->value2);

	printf("Closing Driver\n");      
	close(fd);
}
