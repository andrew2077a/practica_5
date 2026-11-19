#include "comandante.h"
#include <algorithm>
using namespace std;

comandante::comandante(const vector2D& pos, double ancho_, double alto_, double vida_)
    : posicion(pos), ancho(ancho_), alto(alto_), vida(vida_)
{}

const vector2D& comandante::getPosicion() const { return posicion; }
double comandante::getAncho() const  { return ancho; }
double comandante::getAlto() const   { return alto; }
double comandante::getVida() const   { return vida; }
bool comandante::estaMuerto() const  { return vida <= 0.0; }

void comandante::recibirDano(double dano)
{
    vida = max(0.0, vida - dano);
}

// circulo (la bola) contra rectangulo (el comandante)
bool comandante::intersecta(const entidadFisica& e) const
{
    const vector2D& p = e.getPosicion();
    double r = e.getRadio();
    double qx = max(posicion.getX(), min(p.getX(), posicion.getX() + ancho));
    double qy = max(posicion.getY(), min(p.getY(), posicion.getY() + alto));
    double dx = p.getX() - qx;
    double dy = p.getY() - qy;
    return dx * dx + dy * dy <= r * r;
}
