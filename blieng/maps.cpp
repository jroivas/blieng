#include "maps.h"
#include "data.h"
#include <boost/foreach.hpp>

using blieng::Maps;

Maps::Maps(std::string mapname)
{
    loadMap(mapname);
}

void Maps::loadMap(std::string name)
{
    map_name = name;
    map_file = Data::getInstance()->findFile(name + ".json");
    if (map_file != "") {
        std::cout << map_file << "\n";
        map_json = Data::getInstance()->readJson(map_file);
        parseMap();
    }
}

std::string Maps::getMapName()
{
    return map_name;
}

void Maps::setBackgroundImage(std::string filename)
{
    map_image_file = filename;
    solved_map_image_file = filename;
}

std::string Maps::getSolvedMapImageFile()
{
    if (map_image_file == "") return "";
    if (solved_map_image_file == "") {
        solved_map_image_file = blieng::Data::getInstance()->findFile(map_image_file);
    }

    return solved_map_image_file;
}

void Maps::addTown(blieng::Town *town)
{
    towns.push_back(town);
}

void Maps::addPath(blieng::Path path)
{
    paths.push_back(path);
    rev_paths.push_back(path.reversed());
}

blieng::Path Maps::updatePath(blieng::Path path, blieng::Point point)
{
    std::vector<blieng::Path>::iterator pi = paths.begin();
    while (pi != paths.end()) {
        if (*pi == path) {
            paths.erase(pi);
            path.addPoint(point);
            paths.push_back(path);
            return path;
        }
        pi++;
    }
}

blieng::Path Maps::updatePath(blieng::Path path, int index, blieng::Point point)
{
    std::vector<blieng::Path>::iterator pi = paths.begin();
    while (pi != paths.end()) {
        if (*pi == path) {
            paths.erase(pi);
            path.updatePointAt(index, point);
            paths.push_back(path);
            return path;
        }
        pi++;
    }
}

void Maps::parseMap()
{
    if (!map_json.isObject()) return;

    /* Go thorough items */
    //TODO Refactor
    BOOST_FOREACH(std::string mi, map_json.getMemberNames()) {
        Json::Value item_val = Data::getInstance()->getJsonValue(map_json, mi);
        std::cout <<  mi << "\n";
        if (mi == "image" and item_val.isString()) {
            map_image_file = item_val.asString();
            std::cout << " = " <<  map_image_file << "\n";
        }
        else if (mi == "towns" and item_val.isArray()) {
            Json::Value::iterator it = item_val.begin();
            while (it != item_val.end()) {
                if ((*it).isObject()) {
                    Town *town = new Town();
                    BOOST_FOREACH(std::string town_item, (*it).getMemberNames()) {
                        Json::Value town_val = Data::getInstance()->getJsonValue(*it, town_item);
                        if (town_item == "name" and town_val.isString()) {
                                town->setName(town_val.asString());
                        }
                        else if (town_item == "size" and town_val.isNumeric()) {
                                town->setSize(town_val.asUInt());
                        }
                        else if (town_item == "posx" and town_val.isNumeric()) {
                                town->setPositionX(town_val.asDouble());
                        }
                        else if (town_item == "posy" and town_val.isNumeric()) {
                                town->setPositionY(town_val.asDouble());
                        }
                        else if (town_item == "start" and town_val.isNumeric()) {
                                if (town_val.asInt()>0) town->setValue("start", true);
                        }
                        else if (town_item == "zombies" and town_val.isNumeric()) {
                                town->setValue("zombies", town_val.asUInt());
                        }
                        else if (town_item == "population-index" and town_val.isNumeric()) {
                                town->setValue("population-index", town_val.asDouble());
                        }
                    }
                    std::cout << town->toString();
                    if (town->getName() != "") {
                        towns.push_back(town);
                    } else {
                        delete town;
                    }
                }
                it++;
            }
        }
        else if (mi == "paths" and item_val.isArray()) {
            Json::Value::iterator it = item_val.begin();
            while (it != item_val.end()) {
                if ((*it).isArray()) {
                    blieng::Path path;
                    bool ok = false;

                    Json::Value::iterator point_it = (*it).begin();
                    while (point_it != (*it).end()) {
                        if ((*point_it).isArray()) {
                            if ((*point_it).size() >= 2) {
                                Json::Value pt1 =(*point_it)[0];
                                Json::Value pt2 =(*point_it)[1];
                                if (pt1.isNumeric() && pt2.isNumeric()) {
                                    blieng::Point pt(pt1.asDouble(), pt2.asDouble());
                                    path.addPoint(pt);
                                    ok = true;
                                }
                            }
                        }
                        point_it++;
                    }
                    if (ok) {
                        /*paths.push_back(path);
                        rev_paths.push_back(path.reversed());
                        */
                        addPath(path);
                    }
                }
                it++;
            }
        }
    }
}
