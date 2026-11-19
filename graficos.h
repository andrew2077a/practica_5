#ifndef GRAFICOS_H
#define GRAFICOS_H

#include <QMainWindow>
#include <QPixmap>
#include "simulador.h"

class QGraphicsScene;
class QTimer;
class QSpinBox;
class QPushButton;
class QLabel;

class graficos : public QMainWindow
{
    Q_OBJECT

public:
    explicit graficos(QWidget *parent = nullptr);
    ~graficos();

private slots:
    void tick();         // se llama cada ~16 ms
    void alDisparar();   // boton disparar

private:
    Simulador* juego;
    QGraphicsScene* escena;
    QTimer* timer;
    QSpinBox* spinAngulo;
    QSpinBox* spinVelocidad;
    QPushButton* botonDisparar;
    QLabel* etiquetaTurno;
    QPixmap pixComandante;
    QPixmap pixProyectil;
    bool finMostrado;

    void dibujar();      // borra la escena y la dibuja de nuevo
};

#endif // GRAFICOS_H
