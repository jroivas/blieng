/*
 * Copyright 2016 Blistud:io
 */

#ifndef __BLIENG_COMPLEX_H
#define __BLIENG_COMPLEX_H

namespace blieng
{

class Complex
{
public:
    Complex(double, double);
    virtual ~Complex() {}

    Complex plus(Complex);
    Complex plus(double);

    Complex minus(Complex);
    Complex minus(double);

    Complex mul(Complex);
    Complex mul(double);

    Complex div(double);

    Complex conjugate();

    double real() const { return m_real; }
    double imag() const { return m_imag; }

private:
    double m_real;
    double m_imag;
};

}

#endif
