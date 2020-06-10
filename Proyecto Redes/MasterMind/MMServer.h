// Sockets
#include "Serializable.h"
#include "Socket.h"
// Juego
#include "Game.h"
// Threads
#include <thread>

class MMServer {
private:
    // Socket del servidor
    Socket socket;
public:
    // Constructora y destructora
    MMServer(const char *ip, const char *port) 
     : socket(ip, port) {
        socket.bind();
    };
    ~MMServer() {};
    
    // Getters y setters
    Socket getSocket() { return socket; };
};