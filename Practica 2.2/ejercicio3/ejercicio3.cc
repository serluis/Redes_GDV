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
// Tamano maximo posible del nombre
static const size_t MAX_NAME = 20;
// Clase jugador hereda de serializable
class Jugador : public Serializable {
private:
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
        // Apuntar un puntero a data
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
    int from_bin(char *data) { 
        // Apunta al primer dato
        char* tmp = data;
        // Copia los 20 primeros caracteres en name
        memcpy(name, tmp, MAX_NAME);
        // Mueve el puntero manualmente 20 posiciones
        tmp += MAX_NAME * sizeof(char);
        // Copia en pos_x el valor de la posicion 20 de data
        memcpy(&pos_x, tmp, sizeof(int16_t));
        // Mueve el puntero manualmente 1 posicion (2 bytes)
        tmp += sizeof(int16_t);
        // Copia en la pos_y el valor de la posicion 20 + 2 de data
        memcpy(&pos_y, tmp, sizeof(int16_t));
        // Control de errores de copiar los datos
        if (strlen(name) < 0)
            return -1;
        return strlen(name);
    }
    // Getters y Setters
    int16_t getX() { return pos_x; };
    int16_t getY() { return pos_y; };
    char* getName() { return name; };
};

int main(int argc, char **argv) {
    // Crea a un jugador vacio
    Jugador one("", 0, 0);
    // Crear el fichero
    int id = open("data", O_CREAT | O_RDWR, 0666);
    // Calcula el tamano total del archivo
    int total = MAX_NAME + 2 * sizeof(int16_t);
    char data[total];
    // Escribir en el fichero
    int errR = read(id, data, total);
    // Control de errores al escribir
    if (errR == -1) {
        std::cerr << "Error read: " << strerror(errno) << std::endl;
        return -1;
    }
    // Mete al jugador los datos
    int ndat = one.from_bin(data);
    // Control de errores de data
    if (ndat < 0) {
        std::cout << "Error data: " << std::endl;
        return -1;
    }
    // Mostrar por pantalla los datos recibidos
    std::cout << one.getName() << " " << one.getX() << " " << one.getY() << std::endl;
    // Cerrar el archivo al escribir
    close(id);
    return 0;
}