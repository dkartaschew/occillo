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

#include "colours.h"

CPPUNIT_TEST_SUITE_REGISTRATION(colours);

colours::colours() {
}

colours::~colours() {
}

void colours::setUp() {
}

void colours::tearDown() {
}

void colours::testTextureColourGeneration() {
	for (int i = 0; i < (5*5*5); i++) {
		SDL_Color* col = Texture::getColour();
		printf("Colour = %03d %03d %03d %03d\n", col->r, col->g, col->b, col->a);
	}
}

