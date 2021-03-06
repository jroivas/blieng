/*
 * Copyright 2014-2017 Jouni Roivas
 */

#include "blieng/maps.h"

#include <boost/foreach.hpp>
#include <boost/filesystem/path.hpp>

#include <cstdio>
#include <sstream>
#include <string>

#include "blieng/data.h"

using blieng::Maps;
using blieng::BliengJson;

Maps::Maps(
    boost::shared_ptr<blieng::BliengState> _state,
    const std::string &mapname)
    : blieng::BliObject(),
    m_state(_state)
{
    if (!loadMap(mapname)) {
        std::cerr << "ERROR: Can't find map: " << mapname << "\n";
    }
}

bool Maps::loadMap(const std::string &name)
{
    if (m_state->m_data == nullptr) {
        // Not initialized properly
        // TODO throw error?
        return false;
    }
    m_map_name = name;
    m_map_file = m_state->m_data->findFile(name + ".json");
    if (m_map_file != "") {
        //std::cout << m_map_file << "\n";
        m_map_json = m_state->m_data->readJson(m_map_file);
        return parseMap();
    }
    return false;
}

bool Maps::saveMap(const std::string &name)
{
    std::string json = "";

    boost::filesystem::path my_image = boost::filesystem::path(
        m_solved_map_image_file);
    std::string imagefile = my_image.filename().string();

    // FIXME This needs a rewrite
    json += "{\n";
    json += "    \"image\": \"" + imagefile + "\",\n";

    std::string str_towns = "";
    BOOST_FOREACH(blieng::Town *town, m_towns) {
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
        if (town->isValue("start") && town->getBoolValue("start")) {
            tmp << ", \"start\": 1";
        }
        tmp << " }";
        str_towns += tmp.str();
    }
    str_towns += "\n";

    std::string str_paths = "";
    BOOST_FOREACH(blieng::Path path, m_paths) {
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
    return m_map_name;
}

void Maps::setBackgroundImage(const std::string &filename)
{
    m_map_image_file = filename;
    m_solved_map_image_file = filename;
}

std::string Maps::getSolvedMapImageFile()
{
    if (m_map_image_file == "") return "";
    if (m_solved_map_image_file == "") {
        m_solved_map_image_file = m_state->m_data->findFile(m_map_image_file);
    }

    return m_solved_map_image_file;
}

void Maps::addTown(blieng::Town *town)
{
    m_towns.push_back(town);
}

bool Maps::removeTown(blieng::Town *town)
{
    auto ti = m_towns.begin();
    while (ti != m_towns.end()) {
        if (*ti == town) {
            m_towns.erase(ti);
            return true;
        }
        ++ti;
    }
    return false;
}

void Maps::addPath(blieng::Path path)
{
    m_paths.push_back(path);
    m_rev_paths.push_back(path.reversed());
}

blieng::Path Maps::updatePath(blieng::Path path, blieng::Point point)
{
    auto pi = m_paths.begin();
    while (pi != m_paths.end()) {
        if (*pi == path) {
            m_paths.erase(pi);
            path.addPoint(point);
            m_paths.push_back(path);
            return path;
        }
        ++pi;
    }

    return path;
}

blieng::Path Maps::updatePath(blieng::Path path, int index, blieng::Point point)
{
    auto pi = m_paths.begin();
    while (pi != m_paths.end()) {
        if (*pi == path) {
            m_paths.erase(pi);
            path.updatePointAt(index, point);
            m_paths.push_back(path);
            return path;
        }
        ++pi;
    }

    return path;
}

bool Maps::parseMap()
{
    if (!m_map_json.is_object()) return false;

    /* Go thorough items */
    // TODO Refactor
    BOOST_FOREACH(std::string mi, blieng::JsonKeys(m_map_json)) {
        const BliengJson item_val = m_map_json[mi];
        if (mi == "image" && item_val.is_string()) {
            m_map_image_file = item_val;
            //std::cout << " = " << m_map_image_file << "\n";
        }
        else if (mi == "towns" && item_val.is_array()) {
            auto it = item_val.cbegin();
            while (it != item_val.cend()) {
                if (it->is_object()) {
                    Town *town = new Town();
                    BOOST_FOREACH(std::string town_item, blieng::JsonKeys(*it)) {
                        const BliengJson town_val = (*it)[town_item];
                        if (town_item == "name" && town_val.is_string()) {
                            town->setName(town_val);
                        }
                        else if (town_item == "size" && town_val.is_number()) {
                            town->setSize(town_val);
                        }
                        else if (town_item == "posx" && town_val.is_number()) {
                            town->setPositionX(town_val);
                        }
                        else if (town_item == "posy" && town_val.is_number()) {
                            town->setPositionY(town_val);
                        }
                        else if (town_item == "start" && town_val.is_number()) {
                            int v = town_val;
                            if (v > 0) town->setValue("start", true);
                        }
                        else if (town_item == "population-index" && town_val.is_number()) {
                            double v = town_val;
                            town->setValue("population-index", v);
                        }
                        else if (town_item == "population" && town_val.is_number()) {
                            unsigned int v = town_val;
                            town->setValue("population", v);
                        }
                    }
                    if (town->getName() != "") {
                        m_towns.push_back(town);
                    } else {
                        delete town;
                    }
                }
                ++it;
            }
        }
        else if (mi == "paths" and item_val.is_array()) {
            for (BliengJson item : item_val) {
                if (!item.is_array()) continue;

                blieng::Path path;
                bool ok = false;
                for (BliengJson coordinate : item) {
                    if (!coordinate.is_array()) continue;
                    if (coordinate.size() < 2) continue;
                    if (!coordinate[0].is_number() || !coordinate[0].is_number()) continue;
                    double pt1 = coordinate[0];
                    double pt2 = coordinate[1];
                    blieng::Point pt(pt1, pt2);
                    path.addPoint(pt);
                    ok = true;
                }
                if (ok) addPath(path);
            }
            /*
            auto it = item_val.cbegin();
            while (it != item_val.cend()) {
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
        */
        }
    }
    return true;
}
