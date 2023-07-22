#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
int main(int argc, char *argv[])
{
    // MAXARG在kernel/param.h中声明
    char *argvs[MAXARG];
    // 索引
    int index = 0;
    // 缓冲区，用于存放从管道读出的数据
    char buf[512] = {"\0"};
    int n;
    // 检查用户是否提供足够的命令行参数
    if (argc < 2)
    {
        fprintf(2, "Usage: xargs command\n");
        exit(1);
    }
    // 略去 xargs ，用来保存命令行参数
    for (int i = 1; i < argc; ++i)
    {
        argvs[index++] = argv[i];
    }
    // 从标准输入读取每一行
    while ((n = read(0, buf, sizeof(buf))) > 0)
    {
        // 临时缓冲区
        char temp[512] = {"\0"};
        argvs[index] = temp;
        for (int i = 0; i < strlen(buf); ++i)
        {
            // 遇到换行符时，创建子进程
            if (buf[i] == '\n')
            {
                // 创建子进程并在子进程中执行命令
                if (fork() == 0)
                {
                    exec(argv[1], argvs);
                }
                wait(0);
            }
            else
            {
                // 非换行符将管道输出添加到临时缓冲区，用作命令的输入参数
                temp[i] = buf[i];
            }
        }
    }
    exit(0);
}
