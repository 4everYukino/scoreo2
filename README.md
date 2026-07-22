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

项目的配置、编译、测试、安装规则和 DEB 打包均由 CMake 管理，一条命令即可
依次完成全部步骤：

```bash
cmake --workflow --preset release
```

生成的安装包位于 `build/release/packages/`。该 Workflow Preset 需要 CMake
3.25 或更高版本，生成 DEB 还需要 `dpkg-dev`：

```bash
sudo apt install ./build/release/packages/*.deb
```

卸载时使用 `sudo apt remove <产品名>`；如需同时删除配置，使用
`sudo apt purge <产品名>`。

产品名只在顶层 `CMakeLists.txt` 的 `PRODUCT_NAME` 中定义。修改该默认值，或者
在全新构建目录中传入 `-DPRODUCT_NAME=<新名称>`，即可同步修改程序、配置、服务、
安装目录和安装包名称。产品名必须符合 Debian 包名规则，只能使用小写字母、数字、
`+`、`-` 和 `.`。

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
