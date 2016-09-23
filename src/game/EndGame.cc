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
#include "EndGame.h"

EndGame::EndGame(Configuration *config, GameConfiguration* game, Display* display, GameState* state) :
	Menu(config, game, display), gameState(state) {
	backgroundMusic = nullptr;
}

EndGame::~EndGame() {
	Deactivate();
}

bool EndGame::Activate() {
	if (widgets == nullptr) {
		// As we are using text input, enable SDL...
		SDL_StartTextInput();
		const int dh = config->getDisplayHeight();
		const int dw = config->getDisplayWidth();
		const int bh = dh / MENU_ROWS;
		//  const int bw = dw / MENU_COLUMNS;
		//  const int menuA = config->getMenuFocusAlpha();
		//  const int menuNA = config->getMenuNonFocusAlpha();
		TTF_Font * fontTitle = TTF_OpenFont(config->locateResource(*(game->getFontBold())).c_str(), bh * 3 / 2);
		TTF_Font * font = TTF_OpenFont(config->locateResource(*(game->getFont())).c_str(), bh);

		widgets = new std::vector<IUIWidget*>();

		// Background
		Texture * text = new Texture();
		text->loadFromFile(renderer, config->locateResource(*(game->getBackground())), dw, dh);
		background = new UIImage(text);

		// Return button
		text = new Texture();
		text->loadFromText(renderer, _("OK"), font, game->getFontColour());
		IUIWidget* widget = new UIButton(text, (dw / 2) - (text->getWidth() / 2), bh * 15);
		widgets->push_back(widget);
		widget->addEventListener(this);

		// Title.
		newLabel(_("Game Over"), fontTitle, 4);

		// Score
		int score = gameState->getScore();
		char dest[100] = "";
		std::snprintf(dest, 100, _("Score: %d"), score);
		std::string dstr = dest;

		if (score > getLowestScore()) {

			// Player:
			newLabel(dstr, font, 6);
			newLabel(_("Player Name:"), font, 8);

			UIText* textinput = new UIText(TTF_OpenFont(config->locateResource(*(game->getFont())).c_str(), bh), game->getTitleFontColour(), renderer);
			textinput->setXY(dw, bh * 10);
			textinput->setCenter(true);
			textinput->setText(*(config->getPlayerName()));
			textinput->addEventListener(this);
			widgets->push_back(textinput);

		} else {

			// Sorry message...
			newLabel(dstr, font, 6);
			std::string msg = _("Sorry, need to do better next time!");
			newLabel(msg, font, 8);
		}

		// Pointer
		text = new Texture();
		text->loadFromFile(renderer, config->locateResource(*(game->getCursor())));
		cursor = new UICursor(text, dw, dh);

		TTF_CloseFont(fontTitle);
		TTF_CloseFont(font);

		// Setup the music.
		if (game->getTitleMusic() != nullptr) {
			backgroundMusic = Mix_LoadMUS(config->locateResource(*(game->getTitleMusic())).c_str());
			if (backgroundMusic == nullptr) {
				g_info("%s[%d] Could not load background music! SDL_Mixer Error: %s", __FILE__, __LINE__, Mix_GetError());
			}
			// Start music.
			if (backgroundMusic != nullptr) {
				if (Mix_PlayingMusic() != 0) {

					Mix_HaltMusic();
				}
				Mix_PlayMusic(backgroundMusic, -1);
			}
			// Set the music volume
			Mix_VolumeMusic(config->getMusicVolume());
		}
		//SDL_StartTextInput();

	}
	return true;
}

void EndGame::Deactivate() {
	Menu::Deactivate();
	SDL_StopTextInput();

	if (backgroundMusic != nullptr) {
		// Stop music...
		Mix_HaltMusic();
		Mix_FreeMusic(backgroundMusic);
		backgroundMusic = nullptr;
	}
}

void EndGame::event(IUIWidget *widget) {
	// Return handler
	if ((widget == (*widgets)[0]) || (widget == (*widgets)[widgets->size() - 1])) {
		gameState->setState(GAME_END);

		if (gameState->getScore() > getLowestScore()) {
			// Get the player name and create a high score.

			IUIWidget* text = (*widgets)[widgets->size() - 1];
			std::string playerName = ((UIText*) text)->getText();
			HighScore score(playerName, gameState->getScore(), gameState->getLevel(), std::time(nullptr));
			game->getHighScoreTable()->addScore(&score);
			// store the hiscore table. (in case of crash, intermediate scores are not lost).
			game->storeHighScoreTable();
			// store the name for future.
			config->setPlayerName(playerName);
		}
		runstate = EXIT_TRUE;
		return;
	}
}

void EndGame::newLabel(const std::string& text, TTF_Font *font, int y) {
	const int dh = config->getDisplayHeight();
	const int dw = config->getDisplayWidth();
	const int bh = dh / MENU_ROWS;
	IUIWidget* widget = new UILabel(text, font, game->getTitleFontColour(), renderer);
	widget->setXY((dw / 2) - (widget->getWidth() / 2), bh * y);
	widgets->push_back(widget);
}

long EndGame::getLowestScore() {
	// get the table. (sets are sorted.)
	std::set<HighScore>* tbl = game->getHighScoreTable()->getTable();
	// get the last entry.
	std::set<HighScore>::reverse_iterator it = tbl->rbegin();
	return (*it).getScore();
}

