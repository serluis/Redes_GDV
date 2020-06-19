#include "MMServer.h"

int main(int argc, char** argv) {
    // Crear el servidor
    MMServer server(argv[1], argv[2]);
    std::cout << "== Servidor alojado en: " << server.getSocket() << " ==\n" << std::endl;
    
    // ---------------------------------------------------------------------- //
    // --- POOL DE THREADS -------------------------------------------------- //
    // ---------------------------------------------------------------------- //
    
    // Cada dos jugadores se meteran en un thread
    std::vector<std::thread> pool;

    // ---------------------------------------------------------------------- //
    // --- GESTION DE LAS CONEXIONES AL SERVIDOR ---------------------------- //
    // ---------------------------------------------------------------------- //
    
    const int MAXPARTIDAS = 2;
    int partidas = 0;
    while (partidas < MAXPARTIDAS) {
        ++partidas;

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
                std::cout << "== Creado thread con: " << *sock_client_one << " y " << *sock_client_two << " ==" << std::endl;
                // Creamos el servidor que maneja la logica
                GameServer MasterMind(server.getSocket(), sock_client_one, sock_client_two);
                // Ejecutamos comprobamos si el juego continua
                while(!MasterMind.getEnd())
                    MasterMind.update();
                std::cout << "== Fin del juego ==" << std::endl;
            }));
        }
        else std::cout << "== Los clientes no se han conectado correctamente ==" << std::endl;

        // Debug de hilos ejecutandose
        std::cout << "== Estan corriendo " << pool.size() << " partidas ==\n" << std::endl;
    }
    
    // Juntar los threads (el programa no termina hasta que no han terminado todos)
    for (auto &t : pool) {
        t.join();
        // Debug join
        std::cout << "== Conexion terminada ==" << std::endl;
    }

    // Mensaje de desconexion
    std::cout << "== Servidor cerrado ==" << std::endl;

    return 0;
}