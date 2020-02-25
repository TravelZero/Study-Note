# Linux应用编程之进程与信号

## 进程

### 什么是进程

UNIX标准把进程定义为：“一个其中运行着一个或多个线程的地址空间和这些线程所需要的系统资源”。像Linux这样的多任务操作系统可以同时运行多个任程序。每个运行着的程序实例就构成一个进程。

### 进程的基础知识

每个进程都会被分配一个唯一的数字编号，称之为进程标识符或`PID`。它通常是一个取值范围2~32768的正整数。

Linux进程不能对用来存放程序代码的内存区域进行写操作，程序代码是以只读形式加载到内存中的。它可以被多个进程安全的共享。

进程有自己的栈空间，用于保存函数中的局部变量和控制函数的调用与返回。

#### 进程表

Linux进程表就像一个数据结构，其中包括进程的`PID`、进程的状态、命令字符串和其他一些ps命令输出的各类信息。操作系统通过进程的`PID`对它们进行管理，这些`PID`是进程表的索引。

#### 系统进程

一般而言，每个进程都是由另一个称之为父进程的进程启动的，被父进程启动的进程叫子进程。Linux系统启动时，它将运行一个名为init的进程，这是系统运行的第一个进程，它的进程号为1。可以把init进程看作操作系统的进程管理器，它是所有进程的祖先进程。

### 启动新进程

```c
#include <stdlib.h>

int system(const char *string);
```

`system`函数的作用是，运行以字符串形式传递给它的命令，并等待该命令的完成。

代码示例：

```c
#include <stdlib.h>
#include <stdio.h>

int main()
{
    printf("Running ps with system\n");
    system("ps ax");
    printf("Done.\n");
    exit(0);
}
```

#### `exec`系列函数（替换进程映像）

`exec`系列函数由一组相关的函数组成，它们在进程的启动方式和程序参数的表达方式上各有不同。**`exec`可以把当前进程替换为一个新进程**，新进程由`path`或`file`参数指定。

```c
#include <unistd.h>

char **environ;


int execl(const char *path, const char *arg, ..., (char *)0);
int execlp(const char *file, const char *arg, ... (char *)0);
int execle(const char *path, const char *arg, ..., (char *)0, char * const envp[]);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execve(const char *path, char *const argv[], char *const envp[]);
```

参数：

`path`参数表示你要启动程序的名称包括路径名。

`arg`参数表示启动程序所带的参数，一般第一个参数为要执行命令名，不是带路径且`arg`必须以NULL结束。

这些参数分为两大类。`execl`、`execlp`、`execle`的参数个数是可变的，参数以一个空指针结束。`execv`、`execvp`的第二个参数是一个字符串数组。不管哪种情况，新程序在启动时会把`argv`数组中给定的参数传递给`main`函数。

以字母p结尾的函数通过搜索`PATH`环境变量来查找新程序的可执行文件的路径。如果可执行文件不在`PATH`路径中，则需要把包含目录在内的使用绝对路径的文件名作为参数传递给函数。

全局变量`environ`可用来吧一个值传递到新的程序环境中。函数`execle`和`execve`可以通过参数`envp`传递字符串数组作为新程序的环境变量。

代码示例：

```c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("Running ps with execlp\n");
    execlp("ps", "ps", "ax", NULL);
    printf("Done.\n");
    exit(0);
}
```

#### `fork`函数（复制进程映像）

调用`fork`会创建一个新进程。这个系统调用复制当前进程，新进程与原进程几乎一样，执行代码也相同，但是新进程有自己的数据空间、环境和文件描述符。`fork`和`exec`函数结合在一起使用就是创建新进程所需要的一切了。

```c
#include <sys/types.h>
#include <unistd.h>

pid_t fork(void);
```

在父进程中`fork`调用返回的是新的子进程的`PID`，在子进程中`fork`调用返回的是0。父子进程可以通过这一点来进行判断谁是父进程，谁是子进程。

代码示例：

```c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    char *message;
    int n;

    printf("fork program starting\n");
    pid = fork();
    switch(pid) 
    {
    case -1:
        perror("fork failed");
        exit(1);
    case 0:
        message = "This is the child";
        n = 3;
        break;
    default:
        message = "This is the parent";
        n = 5;
        break;
    }

    for(; n > 0; n--) {
        puts(message);
        sleep(1);
    }
    exit(0);
}
```

#### 等待一个进程

当用`fork`启动一个子进程时，子进程就有了它自己的生命周期并将独立运行。但是我们可以获取到子进程结束的信息，以免父进程先与子进程结束，当子进程结束后形成僵尸进程。

```c
#include <sys/types.h>
#include <sys/wait.h>

int wait(int *status);
```

`status`参数如果不是一个空指针,则终止进程的终止状态就存放在status所指向的单元。

父进程一旦调用了`wait`就立即阻塞自己，由`wait`自动分析是否当前进程的某个子进程已经退出，如果让它找到了这样一个已经变成僵尸的子进程，`wait`就会收集这个子进程的信息，并把它彻底销毁后返回；如果没有找到这样一个子进程，`wait`就会一直阻塞在这里，直到有一个出现为止。

#### 僵尸进程

当父进程结束或异常终止而子进程未结束，子进程会自动把`PID`为1的进程（init进程）作为父进程。在子进程结束后，它就会变成僵尸进程，且由init进程管理，它会一直保存到被init进程发现并释放。

```
#include <sys/types.h>
#include <sys/wait.h>

pid_t waitpid(pid_t pid, int *status, int options);
```

`pid`>0：只等待进程ID等于pid的子进程，不管其它已经有多少子进程运行结束退出了，只要指定的子进程还没有结束，`waitpid`就会一直等下去。
`pid`=-1：等待任何一个子进程退出，没有任何限制，此时waitpid和wait的作用一模一样。
`pid`=0：等待同一个进程组中的任何子进程，如果子进程已经加入了别的进程组，waitpid不会对它做任何理睬。
`pid`<-1：等待一个指定进程组中的任何子进程，这个进程组的ID等于pid的绝对值。
`status`参数如果不是一个空指针,则终止进程的终止状态就存放在status所指向的单元。

`option`参数可以改变`waitpid`的行为，它的选项如下：

|     参数     |                             说明                             |
| :----------: | :----------------------------------------------------------: |
|  `WNOHANG`   | 若由`pid`指定的子进程未发生状态改变(没有结束)，则`waitpid()`不阻塞，立即返回0 |
| `WUNTRACED`  |          返回终止子进程信息和因信号停止的子进程信息          |
| `WCONTINUED` |    返回收到`SIGCONT`信号而恢复执行的已停止子进程状态信息     |

## 信号 

信号是UNIX和Linux系统响应某些条件而产生的一个事件。接收到该信号的进程会相应地采取一些行动。信号是由于某些错误条件而生成的，如内存段冲突、浮点处理器错误或非法指令等。它们有shell和终端处理器生成来引起中断，它们还可以作为进程间传递消息或修改行为的一种方式，明确地由一个进程发送给另一个进程。信号可以被生成、捕获、响应或忽略。

信号的宏被定义在头文件`signal.h`中。如下

|  信号名称  |             说明             |
| :--------: | :--------------------------: |
| `SIGABORT` |        *进程异常终止         |
| `SIGALRM`  |           超时警告           |
|  `SIGFPE`  |        *浮点运算异常         |
|  `SIGHUP`  |           连接挂断           |
|  `SIGILL`  |          *非法指令           |
|  `SIGINT`  |           终端中断           |
| `SIGKILL`  | 终止进程（不能被捕获或忽略） |
| `SIGPIPE`  |    向无读进程的管道写数据    |
| `SIGQUIT`  |           终端退出           |
| `SIGSEGV`  |        *无效内存访问         |
| `SIGTERM`  |             终止             |
| `SIGUSR1`  |        用户定义信号1         |
| `SIGUSR2`  |        用户定义信号2         |

**系统对信号的响应视具体实现而定*

如果进程接收到这些信号其中一个，但未安排捕获它，进程将会终止。通常，系统会生成核心转储文件core，并将其存放在当前目录下。

其他信号：

| 信号名称  |             说明             |
| :-------: | :--------------------------: |
| `SIGHLD`  |     子进程已经停止或退出     |
| `SIGCONT` |       继续执行暂停进程       |
| `SIGSTOP` | 停止执行（不能被捕获或忽略） |
| `SIGTSTP` |           终端挂起           |
| `SIGTTIN` |      后台进程尝试读操作      |
| `SIGTTOU` |      后台进程尝试写操作      |

`SIGHLD`信号对管理子进程很有用，默认情况下，它是被忽略的。

`SIGCONT`信号的作用是让进程恢复并继续执行。

程序使用`signal`库函数来处理信号，定义如下：

```c
#include <signal.h>

void (*signal(int sig, void (*func)(int)))(int);
```

`signal`是一个带有`sig`和`func`两个参数的函数。

`sig`给出准备捕获或忽略的信号。

`func`给出接收到指定信号后将要调用的函数。

信号处理函数必须有一个`int`类型的参数（即接收的信号代码）并且返回类型为`void`。

`signal`的返回值是指向之前的信号处理程序的指针。

可以使用两个特殊值之一代替信号处理函数：

|    值     |     说明     |
| :-------: | :----------: |
| `SIG_IGN` |   忽略信号   |
| `SIG_DFL` | 恢复默认行为 |

### 发送信号

进程可以通过调用`kill`函数向包括它本身在内的其他进程发送一个信号。如果进程没有发送该信号的权限，对`kill`函数的调用就将失败。定义如下：

```c
#include <sys/types.h>
#include <signal.h>

int kill(pid_t pid, int sig);
```

`pid`指定进程的进程ID，注意用户的权限，比如普通用户不可以杀死1号进程（init）。

​			`pid`>0：发送信号给指定进程

​            `pid`=0：发送信号给与调用kill函数进程属于同一进程组的所有进程

​            `pid`<0：发送信号给pid绝对值对应的进程组

​            `pid`=-1：发送给进程有权限发送的系统中的所有进程

`sig`给定发送的信号，建议使用信号名（宏名）而不是信号编号。

```c
#include <signal.h>

int raise(int signo);
```

`raise`函数调用是进程会向自身发送信号。

```c
#include <unistd.h>

unsigned int alarm(unsigned int seconds);
```

`alarm`函数提供一个闹钟超时警告功能，调用`alarm`函数经过一段预定时间后会发送一个`SIGALRM`信号。

```c
#include <unistd.h>

int pause(void);
```

`pause`函数可以将程序的执行挂起直到有一个信号出现为止。当`pause`函数被一个信号中断时，将返回-1并把`errno`设为`EINTR`。

代码示例：

```c
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int alarm_fired = 0;

void ding(int sig)
{
    alarm_fired = 1;
}

int main()
{
    pid_t pid;

    printf("alarm application starting\n");

    pid = fork();
    switch(pid) {
    case -1:
      /* Failure */
      perror("fork failed");
      exit(1);
    case 0:
      /* child */
        sleep(5);
        kill(getppid(), SIGALRM);
        exit(0);
    }
    printf("waiting for alarm to go off\n");
    (void) signal(SIGALRM, ding);

    pause();
    if (alarm_fired)
        printf("Ding!\n");

    printf("done\n");
    exit(0);
}
```

**一个健壮的信号接口**`sigaction`，定义如下：

```c
#include <signal.h>

int sigaction(int sig, const struct sigaction *act, struct sigaction *oact);
```

`sig`参数指出要捕获的信号类型，`act`参数指定新的信号处理方式，`oact`参数输出先前信号的处理方式（如果不为NULL的话）。

 `struct sigaction`结构体介绍

```c
struct sigaction {
    void (*sa_handler)(int);
    void (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t sa_mask;
    int sa_flags;
    void (*sa_restorer)(void);
}
```

`sa_handler`此参数和`signal()`的参数`func`相同，代表新的信号处理函数。

`sa_mask`用来设置在处理该信号时暂时将`sa_mask` 指定的信号集搁置。

`sa_flags`用来设置信号处理的其他相关操作，下列的数值可用： 

- `SA_RESETHAND`：当调用信号处理函数时，将信号的处理函数重置为缺省值SIG_DFL。
- `SA_RESTART`：如果信号中断了进程的某个系统调用，则系统自动启动该系统调用。
- `SA_NODEFER` ：一般情况下， 当信号处理函数运行时，内核将阻塞该给定信号。但是如果设置了` SA_NODEFER`标记， 那么在该信号处理函数运行时，内核将不会阻塞该信号。

代码示例：

```c
#include <unistd.h>

void ouch(int sig)
{
    printf("OUCH! - I got signal %d\n", sig);
}

int main()
{
    struct sigaction act;

    act.sa_handler = ouch;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGINT, &act, 0);

  while(1) {
    printf("Hello World!\n");
    sleep(1);
  }
}
```

### 信号集

每个进程都有一个信号屏蔽字，它规定了当前要阻塞递送到该进程的信号集。对于每种可能的信号，该屏蔽字中都有一bit位与之对应。信号数可能会超过一个整型数所包含的二进制位数，因此POSIX.1定义了一个新数据类型sigset_t，它包括一个信号集。`signal.h`中定义了用来处理信号集的函数。

```c
#include <signal.h>

int sigempytset(sigset_t *set);  //初始化set指向的信号集，清除其中的所有信号
int sigfillset(sigset_t *set);   //初始化set指向的信号集，其中包含所有信号
int sigaddset(sigset_t *set, int signo);	//向set中添加一个signo信号
int sigdelset(sigset_t *set, int signo);	//向set中删除一个signo信号
```

```c
int sigismember(sigset_t *set, int signo);	
```

`sigismember`函数判断信号`signo`是否在信号集合`set`中，如果是就返回1，无返回0，调用失败返回-1。

```c
int sigprocmask(int flag,const sigset_t *set,sigset_t *oldset);
```

`sigprocmask`函数用来设置或检查进程的信号屏蔽字，若`set`非空指针，则按照`flag`方式进行设置新的信号屏蔽字，而原先的信号屏蔽字会保存在`oldset`中。

如果参数`set`是空指针，`flag`的值就没有意义了，这样的调用的唯一目的就是把当前信号屏蔽值的值保存到`oldset`中。

`flag`的取值：

|    参数     |               说明                |
| :---------: | :-------------------------------: |
|  SIG_BLOCK  | 在信号屏蔽字中添加参数set中的信号 |
| SIG_UNBLOCK | 从信号屏蔽字中删除参数set中的信号 |
| SIG_SETMASK | 把信号屏蔽字设置为参数set中的信号 |

```c
int sigpending(sigset_t *set);
```

`sigpending`函数会将被阻塞的信号中停留在待处理状态的一组信号写到参数`set`中。

```c
int sigsuspend(const sigset_t *mask);
```

`sigsuspend`函数会将进程的屏蔽字替换为参数`mask`给出的信号集，然后挂起执行。程序将在信号处理函数执行完毕后继续执行。



## 参考资料

《Linux程序设计第4版》