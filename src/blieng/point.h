#ifndef _POINT_H
#define _POINT_H

#include <string>

namespace blieng
{

class Point
{
public:
	Point(double px, double py) : x(px), y(py) { }

	void setX(double px) { x = px; }
	void setY(double py) { y = py; }

	std::string toString();
	bool operator==(const Point &other) const;
	bool operator!=(const Point &other) const {
		return !(*this == other);
	}

	Point *traverse(Point *target, double now, double time);
	double length(Point *another);

	double x;
	double y;
};

}

#endif
