# 并发BUG



## What Types Of Bugs Exist?

> 分为两种BUG:
>
> - Non-Deadlock
> - Deadlock

### Non-Deadlock Bugs

> 非死锁BUG也分为两种：	
>
> - atomicity violation bugs
> - order violation bugs

#### atomicity violation bugs（原子冲突BUG）

> **原子冲突BUG很好解决，加把锁即可**

#### Order-Violation bugs

> **加上条件变量**

#### Non-Deadlock Bugs:Summary

> A large fraction (97%) of non-deadlock bugs studied by Lu et al. are either
> atomicity or order violations. Thus, by carefully thinking about these
> types of bug patterns, programmers can likely do a better job of avoiding
> them. Moreover, as more automated code-checking tools develop, they
> should likely focus on these two types of bugs as they constitute such a
> large fraction of non-deadlock bugs found in deployment.

## Deadlock Bugs

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221028231205232.png" alt="image-20221028231205232" style="zoom:50%;" />

> 这是一个非常容易理解的死锁

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221028231311209.png" alt="image-20221028231311209" style="zoom:50%;" />

> **Thread1 和 Thread2 互相在争抢彼此的Lock——导致死锁**

### Why Do Deadlocks Occur?

> - One reason is that in large code bases, complex dependencies arise
>   between components.
> - Another reason is due to the nature of encapsulation.

### Conditions for Deadlock

> - Mutual exclusion: Threads claimexclusive control of resources that
>   they require (e.g., a thread grabs a lock).
> - Hold-and-wait: Threads hold resources allocated to them (e.g., locks
>   that they have already acquired) while waiting for additional re-
>   sources (e.g., locks that they wish to acquire).
> - No preemption: Resources (e.g., locks) cannot be forcibly removed
>   from threads that are holding them.
> - Circular wait: There exists a circular chain of threads such that each
>   thread holds one or more resources (e.g., locks) that are being re-
>   quested by the next thread in the chain.

如果不满足这四个条件中的任意一个，则不能发生死锁。

### Prevention

#### Circular wait（获取锁的顺序冲突导致BUG）

> - 可以将获取资源（锁）的顺序固定下来
>
> - 或者还有一种办法：使用锁地址来定位
>
> <img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221029000747316.png" alt="image-20221029000747316" style="zoom:35%;" />



#### Hold-and-wait（需要对多个资源进行占有和释放）

>外面加一层prevention锁即可



#### No Preemption（资源（例如，锁）不能从持有它们的线程中强制移除）

> 可能出现活锁的情况（不能同时获得两把锁，然后不断的重复，没有任何意义）<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221102130618492.png" alt="image-20221102130618492" style="zoom:50%;" />
>
> There are solutions to the livelock problem, too:
> for example, one could add a random delay before looping back and try-
> ing the entire thing over again, thus decreasing the odds of repeated in-
> terference among competing threads

