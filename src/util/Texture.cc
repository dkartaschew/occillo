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

Texture::Texture() {
	//Initialize
	width = 0;
	height = 0;
	lastError = nullptr;
	startTime = SDL_GetTicks();
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
	bool res = true;
	destroy();
	std::vector<std::string> frames = StringUtil::split(path, ';');
	unsigned int sz = frames.size();
	for (unsigned int i = 0; i < sz; i++) {
		std::string frame = frames[i];
		if (!frame.empty()) {
			g_info("%s[%d]: Loading file %s", __FILE__, __LINE__, frame.c_str());

			/*
			 * Determine type, if SVG or other.
			 * SVG uses cairo for rendering, other uses SDL_image...
			 */
			const std::string extension = ".svg";
			if (StringUtil::hasExtension(frame, extension)) {
				res = loadSVG(renderer, frame, width, height);
			} else {
				res = loadPNG(renderer, frame, width, height);
			}
			if (res) {
				g_info("%s[%d]: Loaded file %s", __FILE__, __LINE__, frame.c_str());
			} else {
				g_info("%s[%d]: FAILED Loaded file %s", __FILE__, __LINE__, frame.c_str());
			}
		}
	}
	return !texture.empty();
}

bool Texture::loadPNG(SDL_Renderer* renderer, const std::string& path, int width, int height) {
	g_info("%s[%d]: Loading PNG file %s", __FILE__, __LINE__, path.c_str());
	//The final texture
	SDL_Texture* newTexture = nullptr;
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == nullptr) {
		lastError = IMG_GetError();
		g_info("%s[%d] : %s", __FILE__, __LINE__, lastError);
	} else {
		// Ensure new surface is ARGB. (This is what we render SVG to, so use this here as well)
		SDL_Surface* newSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_ARGB8888, 0);
		if (newSurface != nullptr) {
			// Converted OK, so free the old, and set the new.
			SDL_FreeSurface(loadedSurface);
			loadedSurface = newSurface;
		} else {
			// failed to convert, so exit.
			g_info("%s[%d] : %s", __FILE__, __LINE__, SDL_GetError());
			SDL_FreeSurface(loadedSurface);
			return false;
		}

		// scale if needed.
		if (width != -1 || height != -1) {
			if (width == -1) {
				width = loadedSurface->w;
			}
			if (height == -1) {
				height = loadedSurface->h;
			}
			g_info("%s[%d] : Scaling to %d %d", __FILE__, __LINE__, width, height);
			SDL_Surface *n = SDL_CreateRGBSurface(loadedSurface->flags, width, height, loadedSurface->format->BitsPerPixel,
			                                      loadedSurface->format->Rmask, loadedSurface->format->Gmask, loadedSurface->format->Bmask, loadedSurface->format->Amask);
			if (n == nullptr) {
				g_info("%s[%d] : %s", __FILE__, __LINE__, SDL_GetError());
				SDL_FreeSurface(loadedSurface);
				return false;
			}
			// Set the src as no blend mode, so we get a direct copy.
			SDL_SetSurfaceBlendMode(loadedSurface, SDL_BLENDMODE_NONE);
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
	if (newTexture != nullptr) {
		texture.push_back(newTexture);
	}
	return newTexture != nullptr;
}

bool Texture::loadSVG(SDL_Renderer* renderer, const std::string& path, int width, int height) {
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
	if (newTexture != nullptr) {
		texture.push_back(newTexture);
	}
	return newTexture != nullptr;
}

bool Texture::loadFromText(SDL_Renderer* renderer, const std::string& text, TTF_Font *font, SDL_Color* colour) {
	destroy();
	if (renderer == nullptr || font == nullptr || colour == nullptr) {
		return false;
	}
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderUTF8_Blended( font, text.c_str(), *colour);
	if ( textSurface == nullptr ) {
		g_info("%s[%d] : Unable to render text surface! SDL_ttf Error: %s", __FILE__, __LINE__, TTF_GetError());
	} else {
		//Create texture from surface pixels
		SDL_Texture* t = SDL_CreateTextureFromSurface( renderer, textSurface );
		if ( t == nullptr ) {
			g_info("%s[%d] : Unable to create texture %s", __FILE__, __LINE__, SDL_GetError());
		} else {
			//Get image dimensions
			this->texture.push_back(t);
			this->width = textSurface->w;
			this->height = textSurface->h;
		} //Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	//Return success
	return !texture.empty();
}

bool Texture::loadFromText(SDL_Renderer* renderer, const std::string& text, TTF_Font *font, SDL_Color* colour, SDL_Color* outlineColour) {
	destroy();
	if (renderer == nullptr || font == nullptr || colour == nullptr || outlineColour == nullptr) {
		return false;
	}

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
		SDL_Texture* t = SDL_CreateTextureFromSurface( renderer, textBGSurface );
		if ( t == nullptr ) {
			g_info("%s[%d] : Unable to create texture %s", __FILE__, __LINE__, SDL_GetError());
		} else {
			this->texture.push_back(t);
			//Get image dimensions
			this->width = textBGSurface->w;
			this->height = textBGSurface->h;
		} //Get rid of old surface
		SDL_FreeSurface( textBGSurface );
	}
	//Return success
	return !texture.empty();
}

bool Texture::loadFromColour(SDL_Renderer* renderer, SDL_Color* colour, int width, int height) {
	destroy();
	if (renderer == nullptr || colour == nullptr) {
		return false;
	}
	// Create the surface. (ARGB8888)
	SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, OCCILLO_TEXTURE_BPP, 
			OCCILLO_TEXTURE_RMASK, OCCILLO_TEXTURE_GMASK, OCCILLO_TEXTURE_BMASK, OCCILLO_TEXTURE_AMASK);
	if (surface == nullptr) {
		g_info("%s[%d]: SDL_CreateRGBSurface() failed!", __FILE__, __LINE__);
		/* Clean up: */
		//SDL_FreeSurface( surface );
		return false;
	}
	// And fill.
	if(0 != SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, colour->r, colour->g, colour->b, colour->a))){
		g_info("%s[%d] : Unable to fill surface %s", __FILE__, __LINE__, SDL_GetError());
		/* Clean up: */
		SDL_FreeSurface( surface );
		return false;

	}

	// Convert to texture;
	SDL_Texture* t = SDL_CreateTextureFromSurface( renderer, surface );
	if ( t == nullptr ) {
		g_info("%s[%d] : Unable to create texture %s", __FILE__, __LINE__, SDL_GetError());
	} else {
		//Get image dimensions
		this->texture.push_back(t);
		this->width = surface->w;
		this->height = surface->h;
	} //Get rid of old surface
	SDL_FreeSurface( surface );

	//Return success
	return !texture.empty();

}

void Texture::setEmptyTexture(int width, int height) {
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
	unsigned int sz = texture.size();
	for (unsigned int i = 0; i < sz; i++) {
		SDL_Texture* t = texture[i];
		if (t != nullptr) {
			SDL_DestroyTexture(t);
		}
	}
	texture.clear();
	width = 0;
	height = 0;
	lastError = nullptr;
	startTime = SDL_GetTicks();
}

unsigned int Texture::getFrame() {
	const unsigned int sz = texture.size();
	if (sz <= 1) {
		return 0;
	}
	unsigned int i = 0;
	unsigned int delta = (SDL_GetTicks() - startTime); // time in msec
	// Assume 10fps;
	i = (delta / 100); // This will assume 10 frames (i is delta in deci-seconds, or tenths of second)
	// Now cap to sz.
	i = i % sz;
	return i;
}

void Texture::render(SDL_Renderer* renderer, int x, int y) {
	if (texture.empty()) {
		return;
	}
	// Determine the texture to display.
	unsigned int i = getFrame();
	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, width, height };
	SDL_RenderCopy(renderer, texture[i], nullptr, &renderQuad);
}

void Texture::render(SDL_Renderer* renderer, int x, int y, int x1, int x2, int y1, int y2) {
	if (texture.empty()) {
		return;
	}
	// Determine the texture to display.
	unsigned int i = getFrame();
	SDL_Rect renderQuad = {x, y, x2, y2 };
	SDL_Rect clip = {x1, y1, x2, y2 };
	SDL_RenderCopy(renderer, texture[i], &clip, &renderQuad);
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
	unsigned int sz = texture.size();
	for (unsigned int i = 0; i < sz ; i++) {
		SDL_SetTextureBlendMode( texture[i], blending );
	}
}

void Texture::setAlpha( Uint8 alpha ) {
	unsigned int sz = texture.size();
	for (unsigned int i = 0; i < sz ; i++) {
		SDL_SetTextureAlphaMod( texture[i], alpha );
	}
}

SDL_Color* Texture::getColour(){
	static SDL_Color col;
	col.a = 255;
	col.r += 64;
	if(col.r == 0){
		col.g += 64;
	}
	if(col.g == 0){
		col.b += 64;
	}
	return &col;
}
