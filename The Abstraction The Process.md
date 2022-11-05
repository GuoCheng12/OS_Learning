# The Abstraction: The Process

> To implement virtualization of the CPU, and to implement it well, the
> OS will need both some low-level machinery and some high-level in-
> telligence. We call the low-level machinery mechanisms; mechanisms
> are low-level methods or protocols that implement a needed piece of
> functionality.

## The Abstraction: A Process

> 运行在OS的程序，OS给我们提供的抽象就是Process（进程），在任何时刻，我们都可以通过对一个过程在执行的过程中访问。



> 为了理解进程的构成，我们需要理解它的Machine state(机器进程):
>
> - address space
> - registers
> - program counter (PC)
> - stack pointer
> - frame pointer

## Process API

> Though we defer discussion of a real process API until a subsequent
> chapter, here we ﬁrst give some idea of what must be included in any
> interface of an operating system. These APIs, in some form, are available
> on any modern operating system.



> - Create（创造）
> - Destroy（销毁）
> - Wait（等待）
> - Miscellaneous Control（其他操作 e.g 挂起）
> - Status（获取状态）

## Process Creation: A Little More Detail

> One mystery that we should unmask a bit is how programs are trans-
> formed into processes. Speciﬁcally, how does the OS get a program up
> and running? How does process creation actually work?



> 操作系统运行一个程序必须做的第一件事是将它的代码和任何静态数据（例如，初始化的变量）加载到内存中，加载到进程的地址空间中。 程序最初以某种可执行格式驻留在磁盘上（或者，在一些现代系统中，基于Flash的固态硬盘）； 因此，将程序和静态数据加载到内存中的过程需要OS从磁盘读取这些字节并将它们放在内存的某个地方
>
> <img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221104230419492.png" alt="image-20221104230419492" style="zoom:40%;" />

## Process States

> Now that we have some idea of what a process is (though we will
> continue to reﬁne this notion), and (roughly) how it is created, let us talk
> about the different states a process can be in at a given time. The notion
> that a process can be in one of these states arose in early computer systems
> [DV66,V+65]. In a simpliﬁed view, a process can be in one of three states:



> - Running
> - Ready
> - Blocked

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221104232820156.png" alt="image-20221104232820156" style="zoom:40%;" />

## Data Structures

> The OS is a program, and like any program, it has some key data struc-
> tures that track various relevant pieces of information. To track the state
> of each process, for example, the OS likely will keep some kind of pro-
> cess list for all processes that are ready and some additional informa-
> tion to track which process is currently running. The OS must also track,
> in some way, blocked processes; when an I/O event completes, the OS
> should make sure to wake the correct process and ready it to run again.



> ASIDE: DATA STRUCTURE — THE PROCESS LIST
> Operating systems are replete with various important data structures
> that we will discuss in these notes. The process list (also called the task
> list) is the ﬁrst such structure. It is one of the simpler ones, but certainly
> any OS that has the ability to run multiple programs at once will have
> something akin to this structure in order to keep track of all the running
> programs in the system. Sometimes people refer to the individual struc-
> ture that stores information about a process as a Process Control Block
> (PCB), a fancy way of talking about a C structure that contains informa-
> tion about each process (also sometimes called a process descriptor).



