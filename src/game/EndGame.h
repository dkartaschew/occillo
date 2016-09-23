/*
 *    Copyright (C) 2014 Darran Kartaschew
 *
 *    This file is part of Occillo.
 *
 *    Occillo is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Occillo is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Occillo.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef GAME_ENDGAME_H
#define GAME_ENDGAME_H

#include "config.h"

#include "main.h"
#include "IAppState.h"
#include "Configuration.h"
#include "GameConfiguration.h"
#include "GameState.h"
#include "Display.h"
#include "IUIWidget.h"
#include "UIButton.h"
#include "UICursor.h"
#include "UILabel.h"
#include "UIImage.h"
#include "UIText.h"
#include "Menu.h"

#include <cstdlib>
#include <cstdio>
#include <string>
#include <ctime>
#include <SDL_mixer.h>

/**
* @brief Class that implements the End Game application state. This application state creates a high score instance (if applicable).
*/
class EndGame : public Menu {
  public:

	/**
	 * @brief Initialse the EndGame screen on the game.
	 *
	 * @param config The engine configuration.
	 * @param game The game configuration.
	 * @param display The display (for rendering)
	 * @param state The game state that we will save...
	 */
	EndGame(Configuration* config, GameConfiguration* game, Display* display, GameState* state);

	/**
	 * @brief Destroy all elements...
	 */
	virtual ~EndGame();
	/**
	 * Activate the application state.
	 * return TRUE if the application state started correctly.
	 */
	bool Activate();
	/**
	 * Deactivate the application state, eg inform it to clean up.
	 */
	void Deactivate();

	/**
	 * @brief Handle event from the given widget.
	 *
	 * @param widget The widget that created the event.
	 */
	void event(IUIWidget *widget);
  private:

	/**
	 * @brief The current game state...
	 */
	GameState* gameState;

	/**
	 * @brief Create a new label
	 *
	 * @param text The text to display
	 * @param font The font to use
	 * @param y The y line to render...
	 */
	void newLabel(const std::string& text, TTF_Font *font, int y);

	/**
	 * Get the lowest score from the table...
	 * @return the lowest score on the table.
	 */
	long getLowestScore();

	/**
	* @brief The instance of the background music.
	*/
	Mix_Music* backgroundMusic;
};

#endif /* GAME_ENDGAME_H */
