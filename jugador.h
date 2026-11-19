#ifndef JUGADOR_H
#define JUGADOR_H
#include <vector>
#include "vector2D.h"
#include "obstaculo.h"
#include "comandante.h"
#include "proyectil.h"

class jugador
{
private:
    int numero;                              // 0 (izquierda) o 1 (derecha)
    vector2D posCanon;
    double angulo;                           // grados
    double velocidad;
    std::vector<obstaculo*> infraestructura;
    comandante* representante;
public:
    jugador(int numero_, const vector2D& posCanon_);
    ~jugador();

    const vector2D& getPosCanon() const;
    void setAngulo(double a);
    void setVelocidad(double v);

    void agregarObstaculo(obstaculo* o);
    const std::vector<obstaculo*>& getObstaculos() const;
    void setComandante(comandante* c);
    comandante* getComandante() const;

    bool haPerdido() const;
    proyectil* crearProyectil() const;
};

#endif // JUGADOR_H
