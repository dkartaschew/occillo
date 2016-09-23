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

#include "UIWidget.h"

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

#ifndef UITEXT_H
#define UITEXT_H

/**
* @brief A basic Text input widget.
*/
class UIText : public UIWidget {
 public:
	/**
	* @brief Create a new text input widget
	*
	* @param font The font to use
	* @param colour The colour of the text to display
	* @param renderer The SDL renderer.
	*/
	UIText(TTF_Font* font, SDL_Color* colour, SDL_Renderer* renderer);
	~UIText();
	void event(Event e, void* data);
	void render(SDL_Renderer* renderer);
        int getX();
	/**
	* @brief Set if the widget is to be centered on the screen. (Use setX() to set the width of the display).
	*
	* @param center TRUE if the widget is to be centered on the display.
	*/
	void setCenter(bool center);
	/**
	* @brief Get the current text from the widget
	*
	* @return A string representing the text entered into the widget.
	*/
	std::string getText();
	/**
	* @brief Set the text to be displayed
	* 
	* @param text The text to be set and displayed.
	*/
	void setText(std::string& text);
 private:
	/**
	* @brief TRUE of the widget is to be rendered centered on the display. (x is the display width in this case).
	*/
	bool center;
	/**
	* @brief TRUE is the text has been updated, and requires a new texture to be rendered.
	*/
	bool updated;
	/**
	* @brief The font to use to render the text.
	*/
	TTF_Font* font;
	/**
	* @brief The colour to use when rendering the text.
	*/
	SDL_Color* colour;
	/**
	* @brief The text input
	*/
	std::string text;
	/**
	* @brief The last time the cursor was rendered.
	*/
	uint32_t lastTime;
	/**
	* @brief The texture to use for the text input cursor.
	*/
	Texture* cursor;
};

#endif /* UITEXT_H */
