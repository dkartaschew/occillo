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


#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "config.h"

#include <cstdlib>
#include <string>
#include <vector>

// main.h includes the gettext, sdl, etc items
#include "main.h"
#include "IAppState.h"
#include "TextureRepository.h"
#include "Configuration.h"
#include "GameConfiguration.h"
#include "Level.h"
#include "GameState.h"
#include "EndGame.h"

/**
* @brief The Game application state, the state in which levels are created and run.
*/
class Game : public IAppState {
  public:
	/**
	* @brief Create a new game application state
	*
	* @param config The engine configuration
	* @param game The game configuration
	* @param display The display instance.
	*/
	Game(Configuration* config, GameConfiguration* game, Display* display);
	~Game();

	bool Run();
	bool Activate();
	void Deactivate();

  protected:

	/**
	* @brief The engine configuration
	*/
	Configuration* config;
	/**
	* @brief The game configuration.
	*/
	GameConfiguration* game;
	/**
	* @brief The current display instance
	*/
	Display* display;
	/**
	* @brief The current SDL Renderer to use
	*/
	SDL_Renderer* renderer;

	/**
	* @brief completed flag that the current game is complete.
	*/
	bool completed;

	/**
	* @brief The current game state.
	*/
	GameState* gameState;
};

#endif /* MAINMENU_H */
