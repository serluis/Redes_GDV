// Sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
// Entrada salida
#include <iostream>

int main(int argc, char **argv) {
    // res contiene la representación como sockaddr de dirección + puerto
    struct addrinfo hints;
    struct addrinfo *res;

    // ---------------------------------------------------------------------- //
    // -- INICIALIZACIÓN SOCKET & BIND -------------------------------------- //
    // ---------------------------------------------------------------------- //

    // Resetear la memoria desde hints hasta hints + addrinfo
    memset(&hints, 0, sizeof(struct addrinfo));
    // Configuracion
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // Coger la info del servidor
    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);
    // Control de errores de getaddrinfo
    if (rc != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
        return -1;
    }

    // Creacion del socket
    int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sd < 0) {
        std::cerr << "socket: " << std::endl;
    }
    freeaddrinfo(res);

    // ---------------------------------------------------------------------- //
    // -- GESTION DE LAS CONEXIONES AL SERVIDOR ----------------------------- //
    // ---------------------------------------------------------------------- //

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    
    char msg[80];
    memset(&msg, 0, sizeof(msg));

    getnameinfo(res->ai_addr, res->ai_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
    // Conectar al servidor
    int conn = connect(sd, res->ai_addr, res->ai_addrlen);
    if (conn != 0) {
        std::cerr << "connection failed: " << std::endl;
    }
    
    // ---------------------------------------------------------------------- //
    // -- GESTION DE LA CONEXION CLIENTE ------------------------------------ //
    // ---------------------------------------------------------------------- //
    
    // Envio del mesaje con el parametro especificado
    send(sd, (void *)argv[1], strlen(argv[1]), 0);

    ssize_t bytes = recvfrom(sd, msg, sizeof(msg), 0, res->ai_addr, &res->ai_addrlen);
    if (bytes == -1) {
        std::cerr << "recvfrom: " << std::endl;
        return -1;
    }

    std::cout << msg << std::endl;
    return 0;
}