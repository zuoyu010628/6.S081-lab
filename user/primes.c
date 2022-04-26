#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
void go(int p[2])
{
    close(p[1]);

    int l;
    read(p[0], &l, 4);
    printf("prime %d", l);
    
    int l2r[2];
    int n;

    while (read(p[0], &n, 4))
    {
        if (n % l != 0) {
            pipe(l2r);
            go(l2r);
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
    }
    else
    { // parent
        close(p[0]);
        for (int i = 2; i <= 35; i++)
        {
            write(p[1], &i, 4); // int 4bytes
        }
        close(p[1]);
        wait(0); // wati for child and pid_chile == 0
    }
}