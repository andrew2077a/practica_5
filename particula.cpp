#include "particula.h"
#include "datos.h"

particula::particula(const vector2D& pos, const vector2D& vel, double masa_, double radio_,
                     bool usaGravedad_)
    : entidadFisica(pos, vel, masa_, radio_), usaGravedad(usaGravedad_) {}

// x = x0 + v*dt + 1/2*a*dt^2
void particula::actualizar(double dt) {
    vector2D aceleracion(0.0, usaGravedad ? datos::GRAVEDAD : 0.0);
    posicion  += velocidad * dt + aceleracion * (0.5 * dt * dt);
    velocidad += aceleracion * dt;
}

bool particula::getUsaGravedad() const { return usaGravedad; }
void particula::setUsaGravedad(bool g) { usaGravedad = g; }