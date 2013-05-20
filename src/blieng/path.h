#ifndef __PATH_H
#define __PATH_H

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

	Path *combine(Path *another);
	void append(Path *another);

	void reverse();
	Path *reversed();
	Path *copy();

	Point *takeFirst();
	Point *getStart();
	Point *getEnd();

	void addPoint(Point *pt);

	std::string toString();

	double length();
	std::vector<Point *> getPoints() { return points; }

private:
	std::vector<Point *> points;
};

}

#endif
