#include "MMServer.h"

int main(int argc, char** argv) {
    // Crear el servidor
    MMServer server(argv[1], argv[2]);
    std::cout << "== Servidor alojado en: " << server.getSocket() << " ==" << std::endl;
    
    // ---------------------------------------------------------------------- //
    // --- POOL DE THREADS -------------------------------------------------- //
    // ---------------------------------------------------------------------- //
    
    // Cada dos jugadores se meteran en un thread
    std::vector<std::thread> pool;

    // ---------------------------------------------------------------------- //
    // --- GESTION DE LAS CONEXIONES AL SERVIDOR ---------------------------- //
    // ---------------------------------------------------------------------- //
    
    const int PARTIDAS = 2;
    int partida = 0;
    /*while (partida < PARTIDAS) {
        partida++;*/
        // Aceptar cliente 1
        Socket* sock_client_one = server.getSocket().accept();
        std::cout << "== Cliente 1 conectado desde: " << *sock_client_one << " ==" << std::endl;
        // Aceptar cliente 2
        Socket* sock_client_two = server.getSocket().accept();
        std::cout << "== Cliente 2 conectado desde: " << *sock_client_two << " ==" << std::endl;

        // Si los clientes no han dado errores, crear el hilo del juego
        if (sock_client_one != nullptr && sock_client_two != nullptr) {
            // Hilo con el bucle principal del juego
            pool.push_back(std::thread([&]() {
                // Creamos el servidor que maneja la logica
                GameServer MasterMind(server.getSocket(), sock_client_one, sock_client_two);
                // Ejecutamos comprobamos si el juego continua
                while(!MasterMind.getEnd())
                    MasterMind.update();
                std::cout << "== Fin del juego ==" << std::endl;
            }));
        }
        else std::cout << "== Los clientes no se han conectado correctamente ==" << std::endl;

        // Juntar los threads (el programa no termina hasta que no han terminado todos)
        for (auto &t : pool)
            t.join();
            
        // Mensaje de desconexion
        std::cout << "== Conexion terminada ==" << std::endl;
    //}

    return 0;
}