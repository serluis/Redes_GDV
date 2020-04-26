#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <iostream>

/*
argv[0] ---> nombre del programa
argv[1] ---> primer argumento (char *)
./addrinfo www.ucm.es 80
argv[0] = "./addrinfo"
argv[1] = "www.ucm.es"
argv[2] = "80"
|
|
V
res->ai_addr ---> (socket + bind)
|
|
V
host (numeric)./addrinfo 127.0.0.1 80
./addrinfo www.ucm.es http
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
hints.ai_socktype = SOCK_DGRAM;

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
std::cerr << "bind: " << std::endl;
return -1;
}

freeaddrinfo(res);
// ---------------------------------------------------------------------- //
// RECEPCIÓN MENSAJE DE CLIENTE //
// ---------------------------------------------------------------------- //
char buffer[80];
char host[NI_MAXHOST];
char service[NI_MAXSERV];
struct sockaddr client_addr;
socklen_t client_len = sizeof(struct sockaddr);
ssize_t bytes;
do{
    memset(&buffer, 0, sizeof(buffer));
    bytes = recvfrom(sd, buffer, 79 * sizeof(char), 0, &client_addr,&client_len);
    int gni = getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service,
    NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
    // Control de errores
    if(gni != 0){
        std::cerr << "could not resolve hostname " << std::endl;
        return -1;
    }
    if ( bytes == -1)
    {
        std::cerr << "recvfrom: " << std::endl;
        return -1;
    }
    std::cout << "buffer size: " << strlen(buffer) <<std::endl;
    // comando no soportado
    if (strlen(buffer) != 2){
        std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
        std::cout << "El cliente manda comando no soportado " << std::endl;
        char mess[80] = "Error: introduce 't', 'd', 'q': ";
        sendto(sd, mess, strlen(mess), 0, &client_addr, client_len);
    }
    // ejecucion de comandos
    else{
        time_t rawtime;
        struct tm* timeInfo;
        char buffer1[80];
        memset(&buffer1,0,sizeof(buffer1));
        time(&rawtime);
        timeInfo = localtime(&rawtime);
        switch (buffer[0])
        {
            case 'd':
                    {
                    std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
                    //std::cout << "crash...d antes de strftime"<<std::endl;                  
                    strftime(buffer1,sizeof(buffer1),"%F",timeInfo);
                    //std::cout << "crash..d..strftime"<<std::endl;
                    sendto(sd,buffer1, strlen(buffer1), 0, &client_addr, client_len);
                    //std::cout << "crash...d sendto"<<std::endl;
                    }
                    break;
            case 't':
                    {
                    //std::cout << "crash...t antes de strftime"<<std::endl;
                    std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
                    strftime(buffer1,sizeof(buffer1),"%r",timeInfo);
                    //std::cout << "crash.t..strftime"<<std::endl;
                    sendto(sd,buffer1, strlen(buffer1), 0, &client_addr, client_len);
                    //std::cout << "crash...t"<<std::endl;
                    }
                    break;
            case 'q':
                    std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
                    std::cout << "Saliendo..."<<std::endl;
                    break;    
            default:
                    {
                    std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
                    
                    std::cout << "comando no soportado "<<std::endl;
                    }
                    break;
        }
        // informe de cliente
        std::cout << "IP: " << host << " PUERTO: " << service
        << " MENSAJE: " << buffer << std::endl;
        //bytes = recvfrom(sd, buffer, 79 * sizeof(char), 0, &client_addr, &client_len);
    }
}while(!(buffer[0]=='q'&& strlen(buffer)==2));

return 0;
}