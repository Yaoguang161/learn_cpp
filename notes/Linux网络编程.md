# 1.七层协议模型
* `OSI`七层模型: 物理层,数据链路层,网络层,传输层,会话层,表示层,应用层.
* `TCP/IP`层概念模型: 数据链路层,网络层,传输层,应用层.
* 对应协议:
  * 应用层: `http,ftp,nfs,ssh,telnet...`
  * 传输层: `TCP,UDP`
  * 网络层: `IP,ICMP,IGMP`
  * 链路层: 以太网帧协议,`ARP`

# 2.两个模型

* `c/s`模型: `lient-server`
* `b/s`模型: `browser-server  `


|      |                C/S                |              B/S              |
| :--: | :-------------------------------: | :---------------------------: |
| 优点 | 缓存大量数据,协议选择灵活,速度快, |  安全性,跨平台,开发工作量小   |
| 缺点 |    安全性,跨平台,开发工作量小     | 不能缓存大量数据,严格遵守http |


# 3.网络传输流程
* 数据没有封装之前,是不能在网络中传递
* 数据-应用层-传输层-网络层-链路层


# 4.以太网帧协议
`ARP`协议: 根据`IP`地址获取`mac`地址
以太网帧协议: 根据`mac`地址,完成数据包传输


# 5.IP协议
* 版本: `IPv4`,`IPv6` --4位
* `TTL`: `time to live` . 设置数据包在路由节点中的跳转上线.每经过一个路由节点,该值-1,减为0,的路由,有义务将该数据包丢弃.
* 源IP: 32位,  ---4字节
* 目的IP: 32位, ----4字节

`ip`地址: 可以在网络环境中, 唯一标识一台主机

端口号: 可以在网络的一台主机上,唯一标识一个进程

`ip`地址+端口号: 可以在网络环境中,唯一标识一个进程.

# 6.UPD协议
* 16位: 源端口号. 2^16 = 65536;
* 16位: 目的端口号


# 7.IP协议
* 16位: 源端口号. 2^16 = 65536
* 16位: 目的端口号
* 32序号
* 32确认序号
* 6个标志位
* 16个窗口大小 2^16 = 65536


# 8.网络套接字 SOCKET
* 一个文件描述符指向一个套接字(该套接字内部是由内核借助两个缓冲区实现)
* 在通信过程中,套接字一定是成对出现
  

# 9.网络字节序
1. 小端法: (pc地址存储) 高位存高地址,低位存低地址
2. 大端法: (网络存储) 高位存低地址, 低位存高地址.

计算机采用小端法, 网络采用大端法, 所以数据传入网络需要转换.



```c++
#include <arpa/inet.h>

uint32_t htonl(uint32_t hostlong) //本地转网络(ip)     32位
uint16_t htons(uint16_t hostshart) //本地转网络(port)  16位
uint32_t ntohl(uint32_t netlong)   //网络转本地(IP)
uint16_t ntohl(uint16_t netshort)  //网络转本地(port)
```

`192.168.1.11 --> string -->atoi --> int --> htonl -->网络字节序`  很麻烦, 所以封装了两个函数

```c++
#include <arpa/inet.h>

//点分十进制转换成网络字节序
int inet_pton(int af , const char * src, void *dst)  
//网络字节序转换成点分十进制
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
/*
支持IPv4 和 IPv6
可重入函数

其中inet_pton和inet_ntop不仅可以转换IPv4的in_addr, 还可以转换IPv6的in6_add
因此函数接口是void *addrptr.

*/
```



# 10.IP地址转换函数

```c++
//本地字节序(string IP) -----> 网络字节序
int inet_pton(int af, const char *src, void *dst);
	af: AF_INET, AF_INET6
	src: 传入, IP地址(点分十进制)
	dst: 传出, 转换后的 网络字节序的 IP地址
 返回值:
	成功: 1
    异常: 0,  说明src指向的不是一个有效的IP地址(网络中没有这个IP地址)
    失败: -1, IP地址格式错误   
    
 //网络字节序 ------> 本地字节序(string IP)
 const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
	af: AF_INET, AF_INET6
	src: 传入, 网络字节序IP地址
	dst: 传出, 转换后的 本地字节序的IP地址
    size: dst 的大小.
 返回值:
	成功: dst
    失败: NULL   
```



# 11. Sockaddr地址结构

```c++
struct sockaddr_in{
    sa_family_t    sin_family;
    in_port_t      sin_port;
    struct in_addr sin_addr; 
}

struct in_addr{
    uint32_t  s_addr;
}

//代码开始
struct sockaddr_in addr;
addr.sin_family = AF_INET/AF_INET6
addr.sin_port = htons(9527); //本地字节序转网络字节序

int dst;
inet_pton(AF_INET,"192.157.22.45", (void *)&dst);
addr.sin_addr.s_addr = dst;

或
//重点
addr.sin_addr.s_addr = htonl(INADDR_ANY);  //取出系统中有效的任意IP地址,二进制类型
bind(fd,(struct sockaddr *)&addr, size)

```



首先将整个结构体清零, 然后设置地址类型为`AF_INET` 网络地址为`INADDR_ANY`, 这个宏表示本地的任意IP地址,因为服务器可能有多个网卡, 每个网卡都可能绑定多个`IP`地址, 这样设置可以在所有的IP地址上监听, 知道与某个客户端建立了连接时才确定下来到底用哪个`IP`地址.

# 12. Socket模型创建流程图

![image-20230831233245799](C:\Users\Asaki\AppData\Roaming\Typora\typora-user-images\image-20230831233245799.png)

服务器通信一共有三个套接字,不是三对. 一个监听 两个通信



`bind`是往`socket` 绑定 `IP` 和 端口号 `port`

` listen` 设置同时监听上限数, 并非监听.

`accpet` 阻塞监听客户端连接, 这才是监听



# 13. Socket函数

```c++
Socket函数:

#include <sys/socket.h>
int socket(int domain, int type, int protocol);  //创建一个套接字

	domain: AF_INET, AF_INET6, AF_UNIX
    type:   SOCK_STREAM, SOCK_DGRAM   //流式和报式协议, 流式是TCP,报式是UDP
    protocol: 0;

	返回值: 
		   成功: 新套接字对应的文件描述符
           失败: -1  errno
               
```



# 14. bind函数

```c++
#include <arpa/inet.h>
int bind(int sockfd, const struct sockaddr * addr, socklen_t addrlen); //给socket绑定一个地址结构(IP + port)
    sockfd: socket 函数返回值
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr: 传入参数(struct sockaddr *)&addr
    addrlen: sizeof(addr) 地址结构的大小
             
	返回值:
		成功: 0
    	失败: -1 errno错误号
                
```



# 15. listen函数

```c++
int listen(int sockfd, int backlog);  //设置同时与服务器建立连接的上限数.(同时进行三次握手的客户端数量)

	sockfd: socket 函数返回值
    backlog: 上限数值. 最大值 128
    
	返回值:
			成功: 0
            失败: -1 errno                
```



# 16. accept函数

```c++
int accept(int sockfd,  struct sockaddr * addr, socklen_t *addrlen); //阻塞等待客户端建立连接,成功的话, 返回一个与客户端成功连接的socket文件描述符

	sockfd: socket 函数返回值
    addr: 传出参数. 成功与服务器建立连接的那个客户端的地址结构(IP + port)
        
           socklen_t clit_addr_len = sizeof(addr);
    addrlen: 传入传出.  &clit_addr_len
             入: addr的大小 . 出: 客户端addr实际大小.
    返回值:
			成功: 能与服务器进行数据通信的socket 对应的文件描述符.
            失败: -1 ,errno
```

# 17. connect() 函数

```c++
int connect(int socket, const struct sockadr *addr, socklen_t addrlen); //使用现有的socket与服务器建立连接
	sockfd: socket 函数返回值
        	struct sockaddr_in srv_addr;   //服务其地址结构
			srv_addr.sin_family = AF_INET;
			srv_addr.sin_port = 9527;
			srv_addr.sin_addr.s_addr      //跟服务器bind时设定的port完全一致
            inet_pton(AF_INET, "服务器的IP地址", &srv_addr.sin_addr.s_addr);
    addr: 传入参数. 服务器的地址结构
        	inet_pton();
    addrlen: 服务器的地址结构的大小.
    返回值:
		成功: 0
        失败: -1 errno
    如果不使用bind绑定客户端地址结构, 采用 "隐式绑定",由系统实现                
```



# 18.TCP通信流程分析

Server:

	1. `socket()` 创建 `socket`
	1. `bind()`  绑定服务器地址结构
	1. `listen()` 设置监听上限
	1. `accept()` 阻塞监听客户端连接
	1. `read(fd)`: 读`socket`获取客户端数据
	1. 小 -- 大写   `toupper()`
	1. `write(fd)`
	1. `close()`

Client:

	1. `socket()` 创建socket
	1. `connect()`    与服务器建立连接
	1. `write()` 写数据到socket
	1. `read()` 读转换后的数据
	1. 显式读取结果
	1. `close()`



# 服务端开发流程

1. 创建`socket`,返回一个文件描述符`lfd---socket()`
   --该文件描述符用于监听客服端连接
2. 将`lfd`和`IP PORT`进行绑定  ---`bind()`
3. 将`lfd`由主动变为被动监听 --`listen()`
4. 接受一个新的连接,得到一个文件描述符`cfd--accept()`
   ---该文件描述符用于和客服端进行通信的
5. 
``` C++
while(1){
  接收数据---read或者recv
  发送数据---write或者send
}
```
6. 关闭文件描述符---`close(lfd) close(cfd)`

# 客户端开发流程:
1. 创建`socket`,返回一个文件描述符`cfd`---------`socket()`----该文件描述符是用于和服务端通信
2. 连接服务端---------`connect()`
3. 
``` C++
while(1){
  //发送数据 ----write或者send
  //接收数据 -----read或者recv
}

```
4. `close(cfd)`

一些细节:  
1. 调用`accept`函数不是说新建一个连接,而是从以连接队列中取出一个可用连接



# 粘包的概念
* 粘包: 多次数据发送,首位相连,接收端接收的时候不能正确区分第一次发送多少,第二次发送多少.

* 粘包问题分析和解决??
1. 报头+数据  (最主要的解决方法)
如4位的数据长度+数据 ==========>00101234567890
其中0010表示数据长度,1234567890表示10个字节长度的数据.
另外,发送端和接收端可以协商更为复杂的报文结构,这个报文结构就相当于双方约定的一个协议.
2. 添加结尾标记(不建议用,每个字符都要判断一下)
3. 发送定长数据  
    如: 发送20个字节的定长数据.  
    0010xiaozhaobeijing025


# 高并发服务器(多进程)
* 思想: 使用多进程:让父进程监听接收新的连接,子进程处理新的连接(接收和发送数据);父进程负责回收子进程.    
* 处理流程:   
  1. 创建`socket`,得到一个监听的文件描述符`lfd `=====`socket()`
  2. 将`ldf`和`ip`和端口`port`进程绑定-----`bind()`
  3. 设置监听-----`listen()`
  4. 进入`while`
``` C++
    while(1){
      //等待有新的客服端连接到来
      cfd =  accept();
      //fork一个子进程,让子进程去处理数据
      pid = fork();
      if(pid < 0){
        exit(-1);
      }else if(pid >  0){
        //关闭通信描述符cfd
        close(cfd);
      }else if(pid ==0){
        //关闭监听文件描述符
        close(lfd);
        //收发数据
        while(1){
          //读数据
          n = read(cfd,buf,sizeof(buf));
          if(n <=0){
            break;
          }
          //发送数据给对方
          write(cfd,buf,n);
        }
        close(cfd);
        //下面的exit()必须有,防止子进程再去创建子进程
        exit();
      }
    }
```

注意点: `accept`或者`read`函数是阻塞函数,会被信号打断,此时不应该视为一个错误.`errno=EINTR;`

父子进程能够共享的:
* 文件描述符(子进程复制父进程的文件描述符)
* `mmap`共享映射区


# 多线程服务器开发流程
1. 创建`socket`,得到一个监听的文件描述符`lfd` --`socket()`
2. 将`lfd`和`IP`和端口`port`进行绑定----`bind();`
3. 设置监听---`listen()`
4. 
    ```C++
     while(1){
        //接收新的客服端
        cfd = accept();
    
        //创建一个子线程
        pthread_create(&threadID,NULL,thread_work,&cfd);
    
        //设置线程为分离属性
        pthread_detach(threadID);
       }
    
       close(lfd);
    
      子线程执行函数:
        void *thread_work(void *arg){
          //获得参数:通信文件描述符
          int cfd = *(int *) arg;
    
          while(1){
            //读数据
            n = read(cfd,buf,sizeof(buf));
            if(n <= 0){
              break;
            }
            //发送数据
            write(cfd,buf,n);
    
          }
          close(cfd);
        } 
    ```
5. 问题:
   1. 子线程能否关闭`lfd`?    
      子线程不能关闭文件描述符`lfd`,原因是子线程和主线程共享文件描述符,而不是复制的.
   2. 主线程能否关闭`cfd`?  
      主线程不能关闭`cfd`,主线程和子线程还是共享一个`cfd`,而不是复制的,`close`之后`cfd`就会被真正关闭.
   3. 多个子线程共享`cfd`,会有什么问题发生?  