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
    std::vector<int> test = {11, 11, 11, 11};
    Message* msgP1 = new Message(0, test, test);
    server.send(*msgP1, *P1);

    Message* msgP2 = new Message(1, test, test);
    server.send(*msgP1, *P2);
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

    std::vector<int>guess = msgTurn->getGuess();
    std::vector<int>reply = msgTurn->getReply();

    std::cout << "Juegas primero: " << msgTurn->getEndGame() 
        /*<< ", Guess: " << guess[0] << guess[1] << guess[2] << guess[3]
        << ", Reply : " << reply[0] << reply[1] << reply[2] << reply[3] */<< std::endl;
    
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
    memcpy(tmp, &guess, sizeof(guess));
    tmp += 4 * sizeof(int);
    memcpy(tmp, &reply, sizeof(reply));
}

int Message::from_bin(char * bobj) {
    alloc_data(MESSAGE_SIZE);
    char* tmp = bobj;
    memcpy(&endGame, tmp, sizeof(int));
    tmp += sizeof(int);
    memcpy(&guess, tmp, sizeof(guess));
    tmp += 4 * sizeof(int);
    memcpy(&reply, tmp, 4 * sizeof(reply));

    return 0;
}