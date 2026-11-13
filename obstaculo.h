#ifndef OBSTACULO_H
#define OBSTACULO_H
#include "vector2D.h"
#include "entidadFisica.h"
#include "datos.h"


enum class ladoImpacto { ninguno, arriba, abajo, izquierda, derecha, esquina };

class obstaculo
{
private:
    vector2D esquina;
    double alto;
    double ancho;
public:
    obstaculo(const vector2D& esquina_, double ancho_, double alto_);
    obstaculo(const vector2D& esquina_, double lado);

    //acceso
    const vector2D& getEsquina() const;
    double getAncho() const;
    double getAlto() const;

    //Bordes
    double getIzquierda() const;
    double getDerecha() const;
    double getArriba() const;
    double getAbajo() const;
    vector2D getCentro() const;
    //lo hevy :(
    ladoImpacto colisionar(entidadFisica& e,
                           double coefRestitucion = datos::COEF_RESTITUCION) const;
};

#endif // OBSTACULO_H
