/*
 *    Copyright (C) 2014 Darran Kartaschew
 *
 *    This file is part of Occillo.
 *
 *    Occillo is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Occillo is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Occillo.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef UTIL_STRINGUTIL_H
#define UTIL_STRINGUTIL_H

#include <string>
#include <vector>

namespace StringUtil {

	/**
	* @brief Split a string into a vector of strings, based on the delimiter
	*
	* @param value The string to split
	* @param delimiter The delimiter to use
	*
	* @return A vector of strings.
	*/
	std::vector<std::string> split(const std::string& value, const char delimiter);

	/**
	* @brief Determine if the str has the given suffix
	*
	* @param str The string to test
	* @param suffix The required suffix
	*
	* @return TRUE if 'str' has the given suffix.
	*/
	bool hasExtension(const std::string& str, const std::string &suffix);

}

#endif
