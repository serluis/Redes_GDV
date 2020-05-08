#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <vector>

class TrataMsg{
    private:
    int sd;
    public:
    TrataMsg(int sd):sd(sd){
        //haz_mensaje(sd,std::this_thread::get_id());
    }
    void haz_mensaje(int sd)
    {
    // ---------------------------------------------------------------------- //
    // ------------- RECEPCIÓN MENSAJE DE CLIENTE Y RESPUESTA --------------- //
    // ---------------------------------------------------------------------- //
    char buffer[80];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    struct sockaddr client_addr;
    socklen_t client_len = sizeof(struct sockaddr);
    ssize_t bytes;
    do {
        // Libera la memoria del buffer para que no se sobreescriba
        memset(&buffer, 0, sizeof(buffer));
        // Control de errores del buffer
        bytes = recvfrom(sd, buffer, 79 * sizeof(char), 0, &client_addr, &client_len);
        /*if(!((buffer[0] == 'q' && strlen(buffer) <= 2))){
            std::thread detach;
            return;
        }*/
        if (bytes == -1) {
            std::cerr << "recvfrom: " << std::endl; 
            return;           
        }
        // Coger la informacion
        getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

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
            if (buffer[1] == '\n') {
                switch (buffer[0]) {
                    case 't': {
                        // Mensaje en el servidor
                        std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
                        // Respuesta al cliente
                        strftime(msg, sizeof(msg), "%T%p", structtime);
                        sendto(sd, msg, strlen(msg), 0, &client_addr, client_len);

                    } break;
                    case 'd': {
                         // Mensaje en el servidor
                        std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
                        // Respuesta al cliente
                        strftime(msg, sizeof(msg), "%F", structtime);
                        sendto(sd, msg, strlen(msg), 0, &client_addr, client_len);
                    } break;
                    case 'q': {
                        std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
                        std::cout << "Saliendo..." << std::endl;
                    } break;
                    default: {
                        std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
                        std::cout << "Comando no soportado default" << buffer[0] << std::endl;
                    } break;
                }
            }
            else {
                std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
                    std::cout << "Comando no soportado 1else" << buffer << std::endl;
            }
        }
        else {
            std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
            std::cout << "Comando no soportado 2else " << buffer << std::endl;
        }
        std::cout << "THREAD: " << std::this_thread::get_id() << " IP: " << host << " PUERTO: " << service << " MENSAJE: " << buffer << std::endl;
        sleep(1);
    } while (!((buffer[0] == 'q' && strlen(buffer) <= 2)));
    }
    
};
void envelope(int sd){
    TrataMsg cliente(sd);
    cliente.haz_mensaje(sd);
}

int main(int argc, char **argv)
{
    struct addrinfo hints;
    struct addrinfo * res;
    // ---------------------------------------------------------------------- //
    // INICIALIZACIÓN SOCKET & BIND //
    // ---------------------------------------------------------------------- //

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

    if ( rc != 0 )
    {
        std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
        return -1;
    }
    // res contiene la representación como sockaddr de dirección + puerto

    int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if ( bind(sd, res->ai_addr, res->ai_addrlen) != 0 )
    {
        std::cerr << "bind: " << std::endl;
        return -1;
    }
    freeaddrinfo(res);

    // -------------------------------------------------------------------------
    // POOL DE THREADS
    // -------------------------------------------------------------------------
    std::vector<std::thread> pool;

    for (int i = 0; i < 5 ; ++i)
    {        
        pool.push_back(std::thread(envelope,sd));
        //recibir id del thread y guardarla
    }
    for (auto &t: pool)
    {
        t.join();
    }
    //while <5thread cerrar todos.
    close(sd);
    std::cout << "MAIN THREAD TERMINADO" << std::endl; 
    return 0;
}
