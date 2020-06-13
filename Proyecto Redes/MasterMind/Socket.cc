#include "Socket.h"

Socket::Socket(const char * address, const char * port) : sd(-1) {
    // res contiene la representación como sockaddr de dirección + puerto
    struct addrinfo hints;
    struct addrinfo *res;

    // ---------------------------------------------------------------------- //
    // --- INICIALIZACIÓN SOCKET & BIND ------------------------------------- //
    // ---------------------------------------------------------------------- //

    // Resetear la memoria desde hints hasta hints + addrinfo
    memset(&hints, 0, sizeof(struct addrinfo));
    // Configuracion
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // Coger la info del servidor
    int rc = getaddrinfo(address, port, &hints, &res);
    // Control de errores de getaddrinfo
    if (rc != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
        return;
    }

    // res contiene la representación como sockaddr de dirección + puerto
    sa = *res->ai_addr;
    sa_len = res->ai_addrlen;

    sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sd < 0) {
        std::cerr << "socket: error" << std::endl;
    }

    freeaddrinfo(res);
}

int Socket::bind() {
    // Conexion y control de errores
    int bind = ::bind(sd, (const struct sockaddr *) &sa, sa_len);
    if (bind != 0) {
        std::cerr << "bind: " << std::endl;
        return -1;
    }

    // ---------------------------------------------------------------------- //
    // --- PUBLICAR EL SERVIDOR (LISTEN) ------------------------------------ //
    // ---------------------------------------------------------------------- //
    
    int serv = listen(sd, 16);
    // Control de errores de listen
    if (serv < 0) {
        std::cerr << "listen: " << std::endl;
        return -1;
    }

    return bind;
}

Socket* Socket::accept() {
    // Aceptar cualquier conexion entrante
    int accept = ::accept(sd, &sa, &sa_len);
    if (accept == -1) {
        std::cerr << "sdclient: " << std::endl;
        return nullptr;
    }
    // Si la conexion no ha fallado, devolver el socket
    return new Socket(&sa, sa_len, accept);
}

int Socket::connect() {
    int connect = ::connect(sd, &sa, sa_len);
    if (connect == -1) {
        std::cerr << "coonect: " << std::endl;
        return -1;
    }
    return connect;
}

int Socket::recv(Serializable &obj, Socket * &sock) {
    struct sockaddr sa;
    socklen_t sa_len = sizeof(struct sockaddr);

    // Coger la informacion
    char buffer[90];

    std::cout << "Recibiendo de la sd: " << sd << std::endl;
    ssize_t bytes = ::recv(sd, (void*) buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        std::cerr << "bytes: " << bytes << std::endl;
        return -1;
    }

    obj.from_bin(buffer);

    return 0;
}

int Socket::send(Serializable& obj) {
    // Serializar el objeto
    obj.to_bin();
    // DEBUG CON FICHERO PARA SABER QUE CONTIENE DATA
    int id = open("data", O_CREAT | O_RDWR, 0666);
    // Escribir en el fichero
    int errW = write(id, obj.data(), obj.size());
    // Control de errores al escribir
    if (errW == -1) {
        std::cout << "Error write: " << std::endl;
        return -1;
    }
    // Cerrar el archivo al escribir
    close(id);
    // Enviar el objeto binario a sock usando el socket sd
    std::cout << "Enviando a la sd: " << sd << std::endl;
    int err = ::send(sd, obj.data(), obj.size(), 0);
    if (err == -1) {
        std::cerr << "err sendto: " << std::endl;
        return -1;
    }
    return 0;
}

// ---------------------------------------------------------------------- //
// --- OPERADORES ------------------------------------------------------- //
// ---------------------------------------------------------------------- //
    

// Comparar los campos sin_family, sin_addr.s_addr y sin_port
// de la estructura sockaddr_in de los Sockets s1 y s2
bool operator== (const Socket &s1, const Socket &s2) {
    struct sockaddr_in* s1_ = (struct sockaddr_in*)&(s1.sa);
    struct sockaddr_in* s2_ = (struct sockaddr_in*)&(s2.sa);
    // Retornar false si alguno difiere
    return ((s1_->sin_family == s2_->sin_family) && (s1_->sin_addr.s_addr == s2_->sin_addr.s_addr) && (s1_->sin_port == s2_->sin_port));
};

std::ostream& operator<<(std::ostream& os, const Socket& s) {
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    int err = getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST);
    if (err != 0) {
        std::cerr << "getnameinfo: " << gai_strerror(err) << std::endl;
        return os;
    }

    os << host << ":" << serv << ", sd: " << s.sd;

    return os;
};