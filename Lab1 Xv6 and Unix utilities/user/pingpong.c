#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
  int parentToChild[2]; // 父进程向子进程发送数据的管道
  int childToParent[2]; // 子进程向父进程发送数据的管道
  char c = 'A';
  int pid;
  // 创建父进程向子进程发送数据的管道
  if (pipe(parentToChild) < 0)
  {
    printf("parentToChild pipe creation failed\n");
    exit(1);
  }
  // 创建子进程向父进程发送数据的管道
  if (pipe(childToParent) < 0)
  {
    printf("childToParent pipe creation failed\n");
    exit(1);
  }
  // 创建子进程
  if ((pid = fork()) < 0)
  {
    printf("fork failed\n");
    exit(1);
  }
  if (pid == 0)
  {
    // 子进程
    char received_char;
    // 关闭子进程不需要的管道端口
    close(parentToChild[1]);
    close(childToParent[0]);
    // 从父进程读取字节
    if (read(parentToChild[0], &received_char, 1) != 1)
    {
      printf("%d: read error\n", getpid());
      exit(1);
    }
    // 打印接收到的消息和进程ID
    printf("%d: received ping\n", getpid());
    // 写字节到父进程
    if (write(childToParent[1], &received_char, 1) != 1)
    {
      printf("%d: write error\n", getpid());
      exit(1);
    }
    // 关闭管道和子进程
    close(parentToChild[0]);
    close(childToParent[1]);
    exit(0);
  }
  else
  {
    // 父进程
    char received_char;
    // 关闭父进程不需要的管道端口
    close(parentToChild[0]);
    close(childToParent[1]);
    // 发送字节给子进程
    if (write(parentToChild[1], &c, 1) != 1)
    {
      printf("%d: write error\n", getpid());
      exit(1);
    }
    // 从子进程读取字节
    if (read(childToParent[0], &received_char, 1) != 1)
    {
      printf("%d: read error\n", getpid());
      exit(1);
    }
    // 打印接收到的消息和进程ID
    printf("%d: received pong\n", getpid());
    // 关闭管道和父进程
    close(parentToChild[1]);
    close(childToParent[0]);
    exit(0);
  }
}
