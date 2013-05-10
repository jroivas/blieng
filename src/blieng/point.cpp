#include "point.h"
#include <boost/format.hpp>
#include <math.h>

using blieng::Point;

std::string Point::toString()
{
	return (boost::format("%f,%f") % x % y).str();
}

bool Point::operator==(const Point &other) const {
	if (x == other.x && y == other.y) return true;
	return false;
}

Point *Point::traverse(Point *target, double now, double time)
{
	//Travel to another point, in specific time, specify now as current time
	if (target == NULL) return NULL;
	if (now>time) return target;

	double nposx, nposy;
	nposx = target->x - x;
	nposy = target->y - y;
	nposx = (nposx * now / time) + x;
	nposy = (nposy * now / time) + y;

	Point *pnt = new Point(nposx, nposy);
	return pnt;
}

double Point::length(Point *another)
{
	if (another == NULL) return 0;

	double dx = x - another->x;
	double dy = y - another->y;
	if (dx<0) dx *= -1;
	if (dy<0) dy *= -1;

	//return sqrt(dx*dx + dy*dy);
	return dx+dy;
}
