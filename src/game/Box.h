/*
   Copyright (C) 2014 Darran Kartaschew

   This file is part of Occillo.

    Occillo is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Occillo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Occillo.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef BOX_H
#define BOX_H

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX 1
#endif
#endif

#include <cmath>
#include <limits>
#include <algorithm>

/**
 * Representation of an axis aligned box region.
 */
class Box {
public:

    /**
     * @brief Create a new box struct
     *
     * @param x The upper left x co-ordinate.
     * @param y The upper left y co-ordinate.
     * @param w The width of the box
     * @param h The height of the box
     * @param vx The current speed along the x axis.
     * @param vy The current speed along the y axis.
     */
    Box(double x, double y, double w, double h, double vx = 0.0, double vy = 0.0);

    /**
     * Copy the box
     * @param orig
     */
    Box(const Box& orig);

    virtual ~Box();

    /**
     * @brief Checks if the two box bounds overlap/collide
     *
     * @param b1 The first box
     * @param b2 The second box
     *
     * @return TRUE if the boxes are colliding.
     */
    static bool AABBCheck(Box b1, Box b2);

    /**
     * @brief Determine a box that covers the current bounds plus the destination after move.
     *
     * @param b The source box
     *
     * @return The resultant bounds.
     */
    static Box getBroadphaseBox(Box b);

    /**
     * @brief Perform a collision check on the two bound boxes
     *
     * @param b1 Box 1
     * @param b2 Box 2
     *
     * @return Time delta from 0.0 to 1.0 for collision. (1.0 is no collision).
     */
    static double sweptAABB(Box b1, Box b2);

    /**
     * @brief The x co-ordinate.
     */
    double x;
    /**
     * @brief The y co-ordinate.
     */
    double y;
    /**
     * @brief The width
     */
    double w;
    /**
     * @brief The height of the box.
     */
    double h;
    /**
     * @brief The velocity along the x axis.
     */
    double vx;
    /**
     * @brief The velocity along the y axis.
     */
    double vy;

};

#endif /* BOX_H */

