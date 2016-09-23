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

#include <string>

#ifndef UI_ITOSTRING_H
#define UI_ITOSTRING_H

/**
 * @brief Interface for all classes that support a general toString method
 */
class ItoString {
 public:
	virtual ~ItoString() {};

	/**
	 * Get a general string representation of the object.
	 * @return A string representation
	 */
	virtual std::string* toString() = 0;

};


#endif  /* UI_ITOSTRING_H */
