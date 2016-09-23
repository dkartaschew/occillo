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

#include "TextureFadeAnimation.h"

TextureFadeAnimation::TextureFadeAnimation(Texture *texture, int x, int y, uint32_t animationTime, uint32_t gravity)
: UIImage(texture) {
    setX(x);
    setY(y);
    this->animationTime = animationTime;
    this->gravity = gravity;
    this->startTime = SDL_GetTicks();
}

TextureFadeAnimation::~TextureFadeAnimation() {
}

bool TextureFadeAnimation::isComplete() {
    return ((SDL_GetTicks() - startTime) > animationTime);
}

bool TextureFadeAnimation::reset() {
    this->startTime = SDL_GetTicks();
    return true;
}

void TextureFadeAnimation::render(SDL_Renderer* renderer) {
    float timeDelta = (float) (SDL_GetTicks() - startTime);
    float alpha = 255.0f - (timeDelta / (float) animationTime * 255.0f);
    texture->setAlpha((Uint8) alpha);
    int oldY = getY();
    setY(oldY + (timeDelta / 1000.0f * gravity));
    UIImage::render(renderer);
    setY(oldY);
}