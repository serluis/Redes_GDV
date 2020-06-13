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
    int endGame;
    int guess1, guess2, guess3, guess4;
    int reply1, reply2, reply3, reply4;
public:
    // Tamaño del mensaje
    static const size_t MESSAGE_SIZE = 9 * sizeof(int);
    
    // Constructora y destructora
    Message() {};
    Message(const int& end, const int& g1 = 11, const int& g2 = 11, const int& g3 = 11, const int& g4 = 11, 
    const int& r1 = 11, const int& r2 = 11, const int& r3 = 11, const int& r4 = 11)
     : endGame(end), guess1(g1), guess2(g2), guess3(g3), guess4(g4),
        reply1(r1), reply2(r2), reply3(r3), reply4(r4) {};
    ~Message() {};

    // Serializar: Serializar los campos type, nick y message en el buffer _data
    void to_bin();
    // Deserializar: Reconstruir la clase usando el buffer _data
    int from_bin(char * bobj);

    // Getters y setters
    //int* getGuess() { int i[] = { guess1, guess2, guess3, guess4 }; return i; };
    //int* getReply() { int i[] = { reply1, reply2, reply3, reply4 }; return i; };
    int getEndGame() { return endGame; };
};