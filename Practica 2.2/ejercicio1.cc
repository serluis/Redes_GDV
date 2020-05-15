#include "Serializable.h"

#include <iostream>
#include <fstream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class Jugador : public Serializable
{
  public:
    Jugador(const char *_n, int16_t _x, int16_t _y) : x(_x), y(_y)
    {
        strncpy(name, _n, 80);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
        //calcula el tamaño 80+2+2
        int32_t total = 80 * sizeof(char) + 2 * sizeof(int16_t);
        //reserva el tamaño total=80+2+2
        alloc_data(total);
        //crea un puntero de char y lo apunta a data
        char *tmp = _data;
        //copia en tmp lo que ponga en name de tamaño 80
        memcpy(tmp, name, 80);
        // mueve el puntero 80 huecos
        tmp += 80 * sizeof(char);
        //copia en tmp la variable x
        memcpy(tmp, &x, sizeof(int16_t));
        // mueve el puntero un int
        tmp += sizeof(int16_t);
        //copia en tmp lo que pone en y
        memcpy(tmp, &y, sizeof(int16_t));
        //_data ahora esta relleno
    }

    int from_bin(char *data)
    {

        char *tmp = data;

        memcpy(name, tmp, 80);
        tmp += 80 * sizeof(char);
        memcpy(&x, tmp, sizeof(int16_t));
        tmp += sizeof(int16_t);
        memcpy(&y, tmp, sizeof(int16_t));
        return sizeof(data);
    }

  public:
    char name[80];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{
    Jugador one_r("", 0, 0);
    Jugador one_w("Player_ONE", 1, 9);

    one_w.to_bin();
    //one_r.from_bin(one_w.data());

    int identif_ = open("ej1.txt", O_CREAT | O_RDWR, 0666);

    int errW = write(identif_, one_w.data(), one_w.size());
    if (errW == -1)
    {
        std::cout << "Escrito mal" << std::endl;
        std::cout << strerror(errno) << std::endl;
        return -1;
    }
    std::string alfa(one_w.data());
    std::cout << "Escrito " << alfa << " : " << one_w.size() << std::endl;

    char buffer[80 * sizeof(char) + 2 * sizeof(int16_t)];

    int errR = read(identif_, buffer, one_w.size());

    if (errR == -1)
    {
        std::cout << "Leido mal" << std::endl;
        std::cout << strerror(errno) << std::endl;
        return -1;
    }

    one_r.from_bin(buffer);

    std::string beta(one_r.data());
    std::cout << "Escrito en r " << beta << " : " << one_r.size() << std::endl;

    close(identif_);

    std::cout << "Fin" << std::endl;

    return 0;
}
