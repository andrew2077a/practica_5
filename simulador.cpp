#include "simulador.h"
#include <cstdlib>
#include <ctime>
#include "datos.h"
#include <string>
#include <fstream>
#include <iostream>


Simulador::Simulador(): cajas(datos::ANCHO_ESCENA, datos::ALTO_ESCENA) {
    srand(std::time(nullptr));
    for (int i=0;i<10;i++){                      // <= cambiado a < (10 partículas)
        int x= rand()%750;
        int y= rand()%550;
        int vx=(rand()%500)+20;
        int vy=(rand()%500)+20;                  // & cambiado a %
        vector2D pos(x,y);
        vector2D vel(vx,vy);
        particula* p = new particula(pos, vel, 10, (rand() % 10) + 1);
        particulas.push_back(p);
    }
    for (int i=0;i<4;i++){                       // 4 obstáculos, como pide la guía
        int x= rand()%750;
        int y= rand()%550;
        vector2D pos(x,y);
        obstaculo* p = new obstaculo(pos,(rand()%10)+1);   // cuadrado: un solo lado
        obstaculos.push_back(p);
    }
}
void Simulador ::simulando(){
    string arch="simu.txt";
    ofstream archivo(arch);
    try{
        if(!archivo.is_open()){
            throw string("no se pudo abrir el archivo");
        }
    }
    catch (string &error) {
        cout << "Error: " << error;
        return;
    }
    double t = 0.0;
    const double T_MAX = 30.0;
    while (t < T_MAX && particulas.size() > 1) {
        for(particula* p:particulas){
            p->actualizar(datos::DT);
        }
        t += datos::DT;

        int id=0;
        for(particula* p:particulas){
            if(cajas.rebotar(*p)){
                archivo << "COL," << t << ",PARED," << id << "\n";
            }
            for(obstaculo* o:obstaculos){
                if(o->colisionar(*p) != ladoImpacto::ninguno){
                    archivo << "COL," << t << ",OBSTACULO," << id << "\n";
                }
            }
            id++;
        }
        for (size_t i = 0; i < particulas.size(); ++i) {
            for (size_t j = i + 1; j < particulas.size(); ++j) {
                if (particulas[i]->colisionParticula(*particulas[j])) {
                    archivo << "COL," << t << ",PARTICULA," << i << "," << j << "\n";
                    delete particulas[j];
                    particulas.erase(particulas.begin() + j);
                    --j;
                }
            }
        }

        id=0;
        for(particula* p:particulas){
            vector2D posi=p->getPosicion();
            archivo << "POS," << t << "," << id << "," << posi.getX() << "," << posi.getY() << "\n";
            id++;
        }
    }
    archivo.close();
}

Simulador::~Simulador() {
    for (particula* p : particulas) delete p;
    for (obstaculo* o : obstaculos) delete o;
}
