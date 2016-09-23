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

#include "Box.h"

Box::Box(double x, double y, double w, double h, double vx, double vy) :
x(x), y(y), w(w), h(h), vx(vx), vy(vy) {
}

Box::Box(const Box& orig) :
x(orig.x), y(orig.y), w(orig.w), h(orig.h), vx(orig.vx), vy(orig.vy) {
}

Box::~Box() {
}

bool Box::AABBCheck(Box b1, Box b2) {
    return !(b1.x + b1.w < b2.x || b1.x > b2.x + b2.w || b1.y + b1.h < b2.y || b1.y > b2.y + b2.h);
}

Box Box::getBroadphaseBox(Box b) {
    Box broadphasebox(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

    broadphasebox.x = b.vx > 0 ? b.x : b.x + b.vx;
    broadphasebox.y = b.vy > 0 ? b.y : b.y + b.vy;
    broadphasebox.w = b.vx > 0 ? b.vx + b.w : b.w - b.vx;
    broadphasebox.h = b.vy > 0 ? b.vy + b.h : b.h - b.vy;

    return broadphasebox;
}

double Box::sweptAABB(Box b1, Box b2) {
    double xInvEntry, yInvEntry;
    double xInvExit, yInvExit;

    // find the distance between the objects on the near and far sides for both x and y
    if (b1.vx > 0.0) {
        xInvEntry = b2.x - (b1.x + b1.w);
        xInvExit = (b2.x + b2.w) - b1.x;
    } else {
        xInvEntry = (b2.x + b2.w) - b1.x;
        xInvExit = b2.x - (b1.x + b1.w);
    }

    if (b1.vy > 0.0) {
        yInvEntry = b2.y - (b1.y + b1.h);
        yInvExit = (b2.y + b2.h) - b1.y;
    } else {
        yInvEntry = (b2.y + b2.h) - b1.y;
        yInvExit = b2.y - (b1.y + b1.h);
    }

    // find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
    double xEntry, yEntry;
    double xExit, yExit;

    if (b1.vx == 0.0) {
        xEntry = -std::numeric_limits<double>::infinity();
        xExit = std::numeric_limits<double>::infinity();
    } else {
        xEntry = xInvEntry / b1.vx;
        xExit = xInvExit / b1.vx;
    }

    if (b1.vy == 0.0) {
        yEntry = -std::numeric_limits<double>::infinity();
        yExit = std::numeric_limits<double>::infinity();
    } else {
        yEntry = yInvEntry / b1.vy;
        yExit = yInvExit / b1.vy;
    }

    // find the earliest/latest times of collision
    double entryTime = std::max(xEntry, yEntry);
    double exitTime = std::min(xExit, yExit);

    // if there was no collision
    if ((entryTime > exitTime) || ((xEntry < 0.0) && (yEntry < 0.0)) || (xEntry > 1.0) || (yEntry > 1.0)) {
        return 1.0;
    } else {
        // return the time of collision
        return entryTime;
    }
}

