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
#include <mutex>
#include <condition_variable>
// Include estructuras
#include <vector>

// Mutex para la seccion critica
static std::mutex MUTEX;
static std::condition_variable COND;
static bool TERMINA = false;

class Message {
private:
    int sd_;
public:
    // Constructora y destructora
    Message(int sd) : sd_(sd) { };
    ~Message() { };
    // Metodo que maneja mensajes
    void HazMensaje() { 
        // ---------------------------------------------------------------------- //
        // -- PUBLICAR EL SERVIDOR (LISTEN) ------------------------------------- //
        // ---------------------------------------------------------------------- //
        
        int serv = listen(sd_, 16);
        // Control de errores de listen
        if (serv < 0) {
            std::cerr << "listen: " << std::endl;
        }

        // ---------------------------------------------------------------------- //
        // -- GESTION DE LAS CONEXIONES AL SERVIDOR ----------------------------- //
        // ---------------------------------------------------------------------- //
        
        struct sockaddr client_addr;
        socklen_t client_len = sizeof(struct sockaddr);

        char host[NI_MAXHOST];
        char service[NI_MAXSERV];
        
        int sd_client = accept(sd_, &client_addr, &client_len);
        getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
        // Mensade del servidor
        std::cout << "Conexion desde " << host << " " << service << std::endl;

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
                std::unique_lock<std::mutex>lock(MUTEX);
                TERMINA = true;
                COND.notify_one();                
                std::cout << "Conexion terminada" << std::endl;
                return;
            }
            // Mandar el mensaje al cliente
            if (!(buffer[0] == 'Q' && strlen(buffer) <= 2))
                send(sd_client, (void*) buffer, bytes, 0);
            // Mensaje de control
            std::cout << "THREAD: " << std::this_thread::get_id() << std::endl;
            std::cout << "--------------------------" << std::endl;
            // Tiempo de espera para los threads
            sleep(10);
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
    // --- POOL DE THREADS -------------------------------------------------- //
    // ---------------------------------------------------------------------- //
    
    int nThreads = 5;
    std::vector<std::thread> pool;
    for (int i = 0; i < nThreads; ++i) {
        pool.push_back(std::thread([&]() { 
            Message msg(sd);
            msg.HazMensaje();
        }));
    }

    for (int i = 0; i < nThreads; ++i) {
        std::cout << "Thread " << i << ": " << pool.at(i).get_id() << std::endl;
    }
    
    // Mutex
    std::unique_lock<std::mutex> lock(MUTEX);
    COND.wait(lock, [&]() { return TERMINA; });

    for (auto &t : pool) {
        t.detach();
    }

    // Mensaje de desconexion
    std::cout << "Conexion terminada" << std::endl;
    
    // Cerrar el server
    close(sd);

    return 0;
}