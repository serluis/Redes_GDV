#include "Chat.h"

void ChatMessage::to_bin()
{
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
    memcpy(tmp, nick.c_str(), 7 * sizeof(char));
    // Mueve manualmente el puntero 8 posiciones (la 8 sera para '\0')
    tmp += 8 * sizeof(char);
    // Copia en tmp lo que ponga en message
    memcpy(tmp, message.c_str(), 79 * sizeof(char));
   
}

int ChatMessage::from_bin(char * bobj)
{
   //memcpy(static_cast<void >(_data), bobj, MESSAGE_SIZE);
    alloc_data(MESSAGE_SIZE);
    // Apunta al primer dato
    char* tmp = bobj;
    // Copia en tmp lo que ponga en type
    memcpy(&type, tmp, sizeof(int8_t));
    // Mueve manualmente el puntero 1 posicion
    tmp += sizeof(int8_t);
    // Copia en tmp lo que ponga en nick
    char name[8];
    memcpy(name, tmp, 8 * sizeof(char));
    nick=name;
    // Mueve manualmente el puntero 8 posiciones
    tmp += 8 * sizeof(char);
    char msg[80];
    // Copia en tmp lo que ponga en message
    memcpy(msg, tmp, 80 * sizeof(char));
    message=msg;
    
    // Control de errores de copiar los datos
    if ((nick.length() + message.length()) < 0)
        return -1;
    return nick.length() + message.length();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatServer::do_messages()
{
    while (true) {
        // Crear un mensaje vacio que va a rellenar el recv()
        ChatMessage* msg = new ChatMessage();
        // Recibir Mensajes en y en función del tipo de mensaje
        socket.recv(*msg, (Socket*&) *(&socket));
        std::cout<<"antes de switch"<<std::endl;
         switch(msg->type) {
            // - LOGIN: Añadir al vector clients
            case ChatMessage::LOGIN: {
                clients.push_back(&socket);
                std::cout << msg->nick << " se ha unido al chat" << std::endl;
            } break;
             // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)
            case ChatMessage::MESSAGE: {
                std::cout << msg->nick << ": " << msg->message << std::endl;
                for (int i = 0; i < clients.size(); ++i) {
                    if (socket == *clients.at(i)) { /* NO HAGAS NADA */}
                    else socket.send(*msg, *clients.at(i));
                }
            } break;
             // - LOGOUT: Eliminar del vector clients
            case ChatMessage::LOGOUT: {
                std::cout << msg->nick << " se ha ido del chat" << std::endl;
                bool erased = false;
                std::vector<Socket*>::iterator it = clients.begin();
                for (int i = 0; i < clients.size() && !erased; ++i) {
                    if (socket == *clients.at(i)) {
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

void ChatClient::login()
{
    conn = true;
    // Mensaje del login
    std::string msg = "Usuario '" + nick + " se ha registrado.";
    // LO muestra por pantalla
    std::cout << msg << std::endl;
    // Crea un nuevo mensaje
    ChatMessage em(nick, msg);
    // Que sera de tpo LOGIN
    em.type = ChatMessage::LOGIN;
    // Y lo envia al servidor, con el respectivo socket
    socket.send(em, socket);
}

void ChatClient::logout()
{
    // Mensaje del logout
    std::string msg = "Usuario '" + nick + "' se ha desconectado.";
    // LO muestra por pantalla
    std::cout << msg << std::endl;
    // Crea un nuevo mensaje
    ChatMessage em(nick,msg);
    // Que sera de tpo LOGOUT
    em.type=ChatMessage::LOGOUT;
    // Y lo envia al servidor, con el respectivo socket
    socket.send(em,socket);
}

void ChatClient::input_thread()
{
    // Leer stdin con std::getline
    // Enviar al servidor usando socket
    while (conn)
    {
        std::string msg;
        std::getline(std::cin, msg);

        if(msg[0] == 'q' && msg.length() == 1)
        {
            conn = false;
        }
        else
        {
            ChatMessage em(nick, msg);
            em.type = ChatMessage::MESSAGE;

            socket.send(em, socket);
        }
    }
}

void ChatClient::net_thread()
{
    //Recibir Mensajes de red
    //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
    while(conn)
    {
        ChatMessage em;
        socket.recv(em);

        if(em.type == ChatMessage::MESSAGE)
            std::cout << em.nick << ": " << em.message << "\n"; 
        else
            std::cout << em.message << "\n"; 
    }
}