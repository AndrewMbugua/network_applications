//to demonstrate getaddrinfo() & getnameinfo()
/* Takes the name or IP address for its only argument then
 * uses getaddrinfo() to resolve that name or that IP address into an address structure,
 * and the program prints that IP address using getnameinfo()
 * for the text conversion. If multiple addresses are associated with a name,
 * it prints each of them. It also indicates any errors.
 */
#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#ifndef AI_ALL
#define AI_ALL 0x0100
#endif

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage:\n\tlookup hostname\n");
        printf("Example:\n\tip_lookup example.com\n");
        exit(0);
    }
    /* call getaddrinfo() to convert the hostname or address
     * into a struct addrinfo
     */
    printf("Resolving hostname: %s\n", argv[1]);
    struct addrinfo hints;
    memset(&hints,0,sizeof(hints));
    hints.ai_flags = AI_ALL;
    struct addrinfo *peer_address;
    if(getaddrinfo(argv[1],0, &hints, &peer_address))
    {
        fprintf(stderr,"getaddrinfo() failed.(%d)\n", GETSOCKETERRNO());
        return 1;
    }


}

