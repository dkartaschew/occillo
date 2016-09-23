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

#include "UIWidget.h"

#ifndef UICURSOR_H
#define UICURSOR_H

/**
 * @brief A mouse cursor widget.
 */
class UICursor : public UIWidget {
public:
    /**
     * Create a new UI Cursor.
     * @param texture The texture to display
     * @param width The display width.
     * @param height The display height
     */
    UICursor(Texture* texture, uint32_t width, uint32_t height);

    void event(Event e, void* data);
    void render(SDL_Renderer* renderer);
    bool canFocus();

    /**
     * @brief Move the cursor based on the elapsed time since this call last time.
     *
     * @param elapsed The time elapsed.
     * @return TRUE if the cursor moved.
     */
    bool move(double elapsed);
    /**
     * Set the X velocity
     * @param velocity
     */
    void setXVelocity(double velocity);
    /**
     * Set the Y velocity
     * @param velocity
     */
    void setYVelocity(double velocity);
    /**
     * Get a location event from the cursor
     * @return An event for the location of the cursor.
     */
    Event locationEvent();
private:
    /**
     * The maximum X value
     */
    const uint32_t maxX;
    /**
     * The maximum Y value
     */
    const uint32_t maxY;
    /**
     * Velocity of the cursor on the X axis
     */
    double velocityX;
    /**
     * Velocity of the cursor on the Y axis
     */
    double velocityY;
};

#endif /* UICURSOR_H */
