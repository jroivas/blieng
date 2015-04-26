/*
 * Copyright 2014 Blistud:io
 */

#ifndef __BLIENG_POINT_H
#define __BLIENG_POINT_H

#include <string>
#include <iostream>

namespace blieng
{

/**
 * Represent a point in x,y cordinates.
 */
class Point
{
public:
    /**
     * Initialize a invalid or empty point.
     *
     * \param is_valid True if generated point is valid, false if not
     */
    explicit Point(bool is_valid = false) : x(0), y(0), valid(is_valid) { }
    /**
     * Initialize point with X and Y values.
     * Generated point will be valid.
     *
     * \param point_x Point X position
     * \param point_y Point Y position
     */
    Point(double point_x, double point_y) :
        x(point_x), y(point_y), valid(true) { }
    virtual ~Point();

    /**
     * Set a new X position for point.
     *
     * \param point_x New X position for point
     */
    void setX(double point_x) { x = point_x; }
    /**
     * Set a new Y position for point.
     *
     * \param point_y New Y position for point
     */
    void setY(double point_y) { y = point_y; }

    /**
     * Update this point coordinates from other point.
     * Will read coordinates from another point
     * and assign very same coordinates for this one,
     * effectively making copy of the other point.
     *
     * \param another Point to used as source
     */
    void update(Point another);

    /**
     * Present point coordinates as a string.
     *
     * \returns Point coordinates as a string
     */
    std::string toString() const;

    /**
     * Checks if two points are equal.
     * Comparison is done on coordinate level.
     * Points with same coordinates are same.
     * See also \ref update
     *
     * \param other Point to compare to
     * \returns True if points are equal, false otherwise
     */
    bool operator==(const Point &other) const;
    /**
     * Checks if two points are not equal.
     *
     * \param other Point to compare to
     * \returns True if points are not equal, false otherwise
     */
    bool operator!=(const Point &other) const;

    /**
     * Checks if point is valid.
     *
     * \returns True if point is valid, false othewise
     */
    bool isValid() const { return valid; }

    /**
     * Will traverse from this point to target point in specified time.
     * Waypoints are given point by point.
     * The length from this point to target point is divided by time,
     * and progress can be get by giving a current time since travelling started.
     * For example from (0,0) to (100,0) with "time" 40 and "now" 10 will
     * result (25,0) because (100*10/40, 0*10/40).
     *
     * \param target The target point where to travel from this one
     * \param now How much time have elapsed since travelling started
     * \param time The total time which will take to reach target point
     * \returns Point representing linear cordinates at given time of travel
     */
    Point traverse(Point target, double now, double time) const;

    /**
     * Will give position between point and target
     *
     * \param target The target point where to travel from this one
     * \param pos Position in percentage between these two (0..1)
     * \returns Point representing linear cordinates at given position
     */
    Point semiPoint(Point target, double pos) const;
    /**
     * Will calculate length from point to other.
     * Will form line from A to B and caluculate the length.
     * This should not be used as a factual length,
     * but rather a comparable length.
     *
     * \param another Point to use as other end.
     */
    double length(Point another);

    double x;  //!< Point X coordinate
    double y;  //!< Point Y coordinate
    bool valid;  //!< Tells if point is valid
};

}  // namespace blieng

#endif  // __BLIENG_POINT_H
