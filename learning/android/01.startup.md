# 基本概念

## Android 系统架构

Android 大致分为四层架构：Linux 内核层、系统运行库层、应用框架层、应用层。

### Linux 内核层

Android 系统是基于 Linux 内核的，这一层为 Android 设备的各种硬件提供了底层的驱动，如显示驱动、音频驱动、照相机驱动、蓝牙驱动、Wi-Fi 驱动、电源管理等。

### 系统运行库层

这一层通过一些 C/C++库为 Android 系统提供了主要的特性支持。如 SQLite 库提供了数据库的支持，OpenGL|ES 库提供了 3D 绘图的支持，Webkit 库提供了浏览器内核的支持等。

### 应用框架层

这一层主要提供了构建应用程序时可能用到的各种 API，Android 自带的一些核心应用就是使用这些 API 完成的，开发者可以使用这些 API 来构建自己的应用程序。

### 应用层

所有安装在手机上的应用程序都是属于这一层的，比如系统自带的联系人、短信等程序，或者是你从 Google Play 上下载的小游戏，当然还包括你自己开发的程序。

## Android 应用

### 四大组件

Android 系统四大组件分别是 Activity、Service、BroadcastReceiver 和 ContentProvider。其中 Activity 是所有 Android 应用程序的门面，凡是在应用中你看得到的东西，都是放在 Activity 中的。而 Service 就比较低调了，你无法看到它，但它会在后台默默地运行，即使用户退出了应用，Service 仍然是可以继续运行的。BroadcastReceiver 允许你的应用接收来自各处的广播消息，比如电话、短信等，当然，你的应用也可以向外发出广播消息。ContentProvider 则为应用程序之间共享数据提供了可能，比如你想要读取系统通讯录中的联系人，就需要通过 ContentProvider 来实现。

### 丰富的系统控件

### SQLite 数据库

### 强大的多媒体
