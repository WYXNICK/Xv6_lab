# 操作系统课程设计实验报告

[TOC]

## 项目源码仓库链接

https://github.com/WYXNICK/Xv6_lab.git

## Tools

### 安装Linux虚拟机

* 下载虚拟化平台。

  - [VMware Player](http://www.vmware.com/products/player/) (free for Linux and Windows, registration required)

  - 版本：VMware Workstation 17 Player

    <img src="images\image-20230710083923232.png" alt="image-20230710083923232" style="zoom:67%;" />

* 一旦安装了虚拟化平台，接下来为选择的Linux发行版下载启动磁盘映像。
  * [Ubuntu Desktop](http://www.ubuntu.com/download/desktop) is one option
  * 版本：Ubuntu 20.04.6

启动虚拟化平台并创建一个新的（64位）虚拟机。使用下载的Ubuntu映像作为启动盘。

<img src="images\image-20230710091516418.png" alt="image-20230710091516418" style="zoom:67%;" />

### 检查安装情况

![image-20230710111851328](images\image-20230710111851328.png)

## Guidance

任务难度分级：

![image-20230710175842487](images\image-20230710175842487.png)

## Lab: Xv6 and Unix utilities

### 1.Boot xv6(easy)

#### 1）实验目的

该部分实验的目的是对xv6操作系统以及该实验所需的环境进行配置。

#### 2）实验步骤

获取实验室的 xv6 源代码并查看 `util` 分支。

```shell
$ git clone git://g.csail.mit.edu/xv6-labs-2020
$ cd xv6-labs-2020
$ git checkout util
```

![image-20230710221049824](images\image-20230710221049824.png)

构建并运行xv6。

```shell
$ make qemu
```

![image-20230710221540392](images\image-20230710221540392.png)

运行完以上命令后，文件夹xvb-labs-2020内会出现以下内容：

![image-20230711000128077](images\image-20230711000128077.png)

#### 3）实验中遇到的问题和解决办法

* 无法进入虚拟机，显示黑屏。解决：关闭Ubuntu(注意是关闭，不是挂起，只是挂起的话，加速3D图形选项不能勾选)，依次选择虚拟机->设置->显示器，然后把加速3D图形选项的取消选择。

* 当执行完指令需要退出虚拟机时，应选择挂起而不是关闭客户机。挂起操作是一种暂停和恢复客户机的方式，保留当前的数据和状态，并允许在需要时快速恢复。而关闭操作是一种完全终止客户机的方式，会清除所有数据和状态。

* 执行 `make qemu`后没有像预期的一样输出 `init: starting sh`，而是在输出 `qemu-system-riscv64 -machine virt -bios none -kernel kernel/kernel -m 128M -smp 3 nographic -drive file=fs.img,if=none,format=raw,id=x0 -device virtio-blk device,drive=x0,bus=virtio-mmio-bus.0`后该脚本挂起，根据参考文档提示可能原因是软件包` qemu-system-misc` 收到了一个更新，该更新破坏了它与xv6内核的兼容性，文档上通过执行以下命令来卸载该软件包并安装旧版本解决：

  ```shell
   $ sudo apt-get remove qemu-system-misc
   $ sudo apt-get install qemu-system-misc=1:4.2-3ubuntu6
  ```

  但报错：`this version was not found`

  经过尝试，发现可能是Ubuntu版本的问题，原版本为 `22.04.2`，更改为 `20.04.6`后重新新建虚拟机并执行相关命令，正常运行。

  ![image-20230711204513133](images\image-20230711204513133.png)

#### 4）实验心得

本实验让我深入了解操作系统的引导过程，探索了 xv6 操作系统的基本功能和命令行界面，也能够自主解决一些编译错误和配置问题。

### 2.sleep (easy)

#### 1）实验目的

为xv6实现UNIX程序 `Sleep`,并且可以暂停用户指定的时钟数。（tick由xv6内核定义，即计时器芯片两次中断之间的时间间隔）

#### 2）实验步骤

本部分实验主要在系统调用 `sleep`的基础上实现：

1. 在`user`目录下创建 `sleep.c`文件。（代码编辑器选择 `Visual Studio Code`)

![image-20230711095653888](images\image-20230711095653888.png)

2. 将sleep程序添加到Makefile中的 `UPROGS`，这样 `make qemu`可以编译程序，并且可以从xv6 shell执行它。

   ![image-20230711100234217](C:\Users\wyx20\AppData\Roaming\Typora\typora-user-images\image-20230711100234217.png)

3. 编写完成 `sleep.c`程序代码。

   ```c
   #include "kernel/types.h"
   #include "kernel/stat.h"
   #include "user/user.h"
   void main(int argc, char *argv[])
   {
    if (argc < 2)
    {
    fprintf(2, "Error!Usage: %s <num> \n", argv[0]);
    exit(1);
    }
    sleep(atoi(argv[1]));
    exit(0);
   }
   ```

4. 从xv6 shell运行程序。

   不传入参数情况：

   ![image-20230711204903894](C:\Users\wyx20\AppData\Roaming\Typora\typora-user-images\image-20230711204903894.png)

   正常传入参数：

   ![image-20230711111345209](C:\Users\wyx20\AppData\Roaming\Typora\typora-user-images\image-20230711111345209.png)

   程序运行一段时间后结束。

#### 3）实验中遇到的问题和解决办法

* Ubuntu环境下编写代码不方便，考虑使用Visual Studio Code进行代码编写。
* 执行 `make qemu`后命令行挂起。开始以为不影响执行，但发现执行sleep时出现问题，于是考虑上述提到的解决方案并成功解决。

#### 4）实验心得

通过此实验，深入理解了了解 xv6 操作系统中的系统调用机制以及如何在用户程序和内核之间进行交互。观察 `echo.c`、`rm.c`等用户程序的写法，我也学习到如何获取用户传递给程序的命令行参数，这可以帮助我获取并解析 sleep 程序所需的 tick 数。整个过程下来对使用 `make qemu` 启动 xv6 操作系统并在 shell 中执行程序进行测试这个流程也较为熟悉。

### 2. pingpong(easy)

#### 1）实验目的

通过使用UNIX系统调用，在两个进程之间通过一对管道进行字节的"ping-pong"传递。

编写一个程序，父进程向子进程发送一个字节，子进程应打印"`<pid>: received ping`"，其中`<pid>`是子进程的进程ID，然后将字节写入管道传递给父进程并退出；父进程应从子进程读取字节，打印"`<pid>: received pong`"，然后退出。

#### 2）实验步骤

首先在 `user`目录下新建 `pingpong.c`文件。

该实验的具体执行步骤如下：

1. 创建管道： 在代码中使用`pipe()`函数创建了一对管道。通过`pipe()`函数，我们可以获得两个文件描述符，一个用于父进程向子进程发送数据的管道(`parentToChild`)，另一个用于子进程向父进程发送数据的管道(`childToParent`)。

2. Fork子进程： 使用`fork()`函数创建了一个子进程。在`fork()`函数被调用后，操作系统会复制父进程的所有资源和代码到子进程中，并使得父进程和子进程可以并行执行。

3. 子进程的执行： 在`fork()`函数的返回值为0的情况下，表示当前代码正在子进程中执行。子进程首先会关闭不需要的管道端口，即关闭父进程向子进程发送数据的管道写端(`parentToChild[1]`)和关闭子进程向父进程发送数据的管道读端(`childToParent[0]`)。 子进程使用`read()`函数从父进程接收一个字节的数据，然后打印接收到的消息和子进程的进程ID(`getpid()`)。 

   接下来，子进程使用`write()`函数将接收到的字节写入到管道(`childToParent[1]`)，发送给父进程。 最后，子进程关闭管道端口，即关闭父进程向子进程发送数据的管道读端(`parentToChild[0]`)和关闭子进程向父进程发送数据的管道写端(`childToParent[1]`)，然后退出。

4. 父进程的执行： 在`fork()`函数的返回值不为0的情况下，表示当前代码正在父进程中执行。父进程首先关闭不需要的管道端口，即关闭父进程向子进程发送数据的管道读端(`parentToChild[0]`)和关闭子进程向父进程发送数据的管道写端(`childToParent[1]`)。 父进程使用`write()`函数将一个字节的数据发送给子进程，将其写入父进程向子进程发送数据的管道(`parentToChild[1]`)。 

   然后，父进程使用`read()`函数从子进程接收一个字节的数据，读取子进程向父进程发送数据的管道(`childToParent[0]`)。 最后，父进程打印接收到的消息和父进程的进程ID(`getpid()`)，关闭管道端口，即关闭父进程向子进程发送数据的管道写端(`parentToChild[1]`)和关闭子进程向父进程发送数据的管道读端(`childToParent[0]`)，然后退出。

在Makefile文件的`UPROGS`中加入`$U/_pingpong\`。之后在命令行中执行`pingpong`,实验结果如下：

![image-20230711205230848](images\image-20230711205230848.png)

#### 3）实验中遇到的问题和解决办法

该实验中涉及到的进程的`PID`是是各进程的身份标识身份标识,程序一运行系统就会自动分配给进程一个独一无二的PID。进程中止后PID被系统回收，**可能会被继续分配给新运行的程序**。

#### 4）实验心得

本实验主要是帮助理解进程通信中的管道方式。通过使用UNIX系统调用创建管道、使用fork函数创建子进程，并在父子进程之间进行双向通信，明白了如何在两个进程之间传递数据的方法、管道的重要感和fork函数的作用。本实验也是对xv6 及其系统调用的一个拓展。

### 4. primes (moderate)/(hard)

#### 1）实验目的

利用`pipe`和`fork`创建管道，并且利用管道编写素数筛的并发版本。在第一个进程将数字 2 到 35 送入管道中，然后输出区间范围内的所有素数。

#### 2）实验步骤

实验程序的基本流程如下：首先将2、3、4、...数字送入管道的左端，然后第一次消除所有2的倍数，第二次消除所有3的倍数，第三次消除5的倍数，以此类推。

![img](https://swtch.com/~rsc/thread/sieve.gif)

1. 首先还是在 `user`目录下创建 `primes.c`文件。

2. 完成 `primes.c`代码。

   程序的主要执行过程有以下几步：

   * 创建父进程到子进程的管道： 使用`pipe(parentToChild)`创建一个管道，其中`parentToChild`包含两个元素，用于读取和写入数据。

   * 创建子进程： 使用`fork()`创建一个子进程，它继承了父进程的资源和代码。

   * 子进程的执行： 关闭不需要的管道写端，从父进程读取一个整数。如果读取失败，则关闭管道端口并退出子进程。如果能读取，则打印该数字为素数。 创建子进程到孙子进程的管道，并根据返回值判断当前代码是在子进程还是孙子进程中执行。在子进程中，关闭管道的读端，并通过循环从父进程读取数字，将不能整除的数字写入到子进程到孙子进程的管道中。最后，关闭管道的写端并等待孙子进程结束。

   * 父进程的执行： 关闭不需要的管道读端，并使用循环将数字2到35写入父进程到子进程的管道中。写入完成后，关闭管道的写端并等待子进程结束。

3. 在Makefile文件的`UPROGS`中加入`$U/_primes\`。
4. 在命令行中先`make qemu`后执行`primes`,实验结果如下：

![image-20230712205708514](images\image-20230712205708514.png)

#### 3）实验中遇到的问题和解决方法

对于实验提示中所提到的 `It's simplest to directly write 32-bit (4-byte) `int`s to the pipes, rather than using formatted ASCII I/O.`。经过分析主要有以下的一些优势：

* 使用二进制数据可以减少数据传输和存储的字节数，提高数据传输速度和处理效率。而使用格式化的ASCII I/O需要将整数转换为字符表示，并将字符数据进行格式化处理，这增加了处理和传输的开销。
* 二进制数据可以减少占用的存储空间，每个32位整数只需要4字节的存储空间，而ASCII字符表示通常需要更多的字节。
* 代码量和处理逻辑方面也有所简化。

例如，在代码中`write(parentToChild[1], &i, sizeof(i))` 使用 `write()` 函数将整数 `i` 的地址作为指针传递给管道的写端，将整数值写入管道中。

#### 4）实验心得

并发编程可以使某些程序更加简单，提高效率和并行执行能力，而管道在并发编程管道中又扮演了关键角色，有助于实现进程间通信、数据传递和共享、同步和通信。它提供了一种有效的机制，使多个进程能够并发执行并协同工作，从而提高系统的效率和性能。

### 5. find (moderate)

#### 1）实验目的

该实验的目的是编写一个简单版本的UNIX find程序，用于在目录树中查找具有特定名称的所有文件。

#### 2）实验步骤

1. 在 `user`目录下新建 `find.c`文件。

2. `find.c`的代码实现分为以下几步：

   * 实现 `find` 函数：该函数用于递归地在指定的目录树中查找文件名与给定文件名匹配的所有文件。它通过读取目录项，获取文件的状态，并进行类型判断和文件名匹配，来实现文件的查找。具体代码如下：

     ```c
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
     ```

   * 在 `main` 函数中进行参数检查：检查命令行参数数量是否足够，若不足则输出正确用法信息并退出。
   * 调用 `find` 函数：在 `main` 函数中，根据命令行参数传递的路径和文件名参数，调用 `find` 函数进行文件的查找。

3. 在Makefile文件的`UPROGS`中加入`$U/_find\`。

4. 在命令行中先`make qemu`后依次执行命令,实验结果如下：

   ![image-20230716092225044](images\image-20230716092225044.png)

#### 3）实验中遇到的问题和解决办法

C语言中 `strcmp`函数容易使用错误。

```c
int strcmp (const char * str1,const char * str2);
```

strcmp用于比较两个字符串的大小，一个字符一个字符比较，按ASCLL码比较
规定：第一个字符串大于第二个字符串，则返回大于0的数字；第一个字符串等于第二个字符串，则返回0；第一个字符串小于第二个字符串，则返回小于0的数字。注意**相等时是返回0而不是1**！

#### 4）实验心得

这一部分利用了操作系统中文件系统的有关知识，通过写一个 `find`程序，利用递归算法来遍历目录树读取目录项，从而实现文件查找的原理，同时我也加深了对于对字符串操作函数（如 `strcpy`、`strcmp`）和文件操作函数（如 `open`、`read`、`close`）的应用，对于文件路径的认识也更加清晰。

### 6.xargs (moderate)

#### 1）实验目的

编写实现一个简化版的UNIX `xargs` 程序，在标准输入中读取每一行，为每行执行一个命令，并将行作为命令的参数传递。

#### 2）实验步骤

1. 在 `user`目录下新建 `xargs.c`文件。

2. 对于代码实现部分主要分为以下几步：

   * 命令行参数处理：解析并保存命令行参数，检查参数的合法性。

     ```c
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
     ```

   * 读取输入数据：使用 `read` 函数从标准输入逐行读取数据，并保存到缓冲区中。

   * 子进程的创建和命令执行：当读取到一行数据时，调用`fork()`创建子进程并使用 `exec` 函数执行指定的命令，将输入数据作为命令的参数传递给它。

   * 等待子进程结束：在父进程中使用 `wait` 函数等待子进程执行完毕。

   * 继续处理下一行输入：回到第 2 步，继续读取标准输入的下一行数据。

   * 程序的退出：当所有输入数据处理完毕后，调用 `exit` 函数正常退出程序。

     ```c
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
     ```

3. 在Makefile文件的`UPROGS`中加入`$U/_xargs\`。

4. 在命令行中先`make qemu`后依次执行命令,实验结果如下：

   ![image-20230716214718077](images\image-20230716214718077.png)

#### 3）实验中遇到的问题和解决办法

* `MAXARG`已经在 `kernel/param.h`中声明过，不用再次声明。
* 命令行参数错误的情况给出了相应的处理，即显示错误并退出。

#### 4）实验心得

在这个实验中，我深入了解了在操作系统中实现类似于UNIX中xargs程序的功能。通过编写xargs程序，我学会了如何处理标准输入的数据，并将每行数据作为命令的参数来执行命令。xargs 作为一条 Unix 系统的常用命令，它能够将标准输入的数据转换为命令行参数，并执行相应的命令。其意义在于处理大量数据、实现批量操作、提供参数灵活性和提高执行效率。通过xargs，我们可以轻松处理大量输入数据，并将其作为参数传递给命令，简化了日常任务中的数据处理和命令执行操作。

## Lab: system calls

### 1. System call tracing (moderate)

#### 1）实验目的

本实验的实验目的是在xv6内核中添加一个系统调用跟踪功能，用于帮助在后续实验中进行调试。

通过创建一个新的trace系统调用，可以控制系统调用的跟踪行为。该系统调用接受一个整数参数"mask"，其中的位指定要跟踪的系统调用。例如，要跟踪fork系统调用，程序可以调用trace(1 << SYS_fork)，其中SYS_fork是kernel/syscall.h中定义的系统调用号。

如果系统调用的编号在mask中被设置，则需要修改xv6内核，在每个系统调用即将返回时打印一行输出。输出行应包含进程ID、系统调用的名称和返回值，不需要打印系统调用的参数。

trace系统调用应该启用调用它的进程以及它随后创建的所有子进程的跟踪，但不应影响其他进程。

#### 2）实验步骤

1. 将 `$U/_trace\` 添加到 Makefile 中的 UPROGS。

   ```
   UPROGS=\
       $U/_trace\
       ...
   ```

2. 在 `user.h`中添加trace的声明：`int trace(int mask);`

3. 在 `user/usys.pl` 中添加一行生成(user/usys.S)的脚本：`entry("trace");` 

4. 在 kernel/syscall.h 中添加 trace 系统调用序号的宏定义：`#define SYS_trace  22`

5. 在 kernel/proc.h 中给 proc 结构体添加一个成员 变量 mask，表示当前进程需要跟踪的系统调用。

6. 在 kernel/sysproc.c 中新增 sys_trace 函数的具体实现。

   ```c
   uint64
   sys_trace(void)
   {
     int mask;
     // 取a0寄存器中的值返回给mask
     if(argint(0, &mask) < 0)
     return -1;
     // 将mask传给现有进程的mask
     myproc()->mask = mask;
     return 0;
   }
   ```

7. 在 kernel/proc.c 中 fork 函数调用时，添加子进程复制父进程的 mask 的代码。

   ```c
   np->mask = p->mask;
   ```

8. 修改 syscall 函数打印跟踪每个系统调用的输出结果。

   * 新增syscall外部声明：

     ```c
     extern uint64 sys_trace(void);
     ```

   * 添加 `syscall_names`数组以输出调用函数名。

   * 修改syscall函数以实现打印输出。

     ```c
     // 从a7读取系统调用的编号，将1<<num与进程的mask比较，相等则打印
     if ((1 << num) & p->mask)
     {
         printf("%d: syscall %s -> %d\n", p->pid, syscall_names[num], p->trapframe->a0);
     }
     ```

9. 执行程序，实验结果如下：

   ![image-20230717115409947](images\image-20230717115409947.png)

![image-20230717115449189](images\image-20230717115449189.png)

#### 3）实验中遇到的问题和解决方法

第一次运行时执行 `trace 32 grep hello README`报错：

```shell
3 trace: unknown sys call 22
trace: trace failed
```

逐步重新检查后发现是因为只在syscall_names数组中添加了trace,而没有把新增的 trace 系统调用添加到函数指针数组*syscalls[]上所导致的。在该数组中添加 `[SYS_trace]   sys_trace,`执行成功。

#### 4）实验心得

本实验是向xv6添加新的系统调用，即实现系统调用的跟踪功能，通过修改内核和编写用户级程序，可以在系统调用返回时打印相关信息。这有助于我理解系统调用的工作原理以及深入接触xv6内核。系统调用的重要性不言而喻，这一部分的实验的实现过程对于理解还是比较有帮助的。

### 2. Sysinfo (moderate)

#### 1）实验目的

实验的实验目的是在内核中添加一个名为sysinfo的系统调用，该系统调用用于收集关于运行系统的信息。sysinfo系统调用接受一个参数，即指向sysinfo结构体的指针。

内核需要填充该结构体的字段：freemem字段应设置为空闲内存的字节数，nproc字段应设置为状态不是UNUSED的进程数。

#### 2）实验步骤

1. 将 `$U/_sysinfotest\` 添加到 Makefile 中的 UPROGS。

2. 在 `user.h`中添加sysinfo的声明。

   ```c
   struct sysinfo;
   int sysinfo(struct sysinfo *);
   ```

3. 在 `usys.pl` 中添加sysinfo的用户态接口。

   ```
   entry("sysinfo");
   ```

4. 在 `kernel/syscall.h` 中添加 sysinfo 系统调用宏定义。

   ```c
   #define SYS_sysinfo  23
   ```

5. 在 `kernel/syscall.c`中添加系统调用的函数指针、名称等，过程与 `System call tracing`类似。

6. 在 `kernel/kalloc.c` 中添加 free_memory 函数获取空余内存信息。

   ```c
   // 获取空闲内存信息
   uint64 free_memory(void)
   {
       struct run *r;
       // 记录空闲页数
       uint64 num = 0;
       // 添加锁
       acquire(&kmem.lock);
       // r 指向空闲列表
       r = kmem.freelist;
       // 当 r 非空时循环
       while (r)
       {
           // 空闲页数加一
           num++;
           // 指向下一个
           r = r->next;
       }
       // 释放锁
       release(&kmem.lock);
       // 空闲内存为页数乘上页大小
       return num * PGSIZE;
   }
   ```

7. 在 `kernel/proc.c` 中新增函数getprocnum以获取活动状态进程数目。

   ```c
   // 获取活动的进程数
   uint64 getprocnum(void)
   {
       struct proc *p;
       // 计算进程数
       uint64 num = 0;
       // 遍历所有进程
       for (p = proc; p < &proc[NPROC]; p++)
       {
           // 添加锁
           acquire(&p->lock);
           // 判断进程状态是否不是UNUSED
           if (p->state != UNUSED)
           {
               // 进程数加一
               num++;
           }
           // 释放锁
           release(&p->lock);
       }
       return num;
   }
   ```

8. 在defs.h中声明上述两个函数。

9. 在 `kernel/sysproc.c` 中添加 sys_sysinfo 函数的具体实现。

   ```
   uint64 
   sys_sysinfo(void)
   {
       // addr是一个指向sysinfo的用户虚拟地址
       uint64 addr;
       struct sysinfo info;
       struct proc *p = myproc();
       if (argaddr(0, &addr) < 0)
           return -1;
       // 获取空闲内存的字节数
       info.freemem = free_memory();
       // 获取活动的进程数
       info.nproc = getprocnum();
       if (copyout(p->pagetable, addr, (char *)&info, sizeof(info)) < 0)
           return -1;
       return 0;
   }
   ```

10. 执行程序，实验结果如下：

    ![image-20230717173732981](images\image-20230717173732981.png)

#### 3）实验中遇到的问题和解决方法

在完成以上步骤执行 `make qemu`命令时发现报错：

![image-20230717172407563](images\image-20230717172407563.png)

按提示查看syscall.c中sys_sysinfo的函数指针和函数调用名称，发现并无问题，最终经过仔细检查，发现是在`kernel/syscall.h` 文件中 `SYS_sysinfo` 的宏定义不存在，于是添加该定义：`#define SYS_sysinfo  23`，于是错误解决，初始化正常。

#### 4）实验心得

本实验让我学会了如何向操作系统添加新的系统调用，并使用该系统调用来收集和返回系统信息。我也进一步认识了系统调用是用户程序与操作系统之间的桥梁，通过系统调用，用户程序可以访问操作系统提供的各种功能和资源。在学习《操作系统》这门课程时，虚拟内存的页式管理是比较重要的一部分，通过此实验，我学会了如何跟踪和管理系统中的内存分配和释放，也复习巩固了操作系统是如何使用虚拟内存管理来提供每个进程独立的内存空间，并使用页面映射和页面替换算法来实现内存的有效利用和保护。

## Lab: page tables

### 1. Print a page table (easy)

#### 1）实验目的

熟悉并理解RISC-V页面表的工作原理，并提供一个函数来打印页表的内容。通过实现vmprint()函数，将学会如何遍历页面表并打印出每个PTE的信息。

#### 2)实验步骤

1. 在 `kernel/defs.h` 文件中声明函数。

   ```c
   void            vmprint(pagetable_t pagetable);
   ```

2. 在 `kernel/vm.c` 中添加 vmprint()，以及所用到的`print_pte`和`traverse_pagetable`两个函数的具体代码。

   ```c
   void print_pte(pte_t pte, int level,int i)
   {
       // 打印适当数量的缩进
       for (int i = 0; i < level; i++)
       {
           if (i == 0)
               printf("..");
           else
               printf(" ..");
       }
       // 打印PTE和物理地址
       printf("%d: pte %p pa %p\n", i, pte, PTE2PA(pte));
   }
   void traverse_pagetable(pagetable_t pagetable, int level)
   {
       // 遍历每个PTE
       for (int i = 0; i < 512; ++i)
       {
           pte_t pte = pagetable[i];
           if ((pte & PTE_V))
           {
               // 打印当前PTE信息
               print_pte(pte, level,i);
               // 如果PTE指向一个下一级的页表，则递归打印该页表，直到最后一级才读、写、执行
               if ((pte & (PTE_R | PTE_W | PTE_X)) == 0)
                   traverse_pagetable((pagetable_t)PTE2PA(pte), level + 1);
           }
       }
   }
   void vmprint(pagetable_t pagetable)
   {
       // 打印页面表的起始地址
       printf("page table %p\n", pagetable);
       // 开始遍历页面表
       traverse_pagetable(pagetable, 1);
   }
   ```

3. 在`kernel/exec.c`中加入pid=1时的判断，使其在启动时能打印。

   ```c
   if (p->pid == 1) {
       vmprint(p->pagetable);
   }
   ```

4. 启动xv6，命令行会打印输出，实验结果如下：

   ![image-20230718214201738](images\image-20230718214201738.png)

#### 3）实验中遇到的问题和解决办法

* 在实验过程中第一次运行，没有打印输出，检查后发现是没有在 `exec.c return argc;`之前添加pid=1时的判断，加入后其便能在xv6初始时打印输出。

* 运行过程发现有报错

  ```shell
  kernel/vm.c:466:13: error: implicit declaration of function 'print_pte' [-Werror=implicit-function-declaration]
    466 |             print_pte(pte, level,i);
  ```

  检查后发现原因是print_pte函数中i前面的逗号误用为中文。

#### 4）实验心得

通过完成这个实验，我了解了页表的结构和功能，并学会了如何打印和遍历页表的内容。通过编写vmprint函数，我对页表的层级关系、PTE的解析和物理地址的计算有了更深入的理解。

从实验结果可以看出，页0包含顶层页表的条目0和255。它们指向不同的物理地址，表示这些条目是顶层页表的入口。页2是第二级页表的一个页面，其中包含条目0和511。它们指向不同的物理地址，表示这些条目是第二级页表的入口。在用户模式下，进程无法直接读取或写入由页表1映射的内存。这是因为PTE1没有被标记为可读或可写，进程没有权限访问该内存区域。只有在最低级的页表中，才能进行读取和写入操作。

### 2. A kernel page table per process (hard)

#### 1）实验目的

为了能够直接解引用用户指针，每个进程需要有自己的内核页表。本实验要求对struct proc进行修改，以便为每个进程维护一个内核页表，并在调度程序中进行切换，使得内核能够直接使用用户指针，并在切换进程时正确加载相应的页表。这为后续实验中直接解引用用户指针打下了基础。每个进程的内核页表在初始阶段与全局内核页表相同，但随后可以对其进行个性化的修改。

#### 2）实验步骤

1. 在` kernel/proc.h`中添加内核页表。

   ```c
   pagetable_t kpagetable;      //添加内核页表
   ```

2. 先在defs.h中声明，后在 `kernel/vm.c`中增加一个kvmmap_with_certain_page()函数。

   ```c
   // 模拟 kvmmap
   void kvmmap_with_certain_page(pagetable_t pg, uint64 va, uint64 pa, uint64 sz, int perm)
   {
       if (mappages(pg, va, sz, pa, perm) != 0)
           panic("kvmmap with certain page");
   }
   ```

3. 先在defs.h中声明，后在 `kernel/vm.c` 中增加一个kvm_init_one()函数，表示初始化一个内核页表。

   ```c
   // 初始化一个新的内核页表
   pagetable_t kvm_init_once()
   {
       pagetable_t newpg = uvmcreate();
       // 复制已有的页表，但忘记修改第一个参数，非常遗憾
       // 映射 UART 寄存器
       kvmmap_with_certain_page(newpg, UART0, UART0, PGSIZE, PTE_R | PTE_W);
       // 映射 virtio mmio 磁盘接口
       kvmmap_with_certain_page(newpg, VIRTIO0, VIRTIO0, PGSIZE, PTE_R | PTE_W);
       // 映射 CLINT
       kvmmap_with_certain_page(newpg, CLINT, CLINT, 0x10000, PTE_R | PTE_W);
       // 映射 PLIC
       kvmmap_with_certain_page(newpg, PLIC, PLIC, 0x400000, PTE_R | PTE_W);
       // 映射内核文本，设置为可执行和只读
       kvmmap_with_certain_page(newpg, KERNBASE, KERNBASE, (uint64)etext - KERNBASE, PTE_R | PTE_X);
       // 映射内核数据和物理 RAM
       kvmmap_with_certain_page(newpg, (uint64)etext, (uint64)etext, PHYSTOP - (uint64)etext, PTE_R | PTE_W);
       // 映射陷阱入口/出口的跳板到内核的最高虚拟地址
       kvmmap_with_certain_page(newpg, TRAMPOLINE, (uint64)trampoline, PGSIZE, PTE_R | PTE_X);
       return newpg;
   }
   ```

4. 利用上面新增的函数修改kvminit()函数的代码。

   ```c
   void
   kvminit()
   {
     kernel_pagetable = kvm_init_once();
   }
   ```


5. 在 `kernel/proc.c` 中的 allocproc 函数中调用上述新增的函数，以便在初始化进程空间时初始化用户内核页表。

   先修改procinit()函数为：

   ```c
    initlock(&pid_lock, "nextpid");
     for(p = proc; p < &proc[NPROC]; p++) {
         initlock(&p->lock, "proc");
     }
     kvminithart();
   ```

   再修改allocproc()函数，新增以下代码：

   ```c
   // 添加内核页表
     p->kpagetable = kvm_init_once();
     if (p->kpagetable == 0)
     {
         freeproc(p);
         release(&p->lock);
         return 0;
     }
     // 在allocproc函数中进行初始化
     // 为进程的内核栈分配一页内存。
     // 将其映射到高内存地址，并跟随一个无效的守护页。
     char *pa = kalloc();
     if (pa == 0)
         panic("kalloc");
     uint64 va = KSTACK((int)(p - proc));
     kvmmap_with_certain_page(p->kpagetable, va, (uint64)pa, PGSIZE, PTE_R | PTE_W);
     p->kstack = va;
     for(p = proc; p < &proc[NPROC]; p++) {
       acquire(&p->lock);
       if(p->state == UNUSED) {
         goto found;
       } else {
         release(&p->lock);
       }
     }
   ```

6. 在 `kernel/proc.c` 的 scheduler 函数中进行修改。根据要求，需要在每个任务执行结束后切换回 kpagetable。

7. 修改freeproc函数，使其在销毁进程时释放对应的内核页表。

8. 修改 `kernel/vm.c` 中 kvmpa，将使用的全局内核页表地址换成进程自己的内核页表地址。

   ```c
   pte = walk(myproc()->kpagetable, va, 0);
   ```


9. 先 `make qemu`后执行 `usertests`进行测试，实验结果如下：

   ![image-20230722202240803](images\image-20230722202240803.png)

   ![image-20230722202300839](images\image-20230722202300839.png)

#### 3）实验中遇到的问题和解决办法

测试出现报错：

<img src="images\image-20230722185250914.png" alt="image-20230722185250914" style="zoom: 67%;" />

经过仔细核对代码以及调试，发现是在freeproc函数中误将以下的代码注释而导致错误：

```c
 p->sz = 0;
  p->pid = 0;
  p->parent = 0;
  p->name[0] = 0;
  p->chan = 0;
  p->killed = 0;
  p->xstate = 0;
  p->state = UNUSED;
```

在 xv6 中，当一个进程结束执行并准备释放时，它的相关信息和资源应该被正确清理，以便下次创建新的进程时不会受到影响。通过在 `freeproc` 函数中重置进程的状态和相关字段，确保了新创建的进程不会继承旧进程的状态和资源，从而避免了一些可能导致测试失败的问题。

#### 4）实验心得

本实验的整个过程还是比较复杂的，要弄明白每一个函数的具体含义以及功能，尤其是在中间测试出现错误的时候，要从逻辑和代码多方面去分析原因。通过这个实验，我也更深入地理解了虚拟内存管理和页表机制在操作系统中的作用，以及在多进程环境下如何处理内核页表的分配和管理。

### 3. Simplify `copyin/copyinstr` (hard)

#### 1）实验目的

实验的目的是简化 xv6 操作系统中的 copyin 和 copyinstr 函数，使其能够直接解引用用户指针，而不需要在软件中遍历进程页表来进行地址转换。

为了实现这一目标，需要为每个进程的内核页表添加用户地址的映射，以便 copyin_new 和 copyinstr_new 函数可以直接操作用户指针。这样做可以提高内核读取用户内存数据的性能，避免了频繁的页表遍历。实验还要求防止用户进程的地址空间与内核地址空间重叠，限制用户进程的最大大小小于内核最低虚拟地址。

#### 2）实验步骤

1. 先在 `kernel/defs.h`中声明所要用到的几个函数。其中uvmtokvm用于将用户页表转换到内核页表，copyin_new和copyinstr_new分别是对copyin 和 copyinstr 函数的重写。

   ```c
   void uvmtokvm(pagetable_t u, pagetable_t k, uint64 from, uint64 to);
   int copyin_new(pagetable_t pagetable, char *dst, uint64 srcva, uint64 len);
   int copyinstr_new(pagetable_t pagetable, char *dst, uint64 srcva, uint64 max);
   ```

2. 在 `kernel/vm.c`中实现uvmtokvm函数。

   ```c
   void uvmtokvm(pagetable_t userpgtbl, pagetable_t kpagetable, uint64 from, uint64 to)
   {
       // PLIC限制
       if (from > PLIC)
           panic("uvmtokvm: from larger than PLIC");
       from = PGROUNDDOWN(from);
       for (uint64 i = from; i < to; i += PGSIZE)
       {
           pte_t *pte_user = walk(userpgtbl, i, 0);
           pte_t *pte_kernel = walk(kpagetable, i, 1);
           if (pte_kernel == 0)
               panic("uvmtokvm: allocating kernel pagetable fails");
           *pte_kernel = *pte_user;
           *pte_kernel &= ~PTE_U;
       }
   }
   ```

3. 在 fork(),sbrk(),exec()函数中进行修改，使得他们改变用户进程页表后改变用户内核页表。
4. 同样，要对userinit()和growproc()进行修改。
5. 修改copyin 和 copyinstr 函数，用 copyin_new 和 copyinstr_new 进行替换。

6. 运行 `usertests`测试实验结果如下：

   ![image-20230723002712501](images\image-20230723002712501.png)

   ![image-20230723002731901](images\image-20230723002731901.png)

#### 3）实验中遇到的问题和解决办法

对于本实验中最后所提到的问题，即第三个测试 srcva `+ len < srcva` 为什么在copyin_new()中是必需的。其实srcva + len < srcva 是为了检查传入的 srcva 和 len 参数是否存在溢出或越界情况。如果 srcva 和 len 都是合法的无符号整数，而 srcva + len 的结果超过了无符号整数的最大表示范围，则会发生溢出。这种情况下，第一个测试 srcva < srcva 会失败，因为无符号整数不允许出现负数。第二个测试 srcva + len < PGSIZE 也会失败，因为 srcva + len 的结果大于 PGSIZE。

举例说明，假设 srcva = 0xFFFFFFFF，len = 2，则 srcva + len = 0x100000001，超过了无符号整数的最大表示范围（0xFFFFFFFF），导致溢出。因此，第一个测试 srcva < srcva 和第二个测试 srcva + len < PGSIZE 都会通过，但第三个测试 srcva + len < srcva 将返回 true，从而导致 copyin_new 函数返回 -1，表示传入的参数存在越界问题。

#### 4）实验心得

这三个实验相较于之前的实验较为复杂，尤其是调试过程中会遇到各种问题，需要耐心的查询解决。通过此实验我也深入理解了用户地址空间和内核地址空间的映射关系，以及在多进程环境下如何优化内存访问性能和保护系统安全的方法。通过简化 copyin 和 copyinstr 函数，使得内核可以直接解引用用户指针，从而提高了内存读取的效率。

## Lab: traps

### 1. RISC-V assembly (easy)

#### 1）实验目的

通过实际阅读和运行代码来加深对 RISC-V 汇编语言的理解，并帮助自己熟悉在操作系统中进行汇编编程和调试的过程。

#### 2）实验步骤

运行`make fs.img`编译 `user/call.c` 。 阅读 user/call.asm 中函数 `g`、`f` 和 `main` 的代码。回答下面的问题：

1. Which registers contain arguments to functions? For example, which register holds 13 in main's call to `printf`?

   答：a0-a7 储存函数参数；如果参数超过 8 个，则保存在内存中，函数调用的默认第一个参数为函数和本身名字。a2寄存器 存储 13。

2. Where is the call to function `f` in the assembly code for main? Where is the call to `g`? (Hint: the compiler may inline functions.)

   答：main 的汇编代码中没有显示对函数进行调用，但对 f 函数有内联调用。

3. At what address is the function `printf` located?

   答：printf的地址为638。

4. What value is in the register `ra` just after the `jalr` to `printf` in `main`?

   答：Ra 用来保存函数执行以后的下一个执行指令的地址，为 0x38。

5. Run the following code.

   ```
   unsigned int i = 0x00646c72;
   printf("H%x Wo%s", 57616, &i);
   ```

   What is the output? [Here's an ASCII table](http://web.cs.mun.ca/~michael/c/ascii-table.html) that maps bytes to characters.

   答：输出：HE110 World

   The output depends on that fact that the RISC-V is little-endian. If the RISC-V were instead big-endian what would you set `i` to in order to yield the same output? Would you need to change `57616` to a different value?

   答：i 需要设置为 0x726c64，57616 不需要变。

6. In the following code, what is going to be printed after 'y='? (note: the answer is not a  specific value.) Why does this happen?

   ```
   printf("x=%d y=%d", 3);
   ```

   答：y输出的是a2寄存器的内容。

#### 3）实验中遇到的问题和解决办法

对于RISC-V指令集架构了解不充分，通过查阅相关资料解决。

#### 4）实验心得

通过阅读和运行代码的过程加深了对 RISC-V 汇编语言的理解，同时也有助于理解 xv6 操作系统的底层实现以及进行操作系统开发，为后续实验做准备。

### 2. Backtrace (moderate)

#### 1）实验目的

本次实验的目的是实现 backtrace()函数并进行调用。对于调试来说，反向跟踪通常很有用:在错误发生点以上的堆栈上的函数调用列表。

#### 2）实验步骤

1. 在`kernel/defs.h`添加函数声明。

   ```c
   void            printf(char*, ...);
   void            panic(char*) __attribute__((noreturn));
   void            printfinit(void);
   void            backtrace(void);
   ```

2. 在`kernel/riscv.h` 中增加 r_fp()函数的实现，用来读取寄存器 s0。

   ```c
   //为backtrace添加
   static inline uint64
   r_fp()
   {
     uint64 x;
     asm volatile("mv %0, s0" : "=r" (x) );
     return x;
   }
   ```

3. 在`kernel/printf.c`中添加backtrace函数的实现。

   ```c
   void backtrace(void){
     printf("backtrace:\n");
     uint64 fp = r_fp();
     while (PGROUNDDOWN(fp) < PGROUNDUP(fp)){
       printf("%p\n", *(uint64*)(fp - 8));
       fp = *(uint64*)(fp - 16);
     }
   }
   ```

4. 在 `kernel/sysproc.c` 的 sys_sleep()函数中调用 backtrace()。

5. 运行程序，可以观察到实验结果如下：

   ![image-20230728143626067](images\image-20230728143626067.png)

![image-20230728143855150](images\image-20230728143855150.png)

#### 3）实验中遇到的问题和解决办法

一开始发现打印出的堆栈地址与给出的示例不同，后来思考后发现每次运行程序时，栈上的函数调用链会发生变化，而backtrace函数正是通过读取栈上的帧指针来遍历函数调用链的。由于堆栈地址的变化，导致打印的地址稍有不同，但是函数调用链的结构和顺序不变。

#### 4）实验心得

本次实验实现的backtrace函数可以打印当前函数调用的堆栈信息，它对于调试和排查代码问题非常有用，因为它可以展示函数调用的层级关系和返回地址，帮助我们理解代码的执行流程和定位错误的源头。同时也学习了使用addr2line工具，它可以将地址转化，非常方便。

### 3. Alarm (hard)

#### 1）实验目的

这个实验的主要目的是向 xv6 添加定期发出警报的功能，即一个进程可以定期向内核请求。在消耗一定的CPU时间后，内核调用特定的处理程序函数，从而提供了一种在用户空间进行定期操作的方法。这样的功能对于需要限制计算绑定进程的CPU时间，或者希望在进程执行过程中定期执行某些操作的情况很有用。

#### 2）实验步骤

1. 修改 Makefile添加`$U/_alarmtest\` 以使 `alarmtest.c` 编译为 xv6 用户程序。

2. 在user/user.h中添加声明

   ```c
   //添加alarm
   int sigalarm(int ticks, void (*handler)());
   int sigreturn(void);
   ```

3. 在user/usys.pl中添加系统调用的entry，再在kernel/syscall.h和kernel/syscall.c中添加宏定义和声明。

4. 在 kernel/proc.h 中的 struct proc 结构体中添加记录时间间隔, 调用函数地址, 以及 经过时钟数的属性，并在kernel/proc.c中初始化这些属性。

   ```c
   int alarm_interval;
   int alarm_ticks;
   uint64 alarm_handler;
   struct trapframe alarm_trapframe;
   ```

   ```c
   p->alarm_interval = 0;
   p->alarm_ticks = 0;
   p->alarm_handler = 0;
   ```

5. 在 kernel/trap.c 的 usertrap() 中处理冲突，添加：

   ```c
   if (p->alarm_interval){
     if (++p->alarm_ticks == p->alarm_interval){
       memmove(&(p->alarm_trapframe), p->trapframe, sizeof(*(p->trapframe)));
       p->trapframe->epc = p->alarm_handler;
     }
   }
   ```

6. 在kernel/sysproc.c中完善相关函数的实现。

   ```c
   uint64 sys_sigalarm(void){
     int interval;
     uint64 handler;
   
     if (argint(0, &interval) < 0)
       return -1;
     if (argaddr(1, &handler) < 0)
       return -1;
   
     myproc()->alarm_interval = interval;
     myproc()->alarm_handler = handler;
     return 0;
   }
   
   uint64 sys_sigreturn(void){
     memmove(myproc()->trapframe, &(myproc()->alarm_trapframe), sizeof(struct trapframe));
     myproc()->alarm_ticks = 0;
     return 0;
   }
   ```

7. 命令行输入alarmtest,测试实验结果如下：

   ![image-20230728180113750](images\image-20230728180113750.png)

#### 3）实验中遇到的问题和解决办法

对于trap机制的理解上，有三种事件会导致CPU搁置普通指令的执行，强制将控制权转移给处理该事件的特殊代码。一种情况是系统调用，当用户程序执行ecall指令要求内核为其做某事时。另一种情况是异常：一条指令（用户或内核）做了一些非法的事情，如除以零或使用无效的虚拟地址。第三种情况是设备中断，当一个设备发出需要注意的信号时，例如当磁盘硬件完成一个读写请求时。

实验使用trap作为这些情况的通用术语。trap迫使控制权转移到内核；内核保存寄存器和其他状态，以便恢复执行；内核执行适当的处理程序代码（例如，系统调用实现或设备驱动程序）；内核恢复保存的状态，并从trap中返回；代码从原来的地方恢复执行。

#### 4）实验心得

该部分实验与之前添加系统调用的实验部分有许多相似之处，可以借鉴当时的实验步骤作参考。

## Lab: xv6 lazy page allocation

### 1. Eliminate allocation from sbrk() (easy)

#### 1）实验目的

这个实验的实验目的是要求我们修改xv6操作系统中的sbrk()系统调用实现，删除其中的页面分配操作。原先的sbrk(n)系统调用会通过调用growproc()函数来分配n个字节的内存，并返回新分配内存的起始地址（也就是进程旧的大小）。现在的任务是修改sbrk(n)系统调用，使其仅仅通过增加进程的大小（myproc()->sz）来完成内存大小的调整，并且不再进行内存分配操作。

实验要求我们推测一下这样的修改可能导致的结果和问题，同时要求我们进行相应的修改后重新启动xv6并测试。

#### 2）实验步骤

1. 删除sbrk()中释放内存的代码段，改为如下代码段：

   ```c
   myproc()->sz += n; // 只增加 sz 的数值大小，不实际分配内存
   if (n < 0) { 
       // n 小于 0 的情况为释放内存，立即执行
      uvmdealloc(myproc()->pagetable, addr, myproc()->sz);
   }
   ```

2. 运行，实验结果如下：

   ![image-20230729135023586](images\image-20230729135023586.png)

#### 3）实验中遇到的问题和解决办法

1. 如果 n 是负数，则对其对应的内存进行释放（仿照 proc.c 中的 growproc 函数写就行）。
2. 判断堆的空间大小。不能超过 MAXVA，也不能释放小于 0 的地址空间。

#### 4）实验心得

本实验的主要内容就是更改 kernel/sysproc.c 中的 sys_sbrk() 函数，把原来只要申请就分配的逻辑改成申请时仅进行标注，即更改进程的 sz 字段。如果直接运行 `echo hi` 命令会报错，是因为我们还没写分配内存的逻辑。

### 2. Lazy allocation (moderate)

#### 1）实验目的

本实验的目的是通过实现"懒惰分配"（lazy allocation）来优化内存管理和页错误处理。"懒惰分配"是指当一个进程访问尚未分配的内存页时，内核不会立即分配物理内存，而是在需要时进行实际的内存分配和映射。通过这种方式，可以避免预先分配大量的内存，节省内存资源，同时提高了程序的启动速度。

#### 2）实验步骤

1. 修改 `kernel/trap.c` 的 usertrap()，使得在发生 fault page 时，给虚拟地址分配物理页。

   ```c
   ...
   } else if(r_scause() == 13 || r_scause() == 15) {
       uint64 va = r_stval();
       uint64 pa = (uint64)kalloc();
       if (pa == 0) {
         p->killed = 1;
       } else if (va >= p->sz || va <= PGROUNDDOWN(p->trapframe->sp)) {
         kfree((void*)pa);
         p->killed = 1;
       } else {
         va = PGROUNDDOWN(va);
         memset((void*)pa, 0, PGSIZE);
         if (mappages(p->pagetable, va, PGSIZE, pa, PTE_W | PTE_U | PTE_R) != 0) {
           kfree((void*)pa);
           p->killed = 1;
         }
       }
   ... 
   ```

2. 修改`kernel/vm.c` 的 uvmunmap()，将`panic("uvmunmap: walk");`和`panic("uvmunmap: not mapped");`注释，并加上continue。让进程销毁时，对于尚未分配实际物理页的虚拟地 址不做处理。

3. 运行，实验结果如下：

   ![image-20230729180033959](images\image-20230729180033959.png)

#### 3）实验中遇到的问题和解决办法

运行时报错：

![image-20230729175853147](images\image-20230729175853147.png)

检查后发现是在 `uvmunmap` 中发现了没有映射的地址，直接跳过就行，不需要 panic，但注释完以后忘记加上continue导致出错。

#### 4）实验心得

本实验主要的思路就是系统调用的中断码是 8，page fault 的中断码是 13 和 15。因此，这里我们对 r_scause() 中断原因进行判断，如果是 13 或是 15，则说明没有找到地址。错误的虚拟地址被保存在了 STVAL 寄存器中，我们取出该地址进行分配。如果申请物理地址没成功或者虚拟地址超出范围了，那么杀掉进程。

### 3. Lazytests and Usertests (moderate)

#### 1）实验目的

该实验的目的是通过修改内核代码来实现懒惰内存分配，并确保所有的lazytests和usertests都能通过。

#### 2）实验步骤

1. 修改 `kernel/vm.c` 的 uvmcopy()，让进程 fork 时，对于 parent 进程中尚未分配实际物理 页的虚拟地址，不做处理。即注释掉`panic("uvmcopy: pte should exist");`和`panic("uvmcopy: page not present");`

2. 修改 `kernel/vm.c` 的walkaddr()，将虚拟地址翻译为物理地址。如果没找到对应的物理地址，就分配一个。

   ```c
   uint64
   walkaddr(pagetable_t pagetable, uint64 va)
   {
     pte_t *pte;
     uint64 pa;
     if(va >= MAXVA)
       return 0;
     pte = walk(pagetable, va, 0);
     if (pte == 0 || (*pte & PTE_V) == 0){
       struct proc *p = myproc();
       if (va >= p->sz || va <= PGROUNDDOWN(p->trapframe->sp))
         return 0;
       char *pa = kalloc();
       if (pa == 0)
         return 0;
       else {
         memset(pa, 0, PGSIZE);
         if (mappages(p->pagetable, PGROUNDDOWN(va), PGSIZE, (uint64)pa, PTE_W | PTE_R | PTE_U) != 0){
           kfree(pa);
           return 0;
         }
       }
     }
     if((*pte & PTE_U) == 0)
       return 0;
     pa = PTE2PA(*pte);
     if((*pte & PTE_U) == 0)
       return 0;
     pa = PTE2PA(*pte);
     return pa;
   }
   ```

   3. 运行测试：

      ![image-20230729183815330](images\image-20230729183815330.png)

      ![image-20230729184028335](images\image-20230729184028335.png)

      

![image-20230729184139008](images\image-20230729184139008.png)

#### 3）实验中遇到的问题和解决办法

一开始忘记在`kernel/vm.c`加上头文件，加入后解决。

#### 4）实验心得

内存分配需要设置成懒分配（Lazy page allocation）的模式，主要是因为进程在申请内存时，很难精确地知道所需要的内存多大，因此，进程倾向于申请多于所需要的内存。这样会导致一个问题：有些内存可能一直不会使用，申请了很多内存但是使用的很少。懒分配模式就是解决这样一个问题。

解决方法是：分配内存时，只增大进程的内存空间字段值，但并不实际进行内存分配；当该内存段需要使用时，会发现找不到内存页，抛出 page fault 中断，这时再进行物理内存的分配，然后重新执行指令。

最主要的一点，需要注意的是，进程的地址空间是连续的，从 0 开始到 MAXVA。

## Lab: Copy-on-Write Fork for xv6

### 1. Implement copy-on write(hard)

#### 1）实验目的

该实验的目的是在 xv6 内核中实现写时复制（Copy-On-Write，COW）机制。写时复制是一种内存管理优化技术，用于在操作系统的 fork 操作期间高效地处理内存复制。它允许父进程和子进程共享相同的物理内存页，直到其中一个进程修改了共享内存。在这种情况下，会创建一个独立的页副本，并将修改应用于新的副本，从而确保父进程的内存保持不变。

#### 2）实验步骤

1. 在`kernel/kalloc.c`中定义一个用于计数的数组。

   ```c
   uint page_ref[(PHYSTOP - KERNBASE) / PGSIZE];
   ```

2. 在 `kernel/riscv.h` 中宏定义 COW 标记位和计算物理内存页下标的宏函数。

   ```c
   #define PTE_COW (1L << 8)  // copy on write
   #define COW_INDEX(pa) (((uint64)(pa) - KERNBASE) >> 12)
   ```

3. 修改uvmunmap函数，注释掉 `i`f((*pte & PTE_V) == 0)`
      panic("uvmunmap: not mapped");`，防止其在PTE_V非法时panic。

4. 修改kalloc函数，设置计数值为1。

   ```c
   page_ref[COW_INDEX(r)] = 1;
   ```

4. 修改kfree函数，增加判断计数值是否大于1。

   ```c
   void
   kfree(void *pa)
   {
     struct run *r;  
     if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
       panic("kfree");
     if(page_ref[COW_INDEX(pa)] > 1) {
       page_ref[COW_INDEX(pa)]--;
       return;
     }
     page_ref[COW_INDEX(pa)] = 0;
     // Fill with junk to catch dangling refs.
     memset(pa, 1, PGSIZE);
     r = (struct run*)pa;
     acquire(&kmem.lock);
     r->next = kmem.freelist;
     kmem.freelist = r;
     release(&kmem.lock);
   }
   ```

5. 先引入外部变量`extern uint page_ref[];`，再在 `kernel/vm.c` 中修改 uvmcopy 函数。

   ```c
   int uvmcopy(pagetable_t old, pagetable_t new, uint64 sz)
   {
     pte_t *pte;
     uint64 pa, i;
     uint flags;
     //char *mem;
     for(i = 0; i < sz; i += PGSIZE){
       if((pte = walk(old, i, 0)) == 0)
         panic("uvmcopy: pte should exist");
       if((*pte & PTE_V) == 0)
         panic("uvmcopy: page not present");
       pa = PTE2PA(*pte);
       *pte = (*pte & ~PTE_W) | PTE_COW;
       flags = PTE_FLAGS(*pte);
       //if((mem = kalloc()) == 0)
       //  goto err;
       //memmove(mem, (char*)pa, PGSIZE);
       if(mappages(new, i, PGSIZE, pa, flags) != 0){
         //kfree(mem);
         goto err;
       }
       page_ref[COW_INDEX(pa)]++;
     }
     return 0;
   
    err:
     uvmunmap(new, 0, i / PGSIZE, 1);
     return -1;
   }
   ```

6. 修改 `kernel/trap.c` 的 usertrap()，添加中断处理逻辑。

   ```c
   ...
   else if(r_scause() == 15) {
       uint64 va = r_stval();
       if(va >= p->sz)
         p->killed = 1;
       else if(cow_alloc(p->pagetable, va) != 0)
         p->killed = 1;
     }
   ...
   ```

7. 在`kernel/defs.h`中声明，并在`kernel/kalloc.c`中实现cow_alloc函数。

   ```c
   int
   cow_alloc(pagetable_t pagetable, uint64 va) {
     va = PGROUNDDOWN(va);
     if(va >= MAXVA) return -1;
     pte_t *pte = walk(pagetable, va, 0);
     if(pte == 0) return -1;
     uint64 pa = PTE2PA(*pte);
     if(pa == 0) return -1;
     uint64 flags = PTE_FLAGS(*pte);
     if(flags & PTE_COW) {
       uint64 mem = (uint64)kalloc();
       if (mem == 0) return -1;
       memmove((char*)mem, (char*)pa, PGSIZE);
       uvmunmap(pagetable, va, 1, 1);
       flags = (flags | PTE_W) & ~PTE_COW;
       if (mappages(pagetable, va, PGSIZE, mem, flags) != 0) {
         kfree((void*)mem);
         return -1;
       }
     }
     return 0;
   }
   ```

8. 调用上述实现的cow_alloc函数，修改 kernel/vm.c 的 copyout()。

   ```c
   int
   copyout(pagetable_t pagetable, uint64 dstva, char *src, uint64 len)
   {
     uint64 n, va0, pa0;
     while(len > 0){
       va0 = PGROUNDDOWN(dstva);
       pa0 = walkaddr(pagetable, va0);
       if(pa0 == 0)
         return -1;
       n = PGSIZE - (dstva - va0);
       if(n > len)
         n = len;
       memmove((void *)(pa0 + (dstva - va0)), src, n);
       len -= n;
       src += n;
       dstva = va0 + PGSIZE;
     }
     return 0;
   }
   ```

9. 将 `kernel/vm.c` 中 walk()声明在 defs.h 中。

10. 运行测试指令，结果如下：

    ![image-20230730015739254](C:\Users\wyx20\AppData\Roaming\Typora\typora-user-images\image-20230730015739254.png)

    ![image-20230730020034732](C:\Users\wyx20\AppData\Roaming\Typora\typora-user-images\image-20230730020034732.png)

    ![image-20230730020048483](C:\Users\wyx20\AppData\Roaming\Typora\typora-user-images\image-20230730020048483.png)

#### 3）实验中遇到的问题和解决办法

实验过程遇到panic，经过多次debug后发现是kfree函数写的有问题，释放内存页的逻辑不准确。

![image-20230730014543216](images\image-20230730014543216.png)

#### 4）实验心得

在fork系统调用中使用COW机制的目的是延迟物理页面的分配和复制，即在真正需要复制物理页面的时候，再把页面复制给子进程。

COW机制的fork系统调用只为子进程创建一个页表pagetable，并且其中的PTE都指向父进程的物理页。并且会把父进程的页表中的页表项PTE的可写标志位全部置为无效。当父子中任何一个进程要写COW页的时候，CPU会产生一个页表错误。内核错误处理程序处理这个错误，为出错的进程分配一个物理页面，并且把原来的页面拷贝过来。并且把当前进程对这个地址在页表中的映射设置到新的物理页面上，然后把PTE设置为可写的。当页表错误返回到用户态的时候，用户进程就可以写这个页面了。

COW机制的fork系统调用在释放页面内存的时候有点技巧，一个给定的物理页面可能会被多个进程的页表使用，只有当引用数为1的时候，释放页面才会真正释放物理内存。

## Lab: Multithreading

### 1. Uthread: switching between threads (moderate)

#### 1）实验目的

该实验的目的是设计和实现一个用户级线程（User-Level Threading）系统的上下文切换机制，并通过在 xv6 操作系统中添加相应的代码来完成这个任务。用户级线程是一种轻量级的线程实现，由用户程序管理线程的切换和调度，而不依赖于操作系统内核的线程管理机制。

#### 2）实验步骤

1. 修改 user/uthread.c，struct thread 增加成员 struct context，用于线程切换时保存/恢复寄存器信息，并加入到thread中。

   ```c
   struct thread_context{
     uint64 ra;
     uint64 sp;
     uint64 s0;
     uint64 s1;
     uint64 s2;
     uint64 s3;
     uint64 s4;
     uint64 s5;
     uint64 s6;
     uint64 s7;
     uint64 s8;
     uint64 s9;
     uint64 s10;
     uint64 s11;
   };
   ```

2. 修改thread_schedule函数，按照提示加入thread_switch。

   ```c
   thread_switch((uint64)&t->context, (uint64)&current_thread->context);
   ```

3. 修改thread_create函数，添加：

   ```c
   t->context.ra = (uint64)func;
   t->context.sp = (uint64)&t->stack[STACK_SIZE - 1];
   ```

4. 修改 user/uthread_switch.S，实现保存/恢复寄存器。

   ```c
   /* YOUR CODE HERE */
   	sd ra, 0(a0)
           sd sp, 8(a0)
           sd s0, 16(a0)
           sd s1, 24(a0)
           sd s2, 32(a0)
           sd s3, 40(a0)
           sd s4, 48(a0)
           sd s5, 56(a0)
           sd s6, 64(a0)
           sd s7, 72(a0)
           sd s8, 80(a0)
           sd s9, 88(a0)
           sd s10, 96(a0)
           sd s11, 104(a0)
           ld ra, 0(a1)
           ld sp, 8(a1)
           ld s0, 16(a1)
           ld s1, 24(a1)
           ld s2, 32(a1)
           ld s3, 40(a1)
           ld s4, 48(a1)
           ld s5, 56(a1)
           ld s6, 64(a1)
           ld s7, 72(a1)
           ld s8, 80(a1)
           ld s9, 88(a1)
           ld s10, 96(a1)
           ld s11, 104(a1)
   	ret    /* return to ra */
   ```

5. 进行测试，实验结果如下：

   ![image-20230730151249665](images\image-20230730151249665.png)

![image-20230730151308793](images\image-20230730151308793.png)

#### 3）实验中遇到的问题和解决办法

还是编译过程中遇到的问题，不要忘记先声明后使用。

#### 4）实验心得

线程调度的过程主要是保存 contex 上下文状态，因为这里的切换全都是以函数调用的形式，因此这里只需要保存被调用者保存的寄存器（Callee-saved register）即可，调用者的寄存器会自动保存。

在操作系统课程中学过，进程是操作系统资源分配的基本单位，而线程是处理器任务调度和执行的基本单位，而在 xv6 中，一个进程只有一个线程，因此本实验中区分不大。

### 2. Using threads (moderate)

#### 1）实验目的

实验的目的是通过使用线程和锁来探索并行编程。在这个实验中，你将使用 UNIX pthread 线程库，在一个简单的哈希表上进行编程练习。哈希表是一种常见的数据结构，用于存储键值对，并通过哈希函数将键映射到哈希表的索引位置。将利用加锁操作，来解决哈希表 race-condition 导致的数据丢失问题。

#### 2）实验步骤

1. 在`notxv6/ph.c`中定义互斥锁。

   ```c
   pthread_mutex_t lock[NBUCKET];
   ```

2. 再在`notxv6/ph.c`中对互斥锁初始化。

   ```c
   for (int i = 0; i < NBUCKET; i++){
       pthread_mutex_init(&lock[i], NULL);
     } 
   ```

3.  修改 `notxv6/ph.c`，对 put 操作加锁。

   ```c
   pthread_mutex_lock(&lock[i]);
   // the new is new.
   insert(key, value, &table[i], table[i]);
   pthread_mutex_unlock(&lock[i]);
   ```

4. make grade测试，实验结果如下：

   ![image-20230730154449532](images\image-20230730154449532.png)

#### 3）实验中遇到的问题和解决办法

这里用到make grade来进行测试，顺便测试了一下前面几个实验，在make grade时发现前三个实验出现报错：![f949fcec83b991db8a76c1ce06ca570](C:\Users\wyx20\AppData\Local\Temp\WeChat Files\f949fcec83b991db8a76c1ce06ca570.png)

参考他人问题解决方案：http://t.csdn.cn/yyqU8，给python设置软链接，解决。

#### 4）实验心得

在并发哈希表的实现中，锁的粒度是关键问题。使用一个全局锁会导致多个线程竞争，并降低并发性能。为了减小锁的竞争范围，我们可以为每个哈希表槽位添加独立的锁，使得插入操作在不同槽位之间可以并行进行，提高了并发性。这样的设计允许多个线程同时向哈希表插入数据，合理利用多核处理器资源，同时保持了哈希表的正确性。通过合理的锁设计和哈希函数的选择，我们可以实现一个高效且正确的并发哈希表。

### 3. Barrier(moderate)

#### 1）实验目的

本实验的目的是实现一个屏障（Barrier），它是多线程编程中一种用于同步线程的机制。屏障是一个在应用程序中的特定点，要求所有参与的线程都必须等待，直到所有其他参与的线程也到达了该点。在本实验中，将使用 pthread 条件变量来实现屏障，它类似于 xv6 中的sleep和wakeup机制。

#### 2）实验步骤

1. 在barrier.c中实现barrier函数。

   ```c
   pthread_mutex_lock(&bstate.barrier_mutex);
     bstate.nthread++;
     if (bstate.nthread == nthread) {
       bstate.round++;
       bstate.nthread = 0;
       pthread_cond_broadcast(&bstate.barrier_cond);
     }
     else {
       pthread_cond_wait(&bstate.barrier_cond, &bstate.barrier_mutex);
     }
     pthread_mutex_unlock(&bstate.barrier_mutex);
   ```

2. 运行make grade测试：

   ![image-20230730171219579](images\image-20230730171219579.png)

#### 3）实验中遇到的问题和解决办法

对于条件变量部分不是很理解。查阅资料得知条件变量是利用线程间共享的全局变量进行同步的一种机制，主要包括两个动作：一个线程等待"条件变量的条件成立"而挂起；另一个线程使"条件成立"（给出条件成立信号）。为了防止竞争，条件变量的使用总是和一个互斥锁结合在一起。

#### 4）实验心得

对于多线程中的线程调度，过程大致如下：

- 首先是用户线程接收到了时钟中断，强迫CPU从用户空间进程切换到内核，同时在 trampoline 代码中，保存当前寄存器状态到 trapframe 中；
- 在 usertrap 处理中断时，切换到了该进程对应的内核线程；
- 内核线程在内核中，先做一些操作，然后调用 swtch 函数，保存用户进程对应的内核线程的寄存器至 context 对象；
- swtch 函数并不是直接从一个内核线程切换到另一个内核线程；而是先切换到当前 cpu 对应的调度器线程，之后就在调度器线程的 context 下执行 schedulder 函数中；
- schedulder 函数会再次调用 swtch 函数，切换到下一个内核线程中，由于该内核线程肯定也调用了 swtch 函数，所以之前的 swtch 函数会被恢复，并返回到内核线程所对应进程的系统调用或者中断处理程序中。
- 当内核程序执行完成之后，trapframe 中的用户寄存器会被恢复，完成线程调度。

## Lab: locks

### 1. Memory allocator (moderate)

#### 1）实验目的

本实验的目的是优化 xv6 操作系统的内存分配器，减少锁竞争和提高并发性能。在 `kalloctest` 程序中，多个进程会频繁地进行内存分配和释放操作，导致对内核内存分配器的访问产生了很多锁竞争。实验要求通过重新设计内存分配器来减少锁竞争，提高内存分配和释放的并发性。

#### 2）实验步骤

1.  在kernel/kalloc.c中修改原有结构体声明 kmem，以数组形式为每个cpu分配kmem。

   ```c
   struct {
     struct spinlock lock;
     struct run *freelist;
   } kmem[NCPU]; 
   ```

2. 修改 kinit 方法，对 NCPU 个 kmem 结构体初始化。

   ```c
   void
   kinit()
   {
     for (int i = 0; i < NCPU; i++){
       initlock(&kmem[i].lock, "kmem");
     }
     freerange(end, (void*)PHYSTOP);
   }
   ```

3. 修改 kfree 方法，获取当前 cpuid，释放对应 freelist。

   ```c
   r = (struct run*)pa;
   
     push_off(); 
     int id = cpuid();
     acquire(&kmem[id].lock);
     r->next = kmem[id].freelist;
     kmem[id].freelist = r;
     release(&kmem[id].lock);
     pop_off();
   ```

4. 修改 kalloc 方法，当前 CPU freelist 为空时，从其他 CPU freelist 处 获取。

   ```c
   struct run *r;
     push_off();
     int id = cpuid();
     acquire(&kmem[id].lock);
     r = kmem[id].freelist;
     if(r)
       kmem[id].freelist = r->next;
     else {
       int new_id;
       for (new_id = 0; new_id < NCPU; ++new_id){
         if (new_id == id)
           continue;
         acquire(&kmem[new_id].lock);
         r = kmem[new_id].freelist;
         if (r){
           kmem[new_id].freelist = r->next;
           release(&kmem[new_id].lock);
           break;
         }
         release(&kmem[new_id].lock);
       }
     }
     release(&kmem[id].lock);
     pop_off();
     if(r)
       memset((char*)r, 5, PGSIZE); // fill with junk
     return (void*)r;
   ```

5. 测试实验结果如下：

   ![image-20230730203824267](images\image-20230730203824267.png)

   ![image-20230730204326292](images\image-20230730204326292.png)

![image-20230730204437163](images\image-20230730204437163.png)

#### 3）实验中遇到的问题和解决办法

如果有变量定义了但没有使用也会报错unused variable，因此要注意检查有无多余的变量可以注释掉。

#### 4）实验心得

在本实验中，通过优化操作系统的内存分配器，使用多个独立的自由列表和锁来减少锁竞争，提高了内存分配和释放的并发性能。原始的内存分配器使用单一的全局锁来保护自由列表，导致多核 CPU 之间频繁竞争锁，降低了系统的并发性能。通过为每个 CPU 核心维护一个独立的自由列表，并为每个列表分别使用独立的锁，实现了内存分配和释放的并行操作，避免了多核 CPU 的锁竞争，提高了并发性能。

### 2. Buffer cache (hard)

#### 1）实验目的

在本实验中需要优化操作系统的块缓存（bcache）以减少在多个进程之间对 bcache.lock 的竞争。在初始实现中，多个进程对文件系统进行密集操作时会频繁竞争 bcache.lock，导致锁的争用，从而降低了系统性能。我们的目标是通过修改块缓存的设计，使得在多进程访问时，锁的竞争尽量接近零。

#### 2）实验步骤

1. 在 `kernel/bio.c`中定义buckets数量，添加到bcache中。实现hash函数并在defs.h中声明。

   ```c
   struct {
     struct spinlock lock[NBUCKETS];
     struct buf buf[NBUF];
   
     // Linked list of all buffers, through prev/next.
     // Sorted by how recently the buffer was used.
     // head.next is most recent, head.prev is least.
     struct buf head[NBUCKETS];
   } bcache;
   
   int hash(uint blockno){
     return blockno % NBUCKETS;
   }
   ```

2. 修改 `kernel/bio.c`中binit 方法。

   ```c
   void
   binit(void)
   {
     struct buf *b;
     initlock(&bcache.lock, "bcache");
     // Create linked list of buffers
     bcache.head.prev = &bcache.head;
     bcache.head.next = &bcache.head;
     for(b = bcache.buf; b < bcache.buf+NBUF; b++){
       b->next = bcache.head.next;
       b->prev = &bcache.head;
       initsleeplock(&b->lock, "buffer");
       bcache.head.next->prev = b;
       bcache.head.next = b;
     }
   }
   ```

3. 修改 `kernel/bio.c` 中的 bget 方法。

   ```c
    struct buf *b;
     acquire(&bcache.lock);
     // Is the block already cached?
     for(b = bcache.head.next; b != &bcache.head; b = b->next){
       if(b->dev == dev && b->blockno == blockno){
         b->refcnt++;
         release(&bcache.lock);
         acquiresleep(&b->lock);
         return b;
       }
     }
     // Not cached.
     // Recycle the least recently used (LRU) unused buffer.
     for(b = bcache.head.prev; b != &bcache.head; b = b->prev){
       if(b->refcnt == 0) {
         b->dev = dev;
         b->blockno = blockno;
         b->valid = 0;
         b->refcnt = 1;
         release(&bcache.lock);
         acquiresleep(&b->lock);
         return b;
       }
     }
     panic("bget: no buffers");
   ```

4. 修改 `kernel/bio.c` 中的 brelse 方法。

   ```c
    if(!holdingsleep(&b->lock))
       panic("brelse");
   
     releasesleep(&b->lock);
   
     acquire(&bcache.lock);
     b->refcnt--;
     if (b->refcnt == 0) {
       // no one is waiting for it.
       b->next->prev = b->prev;
       b->prev->next = b->next;
       b->next = bcache.head.next;
       b->prev = &bcache.head;
       bcache.head.next->prev = b;
       bcache.head.next = b;
     }
     release(&bcache.lock);
   ```

5. 修改 kernel/bio.c 中的 bpin、bunpin 方法。

   ```c
   void
   bpin(struct buf *b) {
     acquire(&bcache.lock);
     b->refcnt++;
     release(&bcache.lock);
   }
   
   void
   bunpin(struct buf *b) {
     acquire(&bcache.lock);
     b->refcnt--;
     release(&bcache.lock);
   }
   ```

6. 运行测试，结果如下：

   ![image-20230730232032329](images\image-20230730232032329.png)

![image-20230730232048059](images\image-20230730232048059.png)

#### 3）实验中遇到的问题和解决办法

在这个实验中会涉及到链表的写法，bget方法中在找到一个未使用的缓存块用于缓存新的块时，需要将该缓存块从当前的哈希桶链表中摘除，然后插入到新的哈希桶链表中。我使用了双向链表来进行移动。确保在插入和摘除操作时链表指针的正确性，避免出现不完整或不正确的链表结构。

#### 4）实验心得

这个Lab的目的是设计锁，主要是降低多线程情况下对锁的竞争。整体思路就是降低锁的粒度，将一个大锁更换为一些粒度小的锁，这样可以大幅度降低锁的竞争。本实验主要解决 cache 缓存竞争问题，同之前一样，需要设计小粒度的锁。这里缓存区大小是固定的，不好随意更改。由于代码中会用到双向链表，链表操作方面需要确保链表的正确性和并发安全，以保证块缓存的正常工作。同时，需要保证代码逻辑的正确性，避免潜在的死循环和未处理的异常情况。

## Lab: file system

### 1. Large files (moderate)

#### 1）实验目的

本实验的实验目的是要增加xv6文件的最大大小限制。目前，xv6文件的大小受限于inode中的直接块和单级间接块的数量，总共为268个块。这是因为一个inode包含12个直接块号和一个单级间接块号，该单级间接块号引用一个块，其中包含256个更多的块号，总共可以达到12+256=268个块。

在这个实验中，我们需要修改xv6文件系统的代码，以支持inode中的"双级间接块"，其中包含256个指向单级间接块的地址，而每个单级间接块可以包含256个数据块的地址。这样，一个文件的大小可以达到65803个块，或256*256+256+11个块（11代替12，因为我们会为双级间接块牺牲一个直接块号）。

要实现这个目标，需要在inode中添加一个双级间接块，并相应地修改bmap()函数，以支持对双级间接块的查找和分配。确保修改后的文件系统可以成功创建并读写大文件，并通过相关测试。

#### 2）实验步骤

1. 修改 kernel/fs.h中宏定义，以及dinode中对应部分。

   ```c
   #define NDIRECT 11 
   #define NINDIRECT (BSIZE / sizeof(uint))
   #define NDINDIRECT (NINDIRECT * NINDIRECT)
   #define MAXFILE (NDIRECT + NINDIRECT + NDINDIRECT)
   ```

2. 修改 kernel/file.h中的inode部分。

   ```c
   struct inode {
     uint dev;           // Device number
     uint inum;          // Inode number
     int ref;            // Reference count
     struct sleeplock lock; // protects everything below here
     int valid;          // inode has been read from disk?
     short type;         // copy of disk inode
     short major;
     short minor;
     short nlink;
     uint size;
     uint addrs[NDIRECT+1];
   };
   ```

3. 修改 kernel/fs.c 的 bmap 函数,实现 bmap 映射，仿照一级索引实现二级索引。

   ```c
   static uint
   bmap(struct inode *ip, uint bn)
   {
     uint addr, *a;
     struct buf *bp;
     if(bn < NDIRECT){
       if((addr = ip->addrs[bn]) == 0)
         ip->addrs[bn] = addr = balloc(ip->dev);
       return addr;
     }
     bn -= NINDIRECT;
     if (bn < NDINDIRECT){
       if ((addr = ip->addrs[NDIRECT + 1]) == 0)
         ip->addrs[NDIRECT + 1] = addr = balloc(ip->dev);
       bp = bread(ip->dev, addr);
       a = (uint*)bp->data;
       if ((addr = a[bn / NINDIRECT]) == 0){
         a[bn / NINDIRECT] = addr = balloc(ip->dev);
         log_write(bp);
       }
       brelse(bp);
       bp = bread(ip->dev, addr);
       a = (uint*)bp->data;
       if ((addr = a[bn % NINDIRECT]) == 0){
         a[bn % NINDIRECT] = addr = balloc(ip->dev);
         log_write(bp);
       }
       brelse(bp);
       return addr;
     }
     panic("bmap: out of range");
   }
   ```

4. 修改 kernel/fs.c 的 itrunc 函数，确保释放包括两级页表在内的所有块。

   ```c
   struct buf *bp2;
   uint *a2;
     if(ip->addrs[NDIRECT + 1]){
       bp = bread(ip->dev, ip->addrs[NDIRECT + 1]);
       a = (uint*)bp->data;
       for(j = 0; j < NINDIRECT; j++){
         if(a[j]){
           bp2 = bread(ip->dev, a[j]);
           a2 = (uint*)bp2->data;
           for (i = 0; i < NINDIRECT; i++){
             if (a2[i])
               bfree(ip->dev, a2[i]);
           }
           brelse(bp2);
           bfree(ip->dev, a[j]);
           a[j] = 0;
         }
       }
       brelse(bp);
       bfree(ip->dev, ip->addrs[NDIRECT + 1]);
       ip->addrs[NDIRECT + 1] = 0;
     }
   ```

5. 运行测试，实验结果如下：

   ![image-20230731112149072](images\image-20230731112149072.png)

![image-20230731112716839](images\image-20230731112716839.png)

#### 3）实验中遇到的问题和解决办法

修改函数过程中有几个小的编译错误，很快解决了。

#### 4）实验心得

主要是对文件系统这部分加深了理解。

![img](https://pic3.zhimg.com/80/v2-656c90bac78a76fabb611a19d0e92832_720w.webp)

磁盘中不同区域的数据块有不同的功能。第 0 块数据块是启动区域，计算机启动就是从这里开始的；第 1 块数据是超级块，存储了整个磁盘的信息；然后是 log 区域，用于故障恢复；bit map 用于标记磁盘块是否使用；然后是 inode 区域 和 data 区域。

磁盘中主要存储文件的 block 是 inode 和 data。操作系统中，文件的信息是存放在 inode 中的，每个文件对应了一个 inode，inode 中含有存放文件内容的磁盘块的索引信息，用户可以通过这些信息来查找到文件存放在磁盘的哪些块中。inodes 块中存储了很多文件的 inode。

### 2. Symbolic links (moderate)

#### 1）实验目的

此实验的实验目的是向xv6中添加符号链接（Symbolic links）。符号链接是一种特殊类型的文件链接，它通过路径名来引用另一个文件。当打开符号链接时，内核会跟随该链接并访问所引用的文件。与硬链接不同，硬链接只能指向同一磁盘上的文件，而符号链接可以跨越磁盘设备。xv6 中没有符号链接（软链接），实现这个系统调用是一个很好的练习，可以更好地理解路径名查找的工作原理。

#### 2）实验步骤

1. 首先需要增加 symlink 系统调用。

   * 在user/usys.pl中添加：`entry("symlink");`。

   * 在user/user.h中添加：`int symlink(const char*, const char*);`。

   * 在kernel/syscall.h中添加宏定义：`#define SYS_symlink 22`

   * 在kernel/syscall.c中添加：`extern uint64 sys_symlink(void);`和`[SYS_symlink] sys_symlink,`

   * 在knerl/sysfile.c中实现系统调用函数：

     ```c
     uint64
     sys_symlink(void)
     {  
       char path[MAXPATH], target[MAXPATH];
       struct inode *ip;
       // 读取参数
       if(argstr(0, target, MAXPATH) < 0)
         return -1;
       if(argstr(1, path, MAXPATH) < 0)
         return -1;
       // 开启事务
       begin_op();
       // 为这个符号链接新建一个 inode
       if((ip = create(path, T_SYMLINK, 0, 0)) == 0) {
         end_op();
         return -1;
       }
       // 在符号链接的 data 中写入被链接的文件
       if(writei(ip, 0, (uint64)target, 0, MAXPATH) < MAXPATH) {
         iunlockput(ip);
         end_op();
         return -1;
       }
       // 提交事务
       iunlockput(ip);
       end_op();
       return 0;
     }
     ```

   2. 在`kernel/stat.h`中添加一个新的文件类型(T_SYMLINK)来表示符号链接。

   3. 在`kernel/fcntl.h`中定义 O_NOFOLLOW。

   4. 在kernel/sysfile.c中修改sys_open函数。

      ```c
      ...
      if(omode & O_CREATE){
          ip = create(path, T_FILE, 0, 0);
          if(ip == 0){
            end_op();
            return -1;
          }
        } else {
          int max_depth = 20, depth = 0;
          while (1) {
            if ((ip = namei(path)) == 0) {
              end_op();
              return -1;
            }
            ilock(ip);
            if (ip->type == T_SYMLINK && (omode & O_NOFOLLOW) == 0) {
              if (++depth > max_depth) {
                iunlockput(ip);
                end_op();
                return -1;
              }
              if (readi(ip, 0, (uint64)path, 0, MAXPATH) < MAXPATH) {
                iunlockput(ip);
                end_op();
                return -1;
              }
              iunlockput(ip);
            }
            else
              break;
          }
        }
        ...
      ```

   5. 在Malefile中添加 `$U/_symlinktest\`。

   6. 测试，实验结果如下：

      ![image-20230731134247556](images\image-20230731134247556.png)

#### 3）实验中遇到的问题和解决办法

进行测试时发现失败，检查后发现是系统调用没有在Makefile中添加`$U/_symlinktest\`编译声明，补上后测试通过。

![image-20230731133955364](images\image-20230731133955364.png)

#### 4）实验心得

对于硬链接以及符号链接（软链接）的理解：

硬链接是指多个文件名指向同一个inode号码。有以下特点：

- 可以用不同的文件名访问同样的内容；
- 对文件内容进行修改，会影响到所有文件名；
- 删除一个文件名，不影响另一个文件名的访问。

而软链接也是一个文件，但是文件内容指向另一个文件的 inode。打开这个文件时，会自动打开它指向的文件，类似于 windows 系统的快捷方式。

## Lab: mmap (hard)

### 1）实验目的

这个实验的实验目的是向xv6中添加`mmap`和`munmap`系统调用，以实现内存映射文件的功能。`mmap`和`munmap`允许用户程序对它们的地址空间进行详细控制，包括共享内存、将文件映射到进程的地址空间以及作为用户级页面错误处理方案的一部分（例如垃圾回收算法）。在这个实验中，重点是实现内存映射文件相关的功能。

### 2）实验步骤

1. 先在UPROGS中加入mmaptest的编译声明 `$U/_mmaptest\`

2. 在 kernel/syscall.h 中添加sys_mmap，sys_munmap的系统调用码，在 kernel/syscall.c 中添加系统调用函数引用。

   ```c
   #define SYS_mmap   22
   #define SYS_munmap 23
   ```

   ```c
   extern uint64 sys_mmap(void);
   extern uint64 sys_munmap(void);
   
   [SYS_mmap]    sys_mmap,
   [SYS_munmap]  sys_munmap,
   ```

3. 在 user/usys.pl 中添加 entry。

   ```c
   entry("mmap");
   entry("munmap");
   ```

4. 在 user/user.h 中添加系统调用声明。

   ```c
   void* mmap(void *, int, int, int, int, uint);
   int munmap(void *, int);
   ```

5. 在`kernel/proc.h`中根据传入的函数参数，构建一个 VMA 结构体数组，并添加到结构体proc中。

   ```c
   #define VMASIZE 16
   struct vma
   {
     struct file *file;
     int fd;
     int used;
     uint64 addr;
     int length;
     int prot;
     int flags;
     int offset;
   };
   ```

6. 修改usertrap,实现中断处理。

   ```c
   if(r_scause() == 8){
       // system call
       if(p->killed)
         exit(-1);
       // sepc points to the ecall instruction,
       // but we want to return to the next instruction.
       p->trapframe->epc += 4;
       // an interrupt will change sstatus &c registers,
       // so don't enable until done with those registers.
       intr_on();
       syscall();
     } else if((which_dev = devintr()) != 0){
       // ok
     } 
     else if (r_scause() == 13 || r_scause() == 15) {
       uint64 va = r_stval();
       if(va >= p->sz || va > MAXVA || PGROUNDUP(va) == PGROUNDDOWN(p->trapframe->sp))
         p->killed = 1;
   	  else {
         struct vma *vma = 0;
         for (int i = 0; i < VMASIZE; i++) {
           if (p->vma[i].used == 1 && va >= p->vma[i].addr && 
               va < p->vma[i].addr + p->vma[i].length) {
             vma = &p->vma[i];
             break;
           }
         }
   
         if(vma) {
           va = PGROUNDDOWN(va);
           uint64 offset = va - vma->addr;
           uint64 mem = (uint64)kalloc();
           if(mem == 0) {
             p->killed = 1;
           } 
           else {
             memset((void*)mem, 0, PGSIZE);
   		      ilock(vma->file->ip);
             readi(vma->file->ip, 0, mem, offset, PGSIZE);
             iunlock(vma->file->ip);
             int flag = PTE_U;
             if(vma->prot & PROT_READ) flag |= PTE_R;
             if(vma->prot & PROT_WRITE) flag |= PTE_W;
             if(vma->prot & PROT_EXEC) flag |= PTE_X;
             if(mappages(p->pagetable, va, PGSIZE, mem, flag) != 0) {
               kfree((void*)mem);
               p->killed = 1;
             }
           }
         }
       }
     }
     else {
       printf("usertrap(): unexpected scause %p pid=%d\n", r_scause(), p->pid);
       printf("            sepc=%p stval=%p\n", r_sepc(), r_stval());
       p->killed = 1;
     }
   ```

7. 在kernel/sysfile.c中实现sys_mmap和sys_munmap这两个函数。

   ```c
   uint64 sys_mmap(void) {
     uint64 addr;
     int length, prot, flags, fd, offset;
     struct proc *p = myproc();
     struct file *file;
     if(argaddr(0, &addr) || argint(1, &length) || argint(2, &prot) ||
       argint(3, &flags) || argfd(4, &fd, &file) || argint(5, &offset)) 
       return -1;
   
     if(!file->writable && (prot & PROT_WRITE) && flags == MAP_SHARED)
       return -1;
   
     length = PGROUNDUP(length);
     if(p->sz > MAXVA - length)
       return -1;
   
     for(int i = 0; i < VMASIZE; i++) {
       if(p->vma[i].used == 0) {
         p->vma[i].used = 1;
         p->vma[i].addr = p->sz;
         p->vma[i].length = length;
         p->vma[i].prot = prot;
         p->vma[i].flags = flags;
         p->vma[i].fd = fd;
         p->vma[i].file = file;
         p->vma[i].offset = offset;
         filedup(file);
         p->sz += length;
         return p->vma[i].addr;
       }
     }
     return -1;
   }
   
   uint64
   sys_munmap(void)
   {
     uint64 addr;
     int length;
     struct proc *p = myproc();
     struct vma *vma = 0;
     if(argaddr(0, &addr) || argint(1, &length))
       return -1;
     addr = PGROUNDDOWN(addr);
     length = PGROUNDUP(length);
     for(int i = 0; i < VMASIZE; i++) {
       if (addr >= p->vma[i].addr || addr < p->vma[i].addr + p->vma[i].length) {
         vma = &p->vma[i];
         break;
       }
     }
     if(vma == 0) 
       return 0;
     if(vma->addr == addr) {
       vma->addr += length;
       vma->length -= length;
       if(vma->flags & MAP_SHARED)
         filewrite(vma->file, addr, length);
       uvmunmap(p->pagetable, addr, length/PGSIZE, 1);
       if(vma->length == 0) {
         fileclose(vma->file);
         vma->used = 0;
       }
     }
     return 0;
   }
   ```

8. 在kernel/vm.c中修改uvmcopy和uvmunmap，注释掉部分panic。

9. 在kernel/proc.c中修改fork和exit函数。在进程创建和退出时，需要复制和清空相应的文件映射。

   fork:

   ```c
    for(int i = 0; i < VMASIZE; i++) {
       if(p->vma[i].used){
         memmove(&(np->vma[i]), &(p->vma[i]), sizeof(p->vma[i]));
         filedup(p->vma[i].file);
       }
     }
     release(&np->lock);
   ```

   exit:

   ```c
   for(int i = 0; i < VMASIZE; i++) {
       if(p->vma[i].used) {
         if(p->vma[i].flags & MAP_SHARED)
           filewrite(p->vma[i].file, p->vma[i].addr, p->vma[i].length);
         fileclose(p->vma[i].file);
         uvmunmap(p->pagetable, p->vma[i].addr, p->vma[i].length/PGSIZE, 1);
         p->vma[i].used = 0;
       }
     }
   ```

10. 运行测试，实验结果如下：

    ![image-20230731221049786](images\image-20230731221049786.png)

### 3）实验中遇到的问题和解决办法

1. 实验中遇到如下报错。'MAP_SHARED'常量是用来指示内存映射时的共享权限的，通常在使用内存映射相关的系统调用时会用到它。在'kernel/fcntl.h'加入它的声明报错解决。

![image-20230731220950743](images\image-20230731220950743.png)

### 4）实验心得

通过这个实验以及查询一些资料也对mmap的原理有所了解，mmap是一种内存映射文件的方法，即将一个文件或者其它对象映射到进程的地址空间，实现文件磁盘地址和进程虚拟地址空间中一段虚拟地址的一一对映关系。实现这样的映射关系后，进程就可以采用指针的方式读写操作这一段内存，而系统会自动回写脏页面到对应的文件磁盘上，即完成了对文件的操作而不必再调用read,write等系统调用函数。相反，内核空间对这段区域的修改也直接反映用户空间，从而可以实现不同进程间的文件共享。

## Lab: networking

### 1）实验目的

本实验目的是编写一个 xv6 的设备驱动程序，用于处理网络接口卡（NIC）的网络通信。在这个实验中，将使用一个名为 E1000 的网络设备来处理网络通信。对于 xv6 来说，E1000 看起来就像是连接到真实以太网局域网（LAN）的真实硬件一样。

实验的主要任务是在 kernel/e1000.c 文件中完成 e1000_transmit() 和 e1000_recv() 函数，以使驱动程序能够发送和接收数据包。为了完成这个任务，需要理解 E1000 的初始化和操作方法，同时要熟悉 E1000 的相关寄存器和描述符的格式。

### 2）实验步骤

1. 在kernel/e1000.c中实现e1000_transmit()。

   ```c
   int
   e1000_transmit(struct mbuf *m)
   {
     //
     // Your code here.
     //
     // the mbuf contains an ethernet frame; program it into
     // the TX descriptor ring so that the e1000 sends it. Stash
     // a pointer so that it can be freed after sending.
     //
     acquire(&e1000_lock);
     uint index = regs[E1000_TDT];
     if((tx_ring[index].status & E1000_TXD_STAT_DD) == 0){
       release(&e1000_lock);
       return -1;
     }
     if(tx_mbufs[index])
       mbuffree(tx_mbufs[index]);
     tx_mbufs[index] = m;
     tx_ring[index].length = m->len;
     tx_ring[index].addr = (uint64)m->head;
     tx_ring[index].cmd = E1000_TXD_CMD_RS | E1000_TXD_CMD_EOP;
     regs[E1000_TDT] = (index + 1) % TX_RING_SIZE;
     release(&e1000_lock);
     return 0;
   }
   ```

2. 在kernel/e1000.c中实现e1000_recv()。

   ```c
   static void
   e1000_recv(void)
   {
     //
     // Your code here.
     //
     // Check for packets that have arrived from the e1000
     // Create and deliver an mbuf for each packet (using net_rx()).
     //
     uint index = regs[E1000_RDT];
     index = (index + 1) % RX_RING_SIZE;
     while(rx_ring[index].status & E1000_RXD_STAT_DD) {
       rx_mbufs[index]->len = rx_ring[index].length;
       net_rx(rx_mbufs[index]);
       if((rx_mbufs[index] = mbufalloc(0)) == 0)
         panic("e1000");
       rx_ring[index].addr = (uint64)rx_mbufs[index]->head;
       rx_ring[index].status = 0;
       index = (index + 1) % RX_RING_SIZE;
     }
     if(index == 0)
       index = RX_RING_SIZE;
     regs[E1000_RDT] = (index - 1) % RX_RING_SIZE;
   }
   ```

3. 测试。先在终端make server:

   ![image-20230801095116923](images\image-20230801095116923.png)

4. 新建终端，make qemu后执行nettests:

   ![image-20230801095420561](images\image-20230801095420561.png)

### 3）实验中遇到的问题和解决办法

第一次测试忘记make server, 长时间无响应，要先在一个终端make server再在另一个终端nettests，但是make grade可以不需要先make server。

### 4）实验心得

本实验对于两个函数的理解主要为：

1. e1000_transmit(struct mbuf *m) 负责将 m 填到 tx_ring[tx_transmit_index] 中，并环形自增 tx_transmit_index 以便网卡取数据。
2. e1000_recv(void) 负责 从 rx_ring[rx_recv_index] 拿出数据，并通过 net_rx 交由上层的协议/应用去处理。
