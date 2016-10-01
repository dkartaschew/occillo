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

#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <cstdlib>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cairo.h>
#include <librsvg/rsvg.h>

#include "StringUtil.h"

#define OCCILLO_TEXTURE_RMASK 0x00ff0000
#define OCCILLO_TEXTURE_GMASK 0x0000ff00
#define OCCILLO_TEXTURE_BMASK 0x000000ff
#define OCCILLO_TEXTURE_AMASK 0xff000000
#define OCCILLO_TEXTURE_BPP 32
#define OCCILLO_TEXTURE_BYTEPP 4
#define OCCILLO_OUTLINE_SIZE 3

/**
* @brief A Texture to utilise.
*/
class Texture {
  public:
	/**
	* @brief Create a new Texture entity. The texture will not be ready until loaded from file, or rendered from text.
	*/
	Texture();
	
	~Texture();
	/**
	* @brief Load a given image file and create a texture. PNG and SVG files are currently supported.
	*
	* @param renderer The SDL renderer to use.
	* @param path The path of the file to load.
	*
	* @return TRUE if the file was loaded and rendered correctly.
	*/
	bool loadFromFile(SDL_Renderer* renderer, const std::string& path);
	/**
	* @brief Load a given image file and create a texture scaled to the given width and height. PNG and SVG file are currently supported.i
	*
	* @param renderer The SDL renderer to use.
	* @param path The path of the file to load.
	* @param width The width the texture should be scaled to.
	* @param height The height of the texture should be scaled to.
	*
	* @return TRUE if the file was loaded and rendered correctly.
	*/
	bool loadFromFile(SDL_Renderer* renderer, const std::string& path, int width, int height);
	/**
	* @brief Create a new texture based on a text string.
	*
	* @param renderer The SDL renderer to use.
	* @param text The text to render.
	* @param font The SDL Font to use
	* @param colour The SDL Colour to use
	*
	* @return TRUE if the text rendered correctly to a texture.
	*/
	bool loadFromText(SDL_Renderer* renderer, const std::string& text, TTF_Font *font, SDL_Color* colour);
	/**
	* @brief Create a new texture based on a text string. The string will have an outline added.
	*
	* @param renderer The SDL renderer to use.
	* @param text The text to render.
	* @param font The SDL Font to use
	* @param colour The SDL Colour to use
	* @param outlineColour The SDL Colour to use for the outline.
	*
	* @return TRUE if the text rendered correctly to a texture.
	*/
	bool loadFromText(SDL_Renderer* renderer, const std::string& text, TTF_Font *font, SDL_Color* colour, SDL_Color* outlineColour);
	/**
	* @brief Destroy/clear all internal resources of the texture.
	*/
	void destroy();
	/**
	* @brief Render the given texture
	*
	* @param renderer The SDL renderer.
	* @param x The x co-ordinate of the display to render to.
	* @param y The y co-ordinate of the display to render to.
	*/
	void render(SDL_Renderer* renderer, int x, int y);
	/**
	* @brief Render the given texture
	*
	* @param renderer The SDL renderer.
	* @param x The x co-ordinate of the display to render to.
	* @param y The y co-ordinate of the display to render to.
	* @param x1 The upper x position within the texture to render.
	* @param x2 The width of the sub-texture to render.
	* @param y1 The upper y position within the texture to render.
	* @param y2 The height of the sub-texture to render.
	*/
	void render(SDL_Renderer* renderer, int x, int y, int x1, int x2, int y1, int y2);
	/**
	* @brief Get the width of the texture.
	*
	* @return The width of the texture.
	*/
	int getWidth();
	/**
	* @brief Get the height of the texture
	*
	* @return The height of the texture.
	*/
	int getHeight();
	/**
	* @brief Set the bleading mode for the given texture
	*
	* @param blending The blending mode to use during rendering.
	*/
	void setBlendMode( SDL_BlendMode blending );
	/**
	* @brief Set the alpha to use during the render of the texture
	*
	* @param alpha The alpha to use. (0 = transparent, 255 = opaque.)
	*/
	void setAlpha( Uint8 alpha );
	/**
	* @brief Get the last error message that was given during the requested operation.
	*
	* @return The last error message or NULL if no message is present.
	*/
	const char * getLastError();

	/**
	 * @brief Set an empty texture, suitable for unit tests.
	 *
	 * @param width The texture width
	 * @param height The texture height
	 */
	void setEmptyTexture(int width, int height);

  private:
	/**
	* @brief The texture as a SDL Texture.
	*/
	std::vector<SDL_Texture*> texture;
	/**
	* @brief The width of the texture
	*/
	int width;
	/**
	* @brief The height of the texture
	*/
	int height;
	/**
	* @brief A pointer to the last error message.
	*/
	const char * lastError;
	/**
	* @brief The start time of the animation
	*/
	uint32_t startTime;
	/**
	* @brief Load the given SVG file
	*
	* @param renderer The SDL Renderer to use
	* @param path The path to the SVG file
	* @param width The width to render the image as
	* @param height The height to render the image as
	*
	* @return TRUE if the image loaded rendered correctly.
	*/
	bool loadSVG(SDL_Renderer* renderer, const std::string& path, int width, int height);
	/**
	* @brief Load the given PNG file
	*
	* @param renderer The SDL Renderer to use
	* @param path The path to the PNG file
	* @param width The width to render the image as
	* @param height The height to render the image as
	*
	* @return TRUE if the image loaded rendered correctly.
	*/
	bool loadPNG(SDL_Renderer* renderer, const std::string& path, int width, int height);
	/**
	* @brief Cleanup all the given resources.
	*
	* @param rsvg_handle The handle to the SVG renderer
	* @param cr The Cairo handler
	* @param cairo_surf The Cairo based surface
	* @param sdl_surface The SDL surface
	* @param image The given image.
	*/
	void cleanup(RsvgHandle * rsvg_handle, cairo_t * cr, cairo_surface_t * cairo_surf, SDL_Surface * sdl_surface, void * image);

	/**
	* @brief Get the frame number to display
	*
	* @return The frame number to display.
	*/
	unsigned int getFrame();
};

#endif
