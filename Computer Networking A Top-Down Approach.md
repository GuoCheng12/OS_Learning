## **Computer Networking A Top-Down Approach **

### **2.2 The Web and HTTP**

### **Non-Persistent and Persistent Connections 非持续连接和持续连接**



> *区分：是否使用用一个具体的TCP来进行Client和Server的通讯？前者为**持续链接**，后者为**非持续链接***

- **非持续连接**：不断的启动/关闭TCP链接，来达到目的

> *弊端：启动/关闭TCP的过程需要耗时*

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221018002335234.png" alt="image-20221018002335234" style="zoom:30%;" />

1. The HTTP client process initiates a TCP connection to the server
  www.someSchool.edu on port number 80, which is the default port number
  for HTTP. Associated with the TCP connection, there will be a socket at the
  client and a socket at the server.

HTTP客户端进程在端口号80上启动到服务器www.someschool.edu的TCP连接，端口号80是HTTP的默认端口号。 与TCP连接相关联的是，客户端和服务器端各有一个套接字。

2. The HTTP client sends an HTTP request message to the server via its **socket.**
   The request message includes the path name /someDepartment/home
   .index. (We will discuss HTTP messages in some detail below.)

HTTP客户端发送request message 到HTTP服务器

3. The HTTP server process receives the request message via its socket, retrieves
   the object /someDepartment/home.index from its storage (RAM or
   disk), encapsulates the object in an HTTP response message, and sends the
   response message to the client via its socket.

HTTP服务器进程通过套接字接受请求消息，然后在RAM或者DISK中检索，然后用socket将响应消息发送会客户端

4. The HTTP server process tells TCP to close the TCP connection. (But TCP
   doesn’t actually terminate the connection until it knows for sure that the client
   has received the response message intact.)

HTTP 告诉TCP关闭TCP

5. The HTTP client receives the response message. The TCP connection termi-
   nates. The message indicates that the encapsulated object is an HTML file. The
   client extracts the file from the response message, examines the HTML file, and
   finds references to the 10 JPEG objects.

HTTP客户端接受响应消息，TCP连接终止

6. The first four steps are then repeated for each of the referenced JPEG objects.

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221018002544893.png" alt="image-20221018002544893" style="zoom:50%;" />

![image-20221018002602302](/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221018002602302.png)



### **HTTP Request MEssage** 

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221018004445860.png" alt="image-20221018004445860" style="zoom:50%;" />

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221018010040868.png" alt="image-20221018010040868" style="zoom:50%;" />

### **HTTP Respose Message**

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



### **Cookie**

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221018134242630.png" alt="image-20221018134242630" style="zoom:50%;" />



### **Web Caching**

A Web cache—also called a proxy server—is a network entity that satisfies HTTP
requests on the behalf of an origin Web server. The Web cache has its own disk
storage and keeps copies of recently requested objects in this storage. As shown in
Figure 2.11, a user’s browser can be configured so that all of the user’s HTTP requests
are first directed to the Web cache [RFC 7234]. Once a browser is configured, each
browser request for an object is first directed to the Web cache. As an example,
suppose a browser is requesting the object http://www.someschool.edu/
campus.gif. Here is what happens:

1. The browser establishes a TCP connection to the Web cache and sends an HTTP
  request for the object to the Web cache.

2. The Web cache checks to see if it has a copy of the object stored locally. If it
  does, the Web cache returns the object within an HTTP response message to the
  client browser.

3. If the Web cache does not have the object, the Web cache opens a TCP connec-
  tion to the origin server, that is, to www.someschool.edu. The Web cachethen sends an HTTP request for the object into the cache-to-server TCP connection. After receiving this request, the origin server sends the object within an HTTP response to the Web cache.

  4. When the Web cache receives the object, it stores a copy in its local storage and
  sends a copy, within an HTTP response message, to the client browser (over the
  existing TCP connection between the client browser and the Web cache).

  

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221019200552489.png" alt="image-20221019200552489" style="zoom:30%;" />

<img src="/Users/wuguocheng/Library/Application Support/typora-user-images/image-20221019203521809.png" alt="image-20221019203521809" style="zoom:30%;" />



### The Conditional GET

