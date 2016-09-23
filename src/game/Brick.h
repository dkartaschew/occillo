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

#ifndef BRICK_H
#define BRICK_H

#include "Texture.h"
#include "main.h"

#define OCCILLO_BRICK_FLAG_WALL 0x01

#define OCCILLO_BRICK_FLAG_TWO_HIT 0x02
#define OCCILLO_BRICK_FLAG_THREE_HIT 0x03
#define OCCILLO_BRICK_FLAG_FOUR_HIT 0x04
#define OCCILLO_BRICK_FLAG_FIVE_HIT 0x05

#define OCCILLO_BRICK_FLAG_DOUBLE_SCORE 0x10
#define OCCILLO_BRICK_FLAG_TRIPLE_SCORE 0x11
#define OCCILLO_BRICK_FLAG_QUAD_SCORE 0x12
#define OCCILLO_BRICK_FLAG_TEN_SCORE 0x13
#define OCCILLO_BRICK_FLAG_HUNDRED_SCORE 0x14

#define OCCILLO_BRICK_FLAG_EXTRA_LIFE 0x80
#define OCCILLO_BRICK_FLAG_BOMB_QUAD 0x81
#define OCCILLO_BRICK_FLAG_BOMB_ALL 0x82

/**
 * @brief The basic brick
 */
class Brick {
public:
	/**
	 * @brief Create a new brick
	 *
	 * @param texture The texture of the brick.
	 * @param x The x co-ordinate of the brick.
	 * @param y The y co-ordinate of the brick.
	 * @param flags The application flags for the brick.
	 * @param baseValue The base value of all bricks.
	 */
	Brick(Texture* texture, int x, int y, uint8_t flags, int baseValue);
	virtual ~Brick();
	/**
	 * @brief Get the x co-ordinate of the brick.
	 *
	 * @return The x co-ordinate of the brick.
	 */
	int getX();
	/**
	 * @brief Get the y co-ordinate of the brick.
	 *
	 * @return The y co-ordinate of the brick.
	 */
	int getY();
	/**
	 * @brief Get the width of the brick
	 *
	 * @return The width of the brick.
	 */
	int getWidth();
	/**
	 * @brief Get the height of the brick.
	 *
	 * @return The height of the brick.
	 */
	int getHeight();
	/**
	 * @brief Get the current hit count of the brick.
	 *
	 * @return The current hit count.
	 */
	int getHitCount();
	/**
	 * @brief Determine if the brick is a wall element (non destructbale).
	 *
	 * @return TRUE if this is a wall/non destructable element.
	 */
	bool isWall();
	/**
	 * @brief Render the given brick
	 *
	 * @param renderer The renderer to use.
	 */
	void render(SDL_Renderer* renderer);
	/**
	 * @brief Get the score value of the brick.
	 *
	 * @return  The score for a successful hit on the brick.
	 */
	int getValue();
	/**
	 * @brief Handle a collision event.
	 */
	void collision();
	/**
	 * @brief Is this an extra life brick
	 *
	 * @return TRUE if an extra life should be granted.
	 */
	bool isExtraLife();
	/**
   * Get the texture for the brick
   * @return The texture.
   */
  Texture* getTexture();
  /**
	 * @brief Is this a quad destruction brick
	 *
	 * @return TRUE if is a quad destruction brick
	 */
	bool isQuadDestroy();
	/**
	 * @brief Is this an All Brick destruction brick
	 *
	 * @return TRUE if is an All Brick destruction brick
	 */
	bool isAllBrickDestroy();
      
private:
	/**
	 * @brief The texture of the brick.
	 */
	Texture* texture;
	/**
	 * @brief The current hit count.
	 */
	int hitCount;
	/**
	 * @brief The x co-ordinate of the brick.
	 */
	int x;
	/**
	 * @brief The y co-ordinate of the brick.
	 */
	int y;
	/**
	 * @brief The flags for the bricks.
	 * See flags DEFINES for each meaning.
	 */
	uint8_t flags;
	/**
	 * @brief The height of the brick.
	 */
	int sy;
	/**
	 * @brief The width of the brick.
	 */
	int sx;
	/**
	 * @brief The basic score value of the brick.
	 */
	int value;
};

#endif  /* BRICK_H */

