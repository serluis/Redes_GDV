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
    static const size_t MAX_NAME = 20;
    // Cadena del nombre 
    char name[MAX_NAME];
    // Posicion
    int16_t pos_x;
    int16_t pos_y;
public:
    // Constructora y destructora
    Jugador(const char *_n, int16_t _x, int16_t _y) : pos_x(_x), pos_y(_y) {
        strncpy(name, _n, MAX_NAME);
    };
    virtual ~Jugador() {};
    // Serializacion
    void to_bin() { 
        // Calcula el tamano total (20 + 2*2)
        int32_t total = MAX_NAME * sizeof(char) + 2 * sizeof(int16_t);
        // Reserva tantos datos como el tamano
        alloc_data(total);
        // Apunta un puntero a data
        char *tmp = _data;
        // Copia en tmp lo que ponga en name
        memcpy(tmp, name, MAX_NAME);
        // Mueve manualmente el puntero 20 posiciones
        tmp += MAX_NAME * sizeof(char);
        // Copia en la posicion 20 el valor de 'x'
        memcpy(tmp, &pos_x, sizeof(int16_t));
        // Mueve el puntero manualmente 1 posicion (2 bytes)
        tmp += sizeof(int16_t);
        // Copia en la posicion 20 + 2 el valor de 'y'
        memcpy(tmp, &pos_y, sizeof(int16_t));
    }
    int from_bin(char *data) { }
};

int main(int argc, char **argv) {
    // Crear un objeto con valores arbitrarios
    Jugador one("Player_ONE", 12, 345);
    // Rellena los datos
    one.to_bin();
    // Crear el fichero
    int id = open("data", O_CREAT | O_RDWR, 0666);
    // Escribir en el fichero
    int errW = write(id, one.data(), one.size());
    // Control de errores al escribir
    if (errW == -1) {
        std::cout << "Error write: " << strerror(errno) << std::endl;
        return -1;
    }
    // Cerrar el archivo al escribir
    close(id);
    return 0;
}