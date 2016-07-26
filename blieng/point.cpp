/*
 * Copyright 2014 Blistud:io
 */

#include "blieng/point.h"

#include <boost/format.hpp>
#include <math.h>

#include <string>

using blieng::Point;

Point::~Point()
{
    x = 0;
    y = 0;
}

std::string Point::toString() const
{
    return (boost::format("%f,%f") % x % y).str();
}

bool Point::operator==(const Point &other) const
{
    return (x == other.x && y == other.y);
}

bool Point::operator!=(const Point &other) const
{
    return (x != other.x || y != other.y);
}

Point Point::operator+(const Point &other)
{
    Point res(x + other.x, y + other.y);
    return res;
}

Point Point::operator-(const Point &other)
{
    Point res(x - other.x, y - other.y);
    return res;
}

Point &Point::operator+=(const Point &other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Point &Point::operator-=(const Point &other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

void Point::update(Point another)
{
    x = another.x;
    y = another.y;
}

Point Point::semiPoint(Point target, double pos) const
{
    if (pos < 0) return *this;
    if (pos >= 1.0) return target;

    double nposx = target.x - x;
    double nposy = target.y - y;

    nposx *= pos;
    nposy *= pos;

    nposx += x;
    nposy += y;

    return Point(nposx, nposy);
}

Point Point::traverse(Point target, double now, double time) const
{
    // Travel to another point, in specific time, specify now as current time
    if (now <= 0) return *this;
    if (now > time) return target;

    double nposx, nposy;
    nposx = target.x - x;
    nposy = target.y - y;
    nposx = (nposx * now / time) + x;
    nposy = (nposy * now / time) + y;

    return Point(nposx, nposy);
}

double Point::length(Point another)
{
    double dx = x - another.x;
    double dy = y - another.y;
    if (dx < 0) dx *= -1;
    if (dy < 0) dy *= -1;

    // return sqrt(dx*dx + dy*dy);
    return dx+dy;
}

double Point::lengthGeo(Point another)
{
    static const double PI = 3.14159;
    static const double R = 6378.137;

    double dlat = (x - another.x) * PI / 180.0;
    double dlon = (y - another.y) * PI / 180.0;

    double a = sin(dlat / 2) * sin(dlat / 2) +
        cos(another.x * PI / 180.0) * cos(x * PI / 180.0) +
        sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double d = R * c;

    // Meters
    return d * 1000;

    /*
    if (dx < 0) dx *= -1;
    if (dy < 0) dy *= -1;

    // return sqrt(dx*dx + dy*dy);
    return dx+dy;
    */
}
