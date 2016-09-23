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

#include "GameConfiguration.h"

GameConfiguration::GameConfiguration() {
	background = nullptr;
	font = nullptr;
	fontbold = nullptr;
	cursor = nullptr;
	hiscorefile = nullptr;
	titlecolour = 0;
	fontcolour = 0;
	levels = nullptr;
	tcol = nullptr;
	fcol = nullptr;
	ball = nullptr;
	paddle = nullptr;
	lives = nullptr;
	titlemusic = nullptr;
	checkboxactive = nullptr;
	checkboxinactive = nullptr;
	selectprevious = nullptr;
	selectnext = nullptr;
	sliderImage = nullptr;
	highScoreTable = nullptr;
	levels = new std::vector<std::string>();
	initiallives = 3;
	credits = nullptr;
	applicationIcon = nullptr;

	levelNameFadeTime = 5000;
	gameOverFadeTime = 4000;
	brickHitTime = 500;
	brickDestroyTime = 1500;
	bonusScoreFadeTime = 1000;
	bonusLifeFadeTime = 2000;

	numberBricksHigh = 20;
	numberBricksWide = 17;

	defaultStartSpeed = 2.0 / 3.0;
	ballSizeRatio = 3.0 / 4.0;
	titleSizeRatio = 3.0 / 2.0;
	fontSizeRatio = 8.0 / 10.0;

	paddleWidthRatio = 2.0;
	paddleHeightRatio = 1.0;

	brickHitSpeedIncreaseRatio = 3.0;
	awardBonusLife = -1;
}

GameConfiguration::~GameConfiguration() {
	delete background;
	delete font;
	delete fontbold;
	delete cursor;
	delete hiscorefile;
	delete selectnext;
	delete selectprevious;
	delete sliderImage;
	delete ball;
	delete paddle;
	delete lives;
	delete titlemusic;
	delete checkboxactive;
	delete checkboxinactive;
	delete levels;
	if (fcol != nullptr) {
		free(fcol);
	}
	if (tcol != nullptr) {
		free(tcol);
	}
	delete credits;
	delete applicationIcon;
	delete highScoreTable;
}

bool GameConfiguration::load(const std::string& file) {
	GKeyFile *keyfile;
	GKeyFileFlags flags = G_KEY_FILE_NONE;
	GError *error = nullptr;

	keyfile = g_key_file_new();

	g_info("%s[%d] : Looking for game config file %s", __FILE__, __LINE__, file.c_str());
	if (!g_key_file_load_from_file(keyfile, file.c_str(), flags, &error)) {
		g_info("%s[%d] : game config file error: %s", __FILE__, __LINE__, error->message);
		g_key_file_free(keyfile);
		g_error_free(error);
		return false;
	} else {
		g_info("%s[%d] : game config file loaded.", __FILE__, __LINE__);
		try {
			setField(keyfile, background, OCCILLO_GAMECONFIGURATION_SETTING_BACKGROUND);
			setField(keyfile, font, OCCILLO_GAMECONFIGURATION_SETTING_FONT);
			setField(keyfile, fontbold, OCCILLO_GAMECONFIGURATION_SETTING_FONTBOLD);
			setField(keyfile, cursor, OCCILLO_GAMECONFIGURATION_SETTING_CURSOR);
			setField(keyfile, checkboxactive, OCCILLO_GAMECONFIGURATION_SETTING_CHECKBOXACTIVE);
			setField(keyfile, checkboxinactive, OCCILLO_GAMECONFIGURATION_SETTING_CHECKBOXINACTIVE);
			setField(keyfile, selectnext, OCCILLO_GAMECONFIGURATION_SETTING_SELECTNEXT);
			setField(keyfile, selectprevious, OCCILLO_GAMECONFIGURATION_SETTING_SELECTPREVIOUS);
			setField(keyfile, sliderImage, OCCILLO_GAMECONFIGURATION_SETTING_SLIDERIMAGE);
			setField(keyfile, ball, OCCILLO_GAMECONFIGURATION_SETTING_BALL);
			setField(keyfile, paddle, OCCILLO_GAMECONFIGURATION_SETTING_PADDLE);
			setField(keyfile, lives, OCCILLO_GAMECONFIGURATION_SETTING_LIVES);
			setField(keyfile, titlemusic, OCCILLO_GAMECONFIGURATION_SETTING_TITLEMUSIC);
			setField(keyfile, credits, OCCILLO_GAMECONFIGURATION_SETTING_CREDITS);
			setField(keyfile, applicationIcon, OCCILLO_GAMECONFIGURATION_SETTING_GAME_ICON);
			gchar* tmp = (g_key_file_get_string(keyfile, PACKAGE_NAME, OCCILLO_GAMECONFIGURATION_SETTING_HISCOREFILE, nullptr));
			hiscorefile = locateBasePath();
			hiscorefile->append(std::string(tmp));
			g_free(tmp);

			setField(keyfile, titlecolour, OCCILLO_GAMECONFIGURATION_SETTING_TITLECOLOUR);
			setField(keyfile, fontcolour, OCCILLO_GAMECONFIGURATION_SETTING_FONTCOLOUR);
			setField(keyfile, initiallives, OCCILLO_GAMECONFIGURATION_SETTING_INITIAL_LIVES);

			setField(keyfile, levelNameFadeTime, OCCILLO_GAMECONFIGURATION_SETTING_LEVEL_FADE);
			setField(keyfile, gameOverFadeTime, OCCILLO_GAMECONFIGURATION_SETTING_GAMEOVER_FADE);
			setField(keyfile, brickHitTime, OCCILLO_GAMECONFIGURATION_SETTING_BRICK_HIT);
			setField(keyfile, brickDestroyTime, OCCILLO_GAMECONFIGURATION_SETTING_BRICK_DESTROY);
			setField(keyfile, bonusScoreFadeTime, OCCILLO_GAMECONFIGURATION_SETTING_BONUS_FADE);
			setField(keyfile, bonusLifeFadeTime, OCCILLO_GAMECONFIGURATION_SETTING_BONUS_LIFE);
			setField(keyfile, numberBricksHigh, OCCILLO_GAMECONFIGURATION_SETTING_BRICKS_HIGH);
			setField(keyfile, numberBricksWide, OCCILLO_GAMECONFIGURATION_SETTING_BRICKS_WIDE);
			setField(keyfile, defaultStartSpeed, OCCILLO_GAMECONFIGURATION_SETTING_BALL_SPEED);
			setField(keyfile, ballSizeRatio, OCCILLO_GAMECONFIGURATION_SETTING_BALL_SIZE);
			setField(keyfile, titleSizeRatio, OCCILLO_GAMECONFIGURATION_SETTING_TITLE_SIZE);
			setField(keyfile, fontSizeRatio, OCCILLO_GAMECONFIGURATION_SETTING_FONT_SIZE);
			setField(keyfile, paddleHeightRatio, OCCILLO_GAMECONFIGURATION_SETTING_PADDLE_HEIGHT);
			setField(keyfile, paddleWidthRatio, OCCILLO_GAMECONFIGURATION_SETTING_PADDLE_WIDTH);
			setField(keyfile, brickHitSpeedIncreaseRatio, OCCILLO_GAMECONFIGURATION_SETTING_BRICK_HIT_INCREASE);
			setField(keyfile, awardBonusLife, OCCILLO_GAMECONFIGURATION_SETTING_AWARD_LIFE);

			processLevels(g_key_file_get_string_list(keyfile, PACKAGE_NAME, OCCILLO_GAMECONFIGURATION_SETTING_LEVELS, nullptr, nullptr));
		} catch (...) {
			g_info("%s[%d] : game config file load failed.", __FILE__, __LINE__);
			g_key_file_free(keyfile);
			return false;
		}
	}
	g_key_file_free(keyfile);

	// Load and initialise the HighScoreTable
	highScoreTable = new HighScoreTable(OCCILLO_GAMECONFIGURATION_HIGHSCORE_ENTRIES);
	highScoreTable->loadFromFile(*hiscorefile);

	return true;
}

std::string* GameConfiguration::getBackground() {
	return background;
}

std::string* GameConfiguration::getFont() {
	return font;
}

std::string* GameConfiguration::getFontBold() {
	return fontbold;
}

std::string* GameConfiguration::getCursor() {
	return cursor;
}

std::string* GameConfiguration::getCheckboxActive() {
	return checkboxactive;
}

std::string* GameConfiguration::getCheckboxInactive() {
	return checkboxinactive;
}

std::string* GameConfiguration::getSelectNext() {
	return selectnext;
}

std::string* GameConfiguration::getSelectPrevious() {
	return selectprevious;
}

std::string* GameConfiguration::getSliderImage() {
	return sliderImage;
}

std::string* GameConfiguration::getPaddleImage() {
	return paddle;
}

std::string* GameConfiguration::getBallImage() {
	return ball;
}

std::string* GameConfiguration::getLivesImage() {
	return lives;
}

std::string* GameConfiguration::getTitleMusic() {
	return titlemusic;
}

std::string* GameConfiguration::getCreditsFilename() {
	return credits;
}

std::string* GameConfiguration::getApplicationIcon() {
	return applicationIcon;
}

HighScoreTable* GameConfiguration::getHighScoreTable() {
	return highScoreTable;
}

void GameConfiguration::storeHighScoreTable() {
	highScoreTable->storeToFile(*hiscorefile);
	g_info("%s[%d] : Storing High Score File %s", __FILE__, __LINE__, hiscorefile->c_str());
}

int GameConfiguration::getLevelNameFadeTime() {
	return levelNameFadeTime;
}

int GameConfiguration::getGameOverFadeTime() {
	return gameOverFadeTime;
}

int GameConfiguration::getBrickHitTime() {
	return brickHitTime;
}

int GameConfiguration::getBrickDestroyTime() {
	return brickDestroyTime;
}

int GameConfiguration::getBonusScoreFadeTime() {
	return bonusScoreFadeTime;
}

int GameConfiguration::getBonusLifeFadeTime() {
	return bonusLifeFadeTime;
}

int GameConfiguration::getNumberBricksHigh() {
	return numberBricksHigh;
}

int GameConfiguration::getNumberBricksWide() {
	return numberBricksWide;
}

double GameConfiguration::getDefaultStartSpeed() {
	return defaultStartSpeed;
}

double GameConfiguration::getBallSizeRatio() {
	return ballSizeRatio;
}

double GameConfiguration::getTitleSizeRatio() {
	return titleSizeRatio;
}

double GameConfiguration::getFontSizeRatio() {
	return fontSizeRatio;
}

double GameConfiguration::getPaddleWidthRatio() {
	return paddleWidthRatio;
}

double GameConfiguration::getPaddleHeightRatio() {
	return paddleHeightRatio;
}

double GameConfiguration::getBrickHitSpeedIncreaseRatio() {
	return brickHitSpeedIncreaseRatio;
}

int GameConfiguration::getAwardBonusLife() {
  return awardBonusLife;
}

std::string* GameConfiguration::locateBasePath() {
	/*
	 * Since we have cairo, we get glib, so use it's functions to get the
	 * config dir. Note: the returned string is owned by glib.
	 */
	const gchar* baseconfig = g_get_user_config_dir();
	if (baseconfig == nullptr) {
		return nullptr;
	}
	std::string* b = new std::string(baseconfig);
	b->append("/" PACKAGE_NAME "/");
	return b;
}

SDL_Color* GameConfiguration::getFontColour() {
	if (fcol == nullptr) {
		fcol = (SDL_Color*) std::malloc(sizeof (SDL_Color));
		fcol->r = (fontcolour >> 16) & 0xff;
		fcol->g = (fontcolour >> 8) & 0xff;
		fcol->b = (fontcolour & 0xff);
		fcol->a = 0xff;
	}
	return fcol;
}

SDL_Color* GameConfiguration::getTitleFontColour() {
	if (tcol == nullptr) {
		tcol = (SDL_Color*) std::malloc(sizeof (SDL_Color));
		tcol->r = (titlecolour >> 16) & 0xff;
		tcol->g = (titlecolour >> 8) & 0xff;
		tcol->b = (titlecolour & 0xff);
		tcol->a = 0xff;
	}
	return tcol;
}

std::vector<std::string>* GameConfiguration::getLevels() {
	return levels;
}

int GameConfiguration::getInitialLives() {
	return initiallives;
}

void GameConfiguration::processLevels(gchar** levels) {
	gchar** l = levels;
	while (*levels != nullptr) {
		this->levels->push_back(std::string(*levels));
		g_info("%s[%d] : Adding Level %s", __FILE__, __LINE__, *levels);
		levels++;
	}
	// The strings passed in must be freed.
	g_strfreev(l);
}

void GameConfiguration::setField(GKeyFile *keyfile, std::string* &field, const char* key) {
	gchar* str = g_key_file_get_string(keyfile, PACKAGE_NAME, key, nullptr);
	if (str == nullptr) {
		std::string* value = new std::string("");
		field = value;
		g_info("%s[%d] : Game configuration field %s = %s", __FILE__, __LINE__, key, value->c_str());
		return;
	}
	std::string* value = new std::string(str);
	field = value;
	g_free(str);
	g_info("%s[%d] : Game configuration field %s = %s", __FILE__, __LINE__, key, value->c_str());
}

void GameConfiguration::setField(GKeyFile *keyfile, int &field, const char* key) {
	if (g_key_file_has_key(keyfile, PACKAGE_NAME, key, nullptr)) {
		field = g_key_file_get_integer(keyfile, PACKAGE_NAME, key, nullptr);
	}
	g_info("%s[%d] : Game configuration field %s = %d (0x%08x)", __FILE__, __LINE__, key, field, field);
}

void GameConfiguration::setField(GKeyFile *keyfile, double &field, const char* key) {
	if (g_key_file_has_key(keyfile, PACKAGE_NAME, key, nullptr)) {
		field = g_key_file_get_double(keyfile, PACKAGE_NAME, key, nullptr);
	}
	g_info("%s[%d] : Game configuration field %s = %f", __FILE__, __LINE__, key, field);
}

