QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    caja.cpp \
    comandante.cpp \
    entidadFisica.cpp \
    graficos.cpp \
    jugador.cpp \
    main.cpp \
    obstaculo.cpp \
    particula.cpp \
    proyectil.cpp \
    simulador.cpp \
    vector2D.cpp

HEADERS += \
    caja.h \
    comandante.h \
    datos.h \
    entidadFisica.h \
    graficos.h \
    jugador.h \
    obstaculo.h \
    particula.h \
    proyectil.h \
    simulador.h \
    vector2D.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    recursos.qrc
