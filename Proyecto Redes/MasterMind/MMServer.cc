#include "MMServer.h"

int main(int argc, char** argv) {
    // Crear el servidor
    MMServer server(argv[1], argv[2]);

    // ---------------------------------------------------------------------- //
    // --- POOL DE THREADS -------------------------------------------------- //
    // ---------------------------------------------------------------------- //
    // Cada dos jugadores se meteran en un thread
    std::vector<std::thread> pool;

    // ---------------------------------------------------------------------- //
    // --- GESTION DE LAS CONEXIONES AL SERVIDOR ---------------------------- //
    // ---------------------------------------------------------------------- //
    //do {
        int sd_client_one = server.getSocket().accept();
        std::cout << sd_client_one << std::endl;
        int sd_client_two = server.getSocket().accept();
        std::cout << sd_client_two << std::endl;

        pool.push_back(std::thread([&]() {
            GameServer MasterMind(sd_client_one, sd_client_two);
            int i = 0;
            do {
                std::cout << "iteration i = " << i << std::endl; ++i;
                MasterMind.update();
            } while(i <= 100);
        }));
    //} while (true);

    // Juntar los threads (el programa no termina hasta que no han terminado todos)
    for (auto &t : pool)
        t.join();
    // Mensaje de desconexion
    std::cout << "Conexion terminada" << std::endl;

    return 0;
}