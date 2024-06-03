# ch1
## socket
socket就像一个fd. 可以write read
```
int socket(int domain, int type, int protocol);
```

## sockaddr, sockaddr_in
sockaddr和sockaddr_in的区别: sockaddr是用来传参的, sockaddr_in是写代码用的.
两者定义如下:
```
struct sockaddr {
    sa_family_t sin_family; // 地址族, 用AF_INET等
    char sa_data[14]; // 14字节数据, 包括目标地址和端口信息
};
struct sockaddr_in {
    sa_family_t sin_family;
    in_port_t sin_port; // 端口号
    struct in_addr sin_addr; // 网络地址, 32为IP地址
    char sin_zero[8]; // 其他8字节, 不使用
};
```

## inet_addr
inet_addr函数, 将字符串转为ip地址
```
in_addr_t inet_addr(const char *s); 
// 可以这么调用 ip_addr = inet_addr("192.168.5.12"); 
// in_addr_t是4字节, typedef uint32_t in_addr_t;
```

## htons, htonl
hton: host to network.
s: short, l: long.
主机字节转到网络字节, 因为主机可能是big endian, 也可能是little endian
```
uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
```

## bind
第一个参数是socket的fd, 第二个参数是监听的端口和地址等信息, 第三个参数是第二个参数的长度, 单位为字节, 一般用sizeof()
```
int bind(int sockfd, const struct sockaddr *addr, socklen_t len);
```

## listen
设置socket可以排队最多n个, 成功0, 失败-1
```
int listen(int fd, int n);
```

## accept
```
int accept(int fd, struct sockaddr *addr, socklen_t *addr_len);
```


## 关于designated initializers
对于以下struct: 
```c++
struct Example {
    int counts;
    char alpha;
    double score;
};
```
在c语言中, 可以乱序初始化
```c
struct Example example{
    .score = 1.2,
    .counts = 10,
    .alpha = 'u'
};
```
但是c++中, 必须按顺序
```c++
Example example{
    .counts{10},
    .alpha{'q'},
    .score{1.9}
};
```

还有另外的区别: 对于`struct sockaddr_in`
c语言可以嵌套地对`sin_addr`初始化:
```
struct sockaddr_in adr{
    .sin_addr.s_addr = htonl(INADDR_ANY)
};
```
c++中则不能用嵌套初始化, 可以用如下方式
```
sockaddr_in adr{
    .sin_addr{in_addr{htonl(INADDR_ANY)}}
}
```
