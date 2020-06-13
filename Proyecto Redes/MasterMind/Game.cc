#include <Game.h>

// ---------------------------------------------------------------------- //
// --- SERVER ----------------------------------------------------------- //
// ---------------------------------------------------------------------- //

void GameServer::init() {
    // Crear la contrasena que deberan adivinar
    srand(time(NULL));
    for (int i = 0; i < sizeof(pass); ++i) {
        pass[i] = rand() % 10;
    }

    // Debug de la contrasena
    std::cout << "\n> Password: { " << pass[0] << ", " << pass[1] << ", " << pass[2] << ", " << pass[3] << " };\n" << std::endl;

    // Mandar a cada cliente quien es el primero y quien es el segundo
    Message* msgP1 = new Message(0, 1, 2, 3, 4, 8, 9, 10, 11);
    P1->send(*msgP1);
}

void GameServer::update() {
    std::cout << "UPDATE!" << std::endl;
    end = true;
}

// ---------------------------------------------------------------------- //
// --- CLIENT ----------------------------------------------------------- //
// ---------------------------------------------------------------------- //

void GameClient::init() {
    // Recibir si eres el player one o el player two
    Socket* sock;
    Message* msgTurn = new Message();
    server.recv(*msgTurn, sock);

    std::cout << "Juegas primero: " << msgTurn->getEndGame() << std::endl;
    
    playFirst = !msgTurn->getEndGame();
}

void GameClient::handleInput() {
    std::cout << "HANDLE INPUT" << std::endl;
}

void GameClient::update() {
    std::cout << "UPDATE" << std::endl;
    end = true;
}

void GameClient::render() {
    std::cout << "RENDER" << std::endl;
}

void GameClient::drawBoard(XLDisplay& dpy) {

}

// ---------------------------------------------------------------------- //
// --- MESSAGE ---------------------------------------------------------- //
// ---------------------------------------------------------------------- //

void Message::to_bin() {
    alloc_data(MESSAGE_SIZE);
    memset(_data, 0, MESSAGE_SIZE);
    char *tmp = _data;
    memcpy(tmp, &endGame, sizeof(int));
    tmp += sizeof(int);
    memcpy(tmp, &guess1, sizeof(int));
    tmp += sizeof(int);
    memcpy(tmp, &guess2, sizeof(int));
    tmp += sizeof(int);
    memcpy(tmp, &guess3, sizeof(int));
    tmp += sizeof(int);
    memcpy(tmp, &guess4, sizeof(int));
    tmp += sizeof(int);
    memcpy(tmp, &reply1, sizeof(int));
    tmp += sizeof(int);
    memcpy(tmp, &reply2, sizeof(int));
    tmp += sizeof(int);
    memcpy(tmp, &reply3, sizeof(int));
    tmp += sizeof(int);
    memcpy(tmp, &reply4, sizeof(int));
}

int Message::from_bin(char * bobj) {
    alloc_data(MESSAGE_SIZE);
    char* tmp = bobj;
    memcpy(&endGame, tmp, sizeof(int));
    tmp += sizeof(int);
    memcpy(&guess1, tmp, sizeof(int));
    tmp += sizeof(int);
    memcpy(&guess2, tmp, sizeof(int));
    tmp += sizeof(int);
    memcpy(&guess3, tmp, sizeof(int));
    tmp += sizeof(int);
    memcpy(&guess4, tmp, sizeof(int));
    tmp += sizeof(int);
    memcpy(&reply1, tmp, sizeof(int));
    tmp += sizeof(int);
    memcpy(&reply2, tmp, sizeof(int));
    tmp += sizeof(int);
    memcpy(&reply3, tmp, sizeof(int));
    tmp += sizeof(int);
    memcpy(&reply4, tmp, sizeof(int));

    return 0;
}