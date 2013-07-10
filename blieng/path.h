#ifndef __BLIENG_PATH_H
#define __BLIENG_PATH_H

#include <string>
#include <vector>

#include "point.h"

namespace blieng
{

class Path
{
public:
    Path();
    ~Path();

    Path combine(Path another);
    void append(Path another);

    bool isValid();
    void reverse();
    Path reversed();
    Path copy();

    Point takeFirst();
    Point takeLast();
    Point getStart();
    Point getEnd();

    void addPoint(Point pt);
    int getPointIndex(Point pt);
    void updatePointAt(int index, Point new_point);
    void updatePoint(Point point, Point new_point);

    unsigned int size();

    std::string toString();

    double length();
    std::vector<Point> getPoints() const { return points; }

    bool operator==(const Path &other) const;
    bool operator!=(const Path &other) const;

private:
    std::vector<Point> points;
};

}

#endif
