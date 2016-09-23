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

#ifndef GAME_TITLEANIMATION_H
#define GAME_TITLEANIMATION_H

#include "GameAnimation.h"

/**
 * @brief A basic title animation sequence.
 */
class TextureFadeAnimation : public GameAnimation , public UIImage {
  public:
	/**
	 * Create a new title animation sequence.
	 * @param texture The texture to animate. (Is owned by this object).
	 * @param x The x co-ordinate to display.
	 * @param y The y co-ordinate to display.
	 * @param animationTime The time to animate the fade to transparent.
	     * @param gravity The vertical drop rate.
	 */
	TextureFadeAnimation(Texture *texture, int x, int y, uint32_t animationTime, uint32_t gravity = 0);

	virtual ~TextureFadeAnimation();

	bool isComplete();

	bool reset();

	void render(SDL_Renderer* renderer);

  private:
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
};

#endif  /* TITLEANIMATION_H */

