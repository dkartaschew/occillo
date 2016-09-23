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


#ifndef _OPTIONSMENU_H
#define _OPTIONSMENU_H

#include "MainMenu.h"

/**
* @brief The application options menu.
*/
class OptionsMenu : public Menu {
 public:
	/**
	* @brief Create a new Application/Engine options menu
	*
	* @param config The application/engine configuration
	* @param game The game configuration
	* @param display The current display instance
	* @param mainMenu A rerference to the main menu.
	*/
	OptionsMenu(Configuration* config, GameConfiguration* game, Display* display, MainMenu * mainMenu);
	bool Activate();
	void event(IUIWidget* widget);
	void destroy();
 private:
	/**
	* @brief Create a new label UI widget
	*
	* @param text The text to display
	* @param font The font to use to render the text with
	* @param x The x co-ordinate to display the label
	* @param y The y co-ordinate to display the label
	*/
	void newLabel(const std::string& text, TTF_Font *font, int x, int y);
	/**
	* @brief A reference to the main menu
	*/
	MainMenu* mainMenu;
	/**
	* @brief A reference to the selection widget that is hosting the resolutions
	*/
	UISelection<Resolution>* resolutionControl;
	/**
	* @brief Checkbox UI widget for fullscreen
	*/
	UICheckBox* fullscreenControl;
	/**
	* @brief Slider widget for selecting the audio effects volume.
	*/
	UISlider* volumeEffectControl;
	/**
	* @brief Slider widegt for selecting the music volume.
	*/
	UISlider* volumeMusicControl;
	/**
	* @brief Button widget for the Return action.
	*/
	UIButton* btnReturn;
	/**
	* @brief Button widget for the Apply action
	*/
	UIButton* btnApply;
};

#endif /* OPTIONSMENU_H */
