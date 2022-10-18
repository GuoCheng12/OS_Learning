## **Computer Networking A Top-Down Approach **

### **2.2 The Web and HTTP**

- **Non-Persistent and Persistent Connections 非持续连接和持续连接**

> *区分：是否使用用一个具体的TCP来进行Client和Server的通讯？前者为**持续链接**，后者为**非持续链接***

- **非持续连接**：不断的启动/关闭TCP链接，来达到目的

> *弊端：启动/关闭TCP的过程需要耗时*

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221018002335234.png" alt="image-20221018002335234" style="zoom:30%;" />

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221018002544893.png" alt="image-20221018002544893" style="zoom:50%;" />

![image-20221018002602302](/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221018002602302.png)



- **HTTP Request MEssage** 

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221018004445860.png" alt="image-20221018004445860" style="zoom:50%;" />

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221018010040868.png" alt="image-20221018010040868" style="zoom:50%;" />

- **HTTP Respose Message**

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221018112249954.png" alt="image-20221018112249954" style="zoom:50%;" />

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221018112934537.png" alt="image-20221018112934537" style="zoom:50%;" />

> the status code :
>
> **200 OK:** Request succeeded and the information is returned in the response.
>
> **301 Moved Permanently:** Requested object has been permanently moved;
> the new URL is specified in Location: header of the response message. The
> client software will automatically retrieve the new URL.
>
> **400 Bad Request:** This is a generic error code indicating that the request
> could not be understood by the server.
>
> **404 Not Found:** The requested document does not exist on this server.
>
> **505 HTTP Version Not Supported:** The requested HTTP protocol ver-
> sion is not supported by the server.



- **Cookie**

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221018134242630.png" alt="image-20221018134242630" style="zoom:50%;" />