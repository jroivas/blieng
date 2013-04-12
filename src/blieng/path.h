#ifndef __PATH_H
#define __PATH_H

#include <string>
#include <vector>
#include <boost/format.hpp>

namespace blieng
{

class Point
{
public:
	Point(double px, double py) : x(px), y(py) { }

	void setX(double px) { x = px; }
	void setY(double py) { y = py; }

	std::string toString() {
		return (boost::format("%f,%f") % x % y).str();
	}

	double x;
	double y;
};

class Path
{
public:
	Path() {}

	void addPoint(Point *pt) { points.push_back(pt); }
	std::vector<Point *> points;

	std::string toString();
};

}

#endif
