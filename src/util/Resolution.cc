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

#include "Resolution.h"

Resolution::Resolution(int width, int height) : width(width), height(height) {
	resolution = new std::string(std::to_string(width) + " × " + std::to_string(height));
}

Resolution::Resolution() {
	width = 0;
	height = 0;
	resolution = new std::string(std::to_string(width) + " × " + std::to_string(height));
}

Resolution::Resolution(const Resolution& orig) {
	width = orig.width;
	height = orig.height;
	resolution = new std::string(*(orig.resolution));
}

Resolution::~Resolution() {
	if (resolution != nullptr) {
		delete resolution;
		resolution = nullptr;
	}
}

std::string* Resolution::toString() {
	return resolution;
}

int Resolution::getWidth() const {
	return width;
}

int Resolution::getHeight() const {
	return height;
}

