#include<sys/socket.h>
#include<netdb.h>
#include<stdio.h>
#include<ifaddrs.h>
#include<stdlib.h>
#include<time.h>
int main(){
   clock_t begin = clock();
   struct ifaddrs *addresses;
     /* getifaddrs() allocates memory & fills in it a linked list
     of addresses  */
    if(getifaddrs(&addresses)== -1){
     printf("getifaddrs call failed\n");
     return -1;
    }
 
    /* we use a ptr to walk through the linked list of addresses,after considering
    each address, we set address = address ->ifa_next to get the next
    address*/

   /* AF_INET & AF_INET6, for each address we identify the address family
   and for each we print its adapter name & type IPv4 or IPv6*/
   struct ifaddrs *address = addresses;
   while(address){

       int family = address->ifa_addr->sa_family;
       if(family == AF_INET || family == AF_INET6){

           printf("%15s\t", address -> ifa_name);
           printf("%15s\t",family == AF_INET ? "IPv4" : "IPv6" );
           char ap[100]; //stores textual address

           const int family_size = family == AF_INET ?
           sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);

           getnameinfo(address->ifa_addr, family_size, ap, sizeof(ap),0,0, NI_NUMERICHOST);
           printf("\t%s\n",ap);
       }
       address = address->ifa_next;
   }

       freeifaddrs(addresses);
       clock_t end = clock();
       double time_spent = (double)(end - begin);
       printf("\nExecution time:%fms",time_spent);
       return 0;

       
}