#ifndef SIMULADOR_H
#define SIMULADOR_H
#include "jugador.h"
#include "proyectil.h"
#include "caja.h"

enum class estadoJuego { esperando, enVuelo, pausa, fin };

class Simulador
{
private:
    jugador* jugadores[2];
    proyectil* actual;        // nullptr si no hay proyectil en vuelo
    caja cajas;
    int turno;                // 0 o 1
    estadoJuego estado;
    double tiempoPausa;
    int ganador;              // -1 mientras nadie gane

    void construirNivel();
    bool procesarColisiones();   // true si el proyectil ya debe terminar
    void terminarVuelo();
    void cambiarTurno();
    void verificarVictoria();

public:
    Simulador();
    ~Simulador();

    bool disparar();             // usa angulo y velocidad del jugador en turno
    void paso();                 // un tick de DT segundos

    jugador* getJugador(int i) const;
    const proyectil* getProyectil() const;
    int getTurno() const;
    estadoJuego getEstado() const;
    int getGanador() const;
};

#endif // SIMULADOR_H
