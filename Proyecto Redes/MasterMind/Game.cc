#include <Game.h>

void GameServer::init() { }
void GameServer::update() {}

void GameClient::init() {
    XLDisplay::init(width, heigth, "MasterMind");
}

void GameClient::render() {
    XLDisplay& dpy = XLDisplay::display();
    // Dibuja el tablero
    drawBoard(dpy);
}

void GameClient::drawBoard(XLDisplay& dpy) {
    // Color del fondo del tablero
    dpy.set_color(XLDisplay::PERU);
    dpy.rectangleFill(10, 10, 380, 440);
    
    // Color del borde interior del tablero
    dpy.set_color(XLDisplay::GREEN);
    dpy.rectangle(10, 10, 380, 440);

    // Color del borde exterior del tablero
    dpy.set_color(XLDisplay::RED);
    dpy.rectangle(12, 12, 376, 436);


    // Texto que pone MasterMind en el tablero
    dpy.set_color(XLDisplay::SIENNA);
    dpy.text(150, 25, "MasterMind");

    // dibujar los circulos
    dpy.set_color(XLDisplay::SIENNA);
    for(int i = 0; i < 12; ++i) {
        for(int j = 0; j < 4; ++j) {
            dpy.circle(45 * (j + 1), 35 * (i + 1), 10);
            dpy.circle(200 + (j + 1) * 20, 35 * (i + 1), 5);
        }
    }
    
    dpy.flush();
}

void Message::to_bin() {
    alloc_data(MESSAGE_SIZE);
    memset(_data, 0, MESSAGE_SIZE);
    char *tmp = _data;
    memcpy(tmp, guess, sizeof(guess));
    tmp += 4 * sizeof(int);
    memcpy(tmp, reply, sizeof(reply));
    tmp += 4 * sizeof(int);
    memcpy(tmp, &endGame, sizeof(int));
}

int Message::from_bin(char * bobj) {
    alloc_data(MESSAGE_SIZE);
    char* tmp = bobj;
    memcpy(guess, tmp, sizeof(guess));
    tmp += 4 * sizeof(char);
    memcpy(reply, tmp, 4 * sizeof(int));
    tmp += 4 * sizeof(char);
    memcpy(&endGame, tmp, sizeof(int));
    return 0;
}