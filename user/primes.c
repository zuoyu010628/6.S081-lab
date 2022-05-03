#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
void go(int p[2])
{
    close(p[1]);

    int l;
    read(p[0], &l, 4);
    printf("prime %d\n", l);
    
    int l2r[2];
    int n;

    while (read(p[0], &n, 4))
    {
        if (n % l != 0) {
            pipe(l2r);
            if (fork() == 0) {
                go(l2r);
            }
            else {
                write(l2r[1], &n, 4);
            }
        }
    }
}

int main()
{
    int p[2];
    pipe(p);

    if (fork() == 0)
    { // child
        go(p);
        close(p[0]);
    }
    else
    { // parent
        close(p[0]);
        for (int i = 2; i <= 35; i++)
        {
            write(p[1], &i, 4); // int 4bytes
        }
        close(p[1]);
    }
    exit(0);
}