// Dependencias del server
#include "XLDisplay.h"
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#define R 12 //rondas 8
#define C 8  //combinacion
using namespace std;
//Pintara el tablero y las sombras de las chinchetas
void tablero()//tablero
{
    XLDisplay& dpy = XLDisplay::display();
    /*dpy.point(50,50);
    dpy.line(20,20,100,50);
    dpy.circle(45,45,15);
    dpy.rectangle(60,60,30,15);
    XPoint pts[] = {{100,100},{130,130},{100,130},{100,100}};
    dpy.lines(pts, 4);*/
    dpy.set_color(XLDisplay::PERU);
    dpy.rectangleFill(10,10,380,440);
    dpy.set_color(XLDisplay::GREEN);
    dpy.rectangle(10,10,380,440);
    dpy.set_color(XLDisplay::RED);
    dpy.rectangle(12,12,376,436);
    dpy.text(150, 25, "MasterMind");
    dpy.set_color(XLDisplay::SIENNA);
    for(int i=0;i<12;i++){
        for(int j=0;j<4;j++){
            dpy.circle(45*(j+1),35*(i+1),10);
            dpy.circle(200 + (j+1)*20,35*(i+1),5);
        }
    }
    
    dpy.flush();
}
//sera llamado para actualizar la partida en cada paso
void pintar(std::vector<vector<int>> part){
    XLDisplay& dpy = XLDisplay::display();
    dpy.set_color(XLDisplay::PERU);
    dpy.rectangleFill(10,10,380,440);
    dpy.set_color(XLDisplay::GREEN);
    dpy.rectangle(10,10,380,440);
    dpy.set_color(XLDisplay::RED);
    dpy.rectangle(12,12,376,436);
    //dpy.text(150, 25, "MasterMind");
    /*dpy.set_color(XLDisplay::SIENNA);
    for(int i=0;i<12;i++){
        for(int j=0;j<4;j++){
            dpy.circle(45*(j+1),35*(i+1),10);
            dpy.circle(200 + (j+1)*20,35*(i+1),5);
        }
    }*/
    for (int i = 0; i < part.size(); i++) {
		for (int j = 0; j < part.at(i).size(); j++) {
			switch (part.at(i).at(j))
            {
            case 0:
                dpy.set_color(XLDisplay::RED);
                break;
        
            case 1:
                dpy.set_color(XLDisplay::BROWN);
                break;
        
            case 2:
                dpy.set_color(XLDisplay::BLUE);
                break;
        
            case 3:
                dpy.set_color(XLDisplay::YELLOW);
                break;
        
            case 4:
                dpy.set_color(XLDisplay::GREEN);
                break;
        
            case 5:
                dpy.set_color(XLDisplay::PURPLE);
                break;
        
            case 6:
                dpy.set_color(XLDisplay::ORANGE);
                break;
        
            case 7:
                dpy.set_color(XLDisplay::FUCHSIA);
                break;
        
            case 8:
                dpy.set_color(XLDisplay::WHITE);
                break;
        
            case 9:
                dpy.set_color(XLDisplay::BLACK);
                break;
            case 10:
                dpy.set_color(XLDisplay::PERU);
                break;
            case 11:
                dpy.set_color(XLDisplay::SIENNA);
                break;
            default:
                break;
            }

        if(j<4){
            dpy.circle(45*(j+1),35*(i+1),10);
        }
        else{
            dpy.circle(150 + (j+1)*20,35*(i+1),5);
        }
		}
	}
    dpy.flush();
}

int * redondeles(int turno)
{
    bool acabado = false;
    tablero();
    XLDisplay& dpy = XLDisplay::display();
    dpy.set_color(XLDisplay::RED);
    for(int i=0;i<4;i++){
        dpy.circle(45*(i+1),35*(turno+1),10);
    }
    int comb [4] = {0,0,0,0};
    int pos = 0;
    while(!acabado){
        char dir = dpy.wait_key();
        switch (dir)
        {
            case 'w':
                    if(comb[pos] == 9){
                        comb[pos] = 0;
                    }
                    else{
                        comb[pos]++;
                    }           
                    break;
            case 'a':
                    if(pos==0){
                        pos=3;
                    }
                    else{
                        pos--;
                    }
                    break;
            case 's':
                    if(comb[pos] == 0){
                        comb[pos] = 9;
                    }
                    else{
                        comb[pos]--;
                    }
                    break;
            case 'd':
                    if(pos==3){
                        pos=0;
                    }
                    else{
                        pos++;
                    }
                    break;
            case 'l':
                    //acabado = true;
                    turno++;
                    break;
            default:
                break;
        }
        switch (comb[pos])
        {
            case 0:
                dpy.set_color(XLDisplay::RED);
                break;
        
            case 1:
                dpy.set_color(XLDisplay::BROWN);
                break;
        
            case 2:
                dpy.set_color(XLDisplay::BLUE);
                break;
        
            case 3:
                dpy.set_color(XLDisplay::YELLOW);
                break;
        
            case 4:
                dpy.set_color(XLDisplay::GREEN);
                break;
        
            case 5:
                dpy.set_color(XLDisplay::PURPLE);
                break;
        
            case 6:
                dpy.set_color(XLDisplay::ORANGE);
                break;
        
            case 7:
                dpy.set_color(XLDisplay::FUCHSIA);
                break;
        
            case 8:
                dpy.set_color(XLDisplay::WHITE);
                break;
        
            case 9:
                dpy.set_color(XLDisplay::BLACK);
                break;
        
            default:
                break;
        }
        dpy.circle(45*(pos+1),35*(turno+1),10);
    }
    return comb;
}
void wait()
{
    XLDisplay& dpy = XLDisplay::display();

    char k;

    do
    {
         k = dpy.wait_key();
    } while (k != 'q');

    dpy.clear();

    dpy.flush();

    sleep(1);
}
/*void probar(){//recibe puntero del vector de partida
    //bucle principal
            while(){
                //borrar lo pintado?
                dpy.clear();
                dpy.flush();
                //bucle interno cond fin partida win, end
                //seleccionar contraseña pintando
                //OJO AL TURNO
                redondeles(0);
                //posibilidad de salir, cerrar partida en ambos
                //enviar contraseña
                
                //esperar turno oponente
                //recibir respuesta y pintar
                //comprobar ganador
}
void esperar(){//recibe puntero del vector de partida
    //recibir respuesta
                //pintar (metodo aparte)
                //comprobar ganador
}*/

int main(int argc, char** argv)
{
    // fin=1 end, turno=1 tu turno
    int finPartida = 0, turno = 0, ronda = 0;
    //crear un vector de arrays con la partida completa para pintar
   std::vector<vector<int>> part(R);
	
	for (int i = 0; i < part.size(); i++) {
		part[i].resize(C);
		for (int j = 0; j < part.at(i).size(); j++) {
			part.at(i).at(j) = 11;
		}
	}
    //v.at(i)[0] //acceso
    //enviar datos al server
        //socket y bind
    //pintar el tablero
    XLDisplay::init(400, 450, "MasterMind");
    //tablero();
    pintar(part);
    //esperar turno player 1 o 2 por parte del server
        //receive 0000 0000 1 1 
    /*while(ronda < 12 && finPartida == 0){
        if(turno == 1){
           // probar();
        }
        else{//0000 0000 0 0
            //esperar();
        }
        turno++;
    }*/
    int n;
    cin>>n;
    return 0;
}