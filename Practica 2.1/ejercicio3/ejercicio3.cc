#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <iostream>

/*
argv[0] = "./time_server"
argv[1] = "127.0.0.1"
argv[2] = "3000"
argv[3] = "t"
|
|
V
res->ai_addr ---> (socket + bind)
|
|
V
host (numeric)./addrinfo 127.0.0.1 3000
*/
int main(int argc, char **argv)
{
    struct addrinfo hints;
    struct addrinfo * res;

    // ---------------------------------------------------------------------- //
    // INICIALIZACIÓN SOCKET & BIND //
    // ---------------------------------------------------------------------- //
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

    if ( rc != 0 )
    {
        std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
        return -1;
    }
    // res contiene la representación como sockaddr de dirección + puerto

    int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);


    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    // Declaracion del mensaje
    char msg[80];
    memset(&msg, 0, sizeof(msg));
    freeaddrinfo(res);
    
    getnameinfo(res->ai_addr, res->ai_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
    
    sendto(sd, argv[3], strlen(argv[3]), 0,res->ai_addr,res->ai_addrlen);
    //freeaddrinfo(res);

    ssize_t bytes = recvfrom(sd, msg, sizeof(msg), 0, res->ai_addr, &res->ai_addrlen);
    std::cerr << "que recibo?: "<< res->ai_addr<<" y ademas: "<<&res->ai_addrlen<<" cannoname: "<< res->ai_canonname << std::endl;
    if (bytes == -1) {
        std::cerr << "recvfrom: " << std::endl;
        return -1;
    }

    return 0;
}