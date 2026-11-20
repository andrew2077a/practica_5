#ifndef COMANDANTE_H
#define COMANDANTE_H
#include "vector2D.h"
#include "entidadFisica.h"

class comandante
{
private:
    vector2D posicion;   // esquina superior izquierda
    double ancho;
    double alto;
    double vida;
public:
    comandante(const vector2D& pos, double ancho_, double alto_, double vida_);

    const vector2D& getPosicion() const;
    double getAncho() const;
    double getAlto() const;
    double getVida() const;

    void recibirDano(double dano);
    bool estaMuerto() const;
    bool intersecta(const entidadFisica& e) const;
};

#endif // COMANDANTE_H
