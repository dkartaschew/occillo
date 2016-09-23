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

#ifndef UTIL_POINT_H
#define UTIL_POINT_H

/**
 * x,y location point
 */
class Point {
public:
     /**
     * Create a new point at 0,0
     */
    Point();
    /**
     * Create a new point
     * @param x The x co-ordinate 
     * @param y The y co-ordinate 
     */
    Point(const double x, const double y);
    /**
     * Create a new point from an existing point
     * @param orig The point to copy from.
     */
    Point(const Point& orig);
    virtual ~Point();

    /**
     * @brief The x co-ordinate.
     */
    double x;
    /**
     * @brief The y co-ordinate.
     */
    double y;

};

#endif /* UTIL_POINT_H */

