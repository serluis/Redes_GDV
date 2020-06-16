#include "MMClient.h"

int main(int argc, char** argv) {
    // Crea el cliente
    MMClient client(argv[1], argv[2]);
    // Lo conecta al servidor
    int connection = client.getSocket().connect();

    if (connection != -1) {
        std::cout << "== Cliente conectado ==" << std::endl;
        // Crear el juego para el cliente
        GameClient MasterMind(client.getSocket(), 400, 450);

        while(!MasterMind.getEnd()) {
            MasterMind.update();
        }

        std::cout << "== Fin del juego ==" << std::endl;
    }
    else std::cout << "== Error de conexion ==" << std::endl;
    
    return 0;
}