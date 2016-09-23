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

#ifndef GAME_PADDLE_H
#define GAME_PADDLE_H

#include "Texture.h"

/**
* @brief The players paddle.
*/
class Paddle {
  public:
	/**
	* @brief Create a new paddle
	*
	* @param texture The texture to use for the paddle
	* @param x The initial x co-ordinate
	* @param y The y co-cordinate for the paddle
	* @param pxPerSec The number of pixels per second to move during keyboard control.
	* @param minX The minimal x co-ordinate value.
	* @param maxX The maximum x co-ordinate value.
        * @param deadZone The deadzone to use for the controller axis input.
	*/
	Paddle(Texture* texture, int x, int y, int pxPerSec, int minX, int maxX, int deadZone);
	virtual ~Paddle();
	/**
	* @brief Get the current x co-ordinate of the paddle. (The left most position).
	*
	* @return The left most position of the paddle.
	*/
	int getX();
	/**
	* @brief Get the current y co-ordinate of the paddle.
	*
	* @return The upper y co-ordinate of the paddle.
	*/
	int getY();
	/**
	* @brief Get the current width of the paddle in pixels.
	*
	* @return The current width.
	*/
	int getWidth();

	/**
	* @brief Get the height of the paddle in pixels.
	*
	* @return The current height.
	*/
	int getHeight();
	/**
	* @brief Render the paddle to the display using the given renderer
	*
	* @param renderer The renderer to use.
	*/
	void render(SDL_Renderer* renderer);
	/**
	* @brief Have the paddle handle the given SDL event
	*
	* @param e The event.
	*/
	void event(SDL_Event& e);
	/**
	* @brief Move the paddle based on the elapsed time since this call last time.
	*
	* @param timeElapsed The time elapsed.
	*/
	void move(float timeElapsed);
  private:
	/**
	* @brief The texture of the paddle.
	*/
	Texture* texture;
	/**
	* @brief The current left most x co-ordinate.
	*/
	double x;
	/**
	* @brief The current y co-ordinate.
	*/
	double y;
	/**
	* @brief The height of the paddle.
	*/
	int sy;
	/**
	* @brief The width of the paddle.
	*/
	int sx;
	/**
	* @brief The number of pixels per sec to traverse.
	*/
	int pxPerSec;
	/**
	* @brief The minimum x co-ordinate that the paddle is allowed.
	*/
	int minX;
	/**
	* @brief The maximum x co-ordinate that the paddle is allowed.
	*/
	int maxX;
	/**
	* @brief The current velocity along the X axis.
	*/
	int velocityX;
        /**
         * @brief Controller deadzone
         */
        const int deadzone;
};

#endif  /* BRICK_H */

