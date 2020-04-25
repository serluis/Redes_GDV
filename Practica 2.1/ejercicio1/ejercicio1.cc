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
    struct addrinfo *res;
    // Lista de posibles conexiones
    struct sockaddr *sa;

    memset(&hints, 0, sizeof(struct addrinfo));

    for (int i = 0; i < 2; ++i) {
        // Los dos tipos de INET
        if (i == 0) hints.ai_family = AF_INET;
        else hints.ai_family = AF_INET6;

        for (int j = 0; j < 3; ++j) {
            // Los tres tipos de SOCKET
            switch (j) {
                case 0: hints.ai_socktype = SOCK_STREAM; break;
                case 1: hints.ai_socktype = SOCK_DGRAM; break;
                case 2: hints.ai_socktype = SOCK_RAW; break;
            }

            // Coger la informacion
            int rc = getaddrinfo(argv[1], NULL, &hints, &res);
            // Control de errores de la informacion
            if (rc != 0) {
                std::cerr << "Error: " << gai_strerror(rc) << std::endl;
                return -1;
            }

            char host[NI_MAXHOST];
            getnameinfo(res->ai_addr, res->ai_addrlen, host, NI_MAXHOST, NULL, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

            // Salida del programa
            std::cout << host << "  " << res->ai_family << "    " << res->ai_socktype << std::endl;
        }
    }
    freeaddrinfo(res);
    return 0;
}