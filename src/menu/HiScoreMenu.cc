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

#include "HiScoreMenu.h"

#define SCORE_ELEMENTS 2

HiScoreMenu::HiScoreMenu(Configuration* config, GameConfiguration* game, Display* display, MainMenu * mainMenu) :
	Menu(config, game, display) {
	this->mainMenu = mainMenu;
}

bool HiScoreMenu::Activate() {
	if (widgets == nullptr) {

		const int dw = config->getDisplayWidth();
		const int dh = config->getDisplayHeight();
		const int bh = dh / MENU_ROWS;
		const int menuA = config->getMenuFocusAlpha();
		const int menuNA = config->getMenuNonFocusAlpha();

		TTF_Font* fontTitle = TTF_OpenFont(config->locateResource(*(game->getFontBold())).c_str(), bh);
		TTF_Font* font = TTF_OpenFont(config->locateResource(*(game->getFont())).c_str(), bh);
		TTF_Font* fontLabel = TTF_OpenFont(config->locateResource(*(game->getFont())).c_str(), bh * 2 / 3);

		widgets = new std::vector<IUIWidget*>();

		// Background
		background = this->mainMenu->getBackground();
		// Pointer
		cursor = this->mainMenu->getCursor();

		// Title.
		newLabel(_("High Scores"), fontTitle, 3);

		// Credits.
		HighScoreTable* table = game->getHighScoreTable();
		int displayLine = 5;
		for (std::set<HighScore>::iterator it = table->getTable()->begin(); it != table->getTable()->end(); ++it) {
			const HighScore score = *it;
			newScoreLine(score, fontLabel, displayLine++);
			if (displayLine == 15) {
				break;
			}
		}

		// Return button
		Texture* text = new Texture();
		if(!text->loadFromText(renderer, _("Return"), font, game->getFontColour())){
			g_info("%s[%d] : Failed to create Return button texture, setting as missing texture.", __FILE__, __LINE__);
			text->loadFromColour(renderer, Texture::getColour(), 16, 16);
		}
		IUIWidget* widget = new UIButton(text, (dw / 2) - (text->getWidth() / 2), bh * (MENU_ROWS - 3), menuA, menuNA);
		widgets->push_back(widget);
		widget->addEventListener(this);

		// Free the font.
		TTF_CloseFont(font);
		TTF_CloseFont(fontTitle);
		TTF_CloseFont(fontLabel);
	}
	runstate = CONTINUE;
	return true;
}

void HiScoreMenu::newLabel(const std::string& text, TTF_Font *font, int y) {
	
	const int dw = config->getDisplayWidth();
	const int dh = config->getDisplayHeight();
	const int bh = dh / MENU_ROWS;

	if (!text.empty()) {
		IUIWidget* widget = new UILabel(text, font, game->getTitleFontColour(), renderer);
		widget->setXY((dw / 2) - (widget->getWidth() / 2), bh * y);
		widgets->push_back(widget);
	}
}

void HiScoreMenu::newScoreLine(const HighScore& score, TTF_Font *font, int y) {
	const int dw = config->getDisplayWidth();
	const int dh = config->getDisplayHeight();
	const int bh = dh / MENU_ROWS;
	const int bw = dw / MENU_COLUMNS;

	if (!score.getName()->empty()) {
		IUIWidget* widget = new UILabel(*(score.getName()), font, game->getTitleFontColour(), renderer);
		widget->setXY((bw * 8), bh * y);
		widgets->push_back(widget);
	}
	IUIWidget* widget = new UILabel(std::to_string(score.getScore()), font, game->getTitleFontColour(), renderer);
	widget->setXY((bw * 22) - widget->getWidth(), bh * y);
	widgets->push_back(widget);
}

void HiScoreMenu::destroy() {
	deleteWidgets();
}

void HiScoreMenu::event(IUIWidget *widget) {
	// Return handler
	if (widget == (*widgets)[widgets->size() - 1]) {
		runstate = EXIT_TRUE;
		game->storeHighScoreTable();
		return;
	}
}
