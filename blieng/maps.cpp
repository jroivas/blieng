/*
 * Copyright 2014 Blistud:io
 */

#include "blieng/maps.h"

#include <boost/foreach.hpp>
#include <boost/filesystem/path.hpp>

#include <cstdio>
#include <sstream>
#include <string>

#include "blieng/data.h"

using blieng::Maps;

Maps::Maps(
    boost::shared_ptr<blieng::BliengState> _state,
    const std::string &mapname) :
    m_state(_state)
{
    if (!loadMap(mapname)) {
        std::cout << "ERROR: Can't find map: " << mapname << "\n";
    }
}

bool Maps::loadMap(const std::string &name)
{
    if (m_state->m_data == nullptr) {
        // Not initialized properly
        // TODO throw error?
        return false;
    }
    map_name = name;
    map_file = m_state->m_data->findFile(name + ".json");
    if (map_file != "") {
        std::cout << map_file << "\n";
        map_json = m_state->m_data->readJson(map_file);
        return parseMap();
    }
    return false;
}

bool Maps::saveMap(const std::string &name)
{
    std::string json = "";

    boost::filesystem::path my_image = boost::filesystem::path(
        solved_map_image_file);
    std::string imagefile = my_image.filename().string();

    // FIXME This needs a rewrite
    json += "{\n";
    json += "    \"image\": \"" + imagefile + "\",\n";

    std::string str_towns = "";
    BOOST_FOREACH(blieng::Town *town, towns) {
        if (str_towns != "") str_towns += ",\n";
        std::ostringstream tmp;
        tmp << "         ";
        tmp << "{ \"name\": \"" << town->getName();
        tmp << "\", \"size\": " << town->getSize();
        tmp << ", \"posx\": " << town->getPositionX();
        tmp << ", \"posy\": " << town->getPositionY();

        if (town->isValue("population-index")) {
            tmp << ", \"population-index\": "
                << town->getDoubleValue("population-index");
        }
        if (town->isValue("zombies")) {
            tmp << ", \"zombies\": " << town->getUIntValue("zombies");
        }
        if (town->isValue("start") && town->getBoolValue("start")) {
            tmp << ", \"start\": 1";
        }
        tmp << " }";
        str_towns += tmp.str();
    }
    str_towns += "\n";

    std::string str_paths = "";
    BOOST_FOREACH(blieng::Path path, paths) {
        if (str_paths != "") str_paths += ",\n";
        std::ostringstream tmp;
        tmp << "         ";
        tmp << "[";
        std::string str_points;
        BOOST_FOREACH(blieng::Point pt, path.getPoints()) {
            if (str_points.size() > 0) str_points += ", ";
            std::ostringstream tmp_points;
            tmp_points << "[" << pt.x << ", " << pt.y << "]";
            str_points += tmp_points.str();
        }
        tmp << str_points;
        tmp << "]";
        str_paths += tmp.str();
    }
    str_paths += "\n";

    json += "    \"towns\": [\n";
    json += str_towns;
    json += "    ],\n";

    json += "    \"paths\": [\n";
    json += str_paths;
    json += "    ]\n";
    json += "}\n";

    printf("%s\n", json.c_str());

    return m_state->m_data->saveMapJSON(name, json);
}

std::string Maps::getMapName() const
{
    return map_name;
}

void Maps::setBackgroundImage(const std::string &filename)
{
    map_image_file = filename;
    solved_map_image_file = filename;
}

std::string Maps::getSolvedMapImageFile()
{
    if (map_image_file == "") return "";
    if (solved_map_image_file == "") {
        solved_map_image_file = m_state->m_data->findFile(map_image_file);
    }

    return solved_map_image_file;
}

void Maps::addTown(blieng::Town *town)
{
    towns.push_back(town);
}

bool Maps::removeTown(blieng::Town *town)
{
    auto ti = towns.begin();
    while (ti != towns.end()) {
        if (*ti == town) {
            towns.erase(ti);
            return true;
        }
        ++ti;
    }
    return false;
}

void Maps::addPath(blieng::Path path)
{
    paths.push_back(path);
    rev_paths.push_back(path.reversed());
}

blieng::Path Maps::updatePath(blieng::Path path, blieng::Point point)
{
    auto pi = paths.begin();
    while (pi != paths.end()) {
        if (*pi == path) {
            paths.erase(pi);
            path.addPoint(point);
            paths.push_back(path);
            return path;
        }
        ++pi;
    }

    return path;
}

blieng::Path Maps::updatePath(blieng::Path path, int index, blieng::Point point)
{
    auto pi = paths.begin();
    while (pi != paths.end()) {
        if (*pi == path) {
            paths.erase(pi);
            path.updatePointAt(index, point);
            paths.push_back(path);
            return path;
        }
        ++pi;
    }

    return path;
}

bool Maps::parseMap()
{
    if (!map_json->isObject()) return false;

    /* Go thorough items */
    // TODO Refactor
    BOOST_FOREACH(std::string mi, map_json->getMemberNames()) {
        const json_value *item_val = m_state->m_data->getJsonValue(map_json, mi);
        if (mi == "image" && item_val->isString()) {
            map_image_file = item_val->asString();
            std::cout << " = " <<  map_image_file << "\n";
        }
        else if (mi == "towns" && item_val->isArray()) {
            auto it = item_val->u.array.begin();
            while (it != item_val->u.array.end()) {
                if ((*it)->isObject()) {
                    Town *town = new Town();
                    BOOST_FOREACH(std::string town_item, (*it)->getMemberNames()) {
                        const json_value *town_val = m_state->m_data->getJsonValue(*it, town_item);
                        if (town_item == "name" && town_val->isString()) {
                                town->setName(town_val->asString());
                        }
                        else if (town_item == "size" && town_val->isNumeric()) {
                                town->setSize(town_val->asUInt());
                        }
                        else if (town_item == "posx" && town_val->isNumeric()) {
                                town->setPositionX(town_val->asDouble());
                        }
                        else if (town_item == "posy" && town_val->isNumeric()) {
                                town->setPositionY(town_val->asDouble());
                        }
                        else if (town_item == "start" && town_val->isNumeric()) {
                                if (town_val->asInt()>0) town->setValue("start", true);
                        }
                        else if (town_item == "zombies" && town_val->isNumeric()) {
                                town->setValue("zombies", town_val->asUInt());
                        }
                        else if (town_item == "population-index" && town_val->isNumeric()) {
                                town->setValue("population-index", town_val->asDouble());
                        }
                    }
                    std::cout << town->toString();
                    if (town->getName() != "") {
                        towns.push_back(town);
                    } else {
                        delete town;
                    }
                }
                ++it;
            }
        }
        else if (mi == "paths" and item_val->isArray()) {
            auto it = item_val->u.array.begin();
            while (it != item_val->u.array.end()) {
                if ((*it)->isArray()) {
                    blieng::Path path;
                    bool ok = false;

                    auto point_it = (*it)->u.array.begin();
                    while (point_it != (*it)->u.array.end()) {
                        if ((*point_it)->isArray()) {
                            if ((*point_it)->u.array.length >= 2) {
                                json_value *pt1 =(*point_it)->u.array.values[0];
                                json_value *pt2 =(*point_it)->u.array.values[1];
                                if (pt1->isNumeric() && pt2->isNumeric()) {
                                    blieng::Point pt(pt1->asDouble(), pt2->asDouble());
                                    path.addPoint(pt);
                                    ok = true;
                                }
                            }
                        }
                        ++point_it;
                    }
                    if (ok) {
                        addPath(path);
                    }
                }
                ++it;
            }
        }
    }
    return true;
}
