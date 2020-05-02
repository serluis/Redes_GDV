// Sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
// Entrada salida
#include <stdio.h>
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

    getnameinfo(res->ai_addr, res->ai_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
    // Conectar al servidor
    int conn = connect(sd, res->ai_addr, res->ai_addrlen);
    if (conn != 0) {
        std::cerr << "connection failed: " << std::endl;
    }
    
    // ---------------------------------------------------------------------- //
    // -- GESTION DE LA CONEXION CLIENTE ------------------------------------ //
    // ---------------------------------------------------------------------- //
    
    char msg[80];
    
    do {
        // Limpiar el buffer
        memset(&msg, 0, sizeof(msg));
        // Input del usuario
        gets(msg);
        // Envio del mesaje con el parametro especificado
        send(sd, msg, strlen(msg), 0);

        ssize_t bytes = recvfrom(sd, msg, sizeof(msg), 0, res->ai_addr, &res->ai_addrlen);
        if (bytes == -1) {
            std::cerr << "recvfrom: " << std::endl;
            return -1;
        }
        // Output del servidor
        if (!(msg[0] == 'Q' && strlen(msg) <= 2))
        std::cout << msg << std::endl;
    } while (!(msg[0] == 'Q' && strlen(msg) <= 2));

    // Cerrar el cliente
    close(sd);

    return 0;
}