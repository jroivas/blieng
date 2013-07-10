#ifndef __BLIENG_MAPS_H
#define __BLIENG_MAPS_H

#include "bliobject.h"
#include "item.h"
#include "character.h"
#include "town.h"
#include "path.h"
#include <json/reader.h>
#include <vector>

namespace blieng
{

class Maps : public BliObject
{
public:
    Maps(std::string mapname);

    std::string getMapName();

    std::string getMapImageFile() { return map_image_file; }
    std::string getSolvedMapImageFile();
    std::vector<blieng::Town *> getTowns() { return towns; }
    std::vector<blieng::Path> getPaths() { return paths; }
    std::vector<blieng::Path> getRevPaths() { return rev_paths; }

    void setBackgroundImage(std::string filename);

    void addTown(blieng::Town *town);
    void addPath(blieng::Path path);

    blieng::Path updatePath(blieng::Path path, int index, blieng::Point point);
    blieng::Path updatePath(blieng::Path path, blieng::Point point);

    bool saveMap(std::string name);

private:
    void loadMap(std::string name);
    void parseMap();

    std::string map_name;
    std::string map_file;
    Json::Value map_json;

    std::string map_image_file;
    std::string solved_map_image_file;
    std::vector<blieng::Town *> towns;
    std::vector<blieng::Path> paths;
    std::vector<blieng::Path> rev_paths;
};

}
#endif
