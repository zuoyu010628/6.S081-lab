#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
void go(int p[2])
{
    close(p[1]);

    int l;
    if ((read(p[0], &l, 4)) != 4) {
        fprintf(2, "primes: failed to read from parent\n");
        exit(1);
    }
    printf("prime %d\n", l);
    
    int l2r[2];
    int n;
    int flag = 0;
    while (read(p[0], &n, 4) == 4)
    {
        if (n % l != 0) {
            if (flag == 0) {
                pipe(l2r);
                if (fork() == 0) {
                    close(p[0]);
                    go(l2r);
                }
                else {
                    close(l2r[0]);
                    flag = 1;
                }
            }
            write(l2r[1], &n, 4);
        }
    }

    close(p[0]);
    close(l2r[1]);
    //wait for children return
    while (wait(0) != -1) ;
    exit(0);
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
        wait(0);
    }
    exit(0);
}