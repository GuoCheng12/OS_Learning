# Semaphore 

## 1.

> ASIDE: SETTING THE VALUE OF A SEMAPHORE
> We’ve now seen two examples of initializing a semaphore. In the ﬁrst
> case, we set the value to 1 to use the semaphore as a lock; in the second,
> to 0, to use the semaphore for ordering. So what’s the general rule for
> semaphore initialization?
> One simple way to think about it, thanks to Perry Kivolowitz, is to con-
> sider the number of resources you are willing to give away immediately
> after initialization. With the lock, it was 1, because you are willing to
> have the lock locked (given away) immediately after initialization. With
> the ordering case, it was 0, because there is nothing to give away at the
> start; only when the child thread is done is the resource created, at which
> point, the value is incremented to 1. Try this line of thinking on future
> semaphore problems, and see if it helps.

**Sem的作用：**

并行编程（或者说是ording）/ Locks



## 2. mutiple producers and consumers

要加上互斥(mutual exclusion) 且 互斥应该只关注到临界区（critical section 不然有可能会造成死锁）

## 3.Philosophers 哲学家问题

> **Broken Solution:**
>
> We attempt our ﬁrst solution to the problem. Assume we initialize
> each semaphore (in the forks array) to a value of 1. Assume also that
> each philosopher knows its own number (p). We can thus write the
> get forks() and put forks() routine (Figure 31.15, page 15).
>
> it will happen —— **deadlock**

> If each philosopher happens to grab the fork
> on their left before any philosopher can grab the fork on their right, each
> will be stuck holding one fork and waiting for another, forever.



