* **可扩展文件系统**
	* [Physical Disentanglement in a Container-Based File System](#osdi14-1)（**`OSDI'14`** `文件系统` `隔离性`）
	* [Understanding Manycore Scalability of File Systems](#atc16-1)（**`ATC'16`** `文件系统`）
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

<h2 id="atc16-1"></h2>

## Understanding Manycore Scalability of File Systems

[pdf](https://www.usenix.org/system/files/conference/atc16/atc16_paper-min.pdf)

> 使用自己开源的benckmark（FxMark）分析5种文件系统的多核扩展性。这5种文件系统（ext4、XFS、btrfs、F2FS、tmpfs）在很多I/O密集型的应用中存在隐藏的可扩展性瓶颈

Introdution部分有提到了当前文件系统的多核可扩展性问题的研究现状（较少），以及研究这个问题的意义

**可扩展性问题**：“For example, all operations on a directory are sequential regardless of read or write; a file cannot be concurrently updated even if there is no overlap in each update. Moreover, we should revisit the core design of file systems for manycore scalability. For example, the consistency mechanisms like journaling (ext4), copy-on-write (btrfs), and log-structured writing (F2FS) are not scalable”

**Exim[8] email server**："The message delivery heavily utilizes I/O operations. It consists of a series of operations ranging from creating, renaming, and deleting small files in the spool directories to appending the message body to the per-user mail file"

<div align="center"> <img src="img/2.png"/> </div>

* (a) 为Exim在6种文件系统下随core增加时的吞吐量。很明显，文件系统限制了Exim应用的多核可扩展性。ext4NJ (i.e., ext4 with no journaling) 相比于ext4有44%的性能下降，原因是在ext4NJ中，2个独立的锁（i.e., a spinlock for journaling and a mutex for directory update）以一种意想不到的方式发生交错。而在ext4中，当调用`create()`创建一个文件时，为了元数据的一致性首先会尝试获取日志的自旋锁，然后获取新建文件所在目录的互斥锁。这种顺序导致了在尝试获取目录互斥锁时，会有较小的竞争，在ext4NJ中，目录互斥锁的大量竞争影响了性能
* 大多数读写操作都在Page Cache中完成，并且大多数写操作能在后台完成。因此，文件系统的内存结构决定了可扩展性，而跟存储介质无关。因此 (b) 中除了XFS，文件系统在不同存储介质下都表现出相似的性能。XFS的性能与存储介质有很大关系，因为XFS中频繁的元数据更新操作会导致日志数据刷新到磁盘，出现等待
* **细粒度锁与可扩展性**：(a) 中btrfs完全没有表现出多核扩展性，并且性能很差。一个猜想是为了等待存储事件的完成，引入了大量空闲时间。但是从图 (c) 中看出btrfs有67%的时间花在了内核模式上。事实上，btrfs为了同步root node，有47%的CPU时间花在了同步更新上。而Btrfs在访问B-tree node时，会有大量锁竞争，从而影响了多核扩展性

> 上述实验和结论说明，对于应用开发者和文件系统开发者来说，要预测推断文件系统的可扩展性是很难的。要识别文件系统的可扩展性，需要有一套benchmark（也就是作者开发的FXMARK Benckmark Suite）来不断地评估并指导文件系统系统的设计

### FXMARK Benckmark Suite

#### a）**19 microbenchmarks**

19 microbenchmarks are designed for systematically identifying scalability bottlenecks. **stressing 7 different components of file systems**

* 1）path name resolution
* 2）page cache for buffered I/O
* 3）inode management
* 4）disk block management
* 5）file offset to disk block mapping
* 6）directory management
* 7）consistency guarantee mechanism


<div align="center"> <img src="img/3.png"/> </div>

* **microbenchmarks 1-3**

<div align="center"> <img src="img/4.png"/> </div>

图 b）中，当多个进程读一个共享文件中的私有块时，XFS出现扩展性问题。原因是XFS通过共享模式(`down_read()`和`up_read()`)访问文件时，会请求和释放 read/write semaphore。一个 read/write semaphore 内部维护了1个 reader counter ，因此共享模式下每个操作都原子地更新 reader counter

图 c）中，当多个进程读一个共享文件中的共享块时，都出现扩展性问题。因为大量的时间消耗在了page cache的引用计数操作上

#### b）3 well-known I/O-intensive application benchmarks

3 well-known I/O-intensive application benchmarks（Mail server、NoSQL database、File server）to reason about the scalability bottlenecks in I/O-intensive applications



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