#include"kernel/types.h"
#include"user/user.h"
#include"kernel/stat.h"
#include"kernel/fs.h"

void find(char *path, char *name) {
    char buf[512];
    int fd;
    struct stat st;//stat is something have some information about file
    struct dirent de;//about directory? tell you file id and name in this directory 

    //error hint
    if ((fd = open(path, 0)) < 0) {//open path
        fprintf(2, "find: can not open %s\n", path);
        //exit(1);
        return;
    }

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof (buf)) {//i don`t know why
        //printf("%d, %d, %d\n", strlen(path), DIRSIZ, sizeof(buf));
        printf("find: path too long \n");
        return;
    }

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {//search all files in this directory
        //printf("ok\n");
        if (de.inum == 0) {// copy from ls.c & why? maybe 0 don`t belong to any file
            continue;
        }

        if(stat(de.name, &st) < 0) {//get stat about now flie
            fprintf(2, "find: can not stat %s\n", path);
            close(fd);
            //exit(1);
            return;
        }

        if (st.type == T_DIR) {//is a dir , find in the dir
            if (!strcmp("." , de.name) && !strcmp("..", de.name)) {
                printf("this is a dir\n");
                find(de.name, name);
            }
        }

        if (!strcmp(name, de.name)) {
            printf("%s/%s\n", path, de.name);
        }
    }

    close(fd);
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(2, "usage: find ... \n");
        exit(1);
    }

    find(argv[1], argv[2]);
    return 0;
}

