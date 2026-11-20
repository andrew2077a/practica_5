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

// Rutas de los sprites en recursos.qrc
static const QString RUTA_COMANDANTE = ":/sprites/images-removebg-preview.png";
static const QString RUTA_PROYECTIL  = ":/sprites/Red_energy_ball_sprite_2K_20261024221749-removebg-preview.png";

graficos::graficos(QWidget *parent)
    : QMainWindow(parent)
{
    juego = new Simulador();           // Lógica y física del juego
    escena = new QGraphicsScene(this); // Lienzo 2D
    timer = new QTimer(this);          // Reloj del bucle principal
    finMostrado = false;

    // Carga y escala de sprites
    pixComandante = QPixmap(RUTA_COMANDANTE).scaled(60, 80);
    pixProyectil  = QPixmap(RUTA_PROYECTIL).scaled(16, 16);

    // Configuración del área de juego y visor
    escena->setSceneRect(0, 0, datos::ANCHO_ESCENA, datos::ALTO_ESCENA);
    escena->setBackgroundBrush(QColor(200, 225, 245));

    QGraphicsView* vista = new QGraphicsView(escena);
    vista->setFixedSize(int(datos::ANCHO_ESCENA) + 4, int(datos::ALTO_ESCENA) + 4);

    // Controles de interfaz (turno y sliders)
    etiquetaTurno = new QLabel();
    spinAngulo = new QSlider(Qt::Horizontal);
    spinAngulo->setRange(-180, 180);
    spinAngulo->setValue(45);
    spinAngulo->setMinimumWidth(120);

    spinVelocidad = new QSlider(Qt::Horizontal);
    spinVelocidad->setRange(0, int(datos::VEL_MAX_DISPARO));
    spinVelocidad->setValue(400);
    spinVelocidad->setMinimumWidth(120);

    // Etiquetas dinámicas que muestran el valor de cada slider
    QLabel* valAngulo = new QLabel("45");
    QLabel* valVelocidad = new QLabel("400");
    connect(spinAngulo, &QSlider::valueChanged, this,
            [=](int v){ valAngulo->setText(QString::number(v)); });
    connect(spinVelocidad, &QSlider::valueChanged, this,
            [=](int v){ valVelocidad->setText(QString::number(v)); });

    botonDisparar = new QPushButton("Disparar");

    // Fila inferior de controles
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

    // Estructura vertical principal de la ventana
    QWidget* central = new QWidget(this);
    QVBoxLayout* principal = new QVBoxLayout(central);
    principal->addWidget(vista);
    principal->addLayout(controles);
    setCentralWidget(central);

    // Conexiones de eventos (clic y reloj a ~60 FPS)
    connect(botonDisparar, &QPushButton::clicked, this, &graficos::alDisparar);
    connect(timer, &QTimer::timeout, this, &graficos::tick);
    timer->start(16);

    dibujar();
}

graficos::~graficos()
{
    delete juego; // Libera memoria del simulador
}

void graficos::dibujar()
{
    escena->clear(); // Limpia el fotograma anterior

    for (int i = 0; i < 2; i++) {
        jugador* j = juego->getJugador(i);

        // Cañón
        escena->addRect(j->getPosCanon().getX() - 20, j->getPosCanon().getY() - 15,
                        40, 30, QPen(Qt::black), QBrush(Qt::darkGray));

        // Obstáculos vivos y su texto de vida
        for (obstaculo* o : j->getObstaculos()) {
            if (o->estaDestruido()) continue;
            escena->addRect(o->getEsquina().getX(), o->getEsquina().getY(),
                            o->getAncho(), o->getAlto(), QPen(Qt::black),
                            QBrush(i == 0 ? QColor(255, 200, 150) : QColor(170, 200, 255)));
            QGraphicsTextItem* t = escena->addText(QString::number(int(o->getVida())));
            t->setPos(o->getCentro().getX() - 12, o->getCentro().getY() - 12);
        }

        // Comandante vivo y su texto de vida (usa rectángulo si falla el sprite)
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

    // Proyectil activo (centrado restando el radio)
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
    // Pasa los valores de los sliders al jugador en turno y dispara
    jugador* j = juego->getJugador(juego->getTurno());
    j->setAngulo(spinAngulo->value());
    j->setVelocidad(spinVelocidad->value());
    juego->disparar();
}

void graficos::tick()
{
    juego->paso(); // Avanza física
    dibujar();     // Actualiza pantalla

    // Bloquea botón mientras el proyectil vuela y actualiza turno
    bool puede = (juego->getEstado() == estadoJuego::esperando);
    botonDisparar->setEnabled(puede);
    etiquetaTurno->setText(QString("Turno del jugador %1").arg(juego->getTurno() + 1));

    // Detecta fin de partida una sola vez
    if (juego->getEstado() == estadoJuego::fin && !finMostrado) {
        finMostrado = true;
        timer->stop();
        QMessageBox::information(this, "Fin del juego", QString("Gano el jugador %1").arg(juego->getGanador() + 1));
        close();
    }
}
