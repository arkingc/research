## 1.测试准备

* `creat_rename_ct.sh`：创建用于重命名测试的容器
* `creat_cow_ct.sh`：创建用于cow测试的容器
* `iplist`：容器的ip列表
* `dockerMount`：容器数据卷目录，包含了需要在容器内运行的测试程序
  * `rename.c`：容器内执行大量重命名操作
  * `file1`：用于重命名的文件
  * `cow.c`：容器内执行一个cow操作
  * `time_ms.c`：测试pssh是否能让多少容器同时执行操作
  * `print.c`：打印`“hello world”`，测试pssh是否能成功连接容器

## 2.测试分析

* `test_ntimes.sh`：进行n组测试
* `result`：n组测试的结果数据
* `data_analysis.sh`：处理n组测试的结果数据，得到平均值

## 3.清理

* `clear.sh`：删除用于测试的所有容器
