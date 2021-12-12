#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

void xargs(char* args[])
{
    if(fork())
    {
        wait(0);
    }
    else 
    {
        exec(args[0], args);
    }
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        fprintf(2, "usage xargs [command [initial-arguments]]\n");
        exit(1);
    }
    char* command = argv[1];
    char* args[MAXARG+1];
    const int kMaxArgLength = 512;
    char buf[kMaxArgLength+1];
    int nr = 0;
    int eof = 0;
    int argCount = argc - 1;
    char* p = buf;
    args[0] = command;
    for(int i = 2; i < argc; i++)
    {
        args[i-1] = argv[i];
    }
    while(nr < kMaxArgLength)
    {
        int n = read(0, buf+nr, 1);
        if(n == 0)
        {
            eof = 1;
            break;
        }
        if(buf[nr] == '\n')
        {
            buf[nr] = 0;
            argCount++;
            if(argCount > MAXARG)
            {
                fprintf(2, "to many args\n");
                exit(1);
            }
        }
        nr++;
    }
    buf[nr] = 0;
    if(!eof)
    {
        fprintf(2, "args too long\n");
        exit(1);
    }

    for(int i = argc-1; i < argCount; i++)
    {
        args[i] = p;
        p += strlen(p) + 1;
    } 
    args[argCount] = 0;
    xargs(args);
    exit(0);
}