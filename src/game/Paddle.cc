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

#include "Paddle.h"

Paddle::Paddle(Texture* texture, int x, int y, int pxPerSec, int minX, int maxX, int deadZone)
: texture(texture), x((double) x), y((double) y), pxPerSec(pxPerSec), minX(minX), maxX(maxX), deadzone(deadZone) {
    sx = texture->getWidth();
    sy = texture->getHeight();
    velocityX = 0;
}

Paddle::~Paddle() {
    if (texture != nullptr) {
        delete texture;
    }
}

int Paddle::getX() {
    return (int) x;
}

int Paddle::getY() {
    return (int) y;
}

int Paddle::getWidth() {
    return sx;
}

int Paddle::getHeight() {
    return sy;
}

void Paddle::render(SDL_Renderer* renderer) {
    texture->render(renderer, (int) x, (int) y);
}

void Paddle::event(SDL_Event& e) {
    //If a key was pressed
    if ((e.type == SDL_KEYDOWN) && (e.key.repeat == 0)) {
        //Adjust the velocity
        switch (e.key.keysym.sym) {
            case SDLK_LEFT:
                velocityX -= pxPerSec;
                break;
            case SDLK_RIGHT:
                velocityX += pxPerSec;
                break;
        }
    }        //If a key was released
    else if ((e.type == SDL_KEYUP) && (e.key.repeat == 0)) {
        //Adjust the velocity
        switch (e.key.keysym.sym) {
            case SDLK_LEFT:
                velocityX += pxPerSec;
                break;
            case SDLK_RIGHT:
                velocityX -= pxPerSec;
                break;
        }
    } else if (e.type == SDL_MOUSEMOTION) {
        int mouseX = 0;
        int mouseY = 0;
        //Get mouse position
        SDL_GetMouseState(&mouseX, &mouseY);
        x = mouseX;
        if (x < minX) {
            x = minX;
        } else if (x > maxX - sx) {
            x = maxX - sx;
        }
    } else if (e.type == SDL_CONTROLLERAXISMOTION){
        //X axis motion 
        if( e.caxis.axis == 0 ) { //Left of dead zone 
            if( e.caxis.value < -deadzone  || e.caxis.value > deadzone) { 
                float delta = (float)e.caxis.value / 32768.0f; 
                velocityX = ((float)pxPerSec * delta);
            } else {
                velocityX = (0.0f);
            }
        }
    }
}

void Paddle::move(float timeElapsed) {
    //Move the paddle left or right
    x += velocityX * timeElapsed;
    //If the paddle went too far to the left or right
    if (x < minX) {
        x = minX;
    } else if (x > maxX - sx) {
        x = maxX - sx;
    }
}

