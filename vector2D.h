#ifndef VECTOR2D_H
#define VECTOR2D_H

class vector2D
{
private:
    double x;
    double y;
public:
    vector2D(double x=0.0,double y=0.0);
    //acesos
    double getX() const;
    double getY() const;
    void setX(double x_);
    void setY(double y_);
    //operadores
    vector2D operator+(const vector2D& otro) const;
    vector2D operator-(const vector2D& otro) const;
    vector2D operator*(double k) const;
    vector2D operator/(double k) const;
    vector2D& operator+=(const vector2D& otro);
    vector2D& operator-=(const vector2D& otro);
    vector2D& operator*=(double k);
    //ya operaciones para las fisicas
    double magnitud()const;
    double magnitudP2()const;
    vector2D normalizado()const;
    double productoPunto(const vector2D& otro)const;
};
vector2D operator*(double k, const vector2D& v);

#endif // VECTOR2D_H
