#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char *
fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), 0, DIRSIZ - strlen(p));
    return buf;
}

void findFiles(const char *path, const char *name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    if ((fd = open(path, O_RDONLY)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        printf("find: path too long\n");
        exit(1);
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0 || !strcmp(".", de.name) || !strcmp("..", de.name))
        // 注意这个，dirent的第一项的de.name是.，防止无限递归LOL
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0)
        {
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        switch (st.type)
        {
        case T_DEVICE:
        case T_FILE:
            if (!name || strcmp(name, de.name) == 0)
                printf("%s\n", buf);
            break;
        case T_DIR:
            if (!name || !strcmp(de.name, name))
                printf("%s\n", buf);
            memmove(p, de.name, DIRSIZ);
            findFiles(buf, name);
            break;
        }
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if(argc < 2){
        findFiles(".", 0);
        exit(0);
    }
    findFiles(argv[1], argv[2]);
    exit(0);
}
