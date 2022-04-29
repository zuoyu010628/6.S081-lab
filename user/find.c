#include"kernel/types.h"
#include"user/user.h"
#include"kernel/stat.h"
#include"kernel/fs.h"

void find(char *path, char *name) {//from ls to find
    char buf[512], *p;
    int fd;
    struct stat st;//stat is something have some information about file
    struct dirent de;//about directory? tell you file id and name in this directory 

    //error hint
    if ((fd = open(path, 0)) <  0) {//open path
        fprintf(2, "find: can not open %s\n", path);
        //exit(1);
        return;
    }

    if(fstat(fd, &st) < 0) {//get stat about now flie
        fprintf(2, "find: can not stat %s\n", path);
        close(fd);
        //exit(1);
        return;
    }

    switch(st.type) {
        case T_FILE:
        fprintf(2, "usage: find <directory> <filename>\n");
        exit(1);
        break;
        case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof (buf)) {//i don`t know why
        //printf("%d, %d, %d\n", strlen(path), DIRSIZ, sizeof(buf));
        printf("find: path too long \n");
        return;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {//search all files in this directory
        //printf("ok\n");
            if (de.inum == 0 || strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0) {// copy from ls.c & why? maybe 0 don`t belong to any file
                continue;
            }

            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            if (stat(buf, &st) < 0) {
                fprintf(2, "find: can not stat %s", buf);
                continue;
            }

            if (st.type == T_FILE) {
                if(strcmp(de.name, name) == 0) {
                    printf("%s\n", buf);
                }
            }
            else if (st.type == T_DIR) {
                find(buf, name);
            }
        }
        close(fd);
    }
    
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(2, "usage: find <directory> <filename> \n");
        exit(1);
    }

    find(argv[1], argv[2]);
    return 0;
}

