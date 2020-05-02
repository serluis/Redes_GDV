#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

/*
./tcp 0.0.0.0 7777
*/

int main(int argc, char **argv)
{
struct addrinfo hints;
struct addrinfo * res;
// ---------------------------------------------------------------------- //
// INICIALIZACIÓN SOCKET & BIND //
// ---------------------------------------------------------------------- //

memset(&hints, 0, sizeof(struct addrinfo));

hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;

int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

if ( rc != 0 )
{
    std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
return -1;
}

// res contiene la representación como sockaddr de dirección + puerto

int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

if ( bind(sd, res->ai_addr, res->ai_addrlen) != 0 )
{
std::cerr << "error de bind: " << std::endl;
return -1;
}

freeaddrinfo(res);
// ---------------------------------------------------------------------- //
// PUBLICAR EL SERVIDOR (LISTEN) //
// ---------------------------------------------------------------------- //
int oreja;
oreja = listen(sd, 16);
if (oreja<0){
    std::cerr << "error de listen: " << std::endl;
}
// ---------------------------------------------------------------------- //
// GESTION DE LAS CONEXIONES AL SERVIDOR //
// ---------------------------------------------------------------------- //
struct sockaddr client_addr;
socklen_t client_len = sizeof(struct sockaddr);
char host[NI_MAXHOST];
char service[NI_MAXSERV];

int sd_client = accept(sd, &client_addr, &client_len);

getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service,
NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

std::cout << "CONEXION DESDE IP: " << host << " PUERTO: " << service
<< std::endl;

// ---------------------------------------------------------------------- //
// GESTION DE LA CONEXION CLIENTE //
// ---------------------------------------------------------------------- //
char buffer[80];
ssize_t bytes;

do{
memset(&buffer,0,sizeof(buffer));
bytes = recv(sd_client, (void *) buffer, sizeof(char)*79, 0);

if ( bytes <= 0 )
{
std::cout << "conexion terminada en cliente " << std::endl;
return 0;
}

std::cout << "MENSAJE: " << buffer << std::endl;

send(sd_client, (void *) buffer, bytes, 0);
/*std::cout << "contenido antes de while: " << buffer << " buffer 0: "
<<buffer[0]<<" buffer 1: "<< buffer[1]<< " largo buffer: "
<< strlen(buffer) << std::endl;*/

}while(!((buffer[0] == 'Q' && strlen(buffer) <= 2)));

std::cout << "Conexion terminada por orden del cliente. " << std::endl;

return 0;
}