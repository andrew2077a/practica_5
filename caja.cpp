#include "caja.h"
#include <stdexcept>


caja::caja(double ancho_, double alto_) : ancho(ancho_), alto(alto_)
{
    if (ancho_ <= 0.0 || alto_ <= 0.0)
        throw std::invalid_argument("El ancho y el alto de la caja deben ser mayores que cero");
}

double caja::getAncho() const{
    return ancho; }
double caja::getAlto() const{
    return alto; }

bool caja::rebotar(entidadFisica& e) const {
    //copiamos los datos de la entidad
    vector2D pos = e.getPosicion();
    vector2D vel = e.getVelocidad();
    double r = e.getRadio();
    bool choco = false;

    // Pared izquierda: toca (x - r < 0) Y va hacia la izquierda (vx < 0)
    if (pos.getX() - r < 0.0 && vel.getX() < 0.0){
        pos.setX(r);
        vel.setX(-vel.getX());
        choco = true;
    }
    // Pared derecha
    if (pos.getX() + r > ancho && vel.getX() > 0.0){
        pos.setX(ancho - r);
        vel.setX(-vel.getX());
        choco = true;
    }
    // Techo (y menor, porque y crece hacia abajo)
    if (pos.getY() - r < 0.0 && vel.getY() < 0.0){
        pos.setY(r);
        vel.setY(-vel.getY());
        choco = true;
    }
    // Piso
    if (pos.getY() + r > alto && vel.getY() > 0.0){
        pos.setY(alto - r);
        vel.setY(-vel.getY());
        choco = true;
    }
    if (choco){
        e.setPosicion(pos);
        e.setVelocidad(vel);
    }
    return choco;
}
