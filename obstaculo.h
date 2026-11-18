#ifndef OBSTACULO_H
#define OBSTACULO_H
#include "vector2D.h"
#include "entidadFisica.h"
#include "datos.h"


enum class ladoImpacto { ninguno, arriba, abajo, izquierda, derecha, esquina,reposo };

class obstaculo
{
private:
    vector2D esquina;
    double alto;
    double ancho;
    double vida;
    double vidaMax;
    int duenio;
public:
    obstaculo(const vector2D& esquina_, double ancho_, double alto_,
              double vida_ = datos::VIDA_BLOQUE, int dueno_ = 0);
    obstaculo(const vector2D& esquina_, double lado,
              double vida_ = datos::VIDA_BLOQUE, int duenio_ = 0);

    //acceso
    const vector2D& getEsquina() const;
    double getAncho() const;
    double getAlto() const;
    double getVida() const;
    double getVidaMax() const;
    double fraccionVida() const;
    int    getDuenio() const;
    bool   estaDestruido() const;
    void   recibirDano(double dano);

    //Bordes
    double getIzquierda() const;
    double getDerecha() const;
    double getArriba() const;
    double getAbajo() const;
    vector2D getCentro() const;
    //lo hevy :(
    ladoImpacto colisionar(entidadFisica& e,
                           double coefRestitucion = datos::COEF_RESTITUCION) const;
    bool intersecta(const entidadFisica& e) const;
};

#endif // OBSTACULO_H
