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

#include "UIText.h"

UIText::UIText(TTF_Font* font, SDL_Color* colour, SDL_Renderer* renderer) {
	x = 0;
	y = 0;
	listener = nullptr;
	texture = nullptr;
	center = false;
	this->font = font;
	this->colour = colour;
	text = "";
	updated = true;
	lastTime = SDL_GetTicks() / 1000;
	cursor = new Texture();
	if (!cursor->loadFromText(renderer, _("_"), font, colour)) {
		g_info("%s[%d] : Failed to create cursor texture, setting as missing texture.", __FILE__, __LINE__);
		cursor->loadFromColour(renderer, Texture::getColour(), 16, 16);
	}
	focus = false;
}

UIText::~UIText() {
	TTF_CloseFont(font);

	// main texture will be done in the super::~
	if (cursor != nullptr) {
		delete cursor;
		cursor = nullptr;
	}
}

void UIText::event(Event e, void* data) {
	switch (e.event) {
	case SET_FOCUS:
		focus = true;
		break;
	case LOSE_FOCUS:
		focus = false;
		break;
	case KEYDOWN:
	case KEYUP:
	case TEXT_INPUT:
		if (focus) {
			SDL_Event* ev = (SDL_Event*) data;
			if (ev->type == SDL_KEYDOWN) {
				//Handle backspace
				if (ev->key.keysym.sym == SDLK_BACKSPACE && text.length() > 0) {
					//lop off character
					text.pop_back();
					updated = true;
				} else if ((ev->key.keysym.sym == SDLK_c) && (SDL_GetModState() & KMOD_CTRL)) {
					// handle copy
					SDL_SetClipboardText(text.c_str());
				} else if ((ev->key.keysym.sym == SDLK_v) && (SDL_GetModState() & KMOD_CTRL)) {
					// handle paste
					text = SDL_GetClipboardText();
					updated = true;
				} else if (ev->key.keysym.sym == SDLK_KP_ENTER
				           || ev->key.keysym.sym == SDLK_RETURN
				           || ev->key.keysym.sym == SDLK_RETURN2) {
					if (listener != nullptr) {
						listener->event(this);
					}
					updated = true;
				}
			} else if (ev->type == SDL_TEXTINPUT) {
				//Not copy or pasting
				if (!((ev->text.text[ 0 ] == 'c' || ev->text.text[ 0 ] == 'C')
				        && (ev->text.text[ 0 ] == 'v' || ev->text.text[ 0 ] == 'V')
				        && (SDL_GetModState() & KMOD_CTRL))) {
					//Append character
					text += ev->text.text;
					updated = true;
				}
			}
		}
	default:
		break;
	}
}

void UIText::setCenter(bool center) {
	this->center = center;
}

void UIText::render(SDL_Renderer * renderer) {
	uint32_t time = SDL_GetTicks() / 1000;

	if (updated || texture == nullptr) {
		if (texture != nullptr) {
			delete texture;
		}
		texture = new Texture();
		if (!texture->loadFromText(renderer, text, font, colour)) {
			g_info("%s[%d] : Failed to create Label texture, setting as missing texture.", __FILE__, __LINE__);
			texture->loadFromColour(renderer, Texture::getColour(), 16, 16);
		}
		updated = false;
	}
	int xloc = x;
	if (center) {
		// in center mode, the x value is the display width...
		xloc = (x / 2) - (texture->getWidth() / 2);
	}
	texture->render(renderer, xloc, y);
	if (focus && time != lastTime) {
		// update cursor.
		if (time & 01) {
			// odd time, so draw cursor.
			cursor->render(renderer, xloc + texture->getWidth() + 2, y);
		}
	}
}

int UIText::getX() {
	int xloc = x;
	if (center) {
		// in center mode, the x value is the display width...
		if (texture != nullptr) {
			xloc = (x / 2) - (texture->getWidth() / 2);
		} else {
			xloc = (x / 2);
		}
	}
	return xloc;
}

std::string UIText::getText() {
	return text;
}

void UIText::setText(std::string & text) {
	this->text = text;
	updated = true;
}
