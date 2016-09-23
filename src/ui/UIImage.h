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
#include <SDL_image.h>
#include <SDL_ttf.h>

#ifndef UIIMAGE_H
#define UIIMAGE_H

/**
* @brief A simple Image UI Widget
*/
class UIImage : public UIWidget {
 public:
	/**
	 * Create a new UI Image.
	 * @param texture The texture to display
	 */
	UIImage(Texture* texture);
	void event(Event e, void* data);
	void render(SDL_Renderer* renderer);

};

#endif /* UIIMAGE_H */
