#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void redirect(int k, int pd[2])
{
    close(k);
    dup(pd[k]);
    close(pd[0]);
    close(pd[1]);
}

void source()
{
    for(int i = 2; i < 35; i++)
    {
        write(1, &i, sizeof i);
    }
}

void cull(int p)
{
    int n;
    while(read(0, &n, sizeof n) > 0)
    {
        if(n % p != 0)
        {
            write(1, &n, sizeof(n));
        }
    }
}

void sink()
{
    int pd[2]; 
    for(;;)
    {
        int n;
        if(read(0, &n, sizeof n) <= 0)
        {
            return;
        }
        pipe(pd);
        printf("prime %d\n", n);
        if(fork())
        {
            redirect(0, pd);
            continue;
        }
        else 
        {
            redirect(1, pd);     
            cull(n);
            exit(0);
        }
    }

}
int main()
{
    int pd[2];
    pipe(pd);

    if(fork())
    {
        redirect(0, pd);
        sink();
        wait(0);
    }
    else 
    {
        redirect(1, pd);     
        source();
    }
    exit(0);
}