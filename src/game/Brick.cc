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

#include "Brick.h"

Brick::Brick(Texture* texture, int x, int y, uint8_t flags, int baseValue)
	: texture(texture), x(x), y(y), flags(flags) {
	sx = texture->getWidth();
	sy = texture->getHeight();
	switch (flags) {
	case OCCILLO_BRICK_FLAG_TWO_HIT:
		hitCount = 2;
		value = baseValue;
		break;
	case  OCCILLO_BRICK_FLAG_THREE_HIT:
		hitCount = 3;
		value = baseValue;
		break;
	case OCCILLO_BRICK_FLAG_FOUR_HIT:
		hitCount = 4;
		value = baseValue;
		break;
	case OCCILLO_BRICK_FLAG_FIVE_HIT:
		hitCount = 5;
		value = baseValue;
		break;
	case  OCCILLO_BRICK_FLAG_DOUBLE_SCORE:
		hitCount = 1;
		value = baseValue * 2;
		break;
	case OCCILLO_BRICK_FLAG_TRIPLE_SCORE:
		hitCount = 1;
		value = baseValue * 3;
		break;
	case OCCILLO_BRICK_FLAG_QUAD_SCORE:
		hitCount = 1;
		value = baseValue * 4;
		break;
	case OCCILLO_BRICK_FLAG_TEN_SCORE:
		hitCount = 1;
		value = baseValue * 10;
		break;
	case OCCILLO_BRICK_FLAG_HUNDRED_SCORE:
		hitCount = 1;
		value = baseValue * 100;
		break;
	default:
		hitCount = 1;
		value = baseValue;
	}
	if (isWall()) {
		value = 0;
	}
}

Brick::~Brick() {
	// texture not owned...
}

int Brick::getX() {
	return x;
}

int Brick::getY() {
	return y;
}

int Brick::getWidth() {
	return sx;
}

int Brick::getHeight() {
	return sy;
}

int Brick::getHitCount() {
	return hitCount;
}

bool Brick::isWall() {
	return flags == OCCILLO_BRICK_FLAG_WALL;
}

bool Brick::isExtraLife() {
	return flags == OCCILLO_BRICK_FLAG_EXTRA_LIFE;
}

bool Brick::isQuadDestroy() {
  return flags == OCCILLO_BRICK_FLAG_BOMB_QUAD;
}

bool Brick::isAllBrickDestroy(){
  return flags == OCCILLO_BRICK_FLAG_BOMB_ALL;
}

void Brick::render(SDL_Renderer* renderer) {
	if (hitCount > 0) {
		// Initial state, full render.
		texture->render(renderer, x, y);
	}
}

int Brick::getValue() {
	return value;
}

void Brick::collision() {
	if (isWall()) {
		return;
	}
	if (hitCount > 0) {
		hitCount--;
	}
}

Texture* Brick::getTexture() {
	return texture;
}
