#include <Game.h>

// ---------------------------------------------------------------------- //
// --- SERVER ----------------------------------------------------------- //
// ---------------------------------------------------------------------- //

void GameServer::init() {
    // Crear la contrasena que deberan adivinar
    srand(time(NULL));
    for (int i = 0; i < 4; ++i) {
        pass.push_back(rand() % 10);
    }

    // Debug de la contrasena
    std::cout << "\n> Password: { " << pass.at(0) << ", " << pass.at(1) << ", " << pass.at(2) << ", " << pass.at(3) << " };\n" << std::endl;

    // Mandar a cada cliente quien es el primero y quien es el segundo
    Message* msgP1 = new Message(1);
    P1->send(*msgP1);
    Message* msgP2 = new Message(0);
    P2->send(*msgP2);
}

void GameServer::update() {
    // Recibir la propuesta del jugador correspondiente
    Socket* sock;
    Message *msgRecv = new Message();
    if (player1)
        P1->recv(*msgRecv, sock);
    else P2->recv(*msgRecv, sock);

    // Debug pass y guess
    std::cout << "\n> Password: { " << pass.at(0) << ", " << pass.at(1) << ", " << pass.at(2) << ", " << pass.at(3) << " };" << std::endl;
    std::cout << "> Guess: { " << msgRecv->getGuess().at(0) << ", " << msgRecv->getGuess().at(1) << ", " << msgRecv->getGuess().at(2) << ", " << msgRecv->getGuess().at(3) << " };\n" << std::endl;

    // Comprobar si la solucion es correcta: LINEA DE TURNO
    std::vector<int>lt = solution(pass, msgRecv->getGuess());
    

    std::cout << "> linea: { ";
    for(int i = 0; i < 9; ++i)
        std::cout << lt.at(i) << " ";
    std::cout << "};\n" << std::endl;

    Message* msgP1 = new Message(lt.at(0), lt.at(1), lt.at(2), lt.at(3), lt.at(4), lt.at(5), lt.at(6), lt.at(7), lt.at(8));
    P1->send(*msgP1);
    Message* msgP2 = new Message(lt.at(0), lt.at(1), lt.at(2), lt.at(3), lt.at(4), lt.at(5), lt.at(6), lt.at(7), lt.at(8));
    P2->send(*msgP2);

    // Ahora el turno es del otro jugador
    player1 = !player1;
}

std::vector<int> GameServer::solution(std::vector<int> pass, std::vector<int> guess) {
    // Solucion auxiliar
    std::vector<int> sol = { 0, 11, 11, 11, 11, 11, 11, 11, 11 };
    
    // Mete la prueba del jugador a la respuesta
    for (int i = 0; i < 4; i++){
        sol.at(i + 1) = guess.at(i);
    }

    // En mala posicion
    if (guess.at(0) == pass.at(1) || guess.at(0) == pass.at(2) || guess.at(0) == pass.at(3)){
        sol.at(5) = 9;
    }
    if (guess.at(1) == pass.at(0) || guess.at(1) == pass.at(2) || guess.at(1) == pass.at(3)){
        sol.at(6) = 9;
    }
    if (guess.at(2) == pass.at(0) || guess.at(2) == pass.at(1) || guess.at(2) == pass.at(3)){
        sol.at(7) = 9;
    }
    if (guess.at(3) == pass.at(0) || guess.at(3) == pass.at(1) || guess.at(3) == pass.at(2)) {
        sol.at(8) = 9;
    }

    // Correctos
    if (pass.at(0) == guess.at(0)){
        sol.at(5) = 8;
    }
    if (pass.at(1) == guess.at(1)){
        sol.at(6) = 8;
    }
    if (pass.at(2) == guess.at(2)){
        sol.at(7) = 8;
    }
    if (pass.at(3) == guess.at(3)){
        sol.at(8) = 8;
    }
    // Fin de partida correcta
    if(pass.at(0) == guess.at(0) && pass.at(1) == guess.at(1) && pass.at(2) == guess.at(2) && pass.at(3) == guess.at(3)) {
        sol.at(0) = 1;
    }

    return sol;
}
// ---------------------------------------------------------------------- //
// --- CLIENT ----------------------------------------------------------- //
// ---------------------------------------------------------------------- //

void GameClient::init() {
    // Crear el vector historico de jugadas
    game = std::vector<std::vector<int>>(TURNS);
    // Llenar el historico de jugadas
    for (int i = 0; i < game.size(); i++) {
		game.at(i).resize(PINS);
		for (int j = 0; j < game.at(i).size(); j++)
			game.at(i).at(j) = 11;
	}

    // Pintar el tablero
    XLDisplay::init(width, heigth, "MasterMind");
    // Limpiar la pantalla
    dpy.clear();
    // Pintar la partida
    drawBoard(dpy, game);

    // Recibir si eres el player one o el player two
    Socket* sock;
    Message* msgTurn = new Message();
    server.recv(*msgTurn, sock);

    std::cout << "Juegas primero: " << msgTurn->getEndGame() << std::endl;
    
    // Asigna si juegas primero 
    playFirst = msgTurn->getEndGame();
}

void GameClient::update() {
    if (turn < 12) {
        if (playFirst) {
            guess = setGuess(dpy, turn);

            // Serielizar el objeto guess y mandarlo al servidor
            Message* msgSend = new Message(0, guess.at(0), guess.at(1), guess.at(2), guess.at(3));
            server.send(*msgSend);
            // Recibir el mensaje del servidor con las correctas e incorrectas
            Socket* sock;
            Message* msgRecv = new Message();
            server.recv(*msgRecv, sock);

            // Actualiza el historico
            end = msgRecv->getEndGame();
            std::vector<int> rp = msgRecv->getReply();

            for(int h = 0; h < 4; h++) {
                game.at(turn).at(h) = guess.at(h);
                game.at(turn).at(h + 4) = rp.at(h);
            }

            // Debug de la ronda
            std::cout << "> Ronda tuya: ";
            for(int h = 0; h < 8; h++)
                std::cout << game.at(turn).at(h) << " ";
            std::cout << std::endl;

            // Limpiar la pantalla entera porque hay que volver a renderizarla
            dpy.clear();
            drawBoard(dpy, game);

            if (end) {
                // Mensaje por consola
                std::cout << "> HAS GANADO! " << std::endl;
                // Mensaje por ventana del juego
                dpy.set_color(XLDisplay::BLUE);
                dpy.text(125, 150, "TU GANAS!");
                dpy.text(125, 175, "PRESIONA Q PARA SALIR");
                dpy.flush();
                wait(dpy);
            }
        }
        else {
            // Recibir el mensaje del servidor con las correctas e incorrectas
            Socket* sock;
            Message* msgRecv = new Message();
            server.recv(*msgRecv, sock);

            // Actualiza el historico
            end = msgRecv->getEndGame();
            std::vector<int> gs = msgRecv->getGuess();
            std::vector<int> rp = msgRecv->getReply();

            for(int h = 0; h < 4; h++) {
                game.at(turn).at(h) = gs.at(h);
                game.at(turn).at(h + 4) = rp.at(h);
            }

            // Debug de la ronda
            std::cout << "> Ronda otro: ";
            for(int h = 0; h < 8; h++)
                std::cout << game.at(turn).at(h) << " ";
            std::cout << std::endl;

            // Limpiar la pantalla entera porque hay que volver a renderizarla
            dpy.clear();
            drawBoard(dpy, game);

            if (end) {
                // Mensaje por consola
                std::cout << "> TU PIERDES! " << std::endl;
                // Mensaje por ventana del juego
                dpy.set_color(XLDisplay::BLUE);
                dpy.text(125, 150, "TU PIERDES!");
                dpy.text(125, 175, "PRESIONA Q PARA SALIR");
                dpy.flush();
                wait(dpy);
            }
        }
    }
    
    ++turn;
    if (turn == 13) {
        // Fin del juego
        end = true;
        // Mensaje por consola
        std::cout << "> TABLAS! NADIE HA GANADO " << std::endl;
        // Mensaje por ventana del juego
        dpy.set_color(XLDisplay::BLUE);
        dpy.text(125, 150, "TABLAS! NADIE HA GANADO");
        dpy.text(125, 175, "PRESIONA Q PARA SALIR");
        dpy.flush();
        wait(dpy);
    }
    playFirst = !playFirst;
}

void GameClient::wait(XLDisplay& dpy) {
    char k;
    do {
        k = dpy.wait_key();
    } while (k != 'q');

    dpy.clear();
    dpy.flush();

    sleep(1);
}

std::vector<int> GameClient::setGuess(XLDisplay& dpy, int turn) {
    // Booleano de envio de la contrasena
    bool acabado = false;
    // Auxiliar para el cursor
    int sel = 0;

    // Poner los circulos de color rojo con borde negro
    for(int i = 0; i < 4; i++) {
        if (i == 0) {
            dpy.set_color(XLDisplay::BLACK);
            dpy.circleEdge(45 * (i + 1), 35 * (turn + 1), 10);
        }
        dpy.set_color(XLDisplay::RED);
        dpy.circle(45 * (i + 1), 35 * (turn + 1), 10);
    }

    // Return auxiliar de la combinacion
    std::vector<int> comb = {0,0,0,0};
    // Auxiliar para seleccionar la chincheta
    int pos = 0;

    dpy.set_color(XLDisplay::BLACK);
    dpy.circle(45 * (pos + 1), 35 * (turn + 1), 12);
    dpy.circleEdge(45 * (pos + 1), 35 * (turn + 1), 12);

    dpy.set_color(XLDisplay::RED);
    dpy.circle(45 * (pos + 1), 35 * (turn + 1), 10);

    while(!acabado) {
        // Pide una tecla
        char dir = dpy.wait_key();
        switch (dir) {
            case 'w': { if(comb[pos] == 9)  comb[pos] = 0;  else comb[pos]++;   } break;
            case 's': { if(comb[pos] == 0)  comb[pos] = 9;  else comb[pos]--;   } break;
            case 'a': { 
                if (pos == 0)   pos = 3;    else pos--;
                if (sel == 0)   sel = 3;    else sel--;
            } break;
            case 'd': { 
                if (pos == 3)   pos = 0;    else pos++; 
                if (sel == 3)   sel = 0;    else sel++;
            } break;
            // Tecla del intro
            case 'e': { acabado = true; } break;
            default: break;
        }

        // Coloreado del cursor
        for (int i = 0; i < 4; ++i) {
            if (i == pos) 
                dpy.set_color(XLDisplay::BLACK);
            else dpy.set_color(XLDisplay::PERU);

            dpy.circle(45 * (i + 1), 35 * (turn + 1), 12);
            dpy.circleEdge(45 * (i + 1), 35 * (turn + 1), 12);
        }

        for (int i = 0; i < 4; ++i) { 
            colorSelector(dpy, comb[i]);
            dpy.circle(45 * (i + 1), 35 * (turn + 1), 10);
            dpy.set_color(XLDisplay::BLACK);
            dpy.circleEdge(45 * (i + 1), 35 * (turn + 1), 10);
        }
    }
    dpy.flush();
    return comb;
}

void GameClient::colorSelector(XLDisplay& dpy, int col) {
    switch (col) {
        case 0: { dpy.set_color(XLDisplay::RED);     }   break;
        case 1: { dpy.set_color(XLDisplay::BROWN);   }   break;
        case 2: { dpy.set_color(XLDisplay::BLUE);    }   break;
        case 3: { dpy.set_color(XLDisplay::YELLOW);  }   break;
        case 4: { dpy.set_color(XLDisplay::GREEN);   }   break;
        case 5: { dpy.set_color(XLDisplay::PURPLE);  }   break;
        case 6: { dpy.set_color(XLDisplay::ORANGE);  }   break;
        case 7: { dpy.set_color(XLDisplay::FUCHSIA); }   break;
        case 8: { dpy.set_color(XLDisplay::WHITE);   }   break;
        case 9: { dpy.set_color(XLDisplay::BLACK);   }   break;
        case 10: { dpy.set_color(XLDisplay::PERU);   }   break;
        case 11: { dpy.set_color(XLDisplay::SIENNA); }   break;
        default: break;
    }
}
void GameClient::drawBoard(XLDisplay& dpy, std::vector<std::vector<int>> part) {
    // Dibujar el fondo del tablero
    dpy.set_color(XLDisplay::PERU);
    dpy.rectangleFill(10,10,380,440);
    // Decorativo exterior 1
    dpy.set_color(XLDisplay::GREEN);
    dpy.rectangle(10,10,380,440);
    // Decorativo exterior 2
    dpy.set_color(XLDisplay::RED);
    dpy.rectangle(12,12,376,436);

    for (int i = 0; i < part.size(); i++) {
		for (int j = 0; j < part.at(i).size(); j++) {
            // Selector de color
            colorSelector(dpy, part.at(i).at(j));
            // Si son las chinchetas guess
            if(j < 4){
                // Posicion donde se dibuja. Tamano = 10
                dpy.circle(45 * (j + 1), 35 * (i + 1), 10);
                // Reborde de la chincheta
                dpy.set_color(XLDisplay::BLACK);
                dpy.circleEdge(45 * (j + 1), 35 * (i + 1), 10);
            }
            // Si son las chinchetas reply
            else {
                // Posicion donde se dibuja. Tamano = 5
                dpy.circle(150 + (j + 1) * 20, 35 * (i + 1), 5);
                dpy.circleEdge(150 + (j + 1) * 20, 35 * (i + 1), 5);
            }
		}
	}
    dpy.flush();
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