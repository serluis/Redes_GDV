// Sockets
#include "Serializable.h"
#include "Socket.h"
// Juego
#include "Game.h"
// Threads
#include <thread>
// Estructuras
#include <vector>

class MMClient {
private:
    Socket serverSocket;
public:
    // COnstructora y destructora
    MMClient(const char *ip, const char *port) : serverSocket(ip, port) { };
    ~MMClient() {};

    // Getters y setters
    Socket getSocket() { return serverSocket; };
};