// Sockets
#include "Serializable.h"
#include "Socket.h"
// Juego
#include "Game.h"

class MMClient {
private:
    Socket server;
public:
    // COnstructora y destructora
    MMClient(const char *ip, const char *port) : server(ip, port) { };
    ~MMClient() {};

    // Getters y setters
    Socket getSocket() { return server; };
};