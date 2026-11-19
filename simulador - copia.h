#ifndef SIMULADOR_H
#define SIMULADOR_H
#include <vector>
#include "particula.h"
#include "obstaculo.h"
#include "caja.h"
using namespace std;
class Simulador{
private:
    vector<particula*> particulas;
    vector<obstaculo*> obstaculos;
    caja cajas;

public:
    Simulador();
    void simulando();
    void insertarArchivo();
    void ingresarGarfico();
    ~Simulador();
};

#endif // SIMULADOR_H
