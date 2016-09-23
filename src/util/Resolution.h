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

#ifndef UTIL_RESOLUTION_H
#define UTIL_RESOLUTION_H

#include <string>
#include <SDL.h>
#include <SDL_image.h>

#include "ItoString.h"

/**
* @brief A resolution entry used for the configuration UI.
*/
class Resolution : public ItoString {
  public:
	/**
	* @brief Create a new Resolution entry.
	*
	* @param width The width
	* @param height The height
	*/
	Resolution(int width, int height);
	/**
	* @brief Create an empty 0x0 resolution
	*/
	Resolution();
	/**
	* @brief Copy a resolution entry
	*
	* @param orig The entry to copy
	*/
	Resolution(const Resolution& orig);

	virtual ~Resolution();
	/**
	* @brief Get a pointer to a string that represents the resolution
	*
	* @return A Pointer to a string
	*/
	std::string* toString();
	/**
	* @brief get the display width
	*
	* @return The display width
	*/
	int getWidth() const;
	/**
	* @brief Get the display height
	*
	* @return The display height
	*/
	int getHeight() const;

	/**
	* @brief Comparison operator overload, compare width then height
	*
	* @param rhs The rhs
	*
	* @return TRUE is the width is smaller, or if equal, if the heigh is smaller.
	*/
	bool operator<(const Resolution &rhs) const {
		if (width != rhs.width) {
			return width < rhs.width;
		} else {
			return height < rhs.height;
		}
	}

	/**
	* @brief Equality comparison operator overload.
	*
	* @param rhs The rhs
	*
	* @return TRUE if this object is equal to the other
	*/
	bool operator==(const Resolution &rhs) const {
		return width == rhs.width && height == rhs.height;
	}

  private:
	/**
	* @brief A string representation of the resolution.
	*/
	std::string* resolution;
	/**
	* @brief The width of the resolution.
	*/
	int width;
	/**
	* @brief The height of the resolution.
	*/
	int height;
};

#endif  /* UTIL_RESOLUTION_H */

