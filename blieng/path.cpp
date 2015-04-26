/*
 * Copyright 2014 Blistud:io
 */

#include "blieng/path.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

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
    int next = index + 1;
    if (next > size()) {
        return 0.0;
    }
    if (next == size()) {
        next = 0;
    }

    Point a = points[index];
    Point b = points[next];

    double angle_rad = atan2(b.x - a.x, b.y - a.y);
    return angle_rad * 180 / M_PI;
}
