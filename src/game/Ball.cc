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

#ifdef _WIN32
#define NOMINMAX
#endif

#include "Ball.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846 /* pi */
#endif

Ball::Ball(Texture* texture, int x, int y, int pxPerSec) :
	texture(texture), location(x, y) {
	velocity.x = std::cos(240.0 * M_PI / 180.00) * (double)pxPerSec;
	velocity.y = std::sin(240.0 * M_PI / 180.00) * (double)pxPerSec;
}

Ball::~Ball() {
	if (texture != nullptr) {
		delete texture;
	}
}

int Ball::getX() {
	return location.x;
}

int Ball::getY() {
	return location.y;
}

int Ball::getWidth() {
	return texture->getWidth();
}

int Ball::getHeight() {
	return texture->getHeight();
}

void Ball::render(SDL_Renderer* renderer) {
	texture->render(renderer, location.x, location.y);
}

void Ball::move(double timeElapsed) {
	location.x += (velocity.x * timeElapsed);
	location.y += (velocity.y * timeElapsed);
}

void Ball::setXY(int x, int y) {
	this->location.x = x;
	this->location.y = y;
}

void Ball::setDirection(double direction) {
	double pxPerSec = getSpeed();
	velocity.x = std::cos((direction * M_PI) / 180.00) * pxPerSec;
	velocity.y = std::sin((direction * M_PI) / 180.00) * pxPerSec;
}

double Ball::getDirection() {
	return std::atan2(velocity.y, velocity.x);
}

void Ball::setSpeed(double speed) {
	/*    double direction = getDirection();
	    velocity.x = std::cos(direction) * speed;
	    velocity.y = std::sin(direction) * speed; */
	double oldspeed = getSpeed();
#if DEBUG
	g_info("%s[%d] : Ball speed  %f -> %f ", __FILE__, __LINE__, oldspeed, speed);
#endif
	double factor = speed / oldspeed;
	velocity.x = velocity.x * factor;
	velocity.y = velocity.y * factor;
}

double Ball::getSpeed() {
	return std::sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y));
}

double Ball::collision(Brick* brick, double timeElapsed) {
	if (brick == nullptr) {
		return 1.0;
	}
	if (brick->getHitCount() <= 0) {
		return 1.0;
	}
	// Convert the ball/brick into boxes.
	Box ball = Box(location.x, location.y, texture->getWidth(), texture->getHeight(), velocity.x * timeElapsed, velocity.y * timeElapsed);
	Box ballSweep = Box::getBroadphaseBox(ball);
	Box brickBound = Box(brick->getX(), brick->getY(), brick->getWidth(), brick->getHeight(), 0.0, 0.0);

	if (Box::AABBCheck(ballSweep, brickBound)) {
		// BroardPhase collides.
		return Box::sweptAABB(ball, brickBound);
	}
	return 1.0;
}

double Ball::collision(Paddle* paddle, double timeElapsed) {
	// Convert the ball/brick into boxes.
	Box ball = Box(location.x, location.y, texture->getWidth(), texture->getHeight(), velocity.x * timeElapsed, velocity.y * timeElapsed);
	Box ballSweep = Box::getBroadphaseBox(ball);
	Box paddleBound = Box(paddle->getX(), paddle->getY(), paddle->getWidth(), paddle->getHeight(), 0.0, 0.0);

	if (Box::AABBCheck(ballSweep, paddleBound)) {
		// BroardPhase collides.
		return Box::sweptAABB(ball, paddleBound);
	}
	return 1.0;
}

bool Ball::overlaps(Paddle* paddle) {
	// Convert both to boxes.
	Box ball = Box(location.x, location.y, texture->getWidth(), texture->getHeight(), 0.0, 0.0);
	Box paddleBound = Box(paddle->getX(), paddle->getY(), paddle->getWidth(), paddle->getHeight(), 0.0, 0.0);
	return Box::AABBCheck(ball, paddleBound);
}

void Ball::collision(Brick* brick) {
	Box brck = Box(brick->getX(), brick->getY(), brick->getWidth(), brick->getHeight(), 0.0, 0.0);
	double centerX = location.x + (texture->getWidth() / 2);
	double centerY = location.y + (texture->getWidth() / 2);
	// update direction...

	if ((centerX < brck.x) && (velocity.x > 0.00)) {
		velocity.x = -velocity.x;
	} else if (centerX > (brck.x + brck.w) && (velocity.x < 0.00)) {
		velocity.x = -velocity.x;
	}
	if ((centerY > (brck.y + brck.h)) && (velocity.y < 0.00)) {
		velocity.y = -velocity.y;
	} else if ((centerY < brck.y) && (velocity.y > 0.00)) {
		velocity.y = -velocity.y;
	}
	/*  bool collisionHandled = false;
	    if((centerX >= brck.x) && (centerX <= (brck.x + brck.w))){
	        velocity.y = -velocity.y;
	        collisionHandled = true;
	    }
	    if((centerY >= brck.y) && (centerY <= (brck.y + brck.h))){
	        velocity.x = -velocity.x;
	        collisionHandled = true;
	    }
	    if(!collisionHandled){
	        // corner hit.
	        // Determine the x distance from the closest corner.
	    double x = 0;
	        double y = 0;
	        if(centerX < brck.x){
	            x = brck.x - centerX;
	        } else {
	      x = centerX - (brck.x + brck.w);
	        }
	        // Determine the y distance from the closest corner.
	        if(centerY < brck.y){
	            y = brck.y - centerY;
	        } else {
	            y = centerY - (brck.y + brck.h);
	        }
	        // Update the direction.
	    double c = -2 * (velocity.x * x + velocity.y * y) / (x * x + y * y);
	        g_info("%s[%d] : Ball center x y %f %f ", __FILE__, __LINE__, centerX, centerY);
	        g_info("%s[%d] : Brick x y w h %f %f %f %f", __FILE__, __LINE__, brck.x, brck.y , brck.w, brck.h);
	        g_info("%s[%d] : x y c %f %f %f ", __FILE__, __LINE__, x, y , c);
	    velocity.x = velocity.x + c * x;
	    velocity.y = velocity.y + c * y;
	    }
	*/
#if DEBUG
	g_info("%s[%d] : Ball location  %f %f ", __FILE__, __LINE__, location.x, location.y);
	g_info("%s[%d] : Ball direction %f %f ", __FILE__, __LINE__, velocity.x, velocity.y);
#endif
}

void Ball::collision(Paddle* paddle) {
	double paddleMin = paddle->getX();
	double paddleMax = paddle->getWidth();
	double centerX = location.x + (texture->getWidth() / 2);
	double centerY = location.y + (texture->getWidth() / 2);

	// If the center of the ball is below the top of the paddle, then deflect left/right...
	if (centerY > paddle->getY()) {
#if DEBUG
		g_info("%s[%d] : Paddle Collision %f > %d", __FILE__, __LINE__, centerY, paddle->getY());
#endif
		velocity.x = -velocity.x;
		// Determine if we are inside the paddle (can happen with mouse movement),
		// and if so, move our position outside of the paddle.

		// check which to move to.
		double paddleCenter = paddleMin + (paddleMax / 2);
		if (centerX < paddleCenter) {
			// ball is on the left side.
			location.x = paddleMin - texture->getWidth() - 1;
		} else {
			// ball is on the right side.
			location.x = paddleMin + paddleMax + 1;
		}
#if DEBUG
		g_info("%s[%d] : Paddle Overlap relocate %f %f", __FILE__, __LINE__, location.x, location.y);
#endif
	} else {
#if DEBUG
		g_info("%s[%d] : Paddle Overlap/Collision rebound", __FILE__, __LINE__);
#endif
		// above the paddle, change the Y direction...
		velocity.y = -velocity.y;

		// If we have some overlap, then move the ball up...
		if ((location.y + texture->getHeight()) >= paddle->getY()) {
#if DEBUG
			g_info("%s[%d] : Paddle Overlap/Collision Adjustment", __FILE__, __LINE__);
#endif
			location.y = paddle->getY() - texture->getHeight() - 1;
		}
		// we treat the paddle as convex, so adjust the direction -50 +50
		// depending on where within the paddle X we hit.
		centerX -= paddleMin;
		double paddleCenter = paddleMax / 2.0;
		double direction = ((getDirection() / M_PI) * 180.0);
		if (direction < 0.0) {
			direction += 360.0;
		}
		if (centerX < paddleCenter) {
			double ratio = (1.00 - (centerX / paddleCenter)) * (50.0);
			direction -= ratio;
			if (direction < 190.0) {
				direction = 190.0;
			}
		} else {
			double ratio = (1.00 - ((paddleMax - centerX) / paddleCenter)) * (50.0);
			direction += ratio;
			if (direction > 350.0 ) {
				direction = 350.0;
			}
		}
		setDirection(direction);
	}
#if DEBUG
	g_info("%s[%d] : Ball direction %f %f ", __FILE__, __LINE__, velocity.x, velocity.y);
#endif
}

