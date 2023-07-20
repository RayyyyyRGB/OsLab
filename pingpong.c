
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define MAXSIZE 20

int main(int ac, char** av)
{
    int fd1[2];
    int fd2[2];
    int pid;
    int pidSub;
    int pidMain;
    char messageSub[MAXSIZE];
    char messageMain[MAXSIZE];
    
    if(pipe(fd1) == -1)
    {
        printf("create fd1 pipe failed");
        return(-1);
    }
    if(pipe(fd2) == -1)
    {
        printf("create fd2 pipe failed");
        return(-1);
    }
    
    if((pid = fork()) < 0)
    {
        printf("fork failed");
        return(-1);
    }
    else if(pid == 0)
    {
        pidSub = getpid();
        close(fd1[1]);
        close(fd2[0]);
        read(fd1[0],messageSub,MAXSIZE);
        printf("%d: received %s\n",pidSub,messageSub);
        write(fd2[1],"pong",sizeof("pong"));
        close(fd1[0]);
        close(fd2[1]);
        exit(0);
    }
    else if(pid > 0)
    {
        pidMain = getpid();
        close(fd1[0]);
        close(fd2[1]);
        write(fd1[1],"ping",sizeof("ping"));
        read(fd2[0],messageMain,MAXSIZE);
        printf("%d: received %s\n",pidMain,messageMain);
        close(fd1[1]);
        close(fd2[0]);
        wait(0);
        exit(0);
    }
    
    return(0);
}
