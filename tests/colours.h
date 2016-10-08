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

#ifndef TEST_COLOURS_H
#define TEST_COLOURS_H

#include <cppunit/extensions/HelperMacros.h>

#include "Texture.h"

class colours : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(colours);

    CPPUNIT_TEST(testTextureColourGeneration);

    CPPUNIT_TEST_SUITE_END();

public:
    colours();
    virtual ~colours();
    void setUp();
    void tearDown();

private:
    void testTextureColourGeneration();
};

#endif /* COLOURS_H */

