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
    ~Message() {};
    // Metodo que maneja mensajes
    void HazMensaje() {
        // ---------------------------------------------------------------------- //
        // --- RECEPCIÓN MENSAJE DE CLIENTE Y RESPUESTA ------------------------- //
        // ---------------------------------------------------------------------- //
        
        char buffer[80];
        char host[NI_MAXHOST];
        char service[NI_MAXSERV];

        struct sockaddr client_addr;
        socklen_t client_len = sizeof(struct sockaddr);

        do {
            // Libera la memoria del buffer para que no se sobreescriba
            memset(&buffer, 0, sizeof(buffer));
            // Control de errores del buffer
            ssize_t bytes = recvfrom(sd_, buffer, sizeof(buffer), 0, &client_addr, &client_len);
            if (bytes == -1) {
                std::cerr << "recvfrom: " << std::endl;
                return;
            }

            // Coger la informacion
            int err = getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
            // Control de errores en getnameinfo
            if (err != 0) {
                std::cerr << "getnameinfo: " << std::endl;
                return;
            }

            // Tiempo
            time_t now;
            struct tm * structtime;
            time(&now);
            structtime = localtime(&now);
            // Declaracion del mensaje que recibe el cliente
            char msg[sizeof(buffer)];
            memset(&msg, 0, sizeof(msg));
            // MENSAJES
            if (strlen(buffer) <= 2 * sizeof(char)) {
                if (buffer[1] == '\n' || buffer[1] == '\0') {
                    switch (buffer[0]) {
                        case 't': {
                            // Mensaje en el servidor
                            std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
                            // Respuesta al cliente
                            strftime(msg, sizeof(msg), "%T%p", structtime);
                            sendto(sd_, msg, strlen(msg), 0, &client_addr, client_len);
                        } break;
                        case 'd': {
                            // Mensaje en el servidor
                            std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
                            // Respuesta al cliente
                            strftime(msg, sizeof(msg), "%F", structtime);
                            sendto(sd_, msg, strlen(msg), 0, &client_addr, client_len);
                        } break;
                        case 'q': {
                            std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
                            std::cout << "Saliendo..." << std::endl;
                            std::unique_lock<std::mutex>lock(MUTEX);
                            TERMINA = true;
                            COND.notify_one();
                        } break;
                        default: {
                            buffer[strlen(buffer) - 1] = '\0';
                            std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
                            std::cout << "Comando no soportado: " << buffer[0] << std::endl;
                        } break;
                    }
                }
                else {
                    buffer[strlen(buffer) - 1] = '\0';
                    std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
                    std::cout << "Comando no soportado: " << buffer << std::endl;
                }
            }
            else {
                buffer[strlen(buffer) - 1] = '\0';
                std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
                std::cout << "Comando no soportado: " << buffer << std::endl;
            }
            // Mensaje de control
            std::cout << "THREAD: " << std::this_thread::get_id() << std::endl;
            std::cout << "--------------------------" << std::endl;
            // Tiempo de espera para los threads
            sleep(10);
            // Mientras no reciba 'q'
        } while (!(buffer[0] == 'q' && strlen(buffer) <= 2));
    };
};

int main(int argc, char **argv) {
    struct addrinfo hints;
    struct addrinfo * res;

    // ---------------------------------------------------------------------- //
    // --- INICIALIZACIÓN SOCKET & BIND ------------------------------------- //
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

    // Mutex
    std::unique_lock<std::mutex> lock(MUTEX);
    COND.wait(lock, [&]() { return TERMINA; });

    // Juntar todos los hilos
    for (int i = 0; i < pool.size(); ++i) {
        if (pool.at(i).joinable())
            pool.at(i).join();
    }

    // Cerrar el socket
    close(sd);

    return 0;
}