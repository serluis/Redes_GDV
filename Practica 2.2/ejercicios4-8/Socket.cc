// Dependencias de clases
#include "Serializable.h"
#include "Socket.h"
// Dependencias de C
#include <string.h>

Socket::Socket(const char * address, const char * port) : sd(-1) {
    // Construir un socket de tipo AF_INET y SOCK_DGRAM usando getaddrinfo.
    // Con el resultado inicializar los miembros sd, sa y sa_len de la clase
    struct addrinfo hints;
    struct addrinfo * res;

    // ---------------------------------------------------------------------- //
    // --- INICIALIZACIÓN SOCKET & BIND ------------------------------------- //
    // ---------------------------------------------------------------------- //

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int rc = getaddrinfo(address, port, &hints, &res);
    if (rc != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
        return;
    }

    // res contiene la representación como sockaddr de dirección + puerto
    sa = *res->ai_addr;
    sa_len = res->ai_addrlen;
    sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    /*if (bind(sd, sa, sa_len) != 0) {
        std::cerr << "bind: " << std::endl;
        return;
    }*/
    freeaddrinfo(res);
}

int Socket::recv(Serializable &obj, Socket * &sock) {
    struct sockaddr sa;
    socklen_t sa_len = sizeof(struct sockaddr);

    char buffer[MAX_MESSAGE_SIZE];

    ssize_t bytes = ::recvfrom(sd, buffer, MAX_MESSAGE_SIZE, 0, &sa, &sa_len);
    if (bytes <= 0) {
        std::cerr << "bytes: " << std::endl;
        return -1;
    }

    if (sock != 0)
        sock = new Socket(&sa, sa_len);

    obj.from_bin(buffer);

    return 0;
}

int Socket::send(Serializable& obj, const Socket& sock) {
    // Serializar el objeto
    obj.to_bin();
    // Enviar el objeto binario a sock usando el socket sd
    int err = sendto(sd, obj.data(), MAX_MESSAGE_SIZE, 0, &sock.sa, sock.sa_len);
    if (err == -1) {
        std::cerr << "err sendto: " << std::endl;
        return -1;
    }
    return 0;
}

// ------------------ //
// --- OPERADORES --- //
// ------------------ //

// Comparar los campos sin_family, sin_addr.s_addr y sin_port
// de la estructura sockaddr_in de los Sockets s1 y s2
bool operator== (const Socket &s1, const Socket &s2) {
    struct sockaddr_in* s1_ = (struct sockaddr_in*)&s1.sa;
    struct sockaddr_in* s2_ = (struct sockaddr_in*)&s2.sa;
    // Retornar false si alguno difiere
    return (&s1.sa.sa_family == &s2.sa.sa_family) && (s1_->sin_addr.s_addr == s2_->sin_addr.s_addr) && (s1_->sin_port == s2_->sin_port);
};

std::ostream& operator<<(std::ostream& os, const Socket& s) {
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;

    return os;
};