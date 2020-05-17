// Dependencias de clases
#include "Serializable.h"
#include "Socket.h"
// Dependencias de C
#include <unistd.h>
#include <string.h>
// Dependencias de C++
#include <string>
#include <vector>
/**
 *  Mensaje del protocolo de la aplicación de Chat
 *
 *  +-------------------+
 *  | Tipo: uint8_t     | 0 (login), 1 (mensaje), 2 (logout)
 *  +-------------------+
 *  | Nick: char[8]     | Nick incluido el char terminación de cadena '\0'
 *  +-------------------+
 *  |                   |
 *  | Mensaje: char[80] | Mensaje incluido el char terminación de cadena '\0'
 *  |                   |
 *  +-------------------+
 *
 */
// ------------------------------------------------------------------------------ //
// --- Clase para los mensajes del chat ----------------------------------------- //
// ------------------------------------------------------------------------------ //
class ChatMessage : public Serializable {
public:
    static const size_t MESSAGE_SIZE = sizeof(char) * 88 + sizeof(uint8_t);
    // Tipo de mensaje
    enum MessageType {
        LOGIN   = 0,
        MESSAGE = 1,
        LOGOUT  = 2
    };
    // Constructora y destructora
    ChatMessage(){};
    ChatMessage(const std::string& n, const std::string& m):nick(n),message(m){};
    
    // Serielizacion
    void to_bin();
    int from_bin(char * bobj);

    // Tipo de mensaje - MessageType, Nick y Message
    uint8_t type;
    std::string nick;
    std::string message;
};


// ----------------------------------------------------------------------------- //
// --- Clase para el servidor de chat ------------------------------------------ //
// ----------------------------------------------------------------------------- //
class ChatServer {
private:
    // Lista de clientes conectados al servidor de Chat, 
    // representados por su socket
    std::vector<Socket *> clients;
    // Socket del servidor
    Socket socket;
public:
    ChatServer(const char * s, const char * p): socket(s, p) {
        socket.bind();
    };
    /*
     *  Thread principal del servidor recive mensajes en el socket y
     *  lo distribuye a los clientes. Mantiene actualizada la lista de clientes
     */
    void do_messages();
};

// ----------------------------------------------------------------------------- //
// --- Clase para el cliente de chat ------------------------------------------- //
// ----------------------------------------------------------------------------- //

class ChatClient {
private:
    // Socket para comunicar con el servidor
    Socket socket;
    // Nick del usuario
    std::string nick;
public:
    /**
     * @param s dirección del servidor
     * @param p puerto del servidor
     * @param n nick del usuario
     */
    ChatClient(const char * s, const char * p, const char * n)
        : socket(s, p), nick(n){};
    
    // Envía el mensaje de login al servidor
    void login();
    // Envía el mensaje de logout al servidor
    void logout();
    // Rutina principal para el Thread de E/S. 
    // Lee datos de STDIN (std::getline)
    // y los envía por red vía el Socket.
    void input_thread();
    // Rutina del thread de Red. Recibe datos 
    // de la red y los "renderiza"
    // en STDOUT
    void net_thread();
};