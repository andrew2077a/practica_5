#include "simulador.h"
#include "datos.h"

Simulador::Simulador()
    : actual(nullptr), cajas(datos::ANCHO_ESCENA, datos::ALTO_ESCENA),
    turno(0), estado(estadoJuego::esperando), tiempoPausa(0.0), ganador(-1)
{
    jugadores[0] = nullptr;
    jugadores[1] = nullptr;
    construirNivel();
}

Simulador::~Simulador()
{
    delete actual;
    delete jugadores[0];
    delete jugadores[1];
}

// Distribucion igual a la figura 2 del PDF: dos pilares, un techo y el
// comandante en el hueco. El jugador 1 queda como espejo del 0.
void Simulador::construirNivel()
{
    const double W = datos::ANCHO_ESCENA;
    const double H = datos::ALTO_ESCENA;

    const double anchoPilar = 45.0, altoPilar = 90.0;
    const double hueco = 75.0, altoTecho = 45.0;
    const double anchoBase = 2 * anchoPilar + hueco;
    const double margen = 60.0;
    const double vidaPilar = 200.0, vidaTecho = 100.0, vidaComandante = 100.0;
    const double anchoCmd = 40.0, altoCmd = 80.0;

    for (int i = 0; i < 2; i++) {
        double x0 = (i == 0) ? margen : W - margen - anchoBase;

        jugadores[i] = new jugador(i, vector2D(i == 0 ? 40.0 : W - 40.0, 300.0));

        // pilar izquierdo, pilar derecho y techo
        jugadores[i]->agregarObstaculo(
            new obstaculo(vector2D(x0, H - altoPilar), anchoPilar, altoPilar, vidaPilar, i));
        jugadores[i]->agregarObstaculo(
            new obstaculo(vector2D(x0 + anchoPilar + hueco, H - altoPilar),
                          anchoPilar, altoPilar, vidaPilar, i));
        jugadores[i]->agregarObstaculo(
            new obstaculo(vector2D(x0, H - altoPilar - altoTecho),
                          anchoBase, altoTecho, vidaTecho, i));

        // comandante en el hueco
        double xc = x0 + anchoPilar + (hueco - anchoCmd) / 2.0;
        jugadores[i]->setComandante(
            new comandante(vector2D(xc, H - altoCmd), anchoCmd, altoCmd, vidaComandante));
    }
}

bool Simulador::disparar()
{
    if (estado != estadoJuego::esperando) return false;
    actual = jugadores[turno]->crearProyectil();
    estado = estadoJuego::enVuelo;
    return true;
}

void Simulador::paso()
{
    if (estado == estadoJuego::enVuelo && actual) {
        // 4 subpasos de PASO_FISICA = un DT completo (evita atravesar bloques)
        for (int i = 0; i < 4 && estado == estadoJuego::enVuelo; i++) {
            actual->actualizar(datos::PASO_FISICA);
            cajas.rebotar(*actual);
            if (procesarColisiones() || actual->haExpirado())
                terminarVuelo();
        }
    }
    else if (estado == estadoJuego::pausa) {
        tiempoPausa -= datos::DT;
        if (tiempoPausa <= 0.0) {
            cambiarTurno();
            estado = estadoJuego::esperando;
        }
    }
}

bool Simulador::procesarColisiones()
{
    int duenioProy = actual->getDuenio();

    for (int j = 0; j < 2; j++) {
        for (obstaculo* o : jugadores[j]->getObstaculos()) {
            if (o->estaDestruido()) continue;

            // la velocidad se guarda ANTES porque colisionar() la modifica
            double vImpacto = actual->getVelocidad().magnitud();
            ladoImpacto lado = o->colisionar(*actual);

            if (lado == ladoImpacto::ninguno) continue;
            if (lado == ladoImpacto::reposo) return true;   // quedo quieta

            // solo se danan los obstaculos del rival
            if (j != duenioProy)
                o->recibirDano(datos::FACTOR_DANO * actual->getMasa() * vImpacto);
        }
    }

    // comandante rival
    comandante* c = jugadores[1 - duenioProy]->getComandante();
    if (c && !c->estaMuerto() && c->intersecta(*actual)) {
        double vImpacto = actual->getVelocidad().magnitud();
        c->recibirDano(datos::FACTOR_DANO * actual->getMasa() * vImpacto);
        return true;
    }
    return false;
}

void Simulador::terminarVuelo()
{
    delete actual;
    actual = nullptr;
    verificarVictoria();
    if (ganador == -1) {
        estado = estadoJuego::pausa;
        tiempoPausa = datos::PAUSA_ENTRE_TURNOS;
    } else {
        estado = estadoJuego::fin;
    }
}

void Simulador::cambiarTurno()
{
    turno = 1 - turno;
}

void Simulador::verificarVictoria()
{
    for (int i = 0; i < 2; i++)
        if (jugadores[i]->haPerdido())
            ganador = 1 - i;
}

jugador* Simulador::getJugador(int i) const{
    return jugadores[i]; }
const proyectil* Simulador::getProyectil() const{
    return actual; }
int Simulador::getTurno() const {
    return turno; }
estadoJuego Simulador::getEstado() const {
    return estado; }
int Simulador::getGanador() const{
    return ganador; }
