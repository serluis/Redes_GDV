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
    hints.ai_family;
    hints.ai_socktype;

    // Coger la info del servidor
    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);
    // Control de errores de getaddrinfo
    if (rc != 0) {
        std::cerr << "getaddrinfo:" << gai_strerror(rc) << std::endl;
        return -1;
    }

    // Creacion del socket
    int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    
    // Conexion y control de errores
    if (bind(sd, res->ai_addr, res->ai_addrlen) != 0) {
        std::cerr << "bind: " << std::endl;
        return -1;
    }

    freeaddrinfo(res);

    // ---------------------------------------------------------------------- //
    // -- PUBLICAR EL SERVIDOR (LISTEN) ------------------------------------- //
    // ---------------------------------------------------------------------- //
    
    int serv = listen(sd, 16);
    // Control de errores de listen
    if (serv < 0) {
        std::cerr << "error de listen: " << std::endl;
    }

    // ---------------------------------------------------------------------- //
    // -- GESTION DE LAS CONEXIONES AL SERVIDOR ----------------------------- //
    // ---------------------------------------------------------------------- //
    
    struct sockaddr client_addr;
    socklen_t client_len = sizeof(struct sockaddr);

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    
    int sd_client = accept(sd, &client_addr, &client_len);
    getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
    // Mensade del servidor
    std::cout << "Conexion desde " << host << " " << service << std::endl;

    // ---------------------------------------------------------------------- //
    // -- GESTION DE LA CONEXION CLIENTE ------------------------------------ //
    // ---------------------------------------------------------------------- //
    
    char buffer[80];
    ssize_t bytes;
    do {
        // Limpiar 
        memset(&buffer, 0, sizeof(buffer));
        // Recepcion del mensaje
        bytes = recv(sd_client, (void *) buffer, sizeof(char)*79, 0);
        if (bytes <= 0) {
                // Mensaje de desconexion
    std::cout << "Conexion terminada" << std::endl;
            return 0;
        }
        // Mandar el mensaje al cliente
        if (!(buffer[0] == 'Q' && strlen(buffer) <= 2))
            send(sd_client, (void *) buffer, bytes, 0);
    } while(!(buffer[0] == 'Q' && strlen(buffer) <= 2));
    
    // Mensaje de desconexion
    std::cout << "Conexion terminada" << std::endl;
    
    return 0;
}