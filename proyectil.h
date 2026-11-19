#ifndef PROYECTIL_H
#define PROYECTIL_H
#include "particula.h"
#include "datos.h"

class proyectil : public particula
{
private:
    int duenio;          // jugador que lo lanzo (0 o 1)
    double tiempoVida;   // segundos restantes
public:
    proyectil(const vector2D& pos, const vector2D& vel, int duenio_);
    void actualizar(double dt) override;
    int getDuenio() const;
    bool haExpirado() const;
};

#endif // PROYECTIL_H
