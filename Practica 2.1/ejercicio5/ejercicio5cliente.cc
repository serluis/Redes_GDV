// Includes sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
// Includes entrada/salida
#include <iostream>

int main(int argc, char** argv) {
    struct addrinfo hints;
    struct addrinfo * res;

    // ---------------------------------------------------------------------- //
    // --------------------- INICIALIZACIÓN SOCKET & BIND ------------------- //
    // ---------------------------------------------------------------------- //

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    // Coger la informacion
    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);
    // Control de errores de la informacion
    if (rc != 0) {
        std::cerr << "Error: " << gai_strerror(rc) << std::endl;
        return -1;
    }

    // res contiene la representación como sockaddr de dirección + puerto
    int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    freeaddrinfo(res);

    // ---------------------------------------------------------------------- //
    // ------------- ENVIO DEL LOS PARAMETROS AL SERVIDOR ------------------- //
    // ---------------------------------------------------------------------- //

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    // Declaracion del mensaje
    char msg[80];
    memset(&msg, 0, sizeof(msg));
    
    getnameinfo(res->ai_addr, res->ai_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
    
    // Envio del mensaje con el parametro especificado
    sendto(sd, argv[3], strlen(argv[3]), 0, res->ai_addr, res->ai_addrlen);

    ssize_t bytes = recvfrom(sd, msg, sizeof(msg), 0, res->ai_addr, &res->ai_addrlen);
    if (bytes == -1) {
        std::cerr << "recvfrom: " << std::endl;
        return -1;
    }

    std::cout << msg << std::endl;
}