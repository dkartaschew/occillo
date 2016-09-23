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

#include "OptionsMenu.h"

OptionsMenu::OptionsMenu(Configuration* config, GameConfiguration* game, Display* display, MainMenu * mainMenu) :
	Menu(config, game, display) {
	this->mainMenu = mainMenu;
	btnApply = nullptr;
	btnReturn = nullptr;
	volumeEffectControl = nullptr;
	volumeMusicControl = nullptr;
	fullscreenControl = nullptr;
	resolutionControl = nullptr;
}

bool OptionsMenu::Activate() {
	if (widgets == nullptr) {
		const int dh = config->getDisplayHeight();
		const int dw = config->getDisplayWidth();
		const int bh = dh / MENU_ROWS;
		const int btnH = (bh * 3) / 4;
		const int bw = dw / MENU_COLUMNS;
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
		IUIWidget* widget = new UILabel(_("Options"), fontTitle, game->getTitleFontColour(), renderer);
		widget->setXY((dw / 2) - (widget->getWidth() / 2), bh * 3);
		widgets->push_back(widget);

		// Labels
		newLabel(_("Screen Resolution"), fontLabel, 8, 5);
		newLabel(_("Full Screen"), fontLabel, 8, 6);
		newLabel(_("Effects Volume"), fontLabel, 8, 8);
		newLabel(_("Music Volume"), fontLabel, 8, 9);

		// Screen resolution
		std::set<Resolution>* res = new std::set<Resolution>();
		// Add the current one...
		res->insert(Resolution(dw, dh));
		const int displays = SDL_GetNumVideoDisplays();
		for (int d = 0; d < displays; ++d) {
			const int modes = SDL_GetNumDisplayModes(d);
			for (int m = 0; m < modes; ++m) {
				SDL_DisplayMode mode = {SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};
				if (SDL_GetDisplayMode(d, m, &mode) == 0) {
					res->insert(Resolution(mode.w, mode.h));
				}
			}
		}

		// Add in selection UI widget.
		Texture* textPrev = new Texture();
		textPrev->loadFromFile(renderer, config->locateResource(*(game->getSelectPrevious())), btnH, btnH);
		Texture* textNext = new Texture();
		textNext->loadFromFile(renderer, config->locateResource(*(game->getSelectNext())), btnH, btnH);
		resolutionControl = new UISelection<Resolution>(textPrev, textNext, res, renderer, fontLabel,
		        game->getTitleFontColour(), menuA, menuNA);
		resolutionControl->setXY(bw * 17, bh * 5);
		resolutionControl->setSelection(Resolution(dw, dh));
		resolutionControl->addEventListener(this);
		widgets->push_back(resolutionControl);

		// Fullscreen checkbox.
		Texture* textActive = new Texture();
		textActive->loadFromFile(renderer, config->locateResource(*(game->getCheckboxActive())), btnH, btnH);
		Texture* textInactive = new Texture();
		textInactive->loadFromFile(renderer, config->locateResource(*(game->getCheckboxInactive())), btnH, btnH);
		fullscreenControl = new UICheckBox(textActive, textInactive, menuA, menuNA);
		fullscreenControl->setXY(bw * 17, bh * 6);
		fullscreenControl->setActive(config->isFullscreen());
		fullscreenControl->addEventListener(this);
		widgets->push_back(fullscreenControl);

		// Effects Volume
		Texture* textMinus = new Texture();
		textMinus->loadFromFile(renderer, config->locateResource(*(game->getSelectPrevious())), btnH, btnH);
		Texture* textPlus = new Texture();
		textPlus->loadFromFile(renderer, config->locateResource(*(game->getSelectNext())), btnH, btnH);
		Texture* textSlider = new Texture();
		textSlider->loadFromFile(renderer, config->locateResource(*(game->getSliderImage())), bh * 4, btnH);
		volumeEffectControl = new UISlider(textMinus, textPlus, textSlider, 0, MIX_MAX_VOLUME, MIX_MAX_VOLUME / 16,
		                                   menuA, menuNA);
		volumeEffectControl->setXY(bw * 17, bh * 8);
		volumeEffectControl->setValue(config->getSoundEffectVolume());
		volumeEffectControl->addEventListener(this);
		widgets->push_back(volumeEffectControl);

		// Music Volume
		textMinus = new Texture();
		textMinus->loadFromFile(renderer, config->locateResource(*(game->getSelectPrevious())), btnH, btnH);
		textPlus = new Texture();
		textPlus->loadFromFile(renderer, config->locateResource(*(game->getSelectNext())), btnH, btnH);
		textSlider = new Texture();
		textSlider->loadFromFile(renderer, config->locateResource(*(game->getSliderImage())), bh * 4, btnH);
		volumeMusicControl = new UISlider(textMinus, textPlus, textSlider, 0, MIX_MAX_VOLUME, MIX_MAX_VOLUME / 16,
		                                  menuA, menuNA);
		volumeMusicControl->setXY(bw * 17, bh * 9);
		volumeMusicControl->setValue(config->getMusicVolume());
		volumeMusicControl->addEventListener(this);
		widgets->push_back(volumeMusicControl);

		// Apply button
		Texture* text = new Texture();
		text->loadFromText(renderer, _("Apply"), font, game->getFontColour());
		btnApply = new UIButton(text, (dw / 2) - text->getWidth() - (bh / 2), bh * (MENU_ROWS - 3), menuA, menuNA);
		btnApply->addEventListener(this);
		widgets->push_back(btnApply);

		// Return button
		text = new Texture();
		text->loadFromText(renderer, _("Return"), font, game->getFontColour());
		btnReturn = new UIButton(text, (dw / 2) + (bh / 2), bh * (MENU_ROWS - 3), menuA, menuNA);
		btnReturn->addEventListener(this);
		widgets->push_back(btnReturn);

		// Free the font.
		TTF_CloseFont(font);
		TTF_CloseFont(fontTitle);
		TTF_CloseFont(fontLabel);
	}
	runstate = CONTINUE;
	return true;
}

void OptionsMenu::newLabel(const std::string& text, TTF_Font *font, int x, int y) {
	const int dh = config->getDisplayHeight();
	const int dw = config->getDisplayWidth();
	const int bh = dh / MENU_ROWS;
	const int bw = dw / MENU_COLUMNS;

	IUIWidget* widget = new UILabel(text, font, game->getTitleFontColour(), renderer);
	widget->setXY(bw * x, bh * y);
	widgets->push_back(widget);
}

void OptionsMenu::destroy() {
	deleteWidgets();
}

void OptionsMenu::event(IUIWidget *widget) {
	// Return handler
	if (widget == (IUIWidget*) btnReturn) {
		// Exit, but ignore
		runstate = EXIT_TRUE;
		// reset music
		Mix_VolumeMusic(config->getMusicVolume());
		Mix_Volume(-1, config->getSoundEffectVolume());
		return;
	}
	// Apply Handler
	if (widget == (IUIWidget*) btnApply) {
		Resolution res = resolutionControl->getSelection();
		config->setDisplayWidth(res.getWidth());
		config->setDisplayHeight(res.getHeight());
		config->setFullscreen(fullscreenControl->isActive());
		config->setSoundEffectVolume(volumeEffectControl->getValue());
		config->setMusicVolume(volumeMusicControl->getValue());
		// Update the display !
		if (display->update(config)) {
			// display changed, we need to deactivate main menu to rebuild...
			mainMenu->Reactivate();
		}
		// Exit
		runstate = EXIT_TRUE;
		return;
	}
	if (widget == (IUIWidget*) volumeMusicControl) {
		Mix_VolumeMusic(volumeMusicControl->getValue());
		return;
	}
	if (widget == (IUIWidget*) volumeEffectControl) {
		Mix_Volume(-1, volumeEffectControl->getValue());
		return;
	}
}

