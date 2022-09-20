#include "headers.h"

/*
notes
kill server process - sudo kill $(sudo lsof -t -i:port_set)
valgrind test - valgrind --tool=memcheck --leak-check=full ./tcp_server
*/

#define PORT "53574" //port users will be connected to

#define BACKLOG 10 //how many pending connections queue will hold

//get socket address,IPv4 for now
void *get_in_addr(struct sockaddr *sa)
{
if (sa -> sa_family == AF_INET) //AF_INET identifies IP address family
{
    return &(((struct sockaddr_in*)sa)->sin_addr);
}

}

int main(){
/* listen on sock_fd
   receive new connection on few_fd
*/
int sockfd, new_connection;

struct addrinfo hints, *server_info, *p;
struct sockaddr_storage client_addr;
socklen_t sin_size;

//struct signal_action sa; //look into later
int yes = 1; //to modify later

/*INET6_ADDRSTRLEN - for IPv4 mapped addreses,has 46 bytes
getaddrinfo - eliminates IPv4 & IPv6 dependencies
hints - a pointer to a struct addrinfo
*/
memset(&hints, 0 ,sizeof hints);
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE; //use 127.0.0.1

char mapped_addresses[INET6_ADDRSTRLEN]; 
int get_address_info = getaddrinfo(NULL, PORT, &hints, &server_info);


// error checkers
if (get_address_info != 0)
{
//gai_strerror - translates the error codes to a human readable string
fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(get_address_info));
return 1;
}

//loop & bind to 1st address
for (server_info; server_info != NULL; server_info -> ai_next)
{
    if((sockfd = socket(server_info -> ai_family, server_info -> ai_socktype, server_info -> ai_protocol )) == -1)
    {
        perror("server socket error");
        continue;
    }

//setsockopt
if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
{
    perror("setsockopt");
    exit(1);
}

if(bind(sockfd, server_info -> ai_addr, server_info -> ai_addrlen) == -1)
{
    close(sockfd);
    perror("Andrew's server: binding");
    continue;
}
break;
}

freeaddrinfo(server_info);


if(server_info == NULL)
{
    fprintf(stderr, "server failed to bind\n");
    exit(1);
}

if(listen(sockfd, BACKLOG) == -1)
{
    perror("listen");
    exit(1);
}


printf("Andrew's server, waiting for connections.\n");

while(1)
{
    sin_size = sizeof client_addr;
    //await a connection to fd
    new_connection = accept(sockfd,(struct sockaddr*) &client_addr, &sin_size);
    if(new_connection == -1)
    {
    perror("accept");
    continue;
    }

inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr*)&client_addr),mapped_addresses, sizeof mapped_addresses);
printf("Andrew's server: got connection from %s\n", mapped_addresses);
}
close(new_connection);


return 0;
}