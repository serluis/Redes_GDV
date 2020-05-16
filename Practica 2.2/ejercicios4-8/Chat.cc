// Dependencias de otras clases
#include "Chat.h"
// Serializar: Serializar los campos type, nick y message en el buffer _data
void ChatMessage::to_bin() {
   alloc_data(MESSAGE_SIZE);
    // Apuntar un puntero a data
    char *tmp = _data;
    // Copia en tmp lo que ponga en type
    memcpy(tmp, &type, sizeof(uint8_t));
    // Mueve manualmente el puntero 1 posicion
    tmp += sizeof(uint8_t);
    // Copia en tmp lo que ponga en nick
    memcpy(tmp, nick.c_str(), 7 * sizeof(char));
    // Mueve manualmente el puntero 7 posiciones (la 8 sera para '\0')
    tmp += (8 * sizeof(char) - sizeof(char));
    // Escribe el '\0'
    memcpy(tmp, '\0', sizeof(char));
    // Mueve manualmente el puntero 1 posicio (la del '\0')
    tmp += sizeof(char);
    // Copia en tmp lo que ponga en message
    memcpy(tmp, message.c_str(), 79 * sizeof(char));
    // Mueve manualmente el puntero 79 posiciones (la 80 sera para '\0')
    tmp += (80 * sizeof(char) - sizeof(char));
    // Escribe el '\0'
    memcpy(tmp, '\0', sizeof(char));
}
// Deserializar: Reconstruir la clase usando el buffer _data
int ChatMessage::from_bin(char * bobj) {
   //memcpy(static_cast<void >(_data), bobj, MESSAGE_SIZE);
    alloc_data(MESSAGE_SIZE);
    // Apunta al primer dato
    char tmp = bobj;
    // Copia en tmp lo que ponga en type
    memcpy(&type, tmp, sizeof(int8_t));
    // Mueve manualmente el puntero 1 posicion
    tmp += sizeof(int8_t);
    // Copia en tmp lo que ponga en nick
    memcpy(&nick, tmp, 8 * sizeof(char));
    // Mueve manualmente el puntero 8 posiciones
    tmp += 8 * sizeof(char);
    // Copia en tmp lo que ponga en message
    memcpy(&message, tmp, 80 * sizeof(char));
    // Mueve manualmente el puntero 79 posiciones (la 80 sera para '\0')
    tmp += 80 * sizeof(char);
    // Control de errores de copiar los datos
    if ((strlen(nick.c_str()) + strlen(message.c_str())) < 0)
        return -1;
    return strlen(nick.c_str()) + strlen(message.c_str());
}

// ----------------------------------------------------------------------------- //
// --- Chat Server y Client ---------------------------------------------------- //
// ----------------------------------------------------------------------------- //

void ChatServer::do_messages() {
    while (true) {
        //Recibir Mensajes en y en función del tipo de mensaje
        // - LOGIN: Añadir al vector clients
        // - LOGOUT: Eliminar del vector clients
        // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)
    }
}

void ChatClient::login() {
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);
}

void ChatClient::logout() {

}
void ChatClient::input_thread() {
    while (true) {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
    }
}

void ChatClient::net_thread() {
    while(true) {
        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
    }
}