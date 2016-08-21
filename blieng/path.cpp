/*
 * Copyright 2014 Blistud:io
 */

#include "blieng/path.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159
#endif

using blieng::Path;

Path::Path()
{
}

Path::~Path()
{
}

std::string Path::toString() const
{
    std::string res = "";

    bool first = true;
    for (blieng::Point pt : points) {
        if (!first) res += ";";
        first = false;
        res += pt.toString();
    }
    return res;
}

void Path::addPoint(Point pt)
{
    points.push_back(pt);
}

int Path::getPointIndex(Point point)
{
    int index = 0;
    for (blieng::Point pt : points) {
        if (point == pt) return index;
        ++index;
    }
    return -1;
}

blieng::Point Path::getPointAt(unsigned int index)
{
    if (index >=0 && index < size()) {
        return points[index];
    }
    return Point();
}

void Path::updatePointAt(unsigned int index, Point new_point)
{
    if (index >= points.size()) return;

    points[index] = new_point;
}

void Path::updatePoint(Point point, Point new_point)
{
    auto pi = points.begin();
    while (pi != points.end()) {
        if ((*pi) == point) {
            (*pi).update(new_point);
        }
        ++pi;
    }
}

void Path::append(Path another)
{
    points.insert(points.end(), another.points.begin(), another.points.end());
}

Path Path::combine(Path another) const
{
    Path newpath;
    newpath.append(*this);
    newpath.append(another);

    return newpath;
}


blieng::Point Path::takeFirst()
{
    if (points.empty())
        return blieng::Point(false);

    blieng::Point res = points.front();
    points.erase(points.begin());
    return res;
}

blieng::Point Path::takeLast()
{
    if (points.empty())
        return blieng::Point(false);

    blieng::Point res = points.back();
    points.erase(points.end());
    return res;
}

blieng::Point Path::getStart()
{
    if (points.empty())
        return blieng::Point(false);

    return points.front();
}

bool Path::isValid()
{
    return !points.empty();
}

blieng::Point Path::getEnd()
{
    if (points.empty())
        return blieng::Point(false);

    return points.back();
}

void Path::reverse()
{
    std::reverse(points.begin(), points.end());
}

Path Path::copy() const
{
    Path newpath;
    for (blieng::Point pt : points) {
        newpath.addPoint(pt);
    }

    return newpath;
}

Path Path::reversed() const
{
    Path rev_path = this->copy();
    rev_path.reverse();
    return rev_path;
}

double Path::length() const
{
    double len = 0.0;

    blieng::Point f(false);
    for (blieng::Point pt : points) {
        if (f.isValid() && pt != f) {
            len += f.length(pt);
        }
        f = pt;
    }

    return len;
}

double Path::lengthGeo() const
{
    double len = 0.0;

    blieng::Point f(false);
    for (blieng::Point pt : points) {
        if (f.isValid() && pt != f) {
            len += f.lengthGeo(pt);
        }
        f = pt;
    }

    return len;
}

double Path::simpleArea() const
{
    double res = 0;
    blieng::Point f(false);
    for (blieng::Point pt : points) {
        if (f.isValid() && pt != f) {
            res += f.x * pt.y - f.y * pt.x;
        }
        f = pt;
    }

    return fabs(res) / 2.0;
}

template<typename T> std::vector<blieng::Complex> cmplxI(std::pair<blieng::Complex,blieng::Complex> s, T a, blieng::Complex b=blieng::Complex(0,1))
{
    std::vector<blieng::Complex> res;
    blieng::Complex c = s.first;
    blieng::Complex d = s.second;

    d = d - c;
    c = c - a;

    double e = (d * b.conjugate()).imag();
    double e2 = e * e;
    if (! (e * (0 <= e2 && e2 <= e*e))) {
        return res;
    }

    double t2 = (d * c.conjugate()).imag();
    blieng::Complex t3 = b / e;
    blieng::Complex t4 = t3 * t2;

    double v = e * (b * c.conjugate()).imag();
    if (e * (0 <= v && v <= e2)) {
        res.push_back(t4 + a);
    }
    return res;
}

template<typename T> std::vector<std::pair<T,T>> expandTypes(std::vector<T> inp)
{
    std::vector<std::pair<T,T>>  res;
    for (unsigned int i = 0; i < inp.size(); ++i) {
        T a = inp[i];
        T b;
        if (i == inp.size() - 1) {
            b = inp[0];
        } else {
            b = inp[i + 1];
        }
        res.push_back(std::pair<T,T>(a, b));
    }
    return res;
}

double complexArea(std::vector<blieng::Complex> inp)
{
    auto P = expandTypes<blieng::Complex>(inp);
    std::vector<double> i_real;
    for (auto ab : P) {
        blieng::Complex a = ab.first;
        blieng::Complex b = ab.second;
        for (auto e : P) {
            for (auto i : cmplxI<blieng::Complex>(e, a, b - a)) {
                i_real.push_back(i.real());
            }
        }
    }
    std::sort(i_real.begin(), i_real.end());

    auto lr = expandTypes<double>(i_real);
    lr.pop_back();

    std::vector<std::pair<double,double>> bt;
    double ar = 0;
    for (auto tlr : lr) {
        std::vector<double> ij_imag;
        double l = tlr.first;
        double r = tlr.second;
        for (auto e : P) {
            for (auto i : cmplxI<double>(e, l)) {
                for (auto j : cmplxI<double>(e, r)) {
                    ij_imag.push_back(((i + j).conjugate()).imag());
                }
            }
        }
        std::sort(ij_imag.begin(), ij_imag.end());
        auto ij_pairs = expandTypes<double>(ij_imag);
        bool tock = true;
        for (auto i : ij_pairs) {
            if (tock) {
                double b = i.first;
                double t = i.second;
                ar += (t - b) * (r - l) / 2;
            }
            tock ^= true;
        }
    }

    return ar;
}

double Path::area() const
{
    std::vector<blieng::Complex> tmp;
    for (auto pt : points) {
        tmp.push_back(pt.toComplex());
    }
    return complexArea(tmp);
}

double Path::areaGeo() const
{
    Path tmp;
    for (auto pt : points) {
        tmp.addPoint(pt.geoToMeters());
    }
    return tmp.area();
}

bool Path::operator==(const Path &other) const
{
    size_t psize = points.size();
    if (other.points.size() != psize) return false;

    auto a = points.cbegin();
    auto b = other.points.begin();
    while (a != points.cend()) {
        if (*a != *b) return false;
        ++a;
        ++b;
    }

    return true;
}

bool Path::operator!=(const Path &other) const
{
    return !(*this == other);
}

unsigned int Path::size() const
{
    return points.size();
}

double Path::getPartAngle(unsigned int index) const
{
    unsigned int prev = 0;
    if (index > 0) {
        prev = index - 1;
    } else {
        prev = points.size() - 1;
    }
    unsigned int next = index + 1;
    if (next > size()) {
        return 0.0;
    }
    if (next == size()) {
        next = 0;
    }

    if (prev < 0) {
        return 0.0;
    }
    Point p1 = points[index];
    Point p2 = points[next];
    Point p3 = points[prev];

    double a1 = p1.x - p2.x;
    double a2 = p1.y - p2.y;
    double b1 = p1.x - p3.x;
    double b2 = p1.y - p3.y;
    double c1 = p3.x - p2.x;
    double c2 = p3.y - p2.y;

    double p0c = sqrt(a1 * a1 + a2 * a2);
    double p1c = sqrt(b1 * b1 + b2 * b2);
    double p0p1 = sqrt(c1 * c1 + c2 * c2);
    double angle_rad = acos(
        (p1c * p1c + p0c * p0c - p0p1 * p0p1) /
        (2 * p1c * p0c)
        );

    return angle_rad * 180 / M_PI;
}
