#ifndef CAJA_H
#define CAJA_H
#include "entidadFisica.h"
class caja
{
private:
    double ancho;
    double alto;
public:
    caja(double ancho_, double alto_);

    double getAncho() const;
    double getAlto() const;
    bool rebotar(entidadFisica& e) const;
};

#endif // CAJA_H
