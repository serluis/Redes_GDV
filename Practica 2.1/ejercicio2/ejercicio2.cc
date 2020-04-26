// Includes sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
// Includes tiempo
#include <time.h>
// Includes entrada/salida
#include <iostream>

int main(int argc, char **argv) {
    struct addrinfo hints;
    struct addrinfo * res;

    // ---------------------------------------------------------------------- //
    // INICIALIZACIÓN SOCKET & BIND //
    // ---------------------------------------------------------------------- //

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);
    if (rc != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
        return -1;
    }

    // res contiene la representación como sockaddr de dirección + puerto
    int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (bind(sd, res->ai_addr, res->ai_addrlen) != 0) {
        std::cerr << "bind: " << std::endl;
        return -1;
    }
    freeaddrinfo(res);

    // ---------------------------------------------------------------------- //
    // RECEPCIÓN MENSAJE DE CLIENTE //
    // ---------------------------------------------------------------------- //
    char buffer[80];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    struct sockaddr client_addr;
    socklen_t client_len = sizeof(struct sockaddr);

    ssize_t bytes;
    do {
        bytes = recvfrom(sd, buffer, 79 * sizeof(char), 0, &client_addr, &client_len);
        if (bytes == -1) {
            std::cerr << "recvfrom: " << std::endl;
            return -1;
        }
        if (buffer != "t\n" || buffer != "d\n" || buffer != "q\n") {
            char mess[80] = "Error: introduce t, d, q\n";
            sendto(sd, mess, bytes, 0, &client_addr, client_len);
        }
    } while (buffer != "t\n" || buffer != "d\n" || buffer != "q\n");
    
   

    getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

    std::cout << "IP: " << host << " PUERTO: " << service << " MENSAJE: " << buffer << std::endl;
    // ---------------------------------------------------------------------- //
    // RESPUESTA AL CLIENTE //
    // ---------------------------------------------------------------------- //
    time_t now;
    struct tm *info;
    time(&now);
    //info = localtime(&now);
    //std::cout << asctime(info);
    std::cout << now;
    sendto(sd, buffer, bytes, 0, &client_addr, client_len);

    return 0;
}