#  基于 Qt 的 C/S 架构即时通讯软件

一个使用 Qt + MySQL + Redis 实现的跨平台即时通讯软件，基于 C/S 架构，使用 TCP 套接字通信，支持用户认证、多用户实时聊天、数据持久化、消息转发等核心功能。

##  项目简介

本项目基于 Qt5/6 框架，使用 `QTcpServer`/`QTcpSocket` 实现客户端与服务器之间的实时通信。后端采用：

- MySQL：存储用户数据和聊天记录
- Redis：缓存用户状态、提高性能

客户端使用 SQLite 本地管理聊天记录，实现断网恢复和本地查询功能，提升用户体验。

##  项目特点

- 多用户注册与登录管理
- 使用 MySQL 存储用户信息与历史数据
- 使用 Redis 进行缓存与快速响应
- 客户端使用 SQLite 记录本地聊天记录
- 实时多种类型消息发送与接收
- 支持发送离线消息以及多种格式文件
- 消息记录展示
- 支持局域网/公网通信
- 使用 TCP 套接字通信（QTcpSocket/QTcpServer）
- 模块化设计：通信、数据库、Redis 封装清晰

## 🔧 技术栈

| 组件     | 技术说明                    |
| -------- | --------------------------- |
| 开发框架 | Qt 6 / Qt 5（支持跨平台）   |
| 通信协议 | 自定义协议基于 TCP 实现     |
| 架构模式 | C/S 架构（Client / Server） |
| 数据存储 | 	MySQL（持久化存储）+ Redis（高性能缓存）                      |
| 构建工具 | Qt Creator 项目文件（.pro） |
| 第三方库 | hiredis（通过 vcpkg 安装）    |

## 📂 项目结构

```
├── client/               # 客户端代码
│   ├── Core/                          # 核心业务逻辑模块
│   │   ├── chathistorymanager.h/.cpp     # 聊天记录管理
│   │   ├── control.h/.cpp                # 控制器，统一调度逻辑
│   │   ├── currentuser.h/.cpp           # 当前用户数据管理
│   │   ├── datatransfer.h/.cpp          # 数据传输与封装模块

│   ├── Delegate/                     # Qt 视图委托类（用于自定义列表项渲染）
│   │   ├── applydelegate.h/.cpp         # 好友申请项委托
│   │   ├── chatdelegate.h/.cpp          # 聊天消息项委托
│   │   ├── friendsdelegate.h/.cpp       # 好友列表项委托
│   │   ├── messagedelegate.h/.cpp       # 消息委托
│   │   └── strangerdelegate.h/.cpp      # 陌生人列表项委托

│   ├── Model/                        # 数据模型层（MVC 中的 M）
│   │   ├── Packet.h                    # 网络数据包结构定义
│   │   ├── chatmodel.h/.cpp           # 聊天模型
│   │   ├── friendsmodel.h/.cpp        # 好友模型
│   │   ├── group.h                    # 群组结构
│   │   ├── message.h/.cpp             # 消息数据结构
│   │   ├── messagemodel.h/.cpp        # 消息列表模型
│   │   ├── strangermodel.h/.cpp       # 陌生人列表模型
│   │   └── user.h                     # 用户数据结构

│   ├── Utils/                        # 工具类与自定义类型
│   │   └── CustomTypes.h                # 自定义数据类型定义

│   ├── View/                         # 界面相关视图组件（包含登录、主界面等）
│   │   ├── Components/                  # 通用 UI 组件
│   │   │   ├── avatarlabel.h/.cpp           # 头像控件
│   │   │   ├── confirmbox.h/.cpp            # 确认提示框
│   │   │   ├── customtitlebar.h/.cpp        # 自定义标题栏
│   │   │   ├── dialogtitlebar.h/.cpp        # 弹窗标题栏
│   │   │   ├── onlinestatelabel.h/.cpp      # 在线状态显示
│   │   │   └── tipsbox.h/.cpp               # 提示框组件
│
│   │   ├── Login/                      # 登录与注册界面组件
│   │   │   ├── hollow_button.h/.cpp            # 空心按钮
│   │   │   ├── input_box.h/.cpp                # 输入框
│   │   │   ├── login_button.h/.cpp             # 登录按钮
│   │   │   ├── login_form.h/.cpp               # 登录表单
│   │   │   ├── registration_form.h/.cpp        # 注册表单
│   │   │   ├── responsive_form.h/.cpp          # 自适应表单布局
│   │   │   ├── scroll_bar.h/.cpp               # 自定义滚动条
│   │   │   └── transparent_transition_interface.h/.cpp # 动画过渡界面
│
│   │   ├── addfriends.h/.cpp          # 添加好友界面
│   │   ├── creategroup.h/.cpp         # 创建群组界面
│   │   ├── imageviewer.h/.cpp         # 图片查看器
│   │   ├── login.h/.cpp               # 登录入口页
│   │   ├── messagelistview.h/.cpp     # 消息列表视图
│   │   ├── updateavatar.h/.cpp        # 修改头像页面
│   │   └── UserDetailView.h/.cpp      # 用户详情视图
│   │       └── UserDetailView.ui

│   ├── main.cpp                    # 程序入口
│   ├── mainwindow.h/.cpp           # 主窗口（聊天主界面）
│   ├── mainwindow.ui               # 主窗口 UI 文件

│   ├── resource.qrc                # Qt 资源文件（图标/样式等）

│   └── AllChatClient.pro           # Qt 工程配置文件（你提供的内容）

├── server/               # 服务器端代码
│   ├── Core/
│   │   ├── datatransfer.h/.cpp   # 消息收发模块
│   │   ├── database.h/.cpp       # 数据库接口模块
│   │   ├── redisclient.h/.cpp    # Redis 操作模块
│   ├── server.h/.cpp             # 服务端主逻辑
│   ├── main.cpp                  # 程序入口
│   └── AllChatServer.pro         # Qt 工程文件
├── docs/                 # 文档资料与截图
└── README.md             # 项目说明文档
```

##  软件界面截图

**登陆界面**

![image-20250504112315127](https://github.com/user-attachments/assets/895b1711-4106-4a1c-b9ea-23db49b5af8d)


**注册界面**

![image-20250504112338848](https://github.com/user-attachments/assets/f3914f03-7c29-40ce-ac9b-a6d2d1683b8a)


**聊天主界面**

![image-20250504112401557](https://github.com/user-attachments/assets/76e4f3f0-f048-438d-bb35-abd3db9cf316)


**创建群聊窗口**

![image-20250504112422917](https://github.com/user-attachments/assets/ebc0899b-b1d0-4f3f-a9b6-4354a63c513a)


**添加好友窗口**

![image-20250504112730509](https://github.com/user-attachments/assets/8883479f-9d43-46c6-bf51-bef4e06e1118)


![image-20250504112839464](https://github.com/user-attachments/assets/465b566f-e025-4179-b950-3a9cc57327bd)


**修改头像**

![image-20250504112930225](https://github.com/user-attachments/assets/c591d108-a069-441f-b19c-f84006ac7fe6)


**好友管理**

![image-20250504112955773](https://github.com/user-attachments/assets/4c836139-3c57-438b-b7d0-627c6c5ab7a3)




## 第三方依赖

- **hiredis**：轻量级 C 语言 Redis 客户端，需通过 vcpkg 安装：

```
vcpkg install hiredis
```

配置（已在 `.pro` 中添加）：

```
INCLUDEPATH += D:/app/vcpkg/installed/x64-windows/include
LIBS += -LD:/app/vcpkg/installed/x64-windows/lib -lhiredis
```

## 编译与运行

### 环境准备

- 安装 Qt（推荐 Qt 6.5.3）
- 安装 Qt Creator 
- 安装 Redis 并保持运行（本地或远程）

### 编译项目

使用 Qt Creator 打开 `.pro` 文件

### 运行说明

1. 启动redis 与 MySQL 服务
2. 启动服务器程序，监听端口（默认 12345）
3. 启动多个客户端，输入服务器 IP 和端口进行连接
4. 登录后即可聊天
