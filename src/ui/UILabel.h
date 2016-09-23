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

#ifndef UILABEL_H
#define UILABEL_H

/**
 * @brief A simple Label UI Widget. The widget displays a simple line of text.
 */
class UILabel : public UIWidget {
public:
    /**
     * Create a new UI Label.
     * @param text The text to display
     * @param font The font to use.
     * @param colour The colour to render the text.
     * @param renderer The SDL renderer to use for rendering the text.
     */
    UILabel(const std::string& text, TTF_Font* font, SDL_Color* colour, SDL_Renderer* renderer);

    /**
     * Set new text.
     */
    void setText(const std::string& text, TTF_Font* font, SDL_Color* colour, SDL_Renderer* renderer);
    void event(Event e, void* data);
    void render(SDL_Renderer* renderer);
    bool canFocus();
};

#endif /* _UILABEL_H */
