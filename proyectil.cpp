#include "proyectil.h"

proyectil::proyectil(const vector2D& pos, const vector2D& vel, int duenio_)
    : particula(pos, vel, datos::MASA_PROYECTIL, datos::RADIO_PROYECTIL, true),
    duenio(duenio_), tiempoVida(datos::TIEMPO_VIDA_PROYECTIL)
{}

void proyectil::actualizar(double dt)
{
    particula::actualizar(dt);
    tiempoVida -= dt;
}

int proyectil::getDuenio() const  {
    return duenio;
}
bool proyectil::haExpirado() const {
    return tiempoVida <= 0.0;
}
