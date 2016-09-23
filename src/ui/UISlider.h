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
#include "UIButton.h"

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#ifndef UISLIDER_H
#define UISLIDER_H

#define UISLIDER_WIDGET_PADDING 10

/**
* @brief A basic slider UI Widget.
*/
class UISlider : public UIWidget, public IUIEventListener {
 public:
	/**
	* @brief Create a new slider widget
	*
	* @param textPrev The texture to use for the less/prev button
	* @param textNext The texture to use for the more/next button
	* @param textSlider The texture for the actual slider
	* @param minValue The minimum allowed value
	* @param maxValue The maximum allowed value
	* @param step The step increment to use when incrementing/decrementing the selected value.
        * @param focusAlpha The alpha to apply when in focus.
        * @param nonFocusAlpha The alpha to apply when not in focus.
	*/
	UISlider(Texture* textPrev, Texture* textNext, Texture* textSlider, int minValue, int maxValue, int step
                , Uint8 focusAlpha = OCCILLO_DEFAULT_MENUFOCUSALPHA, Uint8 nonFocusAlpha = OCCILLO_DEFAULT_MENUNONFOCUSALPHA);
	~UISlider();
	void event(Event e, void* data);
	void render(SDL_Renderer* renderer);
	int getWidth();
	int getHeight();
	void setX(int x);
	void setY(int y);
	/**
	* @brief Set the value of the slider.
	*
	* @param value The value to set. Must be between min and max values.
	*/
	void setValue(int value);
	/**
	* @brief Get the current set value
	*
	* @return The current set value.
	*/
	int getValue();
	void event(IUIWidget* widget);
        
        bool isComposite();
        std::vector<IUIWidget*>* getChildren();
 private:
	/**
	* @brief The current value
	*/
	int value;
	/**
	* @brief The minimum value to be set.
	*/
	const int min;
	/**
	* @brief The maximum value allowed to be set.
	*/
	const int max;
	/**
	* @brief The increment to use when the next / prev buttons are selected.
	*/
	const int step;
        /**
	* @brief The underlying buttons
	*/
        std::vector<IUIWidget*> buttons;
};

#endif /* UICHECKBOX_H */
