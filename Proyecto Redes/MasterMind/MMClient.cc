#include "MMClient.h"

int main(int argc, char** argv) {
    // Crea el cliente
    MMClient client(argv[1], argv[2]);
    // Lo conecta al servidor
    client.getSocket().connect();

    // Crear el juego para el cliente
    GameClient MasterMind(client.getSocket(), 400, 450);

    while(!MasterMind.getEnd()) {
        MasterMind.handleInput();
        MasterMind.update();
        MasterMind.render();
    }
    
    return 0;
}