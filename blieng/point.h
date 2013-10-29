#ifndef __BLIENG_POINT_H
#define __BLIENG_POINT_H

#include <string>
#include <iostream>

namespace blieng
{

class Point
{
public:
    Point(bool _valid=false) : x(0), y(0), valid(_valid) { }
    Point(double px, double py) : x(px), y(py), valid(true) { }
    virtual ~Point();

    void setX(double px) { x = px; }
    void setY(double py) { y = py; }

    void update(Point another);

    std::string toString();
    bool operator==(const Point &other) const;
    bool operator!=(const Point &other) const;

    bool isValid() const { return valid; }

    Point traverse(Point target, double now, double time);
    double length(Point another);

    double x;
    double y;
    bool valid;
};

}

#endif
