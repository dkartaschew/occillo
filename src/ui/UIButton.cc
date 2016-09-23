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

#include "UIButton.h"

UIButton::UIButton(Texture* texture, int x, int y, Uint8 focusAlpha, Uint8 nonFocusAlpha) {
    this->texture = texture;
    this->x = x;
    this->y = y;
    listener = nullptr;
    mouseDown = false;
    focus = false;
    this->focusAlpha = focusAlpha;
    this->nonFocusAlpha = nonFocusAlpha;
}

void UIButton::event(Event e, void* data) {
    UNUSED(data);
    switch (e.event) {
        case SET_FOCUS:
            focus = true;
            break;
        case LOSE_FOCUS:
            focus = false;
            break;
        case CURSOR_ACTIVATE:
            mouseDown = true;
            break;
        case CURSOR_RELEASE:
            if (mouseDown == true) {
                // call the function on button UP
                if (listener != nullptr) {
                    listener->event(this);
                }
            }
            mouseDown = false;
            break;
        default:
            break;
    }
}

void UIButton::render(SDL_Renderer* renderer) {
    int offset = mouseDown ? 3 : 0;
    if (!focus) {
        texture->setAlpha(nonFocusAlpha);
    }
    texture->render(renderer, x + offset, y + offset);
    texture->setAlpha(focusAlpha);
}

