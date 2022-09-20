## TCP Server

### usage

`` $ make ``

`` $ ./tcp_server ``

![](https://github.com/AndrewMbugua/network_applications/blob/main/tcp-client-server/images/server_listening.png)


![](https://github.com/AndrewMbugua/network_applications/blob/main/tcp-client-server/images/connection_received.png)

#### Open another terminal
`` $ telnet 127.0.0.1 5374 //5374 is the set port number``

![](https://github.com/AndrewMbugua/network_applications/blob/main/tcp-client-server/images/telnet.png)


#### Post usage - Kill port use
`` $ sudo kill $(sudo lsof -t -i:5374)``

