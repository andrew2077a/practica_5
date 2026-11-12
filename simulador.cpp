#include "simulador.h"
#include <cstdlib>
#include <ctime>
#include "datos.h"
#include <string>
#include <fstream>
#include <iostream>


Simulador::Simulador(): cajas(datos::ANCHO_ESCENA, datos::ALTO_ESCENA) {
    srand(std::time(nullptr));
    for (int i=0;i<5;i++){                      // <= cambiado a < (10 partículas)
        int x= rand()%750;
        int y= rand()%550;
        int vx=(rand()%30)+20;
        int vy=(rand()%30)+20;                  // & cambiado a %
        vector2D pos(x,y);
        vector2D vel(vx,vy);
        particula* p = new particula(pos, vel, 10, (rand() % 10) + 1);
        particulas.push_back(p);
    }
    for (int i=0;i<4;i++){                       // 4 obstáculos, como pide la guía
        int x= rand()%750;
        int y= rand()%550;
        vector2D pos(x,y);
        obstaculo* p = new obstaculo(pos,(rand()%10)+1);   // cuadrado: un solo lado
        obstaculos.push_back(p);
    }
}
void Simulador ::simulando(){
    string arch="simu.txt";
    ofstream archivo(arch);
    try{
        if(!archivo.is_open()){
            throw string("no se pudo abrir el archivo");
        }
    }
    catch (string &error) {
        cout << "Error: " << error;
        return;
    }
    double t = 0.0;
    const double T_MAX = 30.0;
    while (t < T_MAX && particulas.size() > 1) {
        for(particula* p:particulas){
            p->actualizar(datos::DT);
        }
        t += datos::DT;

        int id=0;
        for(particula* p:particulas){
            if(cajas.rebotar(*p)){
                archivo << "COL," << t << ",PARED," << id << "\n";
            }
            for(obstaculo* o:obstaculos){
                if(o->colisionar(*p) != ladoImpacto::ninguno){
                    archivo << "COL," << t << ",OBSTACULO," << id << "\n";
                }
            }
            id++;
        }
        for (size_t i = 0; i < particulas.size(); ++i) {
            for (size_t j = i + 1; j < particulas.size(); ++j) {
                if (particulas[i]->colisionParticula(*particulas[j])) {
                    archivo << "COL," << t << ",PARTICULA," << i << "," << j << "\n";
                    delete particulas[j];
                    particulas.erase(particulas.begin() + j);
                    --j;
                }
            }
        }

        id=0;
        for(particula* p:particulas){
            vector2D posi=p->getPosicion();
            archivo << "POS," << t << "," << id << "," << posi.getX() << "," << posi.getY() << "\n";
            id++;
        }
    }
    archivo.close();
}

Simulador::~Simulador() {
    for (particula* p : particulas) delete p;
    for (obstaculo* o : obstaculos) delete o;
}

/*import csv
import matplotlib.pyplot as plt

# Cambia esta ruta si tu simu.txt está en otra carpeta
RUTA = r"C:\Users\gisel\OneDrive\Escritorio\practi_c++\fina2.0\practica_5\simu.txt"

ANCHO, ALTO = 800, 600   # los de datos.h

trayectorias = {}   # id -> ([x...], [y...])
pos_en_t = {}       # (t, id) -> (x, y)
colisiones = []     # (tipo, t, id)

with open(RUTA) as f:
    for fila in csv.reader(f):
        if not fila:
            continue
        if fila[0] == "POS":
            t, i = float(fila[1]), int(fila[2])
            x, y = float(fila[3]), float(fila[4])
            trayectorias.setdefault(i, ([], []))
            trayectorias[i][0].append(x)
            trayectorias[i][1].append(y)
            pos_en_t[(t, i)] = (x, y)
        elif fila[0] == "COL":
            colisiones.append((fila[2], float(fila[1]), int(fila[3])))

fig, ax = plt.subplots(figsize=(9, 7))

# Trayectorias
for i, (xs, ys) in trayectorias.items():
    ax.plot(xs, ys, linewidth=0.8, label=f"Partícula {i}")

# Punto de inicio de cada partícula
for i, (xs, ys) in trayectorias.items():
    ax.scatter(xs[0], ys[0], marker="D", color="green", edgecolors="black",
               s=25, zorder=4,
               label="Inicio" if i == min(trayectorias) else None)
    ax.annotate(str(i), (xs[0], ys[0]), textcoords="offset points",
                xytext=(5, 5), fontsize=7)

# Punto final de cada partícula
for i, (xs, ys) in trayectorias.items():
    ax.scatter(xs[-1], ys[-1], marker="X", color="purple", edgecolors="black",
               s=30, zorder=4,
               label="Final" if i == min(trayectorias) else None)
    ax.annotate(str(i), (xs[-1], ys[-1]), textcoords="offset points",
                xytext=(5, -9), fontsize=7, color="purple")

# Colisiones
estilos = {"PARED": ("o", "blue"), "OBSTACULO": ("s", "red"), "PARTICULA": ("*", "black")}
usados = set()
for tipo, t, i in colisiones:
    if (t, i) in pos_en_t:
        x, y = pos_en_t[(t, i)]
        marcador, color = estilos[tipo]
        ax.scatter(x, y, marker=marcador, color=color, s=15, zorder=3,
                   label=None if tipo in usados else f"Choque: {tipo}")
        usados.add(tipo)

ax.set_xlim(0, ANCHO)
ax.set_ylim(ALTO, 0)   # eje Y invertido: en tu simulación y crece hacia abajo
ax.set_xlabel("Posición X")
ax.set_ylabel("Posición Y")
ax.set_title("Trayectorias de las partículas")
ax.grid(True)
ax.legend(fontsize=7, loc="upper right", ncol=2)
plt.show()

*/
