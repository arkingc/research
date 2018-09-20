* **可扩展文件系统**
	* [Physical Disentanglement in a Container-Based File System](#osdi14-1)（**`OSDI'14`** `文件系统` `隔离性`）
* **读写性能测试分析**
	* [Performance Analysis of Containerized Applications on Local and Remote Storage](#msst17-1)（**`MSST'17`** `SSD` `文件系统` `docker存储驱动`）

<br>
<br>
<br>

<h2 id="osdi14-1"></h2>

## Physical Disentanglement in a Container-Based File System

[pdf](http://research.cs.wisc.edu/adsl/Publications/icefs-osdi14.pdf)

> 设计了一个文件系统IceFS，可以将文件和目录分组放入物理隔离的容器(cube)中，不同cube中文件的I/O及数据不会互相影响，从而提高了隔离性：
>  1. 局部化（错误的影响局部化到具体的cube中；错误恢复局部化到具体的cube中，不同cube中事务的提交并行执行）
>  2. 特殊化（可以实现一些定制化。因为每个cube是隔离的，那么每个cube可以使用不同的日志模式，相当于在“性能/一致性”方面提供了一个开关，可以根据负载的不同提供不同的设置

"One aspect of current system design has remained devoid of isolation: the physical on-disk structures of file systems"——文件系统的物理磁盘结构不具有隔离性，比如使用全局的bitmap来维护inode和block，一个bitmap的block出问题，会影响所有该block相关的文件。总的来说，**逻辑独立的文件系统实体并不是物理独立的**，这会导致不理想的可靠性和性能

在进行事务处理时，IceFS对锁进行了优化。性能上的可扩展性好像也主要是对日志系统的优化

<br>

<h2 id="msst17-1"></h2>

## Performance Analysis of Containerized Applications on Local and Remote Storage

[pdf](http://storageconference.us/2017/Papers/PerformanceAnalysisOfContainerizedApplications.pdf)

> 探究与分析本地与远端存储下Docker存储驱动与主机文件系统的最佳存储参数配置

实验负载配置：

* 32 jobs, 32 queue depth
* 随机读写的块大小：4KB
* 顺序读写的块大小：128KB

### 5.1 Choice of Host FS on Performance

<div align="center"> <img src="img/1.png"/> </div>

* (a) 测试裸机、主机文件系统为EXT4和XFS时，随机读和随机写的性能（benchmark有32个jobs）
	* EXT4的随机读性能相对其它2者更低，原因是EXT4默认情况下进行读操作时也会使用互斥锁，因此会有锁竞争。可以使用`dioread nolock`选项强制移除这个锁，性能便会提高，见 (f)
	* XFS的随机写性能相对其它2者更低，原因是XFS写的时候会竞争一个粗粒度的锁，当线程数增多时，锁竞争加大，性能会下降，见 (e)。而EXT4使用的是细粒度的锁，可以避免竞争

> paper中还测了顺序读写的性能对比，EXT4和XFS与裸机的性能对比不大，所以区别主要在于随机读写。基于上面的实验结果，可以根据应用是随机读敏感还是随机写敏感在2种主机文件系统中进行选择。后续的实验选择的主机文件系统都是XFS（除了使用btrfs存储驱动时）

<br>