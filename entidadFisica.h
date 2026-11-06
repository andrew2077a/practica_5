#ifndef ENTIDADFISICA_H
#define ENTIDADFISICA_H
#include "vector2D.h"

class entidadFisica
{
protected:
    vector2D velocidad;
    vector2D posicion;
private:
    double masa;
    double radio;
public:
    entidadFisica();
    entidadFisica(const vector2D& pos, const vector2D& vel, double masa_, double radio_);
    virtual ~entidadFisica();
    //actualizacion a medida del tiempo
    virtual void actualizar(double dt) = 0;
    //acceso
    const vector2D& getPosicion() const;
    const vector2D& getVelocidad() const;
    double getMasa() const;
    double getRadio() const;
    void setPosicion(const vector2D& p);
    void setVelocidad(const vector2D& v);
    void setMasa(double m);
    void setRadio(double r);
    //magnitudes fisicas
    vector2D momento() const;
    double energiaCinetica() const;

};

#endif // ENTIDADFISICA_H
