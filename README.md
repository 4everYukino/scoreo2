### 项目背景

本项目：

1. 记录了本人从事 Linux C++ 以来执行过的一些项目
2. 提供一个实践个人兴趣爱好模块的地方
3. 提供一个展示、交流的窗口

### 项目简介

本项目主体上是一个轻量级的 HTTP 服务器，在此基础上按个人喜好实现若干子模块，比如：

* 基于 libfuse 的网络文件系统
* 进程管理、调度
* ...

最后以 Linux 守护进程（deamon）场景来进行部署。

### 构建

执行 `bootstrap.sh` 即可。

### 子模块简介

#### HTTP Server

基于 `boost::asio` 搭建，...

事实上，本人首个任职公司的网络框架是基于 ACE 构建的，...

> 可参见 `httpd` 以及 `server` 文件夹

#### FUSE-based File System

基于 `libfuse` 实现了一个用户态的文件系统，...

> 可参见 `fuse` 文件夹

#### Async Process Scheduler

首先基于 `boost::process` 实现了 ...

> 可参见 `process` 文件夹
