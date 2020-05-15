// Include de Serializacion
#include "Serializable.h"
// Includes del programa
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
// Includes de archivos
#include <iostream>
#include <fstream>
#include <string>
// Clase jugador hereda de serializable
class Jugador : public Serializable {
private:
    char name[80];

    int16_t x;
    int16_t y;
public:
    Jugador(const char *_n, int16_t _x, int16_t _y) : x(_x), y(_y) {
        strncpy(name, _n, 80);
    };

    virtual ~Jugador(){};

    void to_bin() {
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
    int from_bin(char *data) {
        char *tmp = data;
        memcpy(name, tmp, 80);
        tmp += 80 * sizeof(char);
        memcpy(&x, tmp, sizeof(int16_t));
        tmp += sizeof(int16_t);
        memcpy(&y, tmp, sizeof(int16_t));
        return sizeof(data);
    }
};

int main(int argc, char **argv)
{
    // Crea 2 obj
    Jugador one_r("", 0, 0);
    Jugador one_w("Player_ONE", 1, 9);

    // convierte W
    one_w.to_bin();

    // Crea o abre el archivo y guarda identif
    // con posib. de leer y escribir
    int identif_ = open("ej1.txt", O_CREAT | O_RDWR, 0666);

    // Escribe en el archivo
    int errW = write(identif_, one_w.data(), one_w.size());
    // Si da error notifica y cierra
    if (errW == -1)
    {
        std::cout << "Escrito mal" << std::endl;
        std::cout << strerror(errno) << std::endl;
        return -1;
    }
    //control
    std::string alfa(one_w.data());
    std::cout << "Escrito " << alfa << " : " << one_w.size() << std::endl;
    //Cierra el archivo
    close(identif_);

    // Vuelve a abrir para restaurar puntero
    int identif_2 = open("ej1.txt", O_CREAT | O_RDWR, 0666);
    // Crea buffer para guardar info
    char buffer[80 * sizeof(char) + 2 * sizeof(int16_t)];

    // Lee de archivo y control error
    int errR = read(identif_2, buffer, one_w.size());

    std::cout << "Buffer: "<< buffer << std::endl;
    if (errR == -1)
    {
        std::cout << "Leido mal" << std::endl;
        std::cout << strerror(errno) << std::endl;
        return -1;
    }

    // Deserializa
    one_r.from_bin(buffer);
    //control de errores
    std::string beta(one_r.data());
    std::cout << "Escrito en r " << beta << " : " << one_r.size() << std::endl;

    // Cierra archivo y finaliza
    std::cout << "Fin" << std::endl;
    close(identif_2);
    return 0;