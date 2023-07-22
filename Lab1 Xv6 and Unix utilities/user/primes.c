#include "kernel/types.h"
#include "user/user.h"
void child(int parentToChild[])
{
    // 关闭子进程不需要的写管道口
    close(parentToChild[1]);
    int i;
    if (read(parentToChild[0], &i, sizeof(i)) == 0)
    {
        // 读不到数字说明素数输出完。关闭管道端口退出。
        close(parentToChild[0]);
        exit(0);
    }
    printf("prime %d\n", i);
    int num = 0;
    // 创建子进程到孙子进程的管道
    int childToGrandchild[2];
    pipe(childToGrandchild);
    int pid;
    // 孙子进程
    if ((pid = fork()) == 0)
    {
        child(childToGrandchild);
    }
    // 子进程
    else
    {
        // 子进程不需要读，关闭对应管道端口
        close(childToGrandchild[0]);
        while (read(parentToChild[0], &num, sizeof(num)) > 0)
        {
            // 不会被整除则输送
            if (num % i != 0)
            {
                write(childToGrandchild[1], &num, sizeof(num));
            }
        }
        close(childToGrandchild[1]);
        wait(0);
    }
    // 子进程结束
    exit(0);
}
int main(int argc, char *argv[])
{
    // 父进程到子进程
    int parentToChild[2];
    pipe(parentToChild);
    int pid;
    // 子进程
    if ((pid = fork()) == 0)
    {
        child(parentToChild);
    }
    // 父进程
    else
    {
        // 子进程不需要读，关闭对应管道端口
        close(parentToChild[0]);
        for (int i = 2; i <= 35; i++)
        {
            write(parentToChild[1], &i, sizeof(i));
        }
        // 关闭管道，避免子进程在 read 函数阻塞
        close(parentToChild[1]);
        wait(0);
    }
    // 父进程结束
    exit(0);
}
