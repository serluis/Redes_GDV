#include "Serializable.h"

#include <iostream>
#include <ofstream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
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
        char * tmp = _data;
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

    int from_bin(char * data)
    {
        char * tmp = data;
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
    Jugador one_w("Player_ONE", 123, 987);
    one_w.to_bin();
    one_r.from_bin(one_w.data());
    //one_r.data();
    std::fstream ej1;
    ej1.open("ej1.txt");
    ej1.write(one_r.data());
    //Serializar y escribir one_w en un fichero
    //Leer el fichero en un buffer y "deserializar" en one_r
    //Mostrar el contenido one_r

    return 0;
}