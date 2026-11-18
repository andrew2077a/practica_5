#ifndef DATOS_H
#define DATOS_H
namespace datos {

// Escenario
const double ANCHO_ESCENA = 800.0;
const double ALTO_ESCENA  = 600.0;

// Fisica
const double GRAVEDAD = 500.0;
const double DT = 1.0 / 60.0;

// Colisiones
const double COEF_RESTITUCION = 0.6;

const double UMBRAL_REPOSO = 15.0;

const double PASO_FISICA = DT / 4.0;
const double MASA_PROYECTIL = 2.0;
const double RADIO_PROYECTIL = 8.0;
const double VEL_MAX_DISPARO  = 800.0;
const double TIEMPO_VIDA_PROYECTIL = 6.0;
const double PAUSA_ENTRE_TURNOS = 1.5;
const double FACTOR_DANO = 0.1;
const double VIDA_BLOQUE = 100.0;

}
#endif // DATOS_H
