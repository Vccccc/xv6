#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

int find(const char* path, const char* name)
{
    struct stat st;
    struct dirent de;
    char buf[512];
    int fd;
    char* p;
    if((fd = open(path, 0)) < 0)
    {
        fprintf(2, "can not open: %s\n", path);
        close(fd);
        return -1;
    }
    if(fstat(fd, &st) < 0)
    {
        fprintf(2, "can not fstat\n");
        close(fd);
        return -1;
    }

    if(st.type != T_DIR)
    {
        fprintf(2, "not dir: %s, %d\n", path, st.type);
        close(fd);
        return -1;
    }

    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        fprintf(2, "find: path too long\n");
        close(fd);
        return -1;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    *p = 0;
    while(read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if(de.inum == 0)
        {
            continue;
        }
        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        {
            continue;
        }
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0; 
        if(stat(buf, &st) < 0)
        {
            fprintf(2, "can not stat: %s\n", buf);
            close(fd);
            return -1;
        }
        if(st.type == T_FILE)
        {
            if(strcmp(de.name, name) == 0)
            {
                printf("%s\n", buf);
            }
        }
        else if(st.type == T_DIR)
        {
            if(find(buf, name) < 0)
            {
                close(fd);
                return -1;
            }
        }
    }
    close(fd);
    return 0;
}

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        fprintf(2, "usage dir filename\n");
        exit(0);
    }

    const char* path = argv[1];
    const char* name = argv[2];
    find(path, name);
    exit(0);
}