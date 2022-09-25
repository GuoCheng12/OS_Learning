## Condition Variable

1. 关the_exit() and thr_join()

​		考虑一下两种情况：

- No State Variable
- No Lock

​		这两种情况都是糟糕的

​		第一种：如果没有变量控制，将会出现出现永远睡眠（想象parent - child）的情况

​		第二种：如果光有条件变量，但是没有锁来达到互斥条件的情况，会出现同时（或者顺序颠倒）进入临界区的情况。

2. Always hold the lock while signaling

​		最好是这样，为了防止出现意外（出现上面第二种情况）

​		hold the lock when calling **signal or wait**

3. synchronization problem —— Producer / Consumer （Bounded buffer）

​		这个problem可以解决99.9%的并发问题：

Single producer / consumer 的时候 if statement可以解决，但是如果，producer 和 consumer大于1 时，就会出问题——要用while statement来取代

上述问题出现的原因是在于，当P1/C1被唤醒之后，不能够立即执行

Thanks to Meas semantics ~ a simple rule to remeber with condition
variables is to always use while loops.

就算处理完上个问题，还有严重的bug会引发：当出现大于(1:1)的生产者消费者的数量时（比如两个生产者，一个消费者），第二次唤醒时，会出现不知道唤醒哪个线程的状况。——如果唤醒了相同类型的情况，则会出现bug（全部sleeping...）

解决完这些问题，我们来讨论一下效率问题：首先要知道效率问题是由什么因素引起的？如果每次只是生产者生产一个元素，消费者使用一个元素的话，这样要不停的切换上下文（消耗）。解决这个问题，我们可以增加更多的buffer，这样可以在sleep前产生更多的值，消费更多的值。(甚至可以计划一下，让pipe为空时再睡觉也不是不行:D)

4. Covering Condition

   当我们在处理像是  **内存管理**  这样的问题，生产者-消费者模型最好使用boastcast()

















