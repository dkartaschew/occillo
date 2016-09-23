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

#include "collision.h"

CPPUNIT_TEST_SUITE_REGISTRATION(collision);

collision::collision() {
}

collision::~collision() {
}

void collision::setUp() {
}

void collision::tearDown() {
}

void collision::testBallPaddle() {
  Texture* btext = new Texture();
	btext->setEmptyTexture(16, 16);
	Ball* ball = new Ball(btext, 16, 0, 32);
	ball->setDirection(90);

	Texture* ptext = new Texture();
	ptext->setEmptyTexture(48, 16);
	Paddle* paddle = new Paddle(ptext, 0, 32, 0, 0, 200, 0);

  double time = ball->collision(paddle, 1); // 1 sec.

	CPPUNIT_ASSERT_EQUAL(0.5, time);
	ball->move(time);
	ball->collision(paddle);

	printf("ball = %d x %d = %f\n", ball->getX(), ball->getY(), time);
	CPPUNIT_ASSERT_EQUAL(16, ball->getY());
	CPPUNIT_ASSERT_EQUAL(16, ball->getX());

  time = ball->collision(paddle, 0.5); // 1 sec.
  CPPUNIT_ASSERT_EQUAL(1.0, time); // no collision.
	ball->move(0.5);

	printf("ball = %d x %d = %f\n", ball->getX(), ball->getY(), time);
	CPPUNIT_ASSERT_EQUAL(0, ball->getY());
	CPPUNIT_ASSERT_EQUAL(15, ball->getX());

}

void collision::testBallPaddleOverlap() {
	Texture* btext = new Texture();
	btext->setEmptyTexture(16, 16);
	Ball* ball = new Ball(btext, 16, 0, 32);
	ball->setDirection(90);

	Texture* ptext = new Texture();
	ptext->setEmptyTexture(48, 16);
	Paddle* paddle = new Paddle(ptext, 0, 12, 0, 0, 200, 0);
  
	printf("ball = %d x %d : %d x %d\n", ball->getX(), ball->getY(), ball->getWidth(), ball->getHeight());
	printf("paddle = %d x %d : %d x %d\n", paddle->getX(), paddle->getY(), paddle->getWidth(), paddle->getHeight());

  double time = ball->collision(paddle, 1); // 1 sec.

  CPPUNIT_ASSERT(ball->overlaps(paddle));
	CPPUNIT_ASSERT_EQUAL(1.0, time);
	ball->collision(paddle);

  CPPUNIT_ASSERT_EQUAL(-4, ball->getY());
  ball->move(1);
	printf("ball = %d x %d = %f\n", ball->getX(), ball->getY(), time);

}

void collision::testBallPaddleTouch() {
	Texture* btext = new Texture();
	btext->setEmptyTexture(16, 16);
	Ball* ball = new Ball(btext, 16, 0, 32);
	ball->setDirection(90);

	Texture* ptext = new Texture();
	ptext->setEmptyTexture(48, 16);
	Paddle* paddle = new Paddle(ptext, 0, 16, 0, 0, 200, 0);

  double time = ball->collision(paddle, 1); // 1 sec.

	CPPUNIT_ASSERT_EQUAL(0.0, time);
	ball->move(time);
	ball->collision(paddle);

	printf("ball = %d x %d = %f\n", ball->getX(), ball->getY(), time);
  ball->move(1.0);
	printf("ball = %d x %d = %f\n", ball->getX(), ball->getY(), time);

  CPPUNIT_ASSERT_EQUAL(-32, ball->getY());
	CPPUNIT_ASSERT_EQUAL(15, ball->getX());

}

