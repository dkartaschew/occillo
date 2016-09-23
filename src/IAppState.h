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

#ifndef IAPPSTATE_H
#define IAPPSTATE_H

/**
* @brief Basic application state.
*/
class IAppState {
 public:
	virtual ~IAppState() {};

	/**
	 * Run the main loop of the application state.
	 * return TRUE if the state is complete and should be deactivated.
	 */
	virtual bool Run() = 0;
	/**
	 * Activate the application state.
	 * return TRUE if the application state started correctly.
	 */
	virtual bool Activate() = 0;
	/**
	 * Deactivate the application state, eg inform it to clean up.
	 */
	virtual void Deactivate() = 0;

};
#endif  /* IAPPSTATE_H */
