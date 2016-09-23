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

#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <glib.h>
#include "config.h"
#include "Configuration.h"
#include "GameConfiguration.h"

// Handle circular reference to game configuration.
#ifndef GameConfiguration
class GameConfiguration;
#endif

/**
 * @brief A display instance.
 */
class Display {
public:
  /**
   * @brief Create a new display instance. This does not initialise the display.
   */
  Display();
  virtual ~Display();
  /**
   * Initialise the display
   * @param config The application configuration
   * @param game The game configuration.
   * @return TRUE if it initialised correctly.
   */
  bool init(Configuration *config, GameConfiguration *game);
  /**
   * Update the display based on information in the system config
   * @param config The application configuration
   * @return TRUE if it initialised correctly.
   */
  bool update(Configuration *config);
  /**
   * Shutdown the display
   */
  void close();
  /**
   * Get the renderer.
   * @return The renderer.
   */
  SDL_Renderer* getRenderer();
  /**
   * Clear the display
   */
  void clear();
  /**
   * Render the image.
   */
  void render();

	/**
	* @brief Generate a screenshot.
	*/
	void screenshot();

private:
  /**
   * @brief SDL Window pointer
   */
  SDL_Window* displayWindow;
  /**
   * @brief SDL Display renderer for 2d acceleration
   */
  SDL_Renderer* displayRenderer;
    /**
   * @brief Game controller.
   */
  SDL_GameController *controller;
  /**
   * @brief The width of the display
   */
  int displayWidth;
  /**
   * @brief The height of the display
   */
  int displayHeight;
  /**
   * @brief Flag to request fullscreen mode for the given resolution.
   */
  bool displayFullscreen;
  /**
   * TRUE if SDL was initialised.
   */
  bool sdl_init;
  /**
   * TRUE if SDL_image was initialised.
   */
  bool sdl_image_init;
  /**
   * TRUE if SDL_mixer was initialised.
   */
  bool sdl_mixer_init;
  /**
   * TRUE if SDL_ttf was initialised.
   */
  bool sdl_ttf_init;
};

#endif  /* DISPLAY_H */

