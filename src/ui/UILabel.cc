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

#include "UILabel.h"

UILabel::UILabel(const std::string& text, TTF_Font* font, SDL_Color* colour, SDL_Renderer* renderer) {
	x = 0;
	y = 0;
	listener = nullptr;
	texture = new Texture();
	texture->loadFromText(renderer, text, font, colour);
        focus = false;
}

void UILabel::setText(const std::string& text, TTF_Font* font, SDL_Color* colour, SDL_Renderer* renderer) {
	if (texture != nullptr) {
		delete texture;
	}
	texture = new Texture();
	texture->loadFromText(renderer, text, font, colour);
}

void UILabel::event(Event e, void* data) {
	UNUSED(e);
        UNUSED(data);
	// NOP
}

void UILabel::render(SDL_Renderer* renderer) {
	texture->render(renderer, x, y);
}

bool UILabel::canFocus(){
    return false;
}

