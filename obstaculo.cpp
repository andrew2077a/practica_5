#include "obstaculo.h"
#include <stdexcept>
#include <algorithm>
#include <cmath>
using namespace std;
obstaculo::obstaculo(const vector2D& esquina_, double ancho_, double alto_,
                     double vida_, int duenio_)
    : esquina(esquina_), alto(alto_), ancho(ancho_),
    vida(vida_), vidaMax(vida_), duenio(duenio_)
{
    if (ancho_ <= 0.0 || alto_ <= 0.0)
        throw invalid_argument("El ancho y el alto del obstaculo deben ser mayores que cero");
    if (vida_ <= 0.0)
        throw invalid_argument("La vida del obstaculo debe ser mayor que cero");
    if (duenio_ != 0 && duenio_ != 1)
        throw invalid_argument("El dueno debe ser 0 o 1");
}
obstaculo::obstaculo(const vector2D& esquina_, double lado, double vida_, int dueno_)
    : obstaculo(esquina_, lado, lado, vida_, dueno_) {}

//acceso
const vector2D& obstaculo::getEsquina() const
{ return esquina; }
double obstaculo::getAncho() const
{ return ancho; }
double obstaculo::getAlto() const
{ return alto; }
//bordes
double obstaculo::getIzquierda() const
{ return esquina.getX(); }
double obstaculo::getDerecha() const
{ return esquina.getX() + ancho; }
double obstaculo::getArriba() const
{ return esquina.getY(); }
double obstaculo::getAbajo() const
{ return esquina.getY() + alto; }
vector2D obstaculo::getCentro() const
{return vector2D(esquina.getX() + ancho / 2.0, esquina.getY() + alto / 2.0);}
double obstaculo::getVida() const
{ return vida; }
double obstaculo::getVidaMax() const
{ return vidaMax; }
double obstaculo::fraccionVida() const
{ return vida / vidaMax; }
int    obstaculo::getDuenio() const
{ return duenio; }
bool   obstaculo::estaDestruido() const
{ return vida <= 0.0; }

void obstaculo::recibirDano(double dano)
{
    if (dano < 0.0)
        throw invalid_argument("El dano no puede ser negativo");
    vida = max(0.0, vida - dano);
}

ladoImpacto obstaculo::colisionar(entidadFisica& e,double coefRestitucion) const
{
    if (coefRestitucion < 0.0 || coefRestitucion > 1.0)
    {
        throw out_of_range("El coeficiente de restitucion debe estar entre 0 y 1");
    }
    vector2D p=e.getPosicion();
    vector2D v=e.getVelocidad();
    double r=e.getRadio();
    //con esto sabemos si los puntos x,y de la particula estan dentro del rec
    //=MAX(minimo rec,MIN(la pocision,maximo rec))
    double qx = max(getIzquierda(), min(p.getX(), getDerecha()));
    double qy = max(getArriba(),    min(p.getY(), getAbajo()));
    vector2D q(qx, qy);
    //aqui vemos si hay choque si la distancia entre la particula
    //y el rec es menor que su radio^2 hay choque
    vector2D d = p - q;
    double dist2 = d.magnitudP2();
    if (dist2 > r * r)
    {
        return ladoImpacto::ninguno;
    }
    vector2D n;
    double penetracion;
    ladoImpacto lado;
    //con esto sabemos que tan metido esta la particula
    if (dist2 > 0.0)
    {
        double dist = sqrt(dist2);
        n = d / dist;
        penetracion = r - dist;
        bool fueraX = p.getX() < getIzquierda() || p.getX() > getDerecha();
        bool fueraY = p.getY() < getArriba()    || p.getY() > getAbajo();

        if (fueraX && fueraY)
            lado = ladoImpacto::esquina;
        else if (fueraX)
            lado = (p.getX() < getIzquierda()) ? ladoImpacto::izquierda : ladoImpacto::derecha;
        else
            lado = (p.getY() < getArriba()) ? ladoImpacto::arriba : ladoImpacto::abajo;
    }
    else {
        // 2b. Centro DENTRO (iba muy rapido): sacar por el lado mas cercano
        double dIzq = p.getX() - getIzquierda();
        double dDer = getDerecha() - p.getX();
        double dArr = p.getY() - getArriba();
        double dAbj = getAbajo() - p.getY();
        double m = min(min(dIzq, dDer),min(dArr, dAbj));

        if (m == dIzq)      { n = vector2D(-1, 0); lado = ladoImpacto::izquierda; }
        else if (m == dDer) { n = vector2D( 1, 0); lado = ladoImpacto::derecha; }
        else if (m == dArr) { n = vector2D(0, -1); lado = ladoImpacto::arriba; }
        else                { n = vector2D(0,  1); lado = ladoImpacto::abajo; }
        penetracion = m + r;
    }
    // 3. Si ya se aleja, no rebotar (evita que vibre pegada)
    double vn = v.productoPunto(n);
    if (vn >= 0.0)
        return ladoImpacto::ninguno;

    // 4. Sacarla del obstaculo y separar la velocidad
    p += n * penetracion;
    vector2D vPerp = n * vn;
    vector2D vPar  = v - vPerp;

    // 5. Apoyada (gravedad): se anula v perpendicular y no cuenta como choque
    if (-vn < datos::UMBRAL_REPOSO) {
        e.setPosicion(p);
        e.setVelocidad(vPar);
        if (vPar.magnitud() < datos::UMBRAL_REPOSO)
            return ladoImpacto::reposo;   // apoyada y casi quieta: desaparece
        return ladoImpacto::ninguno;
    }

    // 6. Rebote inelastico
    e.setPosicion(p);
    e.setVelocidad(vPar - vPerp * coefRestitucion);
    return lado;
}

bool obstaculo::intersecta(const entidadFisica& e) const
{
    const vector2D& p = e.getPosicion();
    double r = e.getRadio();
    // punto del rectangulo mas cercano al centro del circulo
    double qx = max(getIzquierda(), min(p.getX(), getDerecha()));
    double qy = max(getArriba(),    min(p.getY(), getAbajo()));
    double dx = p.getX() - qx;
    double dy = p.getY() - qy;
    return dx * dx + dy * dy <= r * r;
}







