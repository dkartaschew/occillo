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


#ifndef _MAINMENU_H
#define _MAINMENU_H

#include <SDL_mixer.h>
#include "Menu.h"
#include <iostream>
#include <functional>
#include <string>

/**
* @brief The basic main menu
*/
class MainMenu : public Menu {
  public:
	/**
	* @brief Create a Main Menu instance
	*
	* @param config The application/engine configuration
	* @param game The game configuration
	* @param display The current display instance
	*/
	MainMenu(Configuration* config, GameConfiguration* game, Display* display);
	bool Activate();
	void event(IUIWidget* widget);
  private:

	// Variables
	/**
	* @brief The instance of the background music.
	*/
	Mix_Music* backgroundMusic;
};

#endif /* MAINMENU_H */
