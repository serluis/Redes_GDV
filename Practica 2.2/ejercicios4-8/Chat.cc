// Dependencias de otras clases
#include "Chat.h"
// Serializar: Serializar los campos type, nick y message en el buffer _data
void ChatMessage::to_bin() {
    alloc_data(MESSAGE_SIZE);
    // Poner a 0 todos los datos
    memset(_data, 0, MESSAGE_SIZE);
    // Apuntar un puntero a data
    char *tmp = _data;
    // Copia en tmp lo que ponga en type
    memcpy(tmp, &type, sizeof(uint8_t));
    // Mueve manualmente el puntero 1 posicion
    tmp += sizeof(uint8_t);
    // Copia en tmp lo que ponga en nick
    memcpy(tmp, nick.c_str(), strlen(nick.c_str())); // 7 * sizeof(char)
    // Mueve manualmente el puntero 8 posiciones (la 8 sera para '\0')
    tmp += 8 * sizeof(char);
    // Copia en tmp lo que ponga en message
    memcpy(tmp, message.c_str(), strlen(message.c_str()));
}
// Deserializar: Reconstruir la clase usando el buffer _data
int ChatMessage::from_bin(char * bobj) {
    alloc_data(MESSAGE_SIZE);
    // Apunta al primer dato
    char* tmp = bobj;
    // Copia en type lo que ponga en tmp
    memcpy(&type, tmp, sizeof(int8_t));
    // Mueve manualmente el puntero 1 posicion
    tmp += sizeof(int8_t);
    // Copia en nick lo que ponga en tmp
    char name[8];
    memcpy(name, tmp, 8 * sizeof(char));
    nick = name;
    // Mueve manualmente el puntero 8 posiciones
    tmp += 8 * sizeof(char);
    // Copia en message lo que ponga en tmp
    char msg[80];
    memcpy(msg, tmp, 80 * sizeof(char));
    message = msg;
    // Control de errores de copiar los datos
    if (nick.length() + message.length() < 0) 
        return -1;
    return nick.length() + message.length();
}

// ----------------------------------------------------------------------------- //
// --- Chat Server y Client ---------------------------------------------------- //
// ----------------------------------------------------------------------------- //

void ChatServer::do_messages() {
    while (true) {
        // Crear un mensaje vacio que va a rellenar el recv()
        ChatMessage* msg = new ChatMessage();

        Socket *sock;
        // Recibir Mensajes en y en función del tipo de mensaje
        socket.recv(*msg, sock);

        switch(msg->type) {
            // - LOGIN: Añadir al vector clients
            case ChatMessage::LOGIN: {
                clients.push_back(sock);
                std::cout << "<'"<< msg->nick << "' se ha unido al chat>" << std::endl;
            } break;
            // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)
            case ChatMessage::MESSAGE: {
                std::cout << msg->nick << ": " << msg->message << std::endl;
                for (int i = 0; i < clients.size(); ++i) {
                    if (!(*sock == *clients.at(i)))
                        socket.send(*msg, *clients.at(i));
                }
            } break;
            // - LOGOUT: Eliminar del vector clients
            case ChatMessage::LOGOUT: {
                std::cout << "<'"<< msg->nick << "' se ha ido del chat>" << std::endl;
                bool erased = false;
                std::vector<Socket*>::iterator it = clients.begin();
                for (int i = 0; i < clients.size() && !erased; ++i) {
                    if (*sock == *clients.at(i)) {
                        clients.erase(it);
                        erased = true;
                    }
                    ++it;
                }
            } break;
            default: { 
                std::cout << "no msg: " << std::endl;
            } break;
        }
    }
}

void ChatClient::login() {
    // Mensaje del login
    std::string msg = "== Usuario '" + nick + "' se ha registrado. ==";
    // LO muestra por pantalla
    std::cout << msg << std::endl;
    // Crea un nuevo mensaje
    ChatMessage em(nick, msg);
    // Que sera de tpo LOGIN
    em.type = ChatMessage::LOGIN;
    // Y lo envia al servidor, con el respectivo socket
    socket.send(em, socket);
}

void ChatClient::logout() {
    // Mensaje del logout
    std::string msg = "== Usuario '" + nick + "' se ha desconectado. ==";
    // Lo muestra por pantalla
    std::cout << msg << std::endl;
    // Crea un nuevo mensaje
    ChatMessage em(nick,msg);
    // Que sera de tpo LOGOUT
    em.type = ChatMessage::LOGOUT;
    // Y lo envia al servidor, con el respectivo socket
    socket.send(em, socket);
}

void ChatClient::input_thread() {
    while (conn) {
        std::string msg;
        // Leer stdin con std::getline
        std::getline(std::cin, msg);

        // Enviar al servidor usando socket
        if(!(msg[0] == 'q' && msg.length() == 1)) {
             // Crea un nuevo mensaje
            ChatMessage em(nick, msg);
            // Que sera de tpo MESSAGE
            em.type = ChatMessage::MESSAGE;
            // Y lo envia al servidor, con el respectivo socket
            socket.send(em, socket);
        }
        else {
            ChatClient::logout();
            conn = false;
        }
    }
}

void ChatClient::net_thread() {
    while(conn) {
        ChatMessage em;
        // Recibir Mensajes de red
        socket.recv(em);

        // Mostrar en pantalla el mensaje de la forma "nick: mensaje"
        if(em.type == ChatMessage::MESSAGE)
            std::cout << em.nick << ": " << em.message << "\n";
    }
}