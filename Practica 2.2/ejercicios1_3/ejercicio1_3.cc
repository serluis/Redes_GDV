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
  private:

    int16_t pos_x;
    int16_t pos_y;
    
    static const size_t MAX_NAME = 20;
    char name[MAX_NAME];


  public:
    Jugador(const char *_n, int16_t _x, int16_t _y) : pos_x(_x), pos_y(_y)
    {
        strncpy(name, _n, MAX_NAME);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
        //calcula el tamano 20+2+2
        int32_t total = MAX_NAME * sizeof(char) + 2 * sizeof(int16_t);
        //reserva el tamano total=20+2+2
        alloc_data(total);
        //crea un puntero de char y lo apunta a data
        char *tmp = _data;
        //copia en tmp lo que ponga en name de tamano 80
        memcpy(tmp, name, MAX_NAME);
        // mueve el puntero 20 huecos
        tmp += MAX_NAME * sizeof(char);
        //copia en tmp la variable x
        memcpy(tmp, &pos_x, sizeof(int16_t));
        // mueve el puntero un int
        tmp += sizeof(int16_t);
        //copia en tmp lo que pone en y
        memcpy(tmp, &pos_y, sizeof(int16_t));
        //_data ahora esta relleno
    }

    int from_bin(char *data)
    {

        char *tmp = data;

        memcpy(name, tmp, MAX_NAME);
        tmp += MAX_NAME * sizeof(char);
        memcpy(&pos_x, tmp, sizeof(int16_t));
        tmp += sizeof(int16_t);
        memcpy(&pos_y, tmp, sizeof(int16_t));
        if(strlen(name)<0){
            return -1;
        }
        return sizeof(name);
    }
    int16_t getX(){return pos_x;}
    int16_t getY(){return pos_y;}

};

int main(int argc, char **argv)
{
    // Crea 2 obj
    Jugador one_r("", 0, 0);
    Jugador one_w("Player_ONE", 12, 345);

    // convierte W
    one_w.to_bin();

    // Crea o abre el archivo y guarda identif
    // con posib. de leer y escribir
    int identif_ = open("data", O_CREAT | O_RDWR, 0666);

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
    //std::string alfa(one_w.data());
    //std::cout << "Escrito " << alfa << " : " << one_w.size() << std::endl;
    //Cierra el archivo
    close(identif_);

    // Vuelve a abrir para restaurar puntero
    int identif_2 = open("data", O_CREAT | O_RDWR, 0666);
    // Crea buffer para guardar info
    char buffer[20 * sizeof(char) + 2 * sizeof(int16_t)];

    // Lee de archivo y control error
    int errR = read(identif_2, buffer, one_w.size());

    //std::cout << "Buffer: "<< buffer <<" : "<< strlen(buffer) <<" : "<<sizeof(buffer)<< std::endl;
    //std::cout << "Leido: "<< errR << std::endl;
    if (errR == -1)
    {
        std::cout << "Leido mal" << std::endl;
        std::cout << strerror(errno) << std::endl;
        return -1;
    }
    //std::cout << "antes deserializ datos: "<<one_r.getX()<<one_r.getY() << std::endl;
    // Deserializa
    int r_data = one_r.from_bin(buffer);
    //control de errores
    //std::string beta(one_r.data());
    std::cout << "Deserializado: " << 
    one_r.getX() << " : " << 
    one_r.getY() << " : " << 
    buffer << std::endl;

    // Cierra archivo y finaliza
    std::cout << "Fin" << std::endl;
    close(identif_2);
    return 0;
}
