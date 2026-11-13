#include "entidadFisica.h"
#include <stdexcept>
using namespace std;

entidadFisica::entidadFisica() {}
entidadFisica::entidadFisica(const vector2D& vel_,const vector2D& pos,
                             double masa_, double radio_):velocidad(vel_),posicion(pos),masa(1.0),radio(1.0)
{
    setMasa(masa_);
    setRadio(radio_);
}
entidadFisica::~entidadFisica(){}

//aceso
const vector2D& entidadFisica::getPosicion() const{
    return posicion; }

const vector2D& entidadFisica::getVelocidad() const{
    return velocidad; }

double entidadFisica::getMasa() const{
    return masa; }

double entidadFisica::getRadio() const{
    return radio; }

void entidadFisica::setPosicion(const vector2D& p){
    posicion = p; }

void entidadFisica::setVelocidad(const vector2D& v){
    velocidad = v; }

void entidadFisica::setMasa(double m){
    if (m <= 0.0)
        throw invalid_argument("La masa debe ser mayor que cero");
    masa = m;
}

void entidadFisica::setRadio(double r){
    if (r <= 0.0)
        throw invalid_argument("El radio debe ser mayor que cero");
    radio = r;
}
//magnitudes
vector2D entidadFisica::momento() const{
    return velocidad * masa;}

double entidadFisica::energiaCinetica() const{
    return 0.5 * masa * velocidad.magnitudP2();}
