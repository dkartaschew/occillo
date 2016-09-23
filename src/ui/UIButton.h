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

#ifndef _UIBUTTON_H
#define _UIBUTTON_H

/**
 * @brief General push button UI widget.
 */
class UIButton : public UIWidget {
public:
    /**
     * Create a new UI Button.
     * @param texture The texture to display
     * @param x The initial x co-ordinate for the button
     * @param y The initial y co-ordinate for the button
     * @param focusAlpha The alpha to apply when in focus.
     * @param nonFocusAlpha The alpha to apply when not in focus.
     */
    UIButton(Texture* texture, int x, int y, Uint8 focusAlpha = OCCILLO_DEFAULT_MENUFOCUSALPHA, Uint8 nonFocusAlpha = OCCILLO_DEFAULT_MENUNONFOCUSALPHA);

    void event(Event e, void* data);
    void render(SDL_Renderer* renderer);

private:
    /**
     * @brief flag to indicate that the mouse is over the button, and the primary button is pressed.
     */
    bool mouseDown;
    /**
     * The alpha to apply when in focus.
     */
    Uint8 focusAlpha;
    /**
     * The alpha to apply when not in focus.
     */
    Uint8 nonFocusAlpha;
};

#endif /* _UIBUTTON_H */
