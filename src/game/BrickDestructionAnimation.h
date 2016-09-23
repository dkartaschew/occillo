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

#ifndef GAME_BRICKDESTRUCTIONANIMATION_H
#define GAME_BRICKDESTRUCTIONANIMATION_H

#include <vector>
#include "GameAnimation.h"
#include "Point.h"

/**
 * Primary animation that is played when a break is destroyed. This animation is entirely procedural and reuses the
 * bricks texture for the fragment components.
 */
class BrickDestructionAnimation : public GameAnimation {
public:
    /**
     * Create a new title animation sequence.
     * @param texture The texture to animate. (Is owned by this object).
     * @param x The x co-ordinate to display.
     * @param y The y co-ordinate to display.
     * @param animationTime The time to animate the fade to transparent.
     * @param gravity The vertical drop rate.
     * @param small TRUE for small explosion animation.
     */
    BrickDestructionAnimation(Texture *texture, int x, int y, uint32_t animationTime, uint32_t gravity = 0, bool small = false);

    virtual ~BrickDestructionAnimation();

    bool isComplete();

    bool reset();

    void render(SDL_Renderer* renderer);

private:
    /**
     * @brief The base texture to display
     */
    Texture* texture;
    /**
     * @brief The x co-ordinate of the original brick.
     */
    int x;
    /**
     * @brief The y co-ordinate of the original brick.
     */
    int y;
    /**
     * @brief The start time of the animation
     */
    uint32_t startTime;
    /**
     * The time to complete the animate sequence
     */
    uint32_t animationTime;
    /**
     * Gravity constant
     */
    uint32_t gravity;
    /**
     * TRUE for small explosion.
     */
    bool small;
    /**
     * Determine render location for brick
     * @param timeDelta Time seince start (ms)
     * @param x The start x co-ordinate
     * @param y The start y co-ordinate
     * @param gravityX The amount of x mount per sec.
     * @param velocityY The initial y velocity
     * @param gravityY The amount of gravity in the y axis
     * @return 
     */
    Point getPoint(float timeDelta, int x, int y, double gravityX, double velocityY, double gravityY);
    /**
     * Array of factors for calculation of shrapnel points
     */
    double deltas[3*16];
};

#endif /* GAME_BRICKDESTRUCTIONANIMATION_H */

