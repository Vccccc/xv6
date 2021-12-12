#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    int p[2];
    int p2[2];
    char c = 't';
    if(pipe(p) < 0)
    {
        fprintf(2, "pipe\n");
        exit(1);
    }
    if(pipe(p2) < 0)
    {
        fprintf(2, "pipe\n");
        exit(1);
    }

    if(fork() == 0)
    {
        close(p[1]);
        close(p2[0]);
        read(p[0], &c, 1);
        printf("%d: received ping\n", getpid());
        write(p2[1], &c, 1);
        close(p[0]);
        close(p2[1]);
        exit(0);
    }

    close(p[0]);
    close(p2[1]);
    write(p[1], &c, 1);
    read(p2[0], &c, 1);
    printf("%d: received pong\n", getpid());
    close(p[1]);
    close(p2[0]);
    wait(0);
    
    exit(0);
}