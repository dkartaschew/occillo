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

#include "Display.h"

Display::Display() {
	displayRenderer = nullptr;
	displayWindow = nullptr;
	displayFullscreen = false;
	displayHeight = 480;
	displayWidth = 720;
	sdl_init = false;
	sdl_image_init = false;
	sdl_mixer_init = false;
	sdl_ttf_init = false;
	controller = nullptr;
}

Display::~Display() {
	close();
}

bool Display::init(Configuration *config, GameConfiguration* game) {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
		g_info("%s[%d] SDL could not initialize! SDL Error: %s", __FILE__, __LINE__, SDL_GetError());
		success = false;
	} else {
		sdl_init = true;
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			g_info("%s[%d] Warning: Linear texture filtering not enabled!", __FILE__, __LINE__);
		}

		// store our resolution for later.
		displayFullscreen = config->isFullscreen();
		displayHeight = config->getDisplayHeight();
		displayWidth = config->getDisplayWidth();

		std::string* name = game->getGameName();
		std::string gameName;
		if (name == nullptr) {
			gameName = PACKAGE_NAME " " PACKAGE_VERSION;
		} else {
			gameName = *name;
		}

		//Create window
		Uint32 sdl_flags = 0;
		if (config->isFullscreen()) {
			sdl_flags = SDL_WINDOW_FULLSCREEN;
		} else {
			sdl_flags = SDL_WINDOW_SHOWN;
		}
		g_info("%s[%d] Creating Display %d %d %d", __FILE__, __LINE__, displayWidth, displayHeight, displayFullscreen);
		displayWindow = SDL_CreateWindow(gameName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, displayWidth, displayHeight, sdl_flags);

		if (displayWindow == nullptr) {
			g_info("%s[%d] Window could not be created! SDL Error: %s", __FILE__, __LINE__, SDL_GetError());
			success = false;
		} else {

			//Create renderer for window
			displayRenderer = SDL_CreateRenderer(displayWindow, -1, SDL_RENDERER_ACCELERATED);

			if (displayRenderer == nullptr) {
				g_info("%s[%d] Renderer could not be created! SDL Error: %s", __FILE__, __LINE__, SDL_GetError());
				success = false;
			} else {
				//Initialize renderer color
				SDL_SetRenderDrawColor(displayRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;

				if (!(IMG_Init(imgFlags) & imgFlags)) {
					g_info("%s[%d] SDL_image could not initialize! SDL_image Error: %s", __FILE__, __LINE__, IMG_GetError());
					success = false;
				} else {
					sdl_image_init = true;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1) {
					g_info("%s[%d] SDL_ttf could not initialize! SDL_ttf Error: %s", __FILE__, __LINE__, TTF_GetError());
					success = false;
				} else {
					sdl_ttf_init = true;
				}

				//Initialize SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, config->getAudioBufferSize())) {
					g_info("%s[%d] SDL_mixer could not initialize! SDL_Mixer Error: %s", __FILE__, __LINE__, Mix_GetError());
					success = false;
				} else {
					sdl_mixer_init = true;
					// Allocate 16 mixing channels.
					Mix_AllocateChannels(16);
				}

				controller = nullptr;
				//Check for joysticks
				if (SDL_NumJoysticks() < 1) {
					g_info("%s[%d] Warning: No game controllers connected!", __FILE__, __LINE__);
				} else { //Load joystick
					for (int i = 0; i < SDL_NumJoysticks(); ++i) {
						if (SDL_IsGameController(i)) {
							controller = SDL_GameControllerOpen(i);
							if (controller) {
								break;
							} else {
								g_info("%s[%d] Warning: Unable to open game controller! SDL Error: %s", __FILE__, __LINE__, SDL_GetError());
							}
						}
					}
				}

				if (success) {
					// Set icon
					std::string path = *(game->getApplicationIcon());
					path = config->locateResource(path);
					if (!path.empty()) {
						g_info("%s[%d] : Setting application icon: %s", __FILE__, __LINE__, path.c_str());
						SDL_Surface* surface = IMG_Load(path.c_str());
						if (surface == nullptr) {
							g_info("%s[%d] : Failed to load application icon: %s", __FILE__, __LINE__, IMG_GetError());
						} else {
							// The icon is attached to the window pointer
							SDL_SetWindowIcon(displayWindow, surface);
							// ...and the surface containing the icon pixel data is no longer required.
							SDL_FreeSurface(surface);
						}
					}
				}
			}
		}
	}

	return success;
}

bool Display::update(Configuration *config) {
	bool df = config->isFullscreen();
	int dh = config->getDisplayHeight();
	int dw = config->getDisplayWidth();
	if ((df == displayFullscreen) && (dh == displayHeight) && (dh == displayWidth)) {
		// do nothing.
		return false;
	}
	if (displayFullscreen) {
		// we are in full screen switch from fullscreen to do the window resize
		SDL_SetWindowFullscreen(displayWindow, 0);
	}
	// do the resize...
	g_info("%s[%d] Creating Display %d %d %d", __FILE__, __LINE__, dw, dh, df);
	SDL_SetWindowSize(displayWindow, dw, dh);
	if (df) {
		// switch to requested fullscreen
		SDL_SetWindowFullscreen(displayWindow, SDL_WINDOW_FULLSCREEN);
	}
	// record what we set...
	displayFullscreen = df;
	displayHeight = dh;
	displayWidth = dw;
	return true;
}

void Display::close() {
	//Destroy window
	if (displayRenderer != nullptr) {
		SDL_DestroyRenderer(displayRenderer);
	}
	if (displayWindow != nullptr) {
		SDL_DestroyWindow(displayWindow);
	}
	displayWindow = nullptr;
	displayRenderer = nullptr;

	//Quit SDL subsystems
	if (controller != nullptr) {
		SDL_GameControllerClose(controller);
		controller = nullptr;
	}

	if (sdl_mixer_init) {
		Mix_Quit();
	}
	if (sdl_ttf_init) {
		TTF_Quit();
	}
	if (sdl_image_init) {
		IMG_Quit();
	}
	if (sdl_init) {
		SDL_Quit();
	}
}

SDL_Renderer* Display::getRenderer() {
	return displayRenderer;
}

void Display::clear() {
	SDL_SetRenderDrawColor(displayRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(displayRenderer);
}

void Display::render() {
	SDL_RenderPresent(displayRenderer);
}

void Display::screenshot() {
	if (displayRenderer != nullptr) {
		int tm = (int)time(NULL);
		char dest[100] = "";
#ifdef _WIN32
		_snprintf(dest, 100, "screenshot-%d.png", tm);
#else
		std::snprintf(dest, 100, "screenshot-%d.png", tm);
#endif
		std::string dstr = dest;

		SDL_Surface *sshot = SDL_CreateRGBSurface(0, displayWidth, displayHeight, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
		SDL_RenderReadPixels(displayRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
		//SDL_SaveBMP(sshot, "screenshot.bmp");
		IMG_SavePNG(sshot, dstr.c_str());
		SDL_FreeSurface(sshot);
	}
}


