#ifndef __BLIENG_PATH_H
#define __BLIENG_PATH_H

#include <string>
#include <vector>

#include "point.h"

namespace blieng
{

/**
 * Path consists one or more waypoints.
 */
class Path
{
public:
    /**
     * Initialize a empty path.
     */
    Path();
    virtual ~Path();

    /**
     * Combine this path to anohter and return the resulted path.
     *
     * \param another The path to be appended
     * \return Combined path
     */
    Path combine(Path another) const;
    /**
     * Append another path to this path.
     * Modifies this Path.
     *
     * \param another The path to be appended
     */
    void append(Path another);

    /**
     * Check if the path is valid.
     * Validity means that the path has at least one waypoint.
     *
     * \returns True if path is valid, false otherwise
     */
    bool isValid();
    /**
     * Reverse this path.
     * Will reverse the order of waypoints.
     * After this the first waypoint will be the last.
     */
    void reverse();
    /**
     * Get this path reversed.
     * Reverses this path, but difference to \ref reverse
     * is that this is not done in place but
     * rather on a new copy.
     *
     * \returns Reversed path
     */
    Path reversed() const;
    /**
     * Copy of this path.
     * Form a new object and return it.
     *
     * \returns A new copy of this path.
     */
    Path copy() const;

    /**
     * Remove first waypoint and return it.
     *
     * \returns The very first waypoint.
     */
    Point takeFirst();
    /**
     * Remove last waypoint and return it.
     *
     * \returns The last waypoint.
     */
    Point takeLast();
    /**
     * Copy the first waypoint and return it.
     *
     * \returns The very first waypoint.
     */
    Point getStart();
    /**
     * Copy the last waypoint and return it.
     *
     * \returns The last waypoint.
     */
    Point getEnd();

    /**
     * Add a new waypoint to the path.
     *
     * \param pt A new point to append.
     */
    void addPoint(Point pt);
    /**
     * Get index of a point.
     * Goes thorough of all waypoints and
     * returns the index of waypoint matching
     * to the given one.
     *
     * \returns index of the point or negative if not found.
     */
    int getPointIndex(Point pt);
    /**
     * Replace a waypoint with another.
     *
     * \param index Index of the waypoint to replace.
     * \prama new_point New point replacing the old one.
     */
    void updatePointAt(int index, Point new_point);
    /**
     * Replace a waypoint with another.
     * Goes thorough the waypoints and if matching found,
     * will replace it with the new one.
     *
     * \param point The point to be replaced.
     * \prama new_point New point replacing the old one.
     */
    void updatePoint(Point point, Point new_point);

    /**
     * Get the number of waypoints.
     *
     * \returns the number of waypoints.
     */
    unsigned int size();

    /**
     * Shows the path as a string.
     *
     * \returns String representation of the path.
     */
    std::string toString();

    /**
     * Get the length of the path.
     * Length is combined length from start to end.
     * Will calculate length in points.
     *
     * \returns The length of the path.
     */
    double length();
    /**
     * Get all the waypoints.
     *
     * \returns Vector of waypoints.
     */
    std::vector<Point> getPoints() const { return points; }

    /**
     * Check if other path is same as this one.
     * Will compare all the waypoints if they match.
     *
     * \returns True if paths are equal.
     */
    bool operator==(const Path &other) const;
    /**
     * Check if other path are not same.
     * Will compare all the waypoints if they match.
     *
     * \returns True if paths are not equal.
     */
    bool operator!=(const Path &other) const;

private:
    std::vector<Point> points;
};

}

#endif
