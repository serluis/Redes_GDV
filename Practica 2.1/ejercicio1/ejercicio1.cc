// Includes de sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
// Includes de entrada/salida
#include <iostream>
// Funcion principal
int main(int argc, char**argv) {
    struct addrinfo hints;
    struct addrinfo * res;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;

    // Coger la informacion
    int rc = getaddrinfo(argv[1], NULL, &hints, &res);
    // Control de errores de la informacion
    if (rc != 0) {
        std::cerr << "Error: " << gai_strerror(rc) << std::endl;
        return -1;
    }

    for (struct addrinfo * rp = res; rp != NULL; rp = rp->ai_next) {
        char host[NI_MAXHOST];

        getnameinfo((struct sockaddr *)rp->ai_addr, rp->ai_addrlen, host, NI_MAXHOST, NULL, NI_MAXSERV, NI_NUMERICHOST);

        // Salida del programa
        std::cout << host << "  " << rp->ai_family << "    " << rp->ai_socktype << std::endl;
    }
    freeaddrinfo(res);

    return 0;
}