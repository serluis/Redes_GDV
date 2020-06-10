// XLIB11
#include "XLDisplay.h"
// Serilaizacion
#include "Serializable.h"
#include "Socket.h"

// ------------------------------------------------------------------------------ //
// --- Servidor ----------------------------------------------------------------- //
// ------------------------------------------------------------------------------ //

class GameServer {
private:
    // Servidor
    Socket server;
    // Jugadores
    Socket *P1, *P2;
    // Booleano de fin del juego
    bool end;
    // Contraseña
    int pass[4] = {};
public:
    // Constructora y destructora
    GameServer(Socket sock, Socket* player_ONE, Socket* player_TWO) 
     : server(sock), P1(player_ONE), P2(player_TWO), end(false) {
        init();
    };
    ~GameServer() {};

    // Metodos especificos del juego 
    void init();

    // Metodos tradicionales del juego
    void update();

    // Getters y Setters
    bool getEnd() { return end; };
};

// ------------------------------------------------------------------------------ //
// --- Cliente ------------------------------------------------------------------ //
// ------------------------------------------------------------------------------ //

class GameClient {
private:
    // Alto y ancho de la pestaña
    int width, heigth;
    // Booleano de fin del juego
    bool end;
public:
    // Constructora y destructora
    GameClient(int w, int h) 
     : width(w), heigth(h), end(false) {
        init();
    };
    ~GameClient() {};

    // Metodos especificos del juego 
    void init();
    void drawBoard(XLDisplay& dpy);

    // Metodos tradicionales del juego
    void handleInput();
    void render();

    // Getters y Setters
    bool getEnd() { return end; };
};

// ------------------------------------------------------------------------------ //
// --- Mensajes entre el cliente y el servidor ---------------------------------- //
// ------------------------------------------------------------------------------ //

class Message : public Serializable {
private:
    int* guess;
    int* reply;
    int endGame;
public:
    // Tamaño del mensaje
    static const size_t MESSAGE_SIZE = sizeof(int) * 9;
    
    // Constructora y destructora
    Message() {};
    Message(int end, int gue[4] = {}, int rep[4] = {})
     : endGame(end), guess(gue), reply(rep) {};
    ~Message() {};

    // Serializar: Serializar los campos type, nick y message en el buffer _data
    void to_bin();
    // Deserializar: Reconstruir la clase usando el buffer _data
    int from_bin(char * bobj);
};