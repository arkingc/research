## 1.测试准备

* `creat_rename_ct.sh`：创建用于重命名测试的容器
* `creat_cow_ct.sh`：创建用于cow测试的容器
* `iplist`：#容器的ip列表

## 2.测试分析


* `test_ntimes.sh`：进行n组测试
* `result`：n组测试的结果数据
* `data_analysis.sh`：处理n组测试的结果数据，得到平均值

## 3.清理

* `clear.sh`：删除用于测试的所有容器

## 4.辅助

* `cpu_info.sh`：查看cpu信息

<br>

```
creat_rename_ct.sh  #创建用于重命名测试的容器
creat_cow_ct.sh     #创建用于cow测试的容器

iplist              #容器的ip列表
test_ntimes.sh      #进行n组测试
result              #n组测试的结果数据
data_analysis.sh    #处理n组测试的结果数据，得到平均值

clear.sh            #删除用于测试的所有容器

cpu_info.sh         #查看cpu信息
```
