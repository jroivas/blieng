#ifndef _POINT_H
#define _POINT_H

#include <string>
#include <iostream>

namespace blieng
{

class Point
{
public:
	Point(bool valid) : x(0), y(0), valid(valid) { }
	Point(double px, double py) : x(px), y(py), valid(true) { }
	virtual ~Point();

	void setX(double px) { x = px; }
	void setY(double py) { y = py; }

	std::string toString();
	bool operator==(const Point &other) const;
	bool operator!=(const Point &other) const;

	bool isValid() { return valid; }

	Point traverse(Point target, double now, double time);
	double length(Point another);

	double x;
	double y;
	bool valid;
};

}

#endif
