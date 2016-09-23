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


#ifndef CREDITSMENU_H
#define CREDITSMENU_H

#include "config.h"

// main.h includes the gettext, sdl, etc items
#include "Menu.h"
#include "MainMenu.h"

/**
* @brief The Credits Menu
*/
class CreditsMenu : public Menu {
  public:
	/**
	* @brief Create the Credits Menu
	*
	* @param config The application/engine configuration
	* @param game The game configuration
	* @param display The display instance
	* @param mainMenu A reference to the main menu.
	*/
	CreditsMenu(Configuration* config, GameConfiguration* game, Display* display, MainMenu * mainMenu);
	bool Activate();
	void event(IUIWidget* widget);
	void destroy();
  private:
	/**
	* @brief Create a new label UI widget
	*
	* @param text The text to display
	* @param font The font to render the text with
	* @param y The line to display the text on
	*/
	void newLabel(const std::string& text, TTF_Font *font, int y);
	/**
	* @brief A reference to the main menu
	*/
	MainMenu * mainMenu;
};

#endif /* CREDITSMENU_H */
