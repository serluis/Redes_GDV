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
    // Servidor
    Socket server;
    // Alto y ancho de la pestaña
    int width, heigth;
    // Booleano de fin del juego
    bool end;
    // Quien es el primero en jugar
    bool playFirst;
public:
    // Constructora y destructora
    GameClient(Socket sock, int w, int h) 
     : server(sock), width(w), heigth(h), end(false) {
        init();
    };
    ~GameClient() {};

    // Metodos especificos del juego 
    void init();
    void drawBoard(XLDisplay& dpy);

    // Metodos tradicionales del juego
    void handleInput();
    void render();
    void update();

    // Getters y Setters
    bool getEnd() { return end; };
};

// ------------------------------------------------------------------------------ //
// --- Mensajes entre el cliente y el servidor ---------------------------------- //
// ------------------------------------------------------------------------------ //

class Message : public Serializable {
private:
    std::vector<int> guess;
    std::vector<int> reply;
    int endGame;
public:
    // Tamaño del mensaje
    static const size_t MESSAGE_SIZE = sizeof(int) * 9;
    
    // Constructora y destructora
    Message() {};
    Message(const int& end, std::vector<int>& gue, std::vector<int> rep)
     : endGame(end), guess(gue), reply(rep) {};
    ~Message() {};

    // Serializar: Serializar los campos type, nick y message en el buffer _data
    void to_bin();
    // Deserializar: Reconstruir la clase usando el buffer _data
    int from_bin(char * bobj);

    // Getters y setters
    std::vector<int> getGuess() { return guess; };
    std::vector<int> getReply() { return reply; };
    int getEndGame() { return endGame; };
};