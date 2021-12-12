#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main(int argc, char* argv[])
{
    if(argc <= 1){
        fprintf(2, "usage sleep seconds\n");
        exit(1);
    }
    int sec = atoi(argv[1]);
    sleep(sec);
    exit(0);
}