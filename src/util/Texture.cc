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

#include "Texture.h"

/**
* @brief Determine if the str has the given suffix
*
* @param str The string to test
* @param suffix The required suffix
*
* @return TRUE if 'str' has the given suffix.
*/
bool hasExtension(const std::string& str, const std::string &suffix) {
	return str.size() >= suffix.size() &&
	       str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

Texture::Texture() {
	//Initialize
	texture = nullptr;
	width = 0;
	height = 0;
	lastError = nullptr;
}

Texture::~Texture() {
	//Deallocate
	destroy();
}

bool Texture::loadFromFile(SDL_Renderer* renderer, const std::string& path) {
	// Load with no scaling.
	return loadFromFile(renderer, path, -1, -1);
}

bool Texture::loadFromFile(SDL_Renderer* renderer, const std::string& path, int width, int height) {

	g_info("%s[%d]: Loading file %s", __FILE__, __LINE__, path.c_str());

	/*
	 * Determine type, if SVG or other.
	 * SVG uses cairo for rendering, other uses SDL_image...
	 */
	bool res = true;
	const std::string extension = ".svg";
	if (res && hasExtension(path, extension)) {
		res = loadSVG(renderer, path, width, height);
	} else {
		res = loadPNG(renderer, path, width, height);
	}
	if (res) {
		g_info("%s[%d]: Loaded file %s", __FILE__, __LINE__, path.c_str());
	} else {
		g_info("%s[%d]: FAILED Loaded file %s", __FILE__, __LINE__, path.c_str());
	}
	return res;
}

bool Texture::loadPNG(SDL_Renderer* renderer, const std::string& path, int width, int height) {
	//Get rid of preexisting texture
	destroy();
	g_info("%s[%d]: Loading PNG file %s", __FILE__, __LINE__, path.c_str());
	//The final texture
	SDL_Texture* newTexture = nullptr;
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == nullptr) {
		lastError = IMG_GetError();
		g_info("%s[%d] : %s", __FILE__, __LINE__, lastError);
	} else {
		// scale if needed.
		if (width != -1 || height != -1) {
			if (width == -1) {
				width = loadedSurface->w;
			}
			if (height == -1) {
				height = loadedSurface->h;
			}
			g_info("%s[%d] : Scaling to %d %d", __FILE__, __LINE__, width, height);
			SDL_Surface *n = SDL_CreateRGBSurface(loadedSurface->flags, width, height, loadedSurface->format->BitsPerPixel, loadedSurface->format->Rmask, loadedSurface->format->Gmask, loadedSurface->format->Bmask, loadedSurface->format->Amask);
			if (n == nullptr) {
				g_info("%s[%d] : %s", __FILE__, __LINE__, SDL_GetError());
				SDL_FreeSurface(loadedSurface);
				return false;
			}
			if (SDL_BlitScaled(loadedSurface, nullptr, n, nullptr) != 0) {
				g_info("%s[%d] : %s", __FILE__, __LINE__, SDL_GetError());
				SDL_FreeSurface(loadedSurface);
				SDL_FreeSurface(n);
				return false;
			}
			SDL_FreeSurface(loadedSurface);
			loadedSurface = n;
		} else {
			g_info("%s[%d] : Not scaling", __FILE__, __LINE__);
		}
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

		if (newTexture == nullptr) {
			lastError = SDL_GetError();
			g_info("%s[%d] : failed %s", __FILE__, __LINE__, lastError);
		} else {
			//Get image dimensions
			this->width = loadedSurface->w;
			this->height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	this->texture = newTexture;
	return this->texture != nullptr;
}

bool Texture::loadSVG(SDL_Renderer* renderer, const std::string& path, int width, int height) {
	//Get rid of preexisting texture
	destroy();
	//The final texture
	SDL_Texture* newTexture = nullptr;
	cairo_surface_t* cairo_surf = nullptr;
	cairo_t* cr = nullptr;
	RsvgHandle* rsvg_handle = nullptr;
	GError* error = nullptr;
	unsigned char* image = nullptr;
	int rwidth = 0;
	int  rheight = 0;
	int stride = 0;
	float wscale = 1.0;
	float vscale = 1.0;
	RsvgDimensionData dimensions;
	SDL_Surface * sdl_surface = nullptr;

	/* Create an RSVG Handle from the SVG file: */
	rsvg_handle = rsvg_handle_new_from_file(path.c_str(), &error);
	if (rsvg_handle == nullptr) {
		g_info("%s[%d] : Loading SVG file error: %s", __FILE__, __LINE__, error->message);
		g_error_free(error);
		return false;
	}

	rsvg_handle_get_dimensions(rsvg_handle, &dimensions);
	rwidth = dimensions.width;
	rheight = dimensions.height;

	g_info("%s[%d]: Loading SVG file %s (%d x %d)", __FILE__, __LINE__, path.c_str(), rwidth, rheight);

	/* Pick best scale to render to (for the canvas in this instance of Tux Paint) */

	if (width != -1 || height != -1) {
		wscale = (float) width / (float) rwidth;
		vscale = (float) height / (float) rheight;
	} else {
		width = rwidth;
		height = rheight;
	}

	g_info("%s[%d]: Scaling SVG file %s to %d x %d (%f %f scale)", __FILE__, __LINE__, path.c_str(), width, height, wscale, vscale);

	/* scanline width */
	stride = width * OCCILLO_TEXTURE_BYTEPP;

	/* Allocate space for an image: */
	image = (unsigned char *) calloc(stride * height, 1);
	if (image == nullptr) {
		g_warning("%s[%d]: Unable to allocate image buffer to store rendered SVG", __FILE__, __LINE__);
		g_object_unref(rsvg_handle);
		return false;
	}

	/* Create a surface for Cairo to draw into: */
	cairo_surf = cairo_image_surface_create_for_data(image, CAIRO_FORMAT_ARGB32,
	             width, height, stride);

	if (cairo_surface_status(cairo_surf) != CAIRO_STATUS_SUCCESS) {
		g_warning("%s[%d]: cairo_image_surface_create() failed!", __FILE__, __LINE__);
		/* Clean up: */
		cleanup(rsvg_handle, cr, cairo_surf, sdl_surface, image);
		return false;
	}

	/* Create a new Cairo object: */
	cr = cairo_create(cairo_surf);
	if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
		g_warning("%s[%d]: cairo_create() failed!", __FILE__, __LINE__);
		/* Clean up: */
		cleanup(rsvg_handle, cr, cairo_surf, sdl_surface, image);
		return false;
	}

	/* Ask RSVG to render the SVG into the Cairo object: */
	if (width != rwidth || height != rheight) {
		cairo_scale(cr, wscale, vscale);
	}
	rsvg_handle_render_cairo(rsvg_handle, cr);
	cairo_surface_finish(cairo_surf);

	/* Create the SDL surface using the pixel data stored: */
	sdl_surface = SDL_CreateRGBSurfaceFrom((void *) image, width, height,
	                                       OCCILLO_TEXTURE_BPP, stride,
	                                       OCCILLO_TEXTURE_RMASK, OCCILLO_TEXTURE_GMASK, OCCILLO_TEXTURE_BMASK, OCCILLO_TEXTURE_AMASK);

	if (sdl_surface == nullptr) {
		g_warning("%s[%d]: SDL_CreateRGBSurfaceFrom() failed!", __FILE__, __LINE__);
		/* Clean up: */
		cleanup(rsvg_handle, cr, cairo_surf, sdl_surface, image);
		return false;
	}

	g_info("%s[%d] : SDL surface from %d x %d SVG is %d x %d", __FILE__, __LINE__, sdl_surface->w, sdl_surface->h, rwidth, rheight);

	//Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface(renderer, sdl_surface);

	if (newTexture == nullptr) {
		lastError = SDL_GetError();
	} else {
		//Get image dimensions
		this->width = sdl_surface->w;
		this->height = sdl_surface->h;
	}
	/* Clean up: */
	cleanup(rsvg_handle, cr, cairo_surf, sdl_surface, image);

	//Return success
	this->texture = newTexture;
	return this->texture != nullptr;
}

bool Texture::loadFromText(SDL_Renderer* renderer, const std::string& text, TTF_Font *font, SDL_Color* colour) {
	destroy();
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderUTF8_Blended( font, text.c_str(), *colour);
	if ( textSurface == nullptr ) {
		g_info("%s[%d] : Unable to render text surface! SDL_ttf Error: %s", __FILE__, __LINE__, TTF_GetError());
	} else {
		//Create texture from surface pixels
		this->texture = SDL_CreateTextureFromSurface( renderer, textSurface );
		if ( this->texture == nullptr ) {
			g_info("%s[%d] : Unable to create texture %s", __FILE__, __LINE__, SDL_GetError());
		} else {
			//Get image dimensions
			this->width = textSurface->w;
			this->height = textSurface->h;
		} //Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	//Return success
	return this->texture != nullptr;
}

bool Texture::loadFromText(SDL_Renderer* renderer, const std::string& text, TTF_Font *font, SDL_Color* colour, SDL_Color* outlineColour) {
	destroy();
	//Render front text surface
	SDL_Surface* textFGSurface = TTF_RenderUTF8_Blended( font, text.c_str(), *colour );
	TTF_SetFontOutline(font, OCCILLO_OUTLINE_SIZE);
	// Render outline
	SDL_Surface* textBGSurface = TTF_RenderUTF8_Blended( font, text.c_str(), *outlineColour );
	TTF_SetFontOutline(font, 0);
	SDL_Rect rect = { OCCILLO_OUTLINE_SIZE,  OCCILLO_OUTLINE_SIZE, textFGSurface->w, textFGSurface->h};

	// blit text onto its outline
	SDL_SetSurfaceBlendMode(textFGSurface, SDL_BLENDMODE_BLEND);
	SDL_BlitSurface(textFGSurface, nullptr, textBGSurface, &rect);
	SDL_FreeSurface(textFGSurface);

	if ( textBGSurface == nullptr ) {
		g_info("%s[%d] : Unable to render text surface! SDL_ttf Error: %s", __FILE__, __LINE__, TTF_GetError());
	} else {
		//Create texture from surface pixels
		this->texture = SDL_CreateTextureFromSurface( renderer, textBGSurface );
		if ( this->texture == nullptr ) {
			g_info("%s[%d] : Unable to create texture %s", __FILE__, __LINE__, SDL_GetError());
		} else {
			//Get image dimensions
			this->width = textBGSurface->w;
			this->height = textBGSurface->h;
		} //Get rid of old surface
		SDL_FreeSurface( textBGSurface );
	}
	//Return success
	return this->texture != nullptr;
}

void Texture::setEmptyTexture(int width, int height){
  destroy();
	this->width = width;
	this->height = height;
}

void Texture::cleanup(RsvgHandle * rsvg_handle, cairo_t * cr, cairo_surface_t * cairo_surf, SDL_Surface * sdl_surface, void * image) {
	if (rsvg_handle != nullptr) {
    rsvg_handle_close(rsvg_handle, nullptr);
		g_object_unref(rsvg_handle);
	}
	if (cairo_surf != nullptr) {
		cairo_surface_destroy(cairo_surf);
	}
	if (image != nullptr) {
		free(image);
	}
	if (cr != nullptr) {
		cairo_destroy(cr);
	}
	if (sdl_surface != nullptr) {
		SDL_FreeSurface(sdl_surface);
	}
}

void Texture::destroy() {
	//Free texture if it exists
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
		width = 0;
		height = 0;
		lastError = nullptr;
	}
}

void Texture::render(SDL_Renderer* renderer, int x, int y) {
	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, width, height };
	SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);
}

void Texture::render(SDL_Renderer* renderer, int x, int y, int x1, int x2, int y1, int y2) {
	SDL_Rect renderQuad = {x, y, x2, y2 };
	SDL_Rect clip = {x1, y1, x2, y2 };
	SDL_RenderCopy(renderer, texture, &clip, &renderQuad);
}

const char * Texture::getLastError() {
	const char * e = lastError;
	lastError = nullptr;
	return e;
}

int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}

void Texture::setBlendMode( SDL_BlendMode blending ) {
	SDL_SetTextureBlendMode( texture, blending );
}

void Texture::setAlpha( Uint8 alpha ) {
	SDL_SetTextureAlphaMod( texture, alpha );
}
