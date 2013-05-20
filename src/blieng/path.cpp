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

std::string blieng::Path::toString() {
	std::string res = "";
	bool first = true;
	BOOST_FOREACH(blieng::Point *pt, points) {
		if (!first) res += ";";
		first = false;
		res += pt->toString();
	}
	return res;
}

void Path::addPoint(Point *pt)
{
	if (pt != NULL) {
		points.push_back(pt);
	}
}

void blieng::Path::append(blieng::Path *another)
{
	if (another != NULL) {
		BOOST_FOREACH(blieng::Point *pt, another->points) {
			addPoint(pt);
		}
	}
}

blieng::Path *blieng::Path::combine(blieng::Path *another)
{
	blieng::Path *newpath = new blieng::Path();
	BOOST_FOREACH(blieng::Point *pt, points) {
		newpath->addPoint(pt);
	}

	if (another != NULL) {
		BOOST_FOREACH(blieng::Point *pt, another->points) {
			newpath->addPoint(pt);
		}
	}

	return newpath;
}


blieng::Point *blieng::Path::takeFirst()
{
	if (!points.empty()) {
		blieng::Point *res = points.front();
		points.erase(points.begin());
		return res;
	}
	return NULL;
}
blieng::Point *blieng::Path::getStart()
{
	if (!points.empty()) {
		return points.front();
	}
	return NULL;
}

blieng::Point *blieng::Path::getEnd()
{
	if (!points.empty()) {
		return points.back();
	}
	return NULL;
}

void blieng::Path::reverse()
{
	std::reverse(points.begin(), points.end());
}

blieng::Path *blieng::Path::copy()
{
	blieng::Path *newpath = new blieng::Path();
	if (newpath == NULL) return NULL;
	BOOST_FOREACH(blieng::Point *pt, points) {
		newpath->addPoint(pt);
	}

	return newpath;
}

blieng::Path *blieng::Path::reversed()
{
	blieng::Path *pt = this->copy();
	pt->reverse();
	return pt;
}
