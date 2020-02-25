![](E:\Git\Study-Note\linux\linux应用学习笔记\socket\tmp\Socket.jpg)

# Linux应用编程之Socket

## Socket介绍

Socket也称套接字，是一种通信机制。

Socket就是我们建立网络连接的工具。

Socket明确的将客户和服务器区分开。

Socket机制可以实现将多个客户连接到服务器。

## Socket连接

Socket连接过程：

![](E:\Git\Study-Note\linux\linux应用学习笔记\socket\tmp\Socket流程.jpg)

### 服务器端

1. 服务器应用程序用系统调用`socket`来创建一个套接字

2. 用系统调用`bind`给套接字命名

3. 服务器进程开始等待客户进行连接（**系统调用`listen`的作用是：创建一个队列并将其用于存放来自客户的进入连接**）

4. 服务器通过系统调用`accept`来接受客户的连接（**服务器调用`accept`时，它会创建一个与原有的命名`socket`不同的新`socket`。这个新`socket`只用于与这个特定的客户端进行通信**）

   ```c
   #include <sys/types.h>
   #include <sys/socket.h>
   #include <stdio.h>
   #include <sys/un.h>
   #include <unistd.h>
   #include <stdlib.h>
   
   int main()
   {
       int server_sockfd, client_sockfd;
       int server_len, client_len;
       struct sockaddr_un server_address;
       struct sockaddr_un client_address;
   
   /*  1.Remove any old socket and create an unnamed socket for the server.  */
   
       unlink("server_socket");
       server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
   
   /*  2.Name the socket.  */
   
       server_address.sun_family = AF_UNIX;
       strcpy(server_address.sun_path, "server_socket");
       server_len = sizeof(server_address);
       bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
   
   /*  3.Create a connection queue and wait for clients.  */
   
       listen(server_sockfd, 5);
       while(1) 
       {
           char ch;
           printf("server waiting\n");
   
   /*  4.Accept a connection.  */
   
           client_len = sizeof(client_address);
         	client_sockfd = accept(server_sockfd, 
               (struct sockaddr *)&client_address, &client_len);
   
   /*  5.We can now read/write to client on client_sockfd.  */
   
           read(client_sockfd, &ch, 1);
           ch++;
           write(client_sockfd, &ch, 1);
           close(client_sockfd);
       }
   }
   ```

   

### 客户端

1. 客户调用`socket`创建一个未命名的`sokcet`
2.  将服务器的命名套接字作为一个地址来调用`connect`与服务器建立连接

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int sockfd;
    int len;
    struct sockaddr_un address;
    int result;
    char ch = 'A';

/*  1.Create a socket for the client.  */

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

/*  2.Name the socket, as agreed with the server.  */

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, "server_socket");
    len = sizeof(address);

/*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1) {
        perror("oops: client1");
        exit(1);
    }

/*  3.We can now read/write via sockfd.  */

    write(sockfd, &ch, 1);
    read(sockfd, &ch, 1);
    printf("char from server = %c\n", ch);
    close(sockfd);
    exit(0);
}
```

### Socket属性

#### Socket的域

域指定Socket通信中使用的网络介质。

|      域      |               说明               |
| :----------: | :------------------------------: |
|   `AF_UNIX`    |   UNIX域协议（文件系统套接字）   |
|   `AF_INET`    | ARPA因特网协议（UNIX网络套接字） |
|    `AF_ISO`    |           ISO标准协议            |
|    `AF_NS`     |    施乐（Xerox）网络系统协议     |
|    `AF_IPX`    |          Novell IPX协议          |
| `AF_APPLETALK` |          Appletalk DDS           |

#### Socket类型

一个Socket域可能有不同的通信方式，而每种通信方式又有不同的特性。

因特网协议提供了两种通讯协议：流（stream）和数据报（datagram）。

- 流套接字

  流套接字（在某些方面类似于标准的输入/输出流）提供一个有序、可靠、双向字节流的连接。

  由类型`SOCK_STREAM`指定，它们是在`AF_INET`域中通过**TCP/IP**连接实现的。

- 数据报套接字

  与流套接字相反，数据报套接字不建立和维持一个连接。它对可以发送的数据报的长度有限制。数据报作为一个单独的网络消息被传输，它可能丢失、复制或乱序到达。

  由类型`SOCK_DGRAM`指定，它们是在`AF_INET`域中通过UDP/IP连接实现的，提供的是一种无序的不可靠服务。

#### Socket协议

只要底层的传输机制允许不止一个协议来提供要求的套接字类型，我们就可以为套接字选择一个特定的协议。常用的UNIX网络套接字和文件系统套接字不需要选择一个特定的协议，使用默认值即可。

### 创建Socket

`socket`系统调用创建一个套接字并返回一个描述符，该描述符可用来访问该`socket`。

函数原型：

```c
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```

参数含义：

`domain`：指定协议族也就是socket域，可选择的参数见上面介绍域的表格。
`type`：指定socket通信类型，`SOCK_STREAM`或`SOCK_DGRAM`。
`protocol`：指定使用的协议，0表示使用默认协议。

### Socket地址

每个Socket域都有其自己的地址格式。

`AF_UNIX`域Socket地址由`sockaddr_un`描述，定义在`sys/un.h`中。

``` C
struct sockaddr_un{
	sa_family_t sun_family;		/* AF_UNIX */
    char		sun_path[];		/* pathname */
};
```

`AF_UNIX`域中Socket地址由`sun_path`所指定，长度有限制（Linux规定是108个字符）。

`AF_INET`域中，Socket地址由`sockaddr_in`描述，定义在netinet/in.h中。

```C
struct sockaddr_un{
	short int	 		sin_family;		/* AF_INET */
   	unsigned short int 	sin_port;		/* Port number */
    struct in_addr		sin_addr;		/* Internet address */
};
```

IP地址结构`in_addr`定义：

```c
struct in_addr{
	unsigned long int s_addr;
}
```

### Socket命名

要想让通过`socket`调用创建的套接字可以被其他进程使用，服务器就必须给套接字命名。

`AF_UNIX`套接字就会关联到一个文件系统的路径名。`AF_INET`套接字就会关联到一个IP端口号。

```c
#include <sys/socket.h>

int bind(int socket, const struct sockaddr *address, size_t address_len);
```

`bind`系统调用把参数`address`中的地址分配给与文件描述符`socket`关联的未命名套接字。地址结构的长度由参数`address_len`传递。

`bind`调用时需要将一个特定的地址结构指针转化为指向通用地址类型（`structsockaddr *`）。

### 创建Socket队列

为了能够在套接字上接受进入的连接，服务器程序必须创建一个队列来保存未处理的请求。使用`listen`系统调用来完成这一工作。

```c
#include <sys/socket.h>

int listen(int socket,int backlog);
```

`backlog`为可以容纳的未处理连接的最大数目。超过这个数字后，往后的连接会被拒绝。常用的值是5。

### 接受连接

服务器创建并命名套接字之后，可以通过`accept`系统调用来等待客户建立对该套接字的连接。

```c
#include <sys/socket.h>

int accept(int socket, struct sockaddr *address, size_t *address_len);
```

`accept`系统调用只有当有客户程序试图连接到`socket`参数指定的套接字上时才返回。这里客户是指在套接字队列中排第一个的未处理连接。`accept`函数将创建一个新套接字来与客户进行通讯，并且返回新套接字的描述符。

`address`参数应放入连接客户的地址，如果不关心客户地址，也可以将address参数指定为空指针。

`address_len`参数指定客户地址结构体的长度。当这个调用返回时，`address_len`将被设置为连接客户地址结构的实际长度。

### 请求连接

客户程序通过在一个未命名套接字和服务器监听套接字之间建立连接的方法来连接到服务器。是通过`connect`调用来完成这一工作。

```c
#include <sys/socket.h>

int connect(int socket, const struct sockaddr *address, size_t address_len);
```

`socket`参数指定的套接字将连接到参数`address`指定的服务器套接字。`address_len`参数指定`address`指向的结构的长度。

### 关闭Socket

可以调用`close`函数来终止服务器和客户上的套接字连接。

```c
#include <unistd.h>

int close(int fd);
```

应该在连接的两端都关闭套接字。

### 主机字节序与网络字节序

不同的计算机使用不同的字节序来表示整数（数据的大小端模式）。为了使不同类型的计算机通过网络传输的多字节整数达成一致，需要定义一个网络字节序（网络字节序为大端模式）。它们通过定义在头文件`netinet/in.h`中的函数来完成这一工作。

```c
unsigned short int htons(uint16_t hostshort);
unsigned long int htonl(uint32_t hostlong);
unsigned short int ntohs(uint16_t netshort);
unsigned long int ntohl(uint32_t netlong);
```

这些函数将16位和32位整数在主机字节序和标准网络字节序之间进行转换。函数名是与之对应的转换操作的简写形式。例如“host to network, short”（htons，短整数从主机字节序到网络字节序的转换）。

### Socket通信例程

```c
/*
 * 服务器端代码
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

const int MAX_LINE = 2048;
const int PORT = 6000;
const int LISTENQ = 5;

int main(int argc , char **argv)
{
	/*声明服务器地址和客户链接地址*/
	struct sockaddr_in servaddr , cliaddr;

	/*声明服务器监听套接字和客户端链接套接字*/
	int listenfd , connfd;
	pid_t childpid;

	/*声明缓冲区*/
	char buf[MAX_LINE];

	socklen_t clilen;

	/*(1) 初始化监听套接字listenfd*/
	if((listenfd = socket(AF_INET , SOCK_STREAM , 0)) < 0)
	{
		perror("socket error");
		exit(1);
	}//if

	/*(2) 设置服务器sockaddr_in结构*/
	bzero(&servaddr , sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //表明可接受任意IP地址
	servaddr.sin_port = htons(PORT);

	/*(3) 绑定套接字和端口*/
	if(bind(listenfd , (struct sockaddr*)&servaddr , sizeof(servaddr)) < 0)
	{
		perror("bind error");
		exit(1);
	}//if

	/*(4) 监听客户请求*/
	if(listen(listenfd , LISTENQ) < 0)
	{
		perror("listen error");
		exit(1);
	}//if

	/*(5) 接受客户请求*/
	for( ; ; )
	{
		clilen = sizeof(cliaddr);
		if((connfd = accept(listenfd , (struct sockaddr *)&cliaddr , &clilen)) < 0 )
		{
			perror("accept error");
			exit(1);
		}//if

		//新建子进程单独处理链接
		if((childpid = fork()) == 0) 
		{
			close(listenfd);
			//str_echo
			ssize_t n;
			char buff[MAX_LINE];
			while((n = read(connfd , buff , MAX_LINE)) > 0)
			{
				write(connfd , buff , n);
			}
			exit(0);
		}//if
		close(connfd);
	}//for
	
	/*(6) 关闭监听套接字*/
	close(listenfd);
}
```

```c
/*
 * 客户端代码
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

const int MAX_LINE = 2048;
const int PORT = 6000;

/*readline函数实现*/
ssize_t readline(int fd, char *vptr, size_t maxlen)
{
	ssize_t	n, rc;
	char	c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++) {
		if ( (rc = read(fd, &c,1)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;	/* newline is stored, like fgets() */
		} else if (rc == 0) {
			*ptr = 0;
			return(n - 1);	/* EOF, n - 1 bytes were read */
		} else
			return(-1);		/* error, errno set by read() */
	}

	*ptr = 0;	/* null terminate like fgets() */
	return(n);
}


int main(int argc , char ** argv)
{
	/*声明套接字和链接服务器地址*/
	int sockfd;
	struct sockaddr_in servaddr;

	/*判断是否为合法输入*/
	if(argc != 2)
	{
		perror("usage:tcpcli <IPaddress>");
		exit(1);
	}//if

	/*(1) 创建套接字*/
	if((sockfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
	{
		perror("socket error");
		exit(1);
	}//if

	/*(2) 设置链接服务器地址结构*/
	bzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	if(inet_aton(argv[1] , &servaddr.sin_addr) < 0)
	{
		printf("inet_aton error for %s\n",argv[1]);
		exit(1);
	}//if

	/*(3) 发送链接服务器请求*/
	if( connect(sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr)) < 0)
	{
		perror("connect error");
		exit(1);
	}//if

	/*(4) 消息处理*/
	char sendline[MAX_LINE] , recvline[MAX_LINE];
	while(fgets(sendline , MAX_LINE , stdin) != NULL)	
	{
		write(sockfd , sendline , strlen(sendline));

		if(readline(sockfd , recvline , MAX_LINE) == 0)
		{
			perror("server terminated prematurely");
			exit(1);
		}//if

		if(fputs(recvline , stdout) == EOF)
		{
			perror("fputs error");
			exit(1);
		}//if
	}//while

	/*(5) 关闭套接字*/
	close(sockfd);
}
```

## 网络信息

如果给定一个计算机的名字，那就可以通过调用解析地址的主机数据库函数来确定它的IP地址。这些函数是通过查询网络配置文件来完成这一工作，如`/etc/hosts`文件或网络信息服务。

主机库函数在接口头文件`netdb.h`中申明，如下：

```c
#include <netdb.h>

struct host *gethostbyaddr(const void *addr, size_t len, int type);
struct host *gethostbyname(const char *name);
```

这些函数返回的结构至少会包含以下几个成员：

```c
struct hostent{
	char *h_name;		/* 主机名称 */
	char **h_aliases;	/* 别称的列表 */
	int h_addrtype;		/* 地址类型 */
	int h_length;		/* 地址字节长度 */
	char **h_addr_list;	/* 地址的列表 */
}
```

如果没有与我们查询的主机或地址相关的数据项，这些信息函数将会返回一个空指针；

类似的，与服务及相关联端口号有关信息也可以通过一些服务信息函数来获取。如下：

```c
#include <netdb.h>

struct servent *getservbyname(const char *name, const char *proto);
struct servent *getservbyport(int port, const char *proto);
```

`proto`参数指定用于连接该服务的协议，取值为`tcp`或`udp`。

结构`servent`至少包含以下几个成员：

```c
struct servent{
    char *s_name;		/* 服务名称 */
    char **s_aliases;	/* 别称列表 */
    int s_port;			/* IP端口 */
    char *s_proto;		/* 服务协议 */
}
```

相应的IP地址转换函数：

```c
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

char *inet_ntoa(struct in_addr in);
int inet_aton(const char *string, struct in_addr *addr);
```

函数名是与之对应的转换操作的简写形式。例如“network to ascii ”（ntoa，网络字节序地址转换为acsii码地址）。

获取主机名称函数：

```c
#include <unistd.h>

int gethostname(char *name, int namelength);
```

获取的主机名称会被写入参数`name`，而参数`namelength`指定了`name`的长度。成功返回0，失败返回-1。

### Socket选项

可以通过`setsockopt`函数设置选项来控制套接字的行为，它的定义如下：

```c
#include <sys/socket.h>

int setsockopt(int socket, int level, int option_name,
              const void *option_value, size_t option_len);
```

`socket`：标识一个套接口的描述字。

`level`：选项定义的层次；支持`SOL_SOCKET`、`IPPROTO_TCP`、`IPPROTO_IP`和`IPPROTO_IPV6`。

`option_name`：需设置的选项。

`option_value`：指针，指向存放选项待设置的新值的缓冲区。

`option_len`：`option_value`缓冲区长度。

## 多客户

在上面的Socket通信例程的服务器端代码就是一个利用多进程实现的多客户连接服务器。除了多进程外，还有一种方法实现多客户

### `select`系统调用

 在网络程序中，一个进程同时处理多个文件描述符是很常见的情况。`select()`系统调用可以使进程检测同时等待的多个I/O设备，当没有设备准备好时，`select()`阻塞，其中任一设备准备好时，`select()`就返回。

`select()`的调用形式为：

```c
#include <sys/select.h>
#include <sys/time.h>

int select(int maxfd, fd_set *readfds, fd_set *writefds, fe_set *exceptfds, const struct timeval *timeout);
```

参数含义：
`maxfd`：使用时记住这个值为待检测的最大文件描述符加1即可；--------》但是有坑啊！
`readfds`：指定了被读监控的文件描述符集；Those listed in readfds will be watched to see if  characters  become  available  for  reading；
`writefds`：指定了被写监控的文件描述符集；those in writefds will be watched to see if a write will not block
`exceptfds`：指定了被例外条件监控的文件描述符集；
`timeout`：很常用的一个`timeeval`结构，起了定时器的作用：到了指定的时间，无论是否有设备准备好，都返回调用。timeval的结构定义如下：

```c
struct timeval{
        long tv_sec; //表示几秒
        long tv_usec; //表示几微妙
}    
```

 对以上的调用的函数各参数介绍后，说哈他的大致用法：简单粗暴，就是把需要读，写或者其他监控条件的`fd`加入相应的`fd`集用于select进行监控管理，并对这些`fd`进行轮循监控，当有相应的监控条件被触发后，`select`就会返回，接下来就是进行相应的操作；用法中说到加入，监控的字眼，于是就得说说`select`必须了解的几个宏：

```c
void FD_SET(int fd, fd_set *fdset);
```

宏`FD_SET`设置文件描述符集fdset中对应于文件描述符`fd`的位(设置为1)，即将`fd`加入`fdset`集的操作；

```c
void FD_CLR(int fd, fd_set *fdset);
```

宏`FD_CLR`清除文件描述符集`fdset中`对应于文件描述符`fd`的位（设置为0)，与上相反操作；

```c
void FD_ISSET(int fd, fd_set *fdset);
```

在调用`select`后使用`FD_ISSET`来检测文件描述符集`fdset`中对应于文件描述符`fd`的位是否被设置,如果置位，就进行相应操作；

```c
void FD_ZERO(fd_set *fdset);
```

宏`FD_ZERO`清除文件描述符集`fdset`中的所有位(既把所有位都设置为0)。

```c
// About "fd_set"
typedef __kernel_fd_set fd_set;

typedef struct{
    unsigned long fds_bits [__FDSET_LONGS];
} __kernel_fd_set;

#define __FDSET_LONGS   (__FD_SETSIZE/__NFDBITS)
#define __FD_SETSIZE    1024
#define __NFDBITS       (8 * sizeof(unsigned long))
```

**https://www.cnblogs.com/lp1129/articles/2705556.html**  宏相关深入理解；

操作系统通过宏FD_SETSIZE来声明在一个进程中select所能操作的文件描述符的最大数目。通常是1024，既定义FD_SETSIZE为1024，一个整数占4个字节，既32位，那么就是用包含32个元素的整数数组来表示文件描述符集（这是内核代码实现的思路，对理解内核select底层实现很有帮助）；

`selest`系统调用示例：

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>


#define PORT 8888
#define MAX_LINE 2048
#define LISTENQ 20


int main(int argc , char **argv)
{
	int i, maxi, maxfd, listenfd, connfd, sockfd;

	int nready , client[FD_SETSIZE];
	
	ssize_t n, ret;
		
	fd_set rset , allset;

	char buf[MAX_LINE];

	socklen_t clilen;

	struct sockaddr_in servaddr , cliaddr;

	/*(1) 得到监听描述符*/
	listenfd = socket(AF_INET , SOCK_STREAM , 0);

	/*(2) 绑定套接字*/
	bzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	bind(listenfd , (struct sockaddr *)&servaddr , sizeof(servaddr));

	/*(3) 监听*/
	listen(listenfd , LISTENQ);

	/*(4) 设置select*/
	maxfd = listenfd;
	maxi = -1;
	for(i=0 ; i<FD_SETSIZE ; ++i)
	{
		client[i] = -1;
	}//for
	FD_ZERO(&allset);
	FD_SET(listenfd , &allset);

	/*(5) 进入服务器接收请求死循环*/
	while(1)
	{
		rset = allset;
		nready = select(maxfd+1 , &rset , NULL , NULL , NULL);
		
		if(FD_ISSET(listenfd , &rset))
		{
			/*接收客户端的请求*/
			clilen = sizeof(cliaddr);

			printf("\naccpet connection~\n");

			if((connfd = accept(listenfd , (struct sockaddr *)&cliaddr , &clilen)) < 0)
			{
				perror("accept error.\n");
				exit(1);
			}//if		

			printf("accpet a new client: %s:%d\n", inet_ntoa(cliaddr.sin_addr) , cliaddr.sin_port);

			/*将客户链接套接字描述符添加到数组*/
			for(i=0 ; i<FD_SETSIZE ; ++i)
			{
				if(client[i] < 0)
				{
					client[i] = connfd;
					break;
				}//if
			}//for

			if(FD_SETSIZE == i)
			{
				perror("too many connection.\n");
				exit(1);
			}//if

			FD_SET(connfd , &allset);
			if(connfd > maxfd)
				maxfd = connfd;
			if(i > maxi)
				maxi = i;

			if(--nready < 0)
				continue;
		}//if

		for(i=0; i<=maxi ; ++i)
		{
			if((sockfd = client[i]) < 0)
				continue;
			if(FD_ISSET(sockfd , &rset))
			{
				/*处理客户请求*/
				printf("\nreading the socket~~~ \n");
				
				bzero(buf , MAX_LINE);
				if((n = read(sockfd , buf , MAX_LINE)) <= 0)
				{
					close(sockfd);
					FD_CLR(sockfd , &allset);
					client[i] = -1;
				}//if
				else
				{
					printf("clint[%d] send message: %s\n", i , buf);
					if((ret = write(sockfd , buf , n)) != n)	
					{
						printf("error writing to the sockfd!\n");
						break;
					}//if
				}//else
				if(--nready <= 0)
					break;
			}//if
		}//for
	}//while
}
```

## 数据报

在AF_INT域的的数据报由UDP完成，它的代码实现与TCP/IP有些许差异，在UDP连接中`sendto`和`recvform`代替了`read`和`write`。

```c
int  sendto( SOCKET sockfd, const char FAR* buf, int len, int flags,const struct sockaddr FAR* to, int tolen);
```

`sockfd`：一个标识套接口的描述字。
`buf`：包含待发送数据的缓冲区。
`len`：`buf`缓冲区中数据的长度。
`flags`：调用方式标志位。
`to`：（可选）指针，指向目的套接口的地址。
`tolen`：`to`所指地址的长度。

```c
int recvfrom( SOCKET sockfd, char FAR* buf, int len, int flags, struct sockaddr FAR* from, int FAR* fromlen);
```

`sockfd`：标识一个已连接套接口的描述字。
`buf`：接收数据缓冲区。
`len`：缓冲区长度。
`flags`：调用操作方式。
`from`：（可选）指针，指向装有源地址的缓冲区。
`fromlen`：（可选）指针，指向from缓冲区长度值。

如下示例：

```c
/* 客户端 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>


const int SERV_PORT = 6000;

const int MAXLINE = 2048;
void dg_cli(FILE *fp , int sockfd , const struct sockaddr *pservaddr , socklen_t servlen)
{
	int n;
	char sendline[MAXLINE] , recvline[MAXLINE+1];
	
	while(fgets(sendline , MAXLINE , fp) != NULL)
	{
		if(sendto(sockfd , sendline , strlen(sendline) , 0 , pservaddr ,  servlen) < 0)
		{
			perror("sendto error");
			exit(1);
		}//if

		if( ( n = recvfrom(sockfd , recvline , MAXLINE , 0 , NULL , NULL)) < 0)
		{
			perror("recvfrom error");
			exit(1);
		}//if

		recvline[n] = '\0';
		fputs(recvline , stdout);

	}//while
}

int main(int argc , char **argv)
{
	int sockfd , t;
	struct sockaddr_in servaddr;
	if(argc != 2)
	{
		perror("usage: udpcli <IPaddress>");
		exit(1);
	}//if

	bzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	if((t = inet_aton(argv[1], &servaddr.sin_addr)) <= 0)
	{
		perror("inet_aton error");
		exit(1);
	}//if
	
	if((sockfd = socket(AF_INET , SOCK_DGRAM , 0)) < 0)
	{
		perror("socket error");
		exit(1);
	}//if

	dg_cli(stdin , sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr));
	exit(0);
} 
```

```c
/* 服务端 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/ip.h>

const int SERV_PORT = 6000;
const int MAXLINE = 2048;

void dg_echo(int sockfd , struct sockaddr *pcliaddr , socklen_t clilen)
{
	int n;
	socklen_t len;
	char mesg[MAXLINE];

	for( ; ;)
	{
		len = clilen;
		if((n = recvfrom(sockfd , mesg , MAXLINE , 0 , pcliaddr , &len))<0)
		{
			perror("recvfrom error");
			exit(1);
		}//if

		if((n = sendto(sockfd , mesg , n , 0 , pcliaddr , len)) < 0)
		{
			perror("sendto error");
			exit(1);
		}//if
	}//for

}

int main(int argc , char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr , cliaddr;

	bzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	if((sockfd = socket(AF_INET , SOCK_DGRAM , 0)) < 0)
	{
		perror("socket error");
		exit(1);
	}//if

	if(bind(sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr)))
	{
		perror("bind error");
		exit(1);
	}//if

	dg_echo(sockfd , (struct sockaddr *)&cliaddr , sizeof(cliaddr));	
}
```

