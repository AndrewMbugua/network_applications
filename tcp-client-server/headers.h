#ifndef HEADERS

#define MAXDATASIZE 100 //max number of bytes we can get a time
#define PORT "53574" //port users will be connected to
#define BACKLOG 10 //how many pending connections queue will hold

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<signal.h>
#include<time.h>
#include<ifaddrs.h>

//get socket address,IPv4 for now
void *get_in_addr(struct sockaddr *sa)
{
if (sa -> sa_family == AF_INET) //AF_INET identifies IP address family
{
    return &(((struct sockaddr_in*)sa)->sin_addr);
}

}


#endif