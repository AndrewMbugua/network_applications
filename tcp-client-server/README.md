## TCP Server

usage

$ make
$ ./tcp_server

### Open another terminal
$ telnet 127.0.0.1 5374 //5374 is the set port number

### Post usage - Kill port using 
$ sudo kill $(sudo lsof -t -i:5374)

