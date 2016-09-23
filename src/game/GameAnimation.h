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

#ifndef GAME_GAMEANIMATION_H
#define GAME_GAMEANIMATION_H

#include "UIImage.h"

/**
 * @brief Interface for Game Animation elements.
 */
class GameAnimation {
public:

    virtual ~GameAnimation() {
    };
    
    /**
     * @brief Is the animation complete?
     * @return TRUE if the animation sequence is complete
     */
    virtual bool isComplete() = 0;

    /**
     * @brief reset the internal state of the animation sequence
     * @return TRUE if the reset of the animation state completed successfully.
     */
    virtual bool reset() = 0;
    /**
     * Render the UI element.
     * @param renderer The SDL renderer.
     */
    virtual void render(SDL_Renderer* renderer) = 0;
};

#endif /* GAMEANIMATION_H */
