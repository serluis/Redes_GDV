// Dependencias del server
#include "XLDisplay.h"
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#define R 12 //rondas 8
#define C 8  //combinacion
using namespace std;

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
                dpy.set_color(XLDisplay::BLACK);
                dpy.circleEdge(45*(j+1),35*(i+1),10);
            }
            else{
                //dpy.circle(((j+1)*20)+150 ,35*(i+1),5);
                dpy.circle(150 + (j+1)*20,35*(i+1),5);
                dpy.circleEdge(150 + (j+1)*20,35*(i+1),5);
            }
		}
	}
    dpy.flush();
}

std::vector<int> redondeles(int ronda)
{
    bool acabado = false;
    XLDisplay& dpy = XLDisplay::display();
    dpy.set_color(XLDisplay::RED);
    for(int i=0;i<4;i++){
        dpy.circle(45*(i+1),35*(ronda+1),10);
    }
    std::vector<int> comb = {0,0,0,0};
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
                    acabado = true;
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
        dpy.circle(45*(pos+1),35*(ronda+1),10);
        dpy.set_color(XLDisplay::BLACK);
        dpy.circleEdge(45*(pos+1),35*(ronda+1),10);
    }
    dpy.flush();
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

/*cont = jugador*//*comb = server*/
std::vector<int> solucion(const std::vector<int> cont, const std::vector<int> comb){
    //se inicializa el vector a devolver
    //por defecto en no fin
    std::vector<int> sol={0, 11,11,11,11, 11,11,11,11};
    //mete la prueba del jugador a la respuesta
    for(int i=0;i<4;i++){
        sol.at(i+1)=cont.at(i);
    }
    //debug
    cout<<"contraseña metodo: ";
    for(int p=0;p<4;p++){
        cout<<comb.at(p)<<" ";
    }
    cout<<endl;
    // en mala posicion
    if(cont.at(0) == comb.at(1) || cont.at(0) == comb.at(2) 
    || cont.at(0) == comb.at(3)){
        sol[5]=9;
    }
    if(cont.at(1) == comb.at(0) || cont.at(1) == comb.at(2) 
    || cont.at(1) == comb.at(3)){
        sol[6]=9;
    }
    if(cont.at(2) == comb.at(0) || cont.at(2) == comb.at(1) 
    || cont.at(2) == comb.at(3)){
        sol[7]=9;
    }
    if(cont.at(3) == comb.at(0) || cont.at(3) == comb.at(1) 
    || cont.at(3) == comb.at(2)){
        sol[8]=9;
    }
    //correctos
    if(cont.at(0) == comb.at(0)){
        //debug
        cout<<"revision"<<cont.at(0)<<" "<<comb.at(0)<<endl;
        //fin debug
        sol[5]=8;
    }
    if(cont.at(1) == comb.at(1)){
        sol[6]=8;
    }
    if(cont.at(2) == comb.at(2)){
        sol[7]=8;
    }
    if(cont.at(3) == comb.at(3)){
        sol[8]=8;
    }
    //fin de partida correcta
    if(cont.at(0) == comb.at(0) && cont.at(1) == comb.at(1) 
        && cont.at(2) == comb.at(2) && cont.at(3) == comb.at(3))
	{
        sol[0]=1;
    }

    return sol;
}

int main(int argc, char** argv)
{
    // random
    std::vector<int> cont;
	srand(time(NULL));
    for(int i=0;i<4;i++){
	int randomint = rand()%9;
    cont.push_back(randomint);
    }
    //debug
    cout<<"contraseña: ";
    for(int p=0;p<4;p++){
        cout<<cont[p]<<" ";
    }
    cout<<endl;
	//fin debug

    // fin=1 end, turno=1 tu turno
    int finpartida = 0, turno = 1, ronda = 0;
    // combinacion a enviar
    std::vector<int> comb = {0, 0, 0, 0};
    //crear un vector de vectores con la partida completa para pintar
    std::vector<vector<int>> part(R);
	//12*8
	for (int i = 0; i < part.size(); i++) {
		part[i].resize(C);
		for (int j = 0; j < part.at(i).size(); j++) {
			part.at(i).at(j) = 11;
		}
	}
    XLDisplay& dpy = XLDisplay::display();
    //creamos el display

    //enviar datos al server
        //socket y bind

    //pintar el tablero
    
    XLDisplay::init(400, 450, "MasterMind");
    dpy.clear();
    pintar(part);

    //esperar turno player 1 o 2 por parte del server
    // recibe un 1 si es su turno y 0 si no
        //player 1
        //receive 0000 0000 1 
        //player 2
        //receive 0000 0000 0
    //pone ese uno o cero en la variable turno
    
    while(ronda < 12 && finpartida == 0){
        
        if(turno == 1){
            //incluye pintado especial
            //devuelve la contraseña del cliente
            comb = redondeles(ronda);
            /*for(int k=0;k<4;k++){
                part[ronda].at(k)=comb[k];
            }*/
            //enviar combinacion
            //proceso recibir(part);
            std::vector<int> linea = solucion(comb,cont);
            //debug
            cout<<"linea msg: ";
            for(int t=0;t<linea.size();t++){
            cout<<linea[t]<<" ";
            }
            cout<<endl;
            //fin debug
            cout<<"ronda: ";
            for(int h=0;h<8;h++){
            part[ronda].at(h)=linea[h+1];
            cout<<part[ronda].at(h);
            }
            cout<<endl;
            dpy.clear();
            pintar(part);
            //pone finpartida a 0 o 1
            if(linea[0]==1){
                finpartida=1;
                
            }
            if(finpartida==1){
                dpy.set_color(XLDisplay::BLUE);
                dpy.text(150, 150, "Tu ganas!");
                dpy.flush();
                //salir bucle
            }
            else{
                turno=0;
            }
            ronda++;        
        }
        else{            
            //recibe la jugada del oponente
            //proceso recibir(part);//pone finpartida a 0 o 1
            pintar(part);
            //realiza comprobacion de fin de partida
            if(finpartida==1){
                dpy.set_color(XLDisplay::BLUE);
                dpy.text(150, 150, "Tu pierdes!");
                dpy.flush();
                //salir bucle
            }
            else{
                turno=1;
            }
            ronda++;
        }
    }
    //realiza comprobacion de fin de partida
    if(finpartida==0){
        dpy.set_color(XLDisplay::BLUE);
        dpy.text(150, 150, "¡Perdeis ambos!");
        dpy.flush();
    }
    //escribir q para salir y cerrar conexion*/
    wait();
    //int n;
    //cin>>n;
    return 0;
}