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

#include "MainMenu.h"
#include "Game.h"
#include "CreditsMenu.h"
#include "HiScoreMenu.h"
#include "OptionsMenu.h"

MainMenu::MainMenu(Configuration* config, GameConfiguration* game, Display* display) :
	Menu(config, game, display) {
	backgroundMusic = nullptr;
}

bool MainMenu::Activate() {
	if (widgets == nullptr || reactivateRequired) {

		reactivateRequired = false;
		destroy();
		if (backgroundMusic != nullptr) {
			Mix_FreeMusic(backgroundMusic);
			backgroundMusic = nullptr;
		}

		const int dw = config->getDisplayWidth();
		const int dh = config->getDisplayHeight();
		const int bh = dh / MENU_ROWS;
		const int menuA = config->getMenuFocusAlpha();
		const int menuNA = config->getMenuNonFocusAlpha();
		// Set the height of the version string to be 10, or half of brick height.
		const int vh = (bh / 2) > 10 ? 10 : (bh /2); 

		TTF_Font* fontTitle = TTF_OpenFont(config->locateResource(*(game->getFontBold())).c_str(), (bh * game->getTitleSizeRatio()));
		TTF_Font* font = TTF_OpenFont(config->locateResource(*(game->getFont())).c_str(), bh);
		TTF_Font* fontVersion = TTF_OpenFont(config->locateResource(*(game->getFont())).c_str(), vh);
		
		widgets = new std::vector<IUIWidget*>();

		// Background
		Texture* text = new Texture();
		text->loadFromFile(renderer, config->locateResource(*(game->getBackground())), dw, dh);
		background = new UIImage(text);

		// Pointer
		text = new Texture();
		text->loadFromFile(renderer, config->locateResource(*(game->getCursor())), bh, bh);
		cursor = new UICursor(text, dw, dh);

		// Title.
		std::string* name = game->getGameName();
		std::string gameName;
		if(name == nullptr){
			gameName = PACKAGE_NAME;
		} else {
			gameName = *name;
		}
		IUIWidget* widget = new UILabel(_(gameName.c_str()), fontTitle, game->getTitleFontColour(), renderer);
		widget->setXY((dw / 2) - (widget->getWidth() / 2), bh * 3);
		widgets->push_back(widget);
		
		// Start of menu options.
		text = new Texture();
		text->loadFromText(renderer, _("Play"), font, game->getFontColour());
		widget = new UIButton(text, (dw / 2) - (text->getWidth() / 2), bh * 6, menuA, menuNA);
		widgets->push_back(widget);
		widget->addEventListener(this);

		text = new Texture();
		text->loadFromText(renderer, _("High Scores"), font, game->getFontColour());
		widget = new UIButton(text, (dw / 2) - (text->getWidth() / 2), bh * 8, menuA, menuNA);
		widgets->push_back(widget);
		widget->addEventListener(this);

		text = new Texture();
		text->loadFromText(renderer, _("Options"), font, game->getFontColour());
		widget = new UIButton(text, (dw / 2) - (text->getWidth() / 2), bh * 10, menuA, menuNA);
		widgets->push_back(widget);
		widget->addEventListener(this);

		text = new Texture();
		text->loadFromText(renderer, _("Credits"), font, game->getFontColour());
		widget = new UIButton(text, (dw / 2) - (text->getWidth() / 2), bh * 12, menuA, menuNA);
		widgets->push_back(widget);
		widget->addEventListener(this);

		text = new Texture();
		text->loadFromText(renderer, _("Quit"), font, game->getFontColour());
		widget = new UIButton(text, (dw / 2) - (text->getWidth() / 2), bh * 14, menuA, menuNA);
		widgets->push_back(widget);
		widget->addEventListener(this);
		
		// Version information
		 widget = new UILabel((PACKAGE_NAME " " PACKAGE_VERSION), fontVersion, game->getTitleFontColour(), renderer);
		widget->setXY(dw - (widget->getWidth() + 1), dh - (widget->getHeight() + 1));
		widgets->push_back(widget);


		// Free the font.
		TTF_CloseFont(font);
		TTF_CloseFont(fontTitle);
		TTF_CloseFont(fontVersion);

		// Set the mouse to be hidden since we are drawing it ourselves.
		SDL_ShowCursor(SDL_DISABLE);

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
	}
	return true;
}

void MainMenu::event(IUIWidget * widget) {

	// Play handler
	if (widget == (*widgets)[1]) {
		runstate = EXIT_FALSE;
		// Create the game state...
		getApplicationState()->push(new Game(config, game, display));
		g_info("%s[%d]: Starting new game", __FILE__, __LINE__);

		// stop the audio.
		reactivateRequired = true;
		Mix_HaltMusic();
		return;
	}

	// Hiscore handler
	if (widget == (*widgets)[2]) {
		runstate = EXIT_FALSE;
		// Create the Hiscore Menu.
		getApplicationState()->push(new HiScoreMenu(config, game, display, this));
		g_info("%s[%d]: Initialise High Score Menu", __FILE__, __LINE__);
		return;
	}

	// Options handler
	if (widget == (*widgets)[3]) {
		runstate = EXIT_FALSE;
		// Create the credits Menu.
		getApplicationState()->push(new OptionsMenu(config, game, display, this));
		g_info("%s[%d]: Initialiase Options Menu", __FILE__, __LINE__);
		return;
	}

	// Credit handler
	if (widget == (*widgets)[4]) {
		runstate = EXIT_FALSE;
		// Create the credits Menu.
		getApplicationState()->push(new CreditsMenu(config, game, display, this));
		g_info("%s[%d]: Initialise Credits Menu", __FILE__, __LINE__);
		return;
	}

	// Quit handler
	if (widget == (*widgets)[5]) {
		runstate = EXIT_TRUE;
		g_info("%s[%d]: Exit game", __FILE__, __LINE__);
		// Stop music...
		Mix_HaltMusic();
		if (backgroundMusic != nullptr) {
			Mix_FreeMusic(backgroundMusic);
			backgroundMusic = nullptr;
		}
		return;
	}
}
