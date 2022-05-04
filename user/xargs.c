
#include"kernel/types.h"
#include"kernel/param.h"
#include"user/user.h"

char buf[512];
int getline(char* arg[MAXARG], int argc) {
    int idx = 0;
    while (read(0, &buf[idx], 1)) {//read 1 byte every time
        if (buf[idx] == '\n') {//newline
            break;
        }
        idx++;
    }
    //no argument any more
    if (idx == 0) {
        return 0;
    }
    buf[idx] = 0;
    //get a new args
    arg[argc] = buf;
    argc++;
    return argc;
}
int main(int argc, char * argv[]) {
    if (argc < 2) {
        fprintf(2, "usage: xargs <command>\n");
        exit(1);
    }
    
    char *c = malloc(strlen(argv[1]));
    strcpy(c, argv[1]);
    char *arg[MAXARG];
    
    
    for (int i = 1; i < argc; i++) {
        arg[i - 1] = malloc(sizeof(argv[i]));
        strcpy(arg[i - 1], argv[i]);
    }

    int argc_l; 
    while ((argc_l = getline(arg, argc - 1)) != 0)
    {
        arg[argc_l] = 0;//exec last one must be 0
        if (fork() == 0) {
            exec(c, arg);
            fprintf(2, "xargs: exec %s failed\n", c);
            exit(1);
        }
        else {
            wait(0);
        }
    }
    
    exit(0);
    //return 0;
}
// mkdir a
// echo hello > a/b
// mkdir c
// echo hello > c/b
// echo hello > b
// find . b | xargs grep hello
