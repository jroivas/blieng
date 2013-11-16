#include "path.h"
#include <boost/foreach.hpp>
#include <algorithm>
#include <iostream>

using blieng::Path;

Path::Path()
{
}

Path::~Path()
{
}

std::string Path::toString() {
    std::string res = "";
    bool first = true;
    BOOST_FOREACH(blieng::Point pt, points) {
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
    BOOST_FOREACH(blieng::Point pt, points) {
        if (point == pt) return index;
        ++index;
    }
    return -1;
}

void Path::updatePointAt(int index, Point new_point)
{
    std::vector<Point> new_points;
    int cnt = 0;
    BOOST_FOREACH(blieng::Point pt, points) {
        if (cnt == index) {
            new_points.push_back(new_point);
        } else {
            new_points.push_back(pt);
        }
        ++cnt;
    }

    points = new_points;
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
    BOOST_FOREACH(blieng::Point pt, another.points) {
        addPoint(pt);
    }
}

Path Path::combine(Path another)
{
    Path newpath;
    BOOST_FOREACH(blieng::Point pt, points) {
        newpath.addPoint(pt);
    }

    BOOST_FOREACH(blieng::Point pt, another.points) {
        newpath.addPoint(pt);
    }

    return newpath;
}


blieng::Point Path::takeFirst()
{
    if (!points.empty()) {
        blieng::Point res = points.front();
        points.erase(points.begin());
        return res;
    }
    return blieng::Point(false);
}

blieng::Point Path::takeLast()
{
    if (!points.empty()) {
        blieng::Point res = points.back();
        points.erase(points.end());
        return res;
    }
    return blieng::Point(false);
}

blieng::Point Path::getStart()
{
    if (!points.empty()) {
        return points.front();
    }
    return blieng::Point(false);
}

bool Path::isValid()
{
    return !points.empty();
}

blieng::Point Path::getEnd()
{
    if (!points.empty()) {
        return points.back();
    }
    return blieng::Point(false);
}

void Path::reverse()
{
    std::reverse(points.begin(), points.end());
}

Path Path::copy()
{
    Path newpath;
    BOOST_FOREACH(blieng::Point pt, points) {
        newpath.addPoint(pt);
    }

    return newpath;
}

Path Path::reversed()
{
    Path rev_path = this->copy();
    rev_path.reverse();
    return rev_path;
}

double Path::length()
{
    double len = 0.0;

    blieng::Point f(false);
    BOOST_FOREACH(blieng::Point pt, points) {
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

unsigned int Path::size()
{
    return points.size();
}
