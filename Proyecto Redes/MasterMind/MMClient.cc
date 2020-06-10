#include "MMClient.h"

int main(int argc, char** argv) {
    // Crea el cliente
    MMClient client(argv[1], argv[2]);
    // Lo conecta al servidor
    client.getSocket().connect();

    // 
    
    return 0;
}