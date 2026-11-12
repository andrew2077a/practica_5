#include "particula.h"
#include "datos.h"
#include <cmath>
particula::particula(const vector2D& pos, const vector2D& vel, double masa_, double radio_,
                     bool usaGravedad_)
    : entidadFisica(vel, pos, masa_, radio_), usaGravedad(usaGravedad_) {}

void particula::actualizar(double dt) {
    vector2D aceleracion(0.0, usaGravedad ? datos::GRAVEDAD : 0.0);
    posicion  += velocidad * dt + aceleracion * (0.5 * dt * dt);
    velocidad += aceleracion * dt;
}

bool particula::getUsaGravedad() const {
    return usaGravedad; }
void particula::setUsaGravedad(bool g) {
    usaGravedad = g; }



bool particula::colisionParticula(particula& par) {
    vector2D diff = getPosicion() - par.getPosicion();
    double sumaRadios = getRadio() + par.getRadio();

    // (dx^2 + dy^2) <= (r1 + r2)^2  -> hay choque
    if (diff.magnitudP2() > sumaRadios * sumaRadios)
        return false;

    double m1 = getMasa();
    double m2 = par.getMasa();
    double M  = m1 + m2;
    double r1 = getRadio();
    double r2 = par.getRadio();

    // Se calcula todo ANTES de modificar nada
    vector2D nuevaVel = (velocidad * m1 + par.getVelocidad() * m2) / M;  // conservación del momento
    vector2D nuevaPos = (posicion  * m1 + par.getPosicion()  * m2) / M;  // centro de masa
    double nuevoRadio = sqrt(r1 * r1 + r2 * r2);                         // conserva el área

    setMasa(M);
    setRadio(nuevoRadio);
    posicion  = nuevaPos;
    velocidad = nuevaVel;

    return true;
}
