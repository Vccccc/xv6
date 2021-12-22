#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void redirect(int k, int pd[2])
{
    dup2(pd[k], k);
    close(pd[0]);
    close(pd[1]);
}


int main()
{
    int pd[2];
    pipe(pd);
    if(int p = fork())
    {
        redirect(0, pd);
        int pd2[2];
        for(int i = 0; i < 5; i++)
        {
            int n;
            // read(0, &n, sizeof n);
            // printf("%d\n", n);
            fflush(stdout);
            pipe(pd2);
            if(fork())
            {
                redirect(0, pd2);
                read(0, &n, sizeof(n));
                printf("%d\n", n);
                fflush(stdout);
            }
            else 
            {
                redirect(1, pd2);
            
                for(int i = 0; i < 10; i++)
                {
                    int n;
                    if(read(0, &n, sizeof n) == 0)
                        return 0;
                    write(1, &n, sizeof n);
                    fprintf(stderr, "err pid: %d, %d\n", getpid(), n);
                }
                return 0;
            }
        }
        wait(0);
    }
    else 
    {
        redirect(1, pd);
        for(int i = 0; i < 30; i++)
            write(1, &i, sizeof(i));
    }
    return 0;
}