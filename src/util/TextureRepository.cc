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

#include "TextureRepository.h"

#define TR_BOUNDS_CHECK(x) if (index < 0 || index > size - 1) { return x; }

TextureRepository::TextureRepository(int count) {
	size = count;
	textures = new Texture*[size];
	for (int i = 0; i < size; i++) {
		textures[i] = nullptr;
	}
}

TextureRepository::TextureRepository(const TextureRepository& orig) {
	size = orig.size;
	textures = new Texture*[size];
	for (int i = 0; i < size; i++) {
		textures[i] = orig.textures[i];
	}
}

TextureRepository::~TextureRepository() {
	if (textures != nullptr) {
		for (int i = 0; i < size; i++) {
			Texture* t = textures[i];
			if (t != nullptr) {
				t->destroy();
				delete t;
				textures[i] = nullptr;
			}
		}
	}
	delete[] textures;
}

bool TextureRepository::add(int index, Texture* texture) {
	TR_BOUNDS_CHECK(false)
	if (textures[index] != nullptr) {
		Texture * t = textures[index];
		t->destroy();
		delete t;
	}
	textures[index] = texture;
	return true;
}

bool TextureRepository::add(int index, SDL_Renderer* renderer, const std::string& path) {
	return add(index, renderer, path, -1, -1);
}

bool TextureRepository::add(int index, SDL_Renderer* renderer, const std::string& path, int width, int height) {
	Texture* text = new Texture();
	text->loadFromFile(renderer, path, width, height);
	return add(index, text);
}

Texture* TextureRepository::remove(int index) {
	TR_BOUNDS_CHECK(nullptr)
	Texture * t = textures[index];
	textures[index] = nullptr;
	return t;
}

Texture* TextureRepository::get(int index) {
	TR_BOUNDS_CHECK(nullptr)
	return textures[index];
}

