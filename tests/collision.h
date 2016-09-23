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

#ifndef TEST_COLLISION_H
#define TEST_COLLISION_H

#include <cppunit/extensions/HelperMacros.h>

#include "Texture.h"
#include "Ball.h"
#include "Paddle.h"

class collision : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(collision);

    CPPUNIT_TEST(testBallPaddle);
    CPPUNIT_TEST(testBallPaddleOverlap);
    CPPUNIT_TEST(testBallPaddleTouch);

    CPPUNIT_TEST_SUITE_END();

public:
    collision();
    virtual ~collision();
    void setUp();
    void tearDown();

private:
    void testBallPaddleOverlap();
    void testBallPaddle();
		void testBallPaddleTouch();
};

#endif /* COLLISION_H */

