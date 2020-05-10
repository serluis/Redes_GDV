// Sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
// Entrada salida
#include <iostream>
// Includes de threads
#include <thread>
// Include estructuras
#include <vector>

class Message {
private:
    int sd_;
public:
    // Constructora y destructora
    Message(int sd) : sd_(sd) { };
    ~Message() { };
    // Metodo que maneja mensajes
    void HazMensaje(int sd_client) { 
        // ---------------------------------------------------------------------- //
        // -- GESTION DE LA CONEXION CLIENTE ------------------------------------ //
        // ---------------------------------------------------------------------- //
        
        char buffer[80];
        do {
            // Limpiar 
            memset(&buffer, 0, sizeof(buffer));
            // Recepcion del mensaje
            ssize_t bytes = recv(sd_client, (void*) buffer, sizeof(char)*79, 0);
            if (bytes <= 0) {
                // Mensaje de desconexion           
                std::cout << "Conexion terminada" << std::endl;
                std::cout << "THREAD: " << std::this_thread::get_id() << std::endl;
                std::cout << "--------------------------" << std::endl;
                return;
            }
            // Mandar el mensaje al cliente
            if (!(buffer[0] == 'Q' && strlen(buffer) <= 2))
                send(sd_client, (void*) buffer, bytes, 0);
            // Mensaje de control
            buffer[strlen(buffer) - 1] = '\0';
            std::cout << "MENSAJE: " << buffer << std::endl;
            std::cout << "THREAD: " << std::this_thread::get_id() << std::endl;
            std::cout << "--------------------------" << std::endl;
            // Tiempo de espera para los threads
            //sleep(10);
        } while(!(buffer[0] == 'Q' && strlen(buffer) <= 2));
    };
};

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
        std::cerr << "listen: " << std::endl;
    }

    // ---------------------------------------------------------------------- //
    // --- POOL DE THREADS -------------------------------------------------- //
    // ---------------------------------------------------------------------- //
    
    std::vector<std::thread>pool;

    struct sockaddr client_addr;
    socklen_t client_len = sizeof(struct sockaddr);

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    int sd_client;
    do {
        // ---------------------------------------------------------------------- //
        // -- GESTION DE LAS CONEXIONES AL SERVIDOR ----------------------------- //
        // ---------------------------------------------------------------------- //
               
        sd_client = accept(sd, &client_addr, &client_len);
        if (sd_client == -1) {
            std::cerr << "sdclient: " << std::endl;
            return -1;
        }

        getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
        // Mensade del servidor
        std::cout << "Conexion desde " << host << " " << service << std::endl;

        // Threads
        pool.push_back(std::thread([&]() { 
            Message msg(sd);
            msg.HazMensaje(sd_client);
        }));

    } while (sd_client != -1);

    for (auto &t : pool) {
        t.join();
    }

    // Mensaje de desconexion
    std::cout << "Conexion terminada" << std::endl;
    
    // Cerrar el server
    close(sd);

    return 0;
}