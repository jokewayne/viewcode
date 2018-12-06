#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define MAXSTRLEN 255


int main() {
    int fd;
    char msg[MAXSTRLEN];
    fd = open("/dev/char_chat", O_RDWR, S_IRUSR|S_IWUSR);
    if ( fd > 0 ) {
        while(1) {
            printf("please input something:\n");
            scanf("%s",msg);
            printf("get [%s]\n",msg);
            write(fd,msg,strlen(msg));
            if (strcmp(msg,"quit") == 0 ) {
                close(fd);
                break;
            }
        }
    } else {
        printf("char_chat open failed\n");
    }
    return 0;
}

