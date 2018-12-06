#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#define MAXSTRLEN 255

int main() {

    int fd, i ;
    char msg[MAXSTRLEN+1];
    char *index;
    fd = open("/dev/char_chat", O_RDWR,S_IRUSR|S_IWUSR);
    if ( fd > 0 ) {
        while(1) {
            for (i=0;i< (MAXSTRLEN+1);i++) msg[i]='\0';
            read(fd,msg,MAXSTRLEN);
            index = msg;
            while ( index < (msg + MAXSTRLEN) && *index != '\0') {
                printf("[%s]\n",index);
                if ( strcmp(index,"quit") == 0) {
                    close(fd);
                    return 0;
                }

                index += (strlen(index)+2);
            }
        }
    } else {
        printf("sink_chat open failed, %d\n", fd);
    }
    return 0;
}
