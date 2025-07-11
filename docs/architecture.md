## 项目概述

### 项目目标

基于Linux C/C++，使用原生socket API构建多人聊天室项目，支持文本消息收发，用户登入与退出，在线用户列表基本功能。

### 技术栈

- 编程语言：C/C++（clang编译）
- 核心技术：TCP/UDP socket、多线程编程（thread）
- 构建工具：CMake

## 项目架构设计

见项目架构图

## 核心模块划分

### 服务器端

**技术**：Reactor模式、同步I/O模型，阻塞式I/O。

#### 连接监听

io_handler.cc

**功能**：服务器主线程监听socket，接收新连接请求创建子线程处理。

**实现**：一个死循环作为主线程，阻塞式accept调用直到接收到客户端连接请求后，创建子线程，使用thread多线程库。

#### 子线程处理

logic_unit.cc

**功能**：服务器每个子线程都和一个客户端连接进行数据收发。

**实现**：使用recv和send系统调用和连接的客户端进行数据收发。

#### 广播消息

broadcast_message.cc

**功能**：可以将各个用户发送的消息广播给所有用户，并附带用户名。

**实现**：每个子线程调用广播函数将消息回显给所有客户端。

#### 在线用户列表

server.hpp

**功能**：服务器维护一个在线用户列表，决定广播函数的广播对象。

**实现**：使用vector容器存储用户名。接受用户连接后加入该容器，用户退出后（socket关闭）后将该用户移出vector。

#### 头文件

server.hpp

声明服务器端各模块以及定义在线用户列表和socket地址数据结构（指定服务器的）（客户端socket地址数据结构可选（一般用于日志记录，不关心accept中填NULL））。

#### 服务器端主函数
```
server.cc
```

### 客户端

#### 用户登入

log_in.cc

**功能**：用户可以用自定义的用户名登入聊天室。

**实现**：用户第一次准备进入聊天室时，屏幕打印提示信息要求输入用户名(并提示如何退出聊天室)，用户输入后将其传递给服务器。

#### 消息收发

message_handler.cc

**功能**：用户可以自由发送消息，所有客户端都可以收到看到。

**实现**：使用recv和send函数和服务器进行数据收发。

#### 用户登出

log_out.cc

**功能**：用户可以输入"**QUIT**"退出聊天室。

**实现**：客户端检测到输入缓冲区是**QUIT**就调用shutdown关闭socket。

#### 客户端主函数
```
client.cc
```
#### 用户名输入登录函数

username_input.cc

#### 头文件

client.hpp

声明客户端各模块以及定义socket地址数据结构（指定服务器的）。

### 公用文件

#### 读写缓冲区清空函数

mem_reset.hpp

#### 终端设置

terminal_setting.hpp