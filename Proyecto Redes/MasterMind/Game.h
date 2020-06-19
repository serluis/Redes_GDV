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
    // Booleano para saber si juega el player 1
    bool player1;
    // Contraseña
    std::vector<int> pass = {};
public:
    // Constructora y destructora
    GameServer(Socket sock, Socket* player_ONE, Socket* player_TWO) 
     : server(sock), P1(player_ONE), P2(player_TWO), end(false), player1(true) {
        init();
    };
    ~GameServer() {};

    // Metodos especificos del juego 
    void init();
    std::vector<int> solution(std::vector<int>pass, std::vector<int>guess);

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
    // Turnos y chinchetas
    static const int TURNS = 12;
    static const int PINS = 8;
    // Xlib11
    XLDisplay& dpy;
    // Servidor
    Socket server;
    // Alto y ancho de la pestaña
    int width, heigth;
    // Booleano de fin del juego
    bool end;
    // Quien es el primero en jugar
    bool playFirst;
    // Ronda actual
    int turn;
    // Combinacion que manda por mensajes
    std::vector<int> guess = { 0, 0, 0, 0 };
    // Historico de jugadas
    std::vector<std::vector<int>> game;
public:
    // Constructora y destructora
    GameClient(Socket sock, int w, int h) 
     : server(sock), width(w), heigth(h), end(false),
        dpy(XLDisplay::display()), turn(0) {
        init();
    };
    ~GameClient() {};

    // Metodos especificos del juego 
    void init();
    void drawBoard(XLDisplay& dpy, std::vector<std::vector<int>> part);
    void colorSelector(XLDisplay& dpy, int col);
    void wait(XLDisplay& dpy);
    
    // Metodos tradicionales del juego
    void update();

    // Getters y Setters
    std::vector<int> setGuess(XLDisplay& dpy, int turn);
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
    std::vector<int> getGuess() { return { guess1, guess2, guess3, guess4 }; };
    std::vector<int> getReply() { return { reply1, reply2, reply3, reply4 }; };
    int getEndGame() { return endGame; };
};