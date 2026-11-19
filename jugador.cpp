#include "jugador.h"
#include <cmath>

jugador::jugador(int numero_, const vector2D& posCanon_)
    : numero(numero_), posCanon(posCanon_), angulo(45.0), velocidad(400.0),
    representante(nullptr)
{}

jugador::~jugador()
{
    for (obstaculo* o : infraestructura) delete o;
    delete representante;
}

const vector2D& jugador::getPosCanon() const { return posCanon; }
void jugador::setAngulo(double a)    { angulo = a; }
void jugador::setVelocidad(double v) { velocidad = v; }

void jugador::agregarObstaculo(obstaculo* o) { infraestructura.push_back(o); }
const std::vector<obstaculo*>& jugador::getObstaculos() const { return infraestructura; }
void jugador::setComandante(comandante* c) { representante = c; }
comandante* jugador::getComandante() const { return representante; }

bool jugador::haPerdido() const { return representante->estaMuerto(); }

// y crece hacia abajo, por eso la componente vertical es negativa.
// El jugador 0 dispara hacia la derecha y el 1 hacia la izquierda.
proyectil* jugador::crearProyectil() const
{
    double rad = angulo * 3.14159265358979 / 180.0;
    double dir = (numero == 0) ? 1.0 : -1.0;
    vector2D direccion(dir * cos(rad), -sin(rad));
    vector2D pos = posCanon + direccion * 30.0;
    vector2D vel = direccion * velocidad;
    return new proyectil(pos, vel, numero);
}
