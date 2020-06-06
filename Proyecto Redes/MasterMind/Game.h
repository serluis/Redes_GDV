#include "XLDisplay.h"

class GameServer {
private:
    // Jugadores
    int P1, P2;
    // Booleano de fin del juego
    bool end;
public:
    // Constructora y destructora
    GameServer(int player_ONE, int player_TWO) 
     : P1(player_ONE), P2(player_TWO), end(false) {
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

class GameClient {
private:
    // Alto y ancho de la pestaña
    int width, heigth;
    // Booleano de fin del juego
    bool end;
    // Ventana
    XLDisplay& dpy;
public:
    // Constructora y destructora
    GameClient(int w, int h) 
     : width(w), heigth(h), end(false) {
        init();
    };
    ~GameClient() {};

    // Metodos especificos del juego 
    void init();

    // Metodos tradicionales del juego
    void handleInput();
    void render();

    // Getters y Setters
    bool getEnd() { return end; };
};