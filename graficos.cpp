#include "graficos.h"
#include "datos.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>


// =====================================================================
static const QString RUTA_COMANDANTE = ":/sprites/images-removebg-preview.png";
static const QString RUTA_PROYECTIL  = ":/sprites/Red_energy_ball_sprite_2K_20261024221749-removebg-preview.png";
// =====================================================================

graficos::graficos(QWidget *parent)
    : QMainWindow(parent)
{
    juego = new Simulador();
    escena = new QGraphicsScene(this);
    timer = new QTimer(this);
    finMostrado = false;

    pixComandante = QPixmap(RUTA_COMANDANTE).scaled(60, 80);
    pixProyectil  = QPixmap(RUTA_PROYECTIL).scaled(16, 16);

    escena->setSceneRect(0, 0, datos::ANCHO_ESCENA, datos::ALTO_ESCENA);
    escena->setBackgroundBrush(QColor(200, 225, 245));

    QGraphicsView* vista = new QGraphicsView(escena);
    vista->setFixedSize(int(datos::ANCHO_ESCENA) + 4, int(datos::ALTO_ESCENA) + 4);

    etiquetaTurno = new QLabel();
    spinAngulo = new QSlider(Qt::Horizontal);
    spinAngulo->setRange(0, 90);
    spinAngulo->setValue(45);
    spinAngulo->setMinimumWidth(120);

    spinVelocidad = new QSlider(Qt::Horizontal);
    spinVelocidad->setRange(0, int(datos::VEL_MAX_DISPARO));
    spinVelocidad->setValue(400);
    spinVelocidad->setMinimumWidth(120);

    // numeritos que muestran el valor mientras mueves la barra
    QLabel* valAngulo = new QLabel("45");
    QLabel* valVelocidad = new QLabel("400");
    connect(spinAngulo, &QSlider::valueChanged, this,
            [=](int v){ valAngulo->setText(QString::number(v)); });
    connect(spinVelocidad, &QSlider::valueChanged, this,
            [=](int v){ valVelocidad->setText(QString::number(v)); });

    botonDisparar = new QPushButton("Disparar");

    QHBoxLayout* controles = new QHBoxLayout();
    controles->addWidget(etiquetaTurno);
    controles->addStretch();
    controles->addWidget(new QLabel("Angulo:"));
    controles->addWidget(spinAngulo);
    controles->addWidget(valAngulo);
    controles->addWidget(new QLabel("Velocidad:"));
    controles->addWidget(spinVelocidad);
    controles->addWidget(valVelocidad);
    controles->addWidget(botonDisparar);

    QWidget* central = new QWidget(this);
    QVBoxLayout* principal = new QVBoxLayout(central);
    principal->addWidget(vista);
    principal->addLayout(controles);
    setCentralWidget(central);

    connect(botonDisparar, &QPushButton::clicked, this, &graficos::alDisparar);
    connect(timer, &QTimer::timeout, this, &graficos::tick);
    timer->start(16);

    dibujar();
}

graficos::~graficos()
{
    delete juego;
}

void graficos::dibujar()
{
    escena->clear();

    for (int i = 0; i < 2; i++) {
        jugador* j = juego->getJugador(i);

        // canon
        escena->addRect(j->getPosCanon().getX() - 20, j->getPosCanon().getY() - 15,
                        40, 30, QPen(Qt::black), QBrush(Qt::darkGray));

        // obstaculos con su vida
        for (obstaculo* o : j->getObstaculos()) {
            if (o->estaDestruido()) continue;
            escena->addRect(o->getEsquina().getX(), o->getEsquina().getY(),
                            o->getAncho(), o->getAlto(), QPen(Qt::black),
                            QBrush(i == 0 ? QColor(255, 200, 150) : QColor(170, 200, 255)));
            QGraphicsTextItem* t = escena->addText(QString::number(int(o->getVida())));
            t->setPos(o->getCentro().getX() - 12, o->getCentro().getY() - 12);
        }

        // comandante con su vida
        comandante* c = j->getComandante();
        if (!c->estaMuerto()) {
            double x = c->getPosicion().getX();
            double y = c->getPosicion().getY();
            if (pixComandante.isNull())
                escena->addRect(x, y, c->getAncho(), c->getAlto(),
                                QPen(Qt::black), QBrush(Qt::darkGreen));
            else
                escena->addPixmap(pixComandante)->setPos(x, y);
            escena->addText(QString::number(int(c->getVida())))->setPos(x, y - 22);
        }
    }

    // proyectil
    const proyectil* p = juego->getProyectil();
    if (p) {
        double r = datos::RADIO_PROYECTIL;
        double x = p->getPosicion().getX() - r;
        double y = p->getPosicion().getY() - r;
        if (pixProyectil.isNull())
            escena->addEllipse(x, y, 2 * r, 2 * r, QPen(Qt::black), QBrush(Qt::red));
        else
            escena->addPixmap(pixProyectil)->setPos(x, y);
    }
}

void graficos::alDisparar()
{
    jugador* j = juego->getJugador(juego->getTurno());
    j->setAngulo(spinAngulo->value());
    j->setVelocidad(spinVelocidad->value());
    juego->disparar();
}

void graficos::tick()
{
    juego->paso();
    dibujar();

    bool puede = (juego->getEstado() == estadoJuego::esperando);
    botonDisparar->setEnabled(puede);
    etiquetaTurno->setText(QString("Turno del jugador %1").arg(juego->getTurno() + 1));

    if (juego->getEstado() == estadoJuego::fin && !finMostrado) {
        finMostrado = true;
        timer->stop();
        QMessageBox::information(this, "Fin del juego",QString("Gano el jugador %1").arg(juego->getGanador() + 1));
        close();
    }
}
