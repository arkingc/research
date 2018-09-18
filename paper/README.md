* 读写性能测试分析
	* [Performance Analysis of Containerized Applications on Local and Remote Storage](#msst17-1)（**`MSST'17`** `SSD` `docker存储驱动` `文件系统`）

<br>
<br>
<br>

<h2 id="msst17-1"></h2>

## Performance Analysis of Containerized Applications on Local and Remote Storage

[pdf](http://storageconference.us/2017/Papers/PerformanceAnalysisOfContainerizedApplications.pdf)

> 探究与分析SSD环境下Docker存储驱动与主机文件系统的最佳存储参数的配置

实验负载配置：

* 32 jobs, 32 queue depth
* 随机读写的块大小：4KB
* 顺序读写的块大小：128KB

### 5.1 Choice of Host FS on Performance

![](img/1.png)

* (a) 测试裸机、主机文件系统为EXT4和XFS时，随机读和随机写的性能（benchmark有32个jobs）
	* EXT4的随机读性能相对其它2者更低，原因是EXT4默认情况下进行读操作时也会使用互斥锁，因此会有锁竞争。可以使用`dioread nolock`选项强制移除这个锁，性能便会提高，见 (f)
	* XFS的随机写性能相对其它2者更低，原因是XFS写的时候会竞争一个粗粒度的锁，当线程数增多时，锁竞争加大，性能会下降，见 (e)

> paper中还测了顺序读写的性能对比，EXT4和XFS与裸机的性能对比不大，所以区别主要在于随机读写。基于上面的实验结果，可以根据应用是随机读敏感还是随机写敏感在2种主机文件系统中进行选择。后续的实验选择的主机文件系统都是XFS（除了使用btrfs存储驱动时）

<br>