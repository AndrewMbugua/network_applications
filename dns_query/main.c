#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers.h"


int main() {

    char host[100];
    char serv[100];
    getnameinfo(address->ai_addr, address->ai_addrlen,
                host, sizeof(host),
                serv, sizeof(serv),
                0);
    printf("%s %s\n", host, serv);





    return 0;
}
