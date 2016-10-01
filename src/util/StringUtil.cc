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

#include "StringUtil.h"

std::vector<std::string> StringUtil::split(const std::string& value, const char delimiter) {
	std::vector<std::string> results;
	size_t start = 0;
	size_t end = 0;

	while ( end != std::string::npos) {
		end = value.find( delimiter, start);

		// If at end, use length=maxLength.  Else use length=end-start.
		results.push_back( value.substr( start, (end == std::string::npos) ? std::string::npos : end - start));

		// If at end, use start=maxSize.  Else use start=end+delimiter.
		start = (( end > (std::string::npos - 1) ) ?  std::string::npos  :  end + 1);
	}
	return results;
}

bool StringUtil::hasExtension(const std::string& str, const std::string &suffix) {
	return str.size() >= suffix.size() &&
	       str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

