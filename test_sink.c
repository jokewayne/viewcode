#include<fcntl.h>
#include <stdio.h>

#define DEVPATH "/dev/sink" 
enum IOCMD {
        TEST1 = 100,
        TEST2 ,
        PARAM ,
        TEST4 ,
        MONDAY,
        TUESDAY,
        SATURDAY,
        SUNDAY
};


int main(void) {
   int fd = 0;
   fd = open(DEVPATH,O_RDWR);
   if ( fd < 0 ) printf("open dev [%s] failed.\n", DEVPATH);
   printf("fd = %d\n", fd);
   ioctl(fd,TEST1, 1111);
   ioctl(fd,TEST2);
   ioctl(fd,PARAM, 666);
   ioctl(fd,TEST4);
   ioctl(fd,MONDAY);
   ioctl(fd,TUESDAY);
   ioctl(fd,SATURDAY);
   ioctl(fd,SUNDAY);
   ioctl(fd,TEST1,9999);
   ioctl(fd,TEST2);
   close(fd);
   printf("close fd\n");
   return 0;
}
