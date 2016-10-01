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

#ifndef GAME_BALL_H
#define GAME_BALL_H

#include "config.h"

#include <cmath>
#include <limits>
#ifdef _WIN32
#include <algorithm>
#endif
#include "Texture.h"
#include "Brick.h"
#include "Paddle.h"
#include "Box.h"
#include "Point.h"

/**
* @brief Definition of the Ball used within the game.
*/
class Ball {
  public:
	/**
	* @brief Create a new instance of the players ball.
	*
	* @param texture The texture of the ball.
	* @param x The initial x co-ordinate of the ball.
	* @param y The initial y co-ordinate of the ball.
	* @param pxPerSec The number of pixels to move per sec of elasped time when ball is in play
	*/
	Ball(Texture* texture, int x, int y, int pxPerSec);
	virtual ~Ball();
	/**
	* @brief Get the current x co-ordinate of the ball.
	*
	* @return The x co-ordinate.
	*/
	int getX();
	/**
	* @brief Get the current y co-ordinate of the ball.
	*
	* @return The y co-ordinate.
	*/
	int getY();
	/**
	* @brief Get the width of the ball.
	*
	* @return The width of the ball.
	*/
	int getWidth();
	/**
	* @brief Get the height of the ball.
	*
	* @return The height of the ball.
	*/
	int getHeight();
	/**
	* @brief Render the ball to the display using the provided renderer
	*
	* @param renderer The renderer to use.
	*/
	void render(SDL_Renderer* renderer);
	/**
	* @brief Have the ball move along it's path for the given time
	*
	* @param timeElapsed The amount of time since the ball last moved.
	*/
	void move(double timeElapsed);
	/**
	* @brief Handle a collision event with the given brick
	*
	* @param brick The brick the collision occured with.
	*/
	void collision(Brick* brick);
	/**
	* @brief Determine if a collision would occur against the given brick if a move was performed
	* for the given time elapsed value. Not the ball does NOT update it's internal position.
	*
	* @param brick The brick to check collision against.
	* @param timeElapsed The time elapsed.
	*
	* @return A value between 0.0 and 1.0 determining the percentage of time elapsed
	* that would result if a collision occurs. A value of 1.0 means no collision occurred.
	*/
	double collision(Brick* brick, double timeElapsed);
	/**
	* @brief Handle a collision event with the given paddle.
	*
	* @param paddle The paddle the collision occured with.
	*/
	void collision(Paddle* paddle);
	/**
	* @brief Determine if a collision would occur against the given paddle if a move was performed
	* for the given time elapsed value. Not the ball does NOT update it's internal position.
	*
	* @param paddle The paddle to check collision against.
	* @param timeElapsed The time elapsed.
	*
	* @return A value between 0.0 and 1.0 determining the percentage of time elapsed
	* that would result if a collision occurs. A value of 1.0 means no collision occurred.
	*/
	double collision(Paddle* paddle, double timeElapsed);
  /**
	 * @brief Determine if the ball is overlapping the paddle given
	 * @param paddle The paddle to check for overlap.
	 * @return TRUE is the ball and paddle are overlapping each other
	 */
	bool overlaps(Paddle* paddle);
	/**
	* @brief Set the x and y co-ordinate of the ball.
	*
	* @param x The x co-ordinate to set.
	* @param y The y co-ordinate to set.
	*/
	void setXY(int x, int y);
	/**
	* @brief Get the current direction of the ball in radians.
	*
	* @return The direction of the ball.
	*/
	double getDirection();
	/**
	* @brief Set the direction of the ball in degrees.
	*
	* @param direction The direction to set as degrees.
	*/
	void setDirection(double direction);
	/**
	* @brief Get the current speed of the ball as pixels per second.
	*
	* @return The current speed.
	*/
	double getSpeed();
	/**
	* @brief Set the speed of the ball as pixels per second.
	*
	* @param speed The speed to set.
	*/
	void setSpeed(double speed);
  private:
	/**
	* @brief The texture of the ball.
	*/
	Texture* texture;
	/**
	* @brief The current location of the ball.
	*/
	Point location;
	/**
	 * @brief The current velocity of the ball.
	 */
	Point velocity;
};

#endif  /* BRICK_H */

