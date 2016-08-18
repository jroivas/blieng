/*
 * Copyright 2016 Blistud:io
 */

#include "complex.h"

using blieng::Complex;

Complex::Complex(double real, double imag)
    : m_real(real),
    m_imag(imag)
{
}

Complex Complex::mul(Complex other) const
{
    Complex tmp(m_real * other.m_real - m_imag * other.m_imag, m_real * other.m_imag + m_imag * other.m_real);
    return tmp;
}

Complex Complex::mul(double v) const
{
    Complex tmp(m_real * v, m_imag * v);
    return tmp;
}

Complex Complex::div(double v) const
{
    Complex tmp(m_real / v, m_imag / v);
    return tmp;
}

Complex Complex::plus(Complex other) const
{
    Complex tmp(m_real + other.m_real, m_imag + other.m_imag);
    return tmp;
}

Complex Complex::plus(double v) const
{
    Complex tmp(m_real + v, m_imag + v);
    return tmp;
}

Complex Complex::minus(Complex other) const
{
    Complex tmp(m_real - other.m_real, m_imag - other.m_imag);
    return tmp;
}

Complex Complex::minus(double v) const
{
    Complex tmp(m_real - v, m_imag - v);
    return tmp;
}

Complex Complex::conjugate() const
{
    Complex tmp(m_real, -m_imag);
    return tmp;
}

std::string Complex::to_string() const
{
    return std::to_string(m_real) + "+" + std::to_string(m_imag) + "i";
}
