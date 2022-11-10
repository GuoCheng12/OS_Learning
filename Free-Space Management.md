# Free-Space Management

> <u>In this chapter, we take a small detour from our discussion of virtual-</u>
> <u>izing memory to discuss a fundamental aspect of any memory manage-</u>
> <u>ment system, whether it be a malloc library (managing pages of a pro-</u>
> <u>cess’s heap) or the OS itself (managing portions of the address space of a</u>
> <u>process). Speciﬁcally, we will discuss the issues surrounding free-space</u>
> <u>management.</u>

## 有什么样的问题？

在这一章节我们要提出一些假设和解决方案，来解决内存管理中——**内存碎片**等问题

这是一个很“简单”的问题，所以我们有很多种方法来解决它

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221110140148950.png" alt="image-20221110140148950" style="zoom:50%;" />

> **CRUX: HOW TO MANAGE FREE SPACE**
> **How should free space be managed, when satisfying variable-sized re-**
> **quests? What strategies can be used to minimize fragmentation? What**
> **are the time and space overheads of alternate approaches?**

## 怎样解决？

### Basic Strategies

> Now that we have some machinery under our belt, let’s go over some
> basic strategies for managing free space. These approaches are mostly
> based on pretty simple policies that you could think up yourself; try it
> before reading and see if you come up with all of the alternatives (or
> maybe some new ones!).
> The ideal allocator is both fast andminimizes fragmentation. Unfortu-
> nately, because the stream of allocation and free requests can be arbitrary
> (after all, they are determined by the programmer), any particular strat-
> egy can do quite badly given the wrong set of inputs. Thus, we will not
> describe a “best” approach, but rather talk about some basics and discuss
> their pros and cons.

#### Best Fit

> Best Fit策略其实很简单，我们只需要在这个 list 中找到最小且也符合大小的内存块，就进行申请并返回剩余
>
> <img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221110140937349.png" alt="image-20221110140937349" style="zoom:50%;" />

####  Worst Fit

> Worst Fit的策略和Best Fit相反，是去寻找list当中最大的那个，来进行分配并返回
>
> <img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221110141127014.png" alt="image-20221110141127014" style="zoom:50%;" />

#### First Fit

> First Fit方法和Best Fit方法很类似，但是它不必要遍历整个list，只要碰到满足条件即可调用并返回



## Other Apporaches

还有什么办法吗？

> Beyond the basic approaches described above, there have been a host
> of suggested techniques and algorithms to improve memory allocation in
> some way. We list a few of them here for your consideration (i.e., to make
> you think about a little more than just best-ﬁt allocation).



### Segregated Lists

> Segregated Lists的方法其实不难理解，即就是将相同内存大小的块链接起，固定size大小的list来分配给调用者。
>
> 给OS带来的困难也是有的
>
> **how much memory should one dedicate to the pool of memory that serves specialized requests of a given size, as opposed to the general pool?**
>
> 答：用一个好的分配器——Slab
>
> <u>Speciﬁcally, when the kernel boots up, it allocates a number of object</u>
> <u>caches for kernel objects that are likely to be requested frequently (such as</u>
> <u>locks, ﬁle-system inodes, etc.); the object caches thus are each segregated</u>
> <u>free lists of a given size and serve memory allocation and free requests</u>
> <u>quickly. When a given cache is running low on free space, it requests</u>
> <u>some slabs of memory from a more general memory allocator (the to-</u>
> <u>tal amount requested being a multiple of the page size and the object in</u>
> <u>question). Conversely, when the reference counts of the objects within</u>
> <u>a given slab all go to zero, the general allocator can reclaim them from</u>
> <u>the specialized allocator, which is often done when the VM system needs</u>
> <u>more memory.</u>
>
> 但初始化和破坏数据结构是非常昂贵的



### Buddy Allocation

> Buddy Allocation 分配策略本质上就是一棵二叉树，不断的分叉分叉分叉，找到合适的资源然后返回回去
>
> <img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221110143448500.png" alt="image-20221110143448500" style="zoom:50%;" />



## 总结

在现代计算机系统中，制造一个快速、空间高效、可伸缩的分配器，以适应广泛的工作负载仍然是一个持续的挑战。