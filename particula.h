#ifndef PARTICULA_H
#define PARTICULA_H
#include "entidadFisica.h"

class particula : public entidadFisica
{
private:
    bool usaGravedad;
public:
    particula(const vector2D& pos, const vector2D& vel, double masa_, double radio_,
              bool usaGravedad_ = true);
    //movimiento
    void actualizar(double dt)override;
    //acceso
    bool getUsaGravedad() const;
    void setUsaGravedad(bool g);
};
#endif // PARTICULA_H
