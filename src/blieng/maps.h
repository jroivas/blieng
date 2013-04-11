#ifndef __MAPS_H
#define __MAPS_H

#include "bliobject.h"
#include "item.h"
#include "character.h"
#include "town.h"
#include <json/reader.h>
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

class Maps : public BliObject
{
public:
	Maps(std::string mapname);

	std::string getMapImageFile() { return map_image_file; }
	std::string getSolvedMapImageFile();
	std::vector<blieng::Town *> getTowns() { return towns; }
	std::vector<blieng::Path *> getPaths() { return paths; }

private:
	void loadMap(std::string name);
	void parseMap();

	std::string map_name;
	std::string map_file;
	Json::Value map_json;

	std::string map_image_file;
	std::string solved_map_image_file;
	std::vector<blieng::Town *> towns;
	std::vector<blieng::Path *> paths;
};

}
#endif
