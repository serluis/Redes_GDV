#include "MMClient.h"

int main(int argc, char** argv) {
    // Crea el cliente
    MMClient client(argv[1], argv[2]);
    
    client.getSocket().connect();

    while (true) {}
    
    return 0;
}