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

#include "UICursor.h"

UICursor::UICursor(Texture* texture, uint32_t width, uint32_t height) :
maxX(width), maxY(height), velocityX(0.0), velocityY(0.0) {
				this->texture = texture;
				x = 0;
				y = 0;
				listener = nullptr;
				focus = false;
}

void UICursor::event(Event e, void* data) {
    UNUSED(data);
    switch (e.event) {
        case CURSOR_RELEASE:
            // call the function on button UP
            if (listener != nullptr) {
                listener->event(this);
            }
            break;
        case CURSOR_MOVEMENT:
            x = e.x;
            y = e.y;
            break;
        default:
            break;
    }
}

void UICursor::render(SDL_Renderer* renderer) {
    texture->render(renderer, x - 11, y - 2);
}

bool UICursor::canFocus() {
    return false;
}

bool UICursor::move(double elapsed) {
    if (velocityX == 0.0f && velocityY == 0.0f) {
        return false;
    }
    if (velocityX != 0.0f) {
        int nx = x + (velocityX * elapsed);
        //If the paddle went too far to the left or right
        if (nx < 0) {
            nx = 0;
        } else if (nx > (int)maxX) {
            nx = maxX;
        }
        x = nx;
    }
    if (velocityY != 0.0f) {
        int ny = y + (velocityY * elapsed);
        //If the paddle went too far to the left or right
        if (ny < 0) {
            ny = 0;
        } else if (ny > (int)maxY) {
            ny = maxY;
        }
        y = ny;
    }
    return true;
}

void UICursor::setXVelocity(double velocity) {
    this->velocityX = velocity;
}

void UICursor::setYVelocity(double velocity) {
    this->velocityY = velocity;
}

Event UICursor::locationEvent() {
    return Event(CURSOR_MOVEMENT, x, y);
}

