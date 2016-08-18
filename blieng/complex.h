/*
 * Copyright 2016 Blistud:io
 */

#ifndef __BLIENG_COMPLEX_H
#define __BLIENG_COMPLEX_H

#include <string>

namespace blieng
{

class Complex
{
public:
    Complex(double, double);
    virtual ~Complex() {}

    Complex plus(Complex) const;
    Complex plus(double) const;

    Complex minus(Complex) const;
    Complex minus(double) const;

    Complex mul(Complex) const;
    Complex mul(double) const;

    Complex div(double) const;

    Complex conjugate() const;

    double real() const { return m_real; }
    double imag() const { return m_imag; }

    bool operator==(Complex other) const
    {
        return m_real == other.m_real && m_imag == other.m_imag;
    }
    bool operator!=(Complex other) const
    {
        return m_real != other.m_real || m_imag != other.m_imag;
    }
    Complex operator=(Complex other) const
    {
        return Complex(m_real, m_imag);
    }

    Complex operator+(Complex other) const
    {
        return plus(other);
    }
    Complex operator+(double other) const
    {
        return plus(other);
    }
    Complex operator-(Complex other) const
    {
        return minus(other);
    }
    Complex operator-(double other) const
    {
        return minus(other);
    }
    Complex operator*(Complex other) const
    {
        return mul(other);
    }
    Complex operator*(double other) const
    {
        return mul(other);
    }
    /*
    Complex operator/(Complex other) const
    {
        return div(other);
    }
    */

    std::string to_string() const;


private:
    double m_real;
    double m_imag;
};

}

#endif
