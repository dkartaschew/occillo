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

#ifndef UICHECKBOX_H
#define UICHECKBOX_H

/**
 * @brief A Check Box button UI Widget
 */
class UICheckBox : public UIWidget {
public:
    /**
     * @brief Create a new check box button widget
     *
     * @param active The texture to use for when the button is 'checked'
     * @param inactive The texture to use for when the button is 'unchecked'
     * @param focusAlpha The alpha to apply when in focus.
     * @param nonFocusAlpha The alpha to apply when not in focus.
     */
    UICheckBox(Texture* active, Texture* inactive, Uint8 focusAlpha = OCCILLO_DEFAULT_MENUFOCUSALPHA, Uint8 nonFocusAlpha = OCCILLO_DEFAULT_MENUNONFOCUSALPHA);
    ~UICheckBox();
    void event(Event e, void* data);
    void render(SDL_Renderer* renderer);
    /**
     * @brief Set is the checkbox is to be active (checked) or not.
     *
     * @param active TRUE if the checkbox is active/checked.
     */
    void setActive(bool active);
    /**
     * @brief Get is the checkbox is active or in a checked state
     *
     * @return TRUE if the checkbox is active/checked, FALSE otherwise. 
     */
    bool isActive();
private:
    /**
     * @brief Flag to indicate if the checkbox is active.
     */
    bool active;
    /**
     * @brief Flag to indicate if the mouse is over the widget, and the primary mouse button is pressed.
     */
    bool mouseDown;
    /**
     * @brief Reference to the inactive texture.
     */
    Texture* textureInactive;
    /**
     * The alpha to apply when in focus.
     */
    Uint8 focusAlpha;
    /**
     * The alpha to apply when not in focus.
     */
    Uint8 nonFocusAlpha;
};

#endif /* UICHECKBOX_H */
