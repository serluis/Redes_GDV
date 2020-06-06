// Sockets
#include "Serializable.h"
#include "Socket.h"
// Juego
#include "Game.h"
// Threads
#include <thread>
// Estructuras
#include <vector>

class MMServer {
private:
    // 2 clientes
    Socket* client_one;
    Socket* client_two;
    // Socket del servidor
    Socket socket;
public:
    // Constructora y destructora
    MMServer(const char *ip, const char *port) : socket(ip, port) {
        socket.bind();
    };
    ~MMServer() {};
    
    // Getters y setters
    Socket getSocket() { return socket; };
};