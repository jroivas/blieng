/*
 * Copyright 2014 Blistud:io
 */

#ifndef __BLIENG_MAPS_H
#define __BLIENG_MAPS_H

#include <string>
#include <vector>

#include "blieng/blieng.h"
#include "blieng/bliobject.h"
#include "blieng/item.h"
#include "blieng/character.h"
#include "blieng/town.h"
#include "blieng/path.h"
#include "blieng/json.h"

namespace blieng
{

/**
 * Handler for map data.
 * Map includes different features, including towns, paths, etc.
 */
class Maps : public BliObject
{
public:
    /**
     * Initialized the map with data backend and name of the map.
     * Automatically loads the map if possible.
     *
     * \param state Shared state instance
     * \param mapname Name of the map to load
     */
    Maps(
        boost::shared_ptr<blieng::BliengState> state,
        const std::string &mapname);

    /**
     * Get the name of current map
     *
     * \returns Map name
     */
    std::string getMapName() const;

    /**
     * Get background image file name.
     *
     * \returns Background image file name
     */
    std::string getMapImageFile() const {
        return map_image_file;
    }
    /**
     * Get background image file location in data backend.
     * File might be located inside a data file.
     * It might be a good idea to make a loader class which handles
     * loading the image for UI parts.
     *
     * \returns Background image file name
     */
    std::string getSolvedMapImageFile();
    /**
     * Get list of of town.
     * All loaded towns should be listed here.
     *
     * \returns Vector of Town objects.
     */
    std::vector<blieng::Town *> getTowns() { return towns; }
    /**
     * Get list of path and roads in the map.
     * Parses path locations and give waypoints.
     * Path is actually a collection of (x,y) points
     *
     * \returns Vector of path data.
     */
    std::vector<blieng::Path> getPaths() const { return paths; }
    /**
     * Get paths, with reverse order waypoints.
     * Like \ref getPaths but paths waypoints are in reverse order.
     * If original path is from point A to B and finally C, reverse path
     * will be from C to B and finally A
     *
     * \returns Vector of path data.
     */
    std::vector<blieng::Path> getRevPaths() const { return rev_paths; }

    /**
     * Change the background image file.
     *
     * \param filename New filename to be set as background image.
     */
    void setBackgroundImage(const std::string &filename);

    /**
     * Add a new town to the map.
     *
     * \param town Town object to be added on the map
     */
    void addTown(blieng::Town *town);
    /**
     * Removes as town from the map.
     *
     * \param town Town object to be removed from map
     * \returns True on success, false otherwise
     */
    bool removeTown(blieng::Town *town);
    /**
     * Add a path to the map.
     *
     * \param path Path to be added on the map
     */
    void addPath(blieng::Path path);

    /**
     * Update path, change a waypoint to another.
     *
     * \param path The path to modify
     * \param index Index of the waypoint to modify
     * \param point The new point to be set on index
     */
    blieng::Path updatePath(blieng::Path path, int index, blieng::Point point);
    /**
     * Update path, append a new waypoint.
     *
     * \param path The path to modify
     * \param point The new point to be appended to the path
     */
    blieng::Path updatePath(blieng::Path path, blieng::Point point);

    /**
     * Will save the current map to a file
     *
     * \param name File name of the target map
     */
    bool saveMap(const std::string &name);

private:
    /**
     * Load map data from a file
     *
     * \param name File name of the target map
     */
    bool loadMap(const std::string &name);
    bool parseMap();

    std::string map_name;
    std::string map_file;
    json_value *map_json;

    boost::shared_ptr<blieng::BliengState> m_state;

    std::string map_image_file;
    std::string solved_map_image_file;
    std::vector<blieng::Town *> towns;
    std::vector<blieng::Path> paths;
    std::vector<blieng::Path> rev_paths;
};

}  // namespace blieng

#endif  // __BLIENG_MAPS_H
