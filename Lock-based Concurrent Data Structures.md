# Lock-based Concurrent Data Structures

## Concurrent Counters

> 考虑到Scalable可伸展性，有一个东西叫——**approximate counters**
>
> The approximate counter works by representing a single logical counter
> via numerous local physical counters, one per CPU core, as well as a single
> global counter.

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221019133834240.png" alt="image-20221019133834240" style="zoom:50%;" />

## Concurrent Linked Lists



> 链表中，容易因为malloc这样的函数并不是线程安全的，所以容易发生，由于malloc在获取锁之后失败导致的BUG

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221019140921585.png" alt="image-20221019140921585" style="zoom:30%;" />

> 解决办法很简单，只需要改变一下顺序，将lock的设置和释放只发生在临界区(critical section)

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221019141155173.png" alt="image-20221019141155173" style="zoom:30%;" />

###  Scaling Linked Lists

> 考虑到可扩展性（高并发），同样也设计出了高并发下的链表<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221019141721214.png" alt="image-20221019141721214" style="zoom:33%;" />

> 但是遗憾的事，这样很难比单个链表要快，因为列表的遍历每次的获取和释放锁的开销是非常庞大的
>
> （有待考虑）...





