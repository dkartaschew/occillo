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

#include "Game.h"

Game::Game(Configuration* config, GameConfiguration* game, Display* display) :
	config(config), game(game), display(display)  {
	renderer = display->getRenderer();
	gameState = new GameState();
	gameState->addLives(game->getInitialLives());
	gameState->setState(NEXT_LEVEL);
	completed = false;
}

Game::~Game() {
	Deactivate();
}

bool Game::Run() {
	return false;
}

bool Game::Activate() {
	if (gameState->getState() == NEXT_LEVEL) {
		// Create the instance of the next level and push onto the application state stack.
		SDL_SetRelativeMouseMode(SDL_TRUE);
		std::vector<std::string>* levels = game->getLevels();
		int currentLevel = gameState->getLevel() % levels->size();
		gameState->incLevel();
		gameState->setState(GAME_CONTINUE);
		IAppState* level = new Level(config, game, display, gameState, (*levels)[currentLevel]);
		getApplicationState()->push(level);
		return true;
	}
	if (!completed && gameState->getState() == GAME_END) {
		// The game has completed, so create a End Game application state and push onto the application state stack.
		completed = true;
		IAppState* endGame = new EndGame(config, game, display, gameState);
		gameState->setState(GAME_CONTINUE);
		SDL_SetRelativeMouseMode(SDL_FALSE);
		getApplicationState()->push(endGame);
		return true;
	}
	// nothing left to activate/construct so exit...
	return false;
}

void Game::Deactivate() {
	if (gameState != nullptr) {
		delete gameState;
		gameState = nullptr;
	}
}

