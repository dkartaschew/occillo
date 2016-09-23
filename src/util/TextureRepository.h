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

#ifndef TEXTUREREPOSITORY_H
#define	TEXTUREREPOSITORY_H

#include "Texture.h"

/**
* @brief A texture manager for maintaining a collection of textures.
*/
class TextureRepository {
 public:
	/**
	 * Create a texture repository of the given size.
	 * @param count The number of elements the repository is to support.
	 */
	TextureRepository(int count);
	/**
	 * Create a shallow copy of the Texture Repository. (Textures are shared)
	 * @param orig
	 */
	TextureRepository(const TextureRepository& orig);
	/**
	 * Delete the Texture repository. This is a DEEP destroy, as all Textures contained
	 * are also deleted as well.
	 */
	virtual ~TextureRepository();
	/**
	 * Add a new Texture to the repository.
	 * <p>
	 * If the index given is already in use, the existing texture is freed and replaced.
	 * @param index The index the texture is to reside in,
	 * @param texture The Texture to add.
	 * @return TRUE if the texture was added. FALSE if not. (eg already exists in the repository).
	 */
	bool add(int index, Texture* texture);
	/**
	 * Add a new Texture to the repository.
	 * <p>
	 * If the index given is already in use, the existing texture is freed and replaced.
	 * @param index The index the texture is to reside in,
	 * @param renderer The render to use
	 * @param path The path to the file.
	 * @return TRUE if the texture was added.
	 */
	bool add(int index, SDL_Renderer* renderer, const std::string& path);
	/**
	Add a new Texture to the repository.
	 * <p>
	 * If the index given is already in use, the existing texture is freed and replaced.
	 * @param index The index the texture is to reside in,
	 * @param renderer The render to use
	 * @param path The path to the file.
	 * @param width The width of the texture to scale to
	 * @param height The height the texture should scale to.
	 * @return  TRUE if the texture was added.
	 */
	bool add(int index, SDL_Renderer* renderer, const std::string& path, int width, int height);
	/**
	 * Remove a texture from the given index, and return it.
	 * @param index The index to remove.
	 * @return The texture that was in the position.
	 */
	Texture* remove(int index);
	/**
	 * Get the texture at the given index.
	 * @param index The index to get.
	 * @return A pointer to the texture.
	 */
	Texture* get(int index);
 private:
	/**
	 * Pointer to array of textures.
	 */
	Texture **textures;
	/**
	 * Size
	 */
	int size;

};

#endif	/* TEXTUREREPOSITORY_H */

