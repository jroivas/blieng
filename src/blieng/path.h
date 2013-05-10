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
	Path() {}

	Path *combine(Path *another);
	void append(Path *another);

	Point *takeFirst();
	Point *getStart();
	Point *getEnd();

	void addPoint(Point *pt) { points.push_back(pt); }
	std::vector<Point *> points;

	std::string toString();
};

}

#endif
