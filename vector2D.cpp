#include "vector2D.h"
#include <cmath>

vector2D::vector2D(double _x,double _y):x(_x),y(_y)
{}
//acesos
double vector2D::getX() const
{return x;}
double vector2D::getY() const
{return y;}
void vector2D::setX(double x_)
{x=x_;}
void vector2D::setY(double y_)
{y=y_;}
//operadores
vector2D vector2D::operator+(const vector2D& otro) const
{return vector2D(x+otro.x,y+otro.y);}
vector2D vector2D::operator-(const vector2D& otro) const
{return vector2D(x-otro.x,y-otro.y);}
vector2D vector2D::operator*(double k) const
{return vector2D(x*k,y*k);}
vector2D vector2D::operator/(double k) const
{return vector2D(x/k,y/k);}
vector2D& vector2D::operator+=(const vector2D& otro)
{
    x += otro.x;
    y += otro.y;
    return *this;
}
vector2D& vector2D::operator-=(const vector2D& otro)
{
    x -= otro.x;
    y -= otro.y;
    return *this;}
vector2D& vector2D::operator*=(double k)
{
    x*=k;
    y*=k;
    return *this;
}
//operaciones
double vector2D::magnitud() const
{return sqrt(x*x +y*y);}
double vector2D::magnitudP2()const
{return x*x + y*y;}
vector2D vector2D::normalizado()const
{
    double n = magnitud();
    if (n > 0.0)
        return vector2D(x / n, y / n);
    return vector2D(0.0, 0.0);
}

double vector2D:: productoPunto(const vector2D& otro)const
{return x*otro.x + y*otro.y;}

vector2D operator*(double k, const vector2D& v)
{ return v * k; }




