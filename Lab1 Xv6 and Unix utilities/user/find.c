#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    // 打开指定路径目录，并获取该目录文件状态
    if ((fd = open(path, 0)) < 0)
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
    if (st.type == T_FILE)
    {
        fprintf(2, "find: can't find files in a file\n");
        exit(1);
    }
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        printf("find: path too long\n");
        exit(1);
    }
    strcpy(buf, path);
    // 指针p指向buf(path)的末尾
    p = buf + strlen(buf);
    // 在路径末尾添加/
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0)
        {
            fprintf(2, "find: cannot stat %s\n", buf);
            continue;
        }
        // 如果目录项的名称是 "." 或 ".."，则跳过当前迭代，继续递归查找
        if (st.type == T_DIR && strcmp(".", p) != 0 && strcmp("..", p) != 0)
            find(buf, filename);
        else if (st.type == T_FILE && strcmp(p, filename) == 0)
            printf("%s\n", buf);
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    // 检查用户是否提供足够的命令行参数
    if (argc < 3)
    {
        fprintf(2, "Usage: find <path> <filename>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}