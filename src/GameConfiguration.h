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

#ifndef GAMECONFIGURATION_H
#define GAMECONFIGURATION_H
#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <SDL.h>
#include <glib.h>
#include <cerrno>

#include "config.h"
#include "main.h"
#include "HighScoreTable.h"

#define OCCILLO_GAMECONFIGURATION_SETTING_FONT "font"
#define OCCILLO_GAMECONFIGURATION_SETTING_FONTBOLD "fontbold"
#define OCCILLO_GAMECONFIGURATION_SETTING_TITLECOLOUR "titlecolour"
#define OCCILLO_GAMECONFIGURATION_SETTING_FONTCOLOUR "fontcolour"
#define OCCILLO_GAMECONFIGURATION_SETTING_BACKGROUND "background"
#define OCCILLO_GAMECONFIGURATION_SETTING_CURSOR "cursor"
#define OCCILLO_GAMECONFIGURATION_SETTING_LEVELS "levels"
#define OCCILLO_GAMECONFIGURATION_SETTING_HISCOREFILE "hiscorefile"
#define OCCILLO_GAMECONFIGURATION_SETTING_CHECKBOXACTIVE "checkboxactive"
#define OCCILLO_GAMECONFIGURATION_SETTING_CHECKBOXINACTIVE "checkboxinactive"
#define OCCILLO_GAMECONFIGURATION_SETTING_SELECTPREVIOUS "selectprevious"
#define OCCILLO_GAMECONFIGURATION_SETTING_SELECTNEXT "selectnext"
#define OCCILLO_GAMECONFIGURATION_SETTING_SLIDERIMAGE "sliderimage"
#define OCCILLO_GAMECONFIGURATION_SETTING_PADDLE "paddle"
#define OCCILLO_GAMECONFIGURATION_SETTING_BALL "ball"
#define OCCILLO_GAMECONFIGURATION_SETTING_INITIAL_LIVES "initiallives"
#define OCCILLO_GAMECONFIGURATION_SETTING_LIVES "lives"
#define OCCILLO_GAMECONFIGURATION_SETTING_TITLEMUSIC "titlemusic"
#define OCCILLO_GAMECONFIGURATION_SETTING_CREDITS "credits"
#define OCCILLO_GAMECONFIGURATION_HIGHSCORE_ENTRIES 10
#define OCCILLO_GAMECONFIGURATION_SETTING_GAME_ICON "gameicon"
#define OCCILLO_GAMECONFIGURATION_SETTING_LEVEL_FADE "levelNameFadeTime"
#define OCCILLO_GAMECONFIGURATION_SETTING_GAMEOVER_FADE "gameOverFadeTime"
#define OCCILLO_GAMECONFIGURATION_SETTING_BRICK_HIT "brickHitTime"
#define OCCILLO_GAMECONFIGURATION_SETTING_BRICK_DESTROY "brickDestroyTime"
#define OCCILLO_GAMECONFIGURATION_SETTING_BONUS_FADE "bonusScoreFadeTime"
#define OCCILLO_GAMECONFIGURATION_SETTING_BONUS_LIFE "bonusLifeFadeTime"
#define OCCILLO_GAMECONFIGURATION_SETTING_BRICKS_HIGH "numberBricksHigh"
#define OCCILLO_GAMECONFIGURATION_SETTING_BRICKS_WIDE "numberBricksWide"
#define OCCILLO_GAMECONFIGURATION_SETTING_BALL_SPEED "defaultStartSpeed"
#define OCCILLO_GAMECONFIGURATION_SETTING_BALL_SIZE "ballSizeRatio"
#define OCCILLO_GAMECONFIGURATION_SETTING_TITLE_SIZE "titleSizeRatio"
#define OCCILLO_GAMECONFIGURATION_SETTING_FONT_SIZE "fontSizeRatio"
#define OCCILLO_GAMECONFIGURATION_SETTING_PADDLE_HEIGHT "paddleHeightRatio"
#define OCCILLO_GAMECONFIGURATION_SETTING_PADDLE_WIDTH "paddleWidthRatio"
#define OCCILLO_GAMECONFIGURATION_SETTING_BRICK_HIT_INCREASE "brickHitSpeedIncreaseRatio"
#define OCCILLO_GAMECONFIGURATION_SETTING_AWARD_LIFE "awardBonusLife"

/**
* @brief The main game configuration.
*/
class GameConfiguration {
  public:
	/**
	 * Create a game configuration with default settings
	 */
	GameConfiguration();
	/**
	 *
	 */
	virtual ~GameConfiguration();
	/**
	 * Load configuration from file
	 * @param file The file to load from
	 * @return TRUE if successful
	 */
	bool load(const std::string& file);
	/**
	 * Get the image filename to use as a background;
	 * @return A string that defines the background image.
	 */
	std::string* getBackground();
	/**
	 * Get the font to use
	 * @return The filename of the font.
	 */
	std::string* getFont();
	/**
	 * Get the bold font to use
	 * @return The filename of the font.
	 */
	std::string* getFontBold();
	/**
	 * Get the cursor to use
	 * @return The filename of the cursor.
	 */
	std::string* getCursor();
	/**
	 * Get the checkbox active image to use
	 * @return The filename of the image.
	 */
	std::string* getCheckboxActive();
	/**
	 * Get the checkbox inactive image to use
	 * @return The filename of the image.
	 */
	std::string* getCheckboxInactive();
	/**
	 * Get the select next image to use
	 * @return The filename of the image.
	 */
	std::string* getSelectNext();
	/**
	 * Get the select previous to use
	 * @return The filename of the image.
	 */
	std::string* getSelectPrevious();
	/**
	 * Get the image for the volume slider
	 */
	std::string* getSliderImage();
	/**
	 * Get the image for the paddle
	 */
	std::string* getPaddleImage();
	/**
	 * Get the image for the ball
	 */
	std::string* getBallImage();
	/**
	 * Get the image for the lives icon...
	 */
	std::string* getLivesImage();

	/**
	* @brief Get the file to play during title screens.
	*
	* @return  The filename of the music file.
	*/
	std::string* getTitleMusic();

	/**
	 * Get the name of the high score file. (no path)
	 * @return The filename of the high score file.
	 */
	HighScoreTable* getHighScoreTable();
	/**
	 * Store the high score table to disk.
	 */
	void storeHighScoreTable();
	/**
	 * Get the font colour;
	 * @return The primary font colour;
	 */
	SDL_Color* getFontColour();
	/**
	 * Get the title font colour;
	 * @return The title font colour;
	 */
	SDL_Color* getTitleFontColour();
	/**
	 * Get a vector to all the levels in the game.
	 * @return a vector to all the levels.
	 */
	std::vector<std::string>* getLevels();

	/**
	 * Get the number of lives.
	 * @return The number of lives a user should start with.
	 */
	int getInitialLives();

	/**
	* @brief Get the filename of the credits
	*
	* @return The filename of the file that contains all game credits.
	*/
	std::string* getCreditsFilename();

	/**
	* @brief Get the filename of the application icon. (PNG)
	*
	* @return The application icon.
	*/
	std::string* getApplicationIcon();

	/**
	* @brief Get the time in msec to fade the level name
	*
	* @return The fade time
	*/
	int getLevelNameFadeTime();

	/**
	* @brief Get the time in msec for the Game Over text
	*
	* @return The fade time
	*/
	int getGameOverFadeTime();

	/**
	* @brief Get the time in msec for brick hit animation time
	*
	* @return The time
	*/
	int getBrickHitTime();

	/**
	* @brief Get the time in msec for the brick destruction animation time
	*
	* @return The time
	*/
	int getBrickDestroyTime();

	/**
	* @brief Get the Bonus score fade time.
	*
	* @return The fade time
	*/
	int getBonusScoreFadeTime();

	/**
	* @brief Get the fade time for the +1 Life text animation.
	*
	* @return The fade time
	*/
	int getBonusLifeFadeTime();

	/**
	* @brief Get the number of bricks high the play field is
	*
	* @return The height of the playfield
	*/
	int getNumberBricksHigh();

	/**
	* @brief Get the number of bricks wide the play field is
	*
	* @return The width of the play field.
	*/
	int getNumberBricksWide();

	/**
	* @brief Get the start speed of the ball as a ratio of the screen width.
	* Essential the screen width multiply this value is the pixels per
	* Second the ball will travel.
	* Note: Each level also has a ratio that is applied in addition to this ratio.
	*
	* @return The ratio of screen width for the ball speed.
	*/
	double getDefaultStartSpeed();

	/**
	* @brief Get the size of the ball as a ratio of the height of a brick.
	*
	* @return The size ratio that the ball will be in relation to the size
	* of a brick.
	*/
	double getBallSizeRatio();

	/**
	* @brief Get the size of the title font as a ration of the height of a brick.
	*
	* @return The size ratio that the title font will be in relation to the height
	* of a brick.
	*/
	double getTitleSizeRatio();

	/**
	* @brief Get the size of the bonus font as a ration of the height of a brick.
	*
	* @return The size ratio that the bonus font will be in relation to the height
	* of a brick.
	*/
	double getFontSizeRatio();

	/**
	* @brief The desired width of the paddle as a ration of brick width.
	*
	* @return The desired width of the paddle
	*/
	double getPaddleWidthRatio();

	/**
	* @brief The paddle height as a ratio of brick height
	*
	* @return The desired height of the paddle
	*/
	double getPaddleHeightRatio();

	/**
	* @brief Get the ratio to increase the ball speed by on every brick destroy.
	* On every hit of a brick, the ball speed is increased by the initial speed of the ball
	* divided by the total possible number of bricks in the play area. This ratio is multiplied
	* by the default increase rate to define the actual increase in speed. 
	*
	* @return The speed increate ratio for each brick hit.
	*/
	double getBrickHitSpeedIncreaseRatio();

	/**
	* @brief Get the score value at which a bonus life is awared. A value of -1, means to not award bonus lives.
	*
	* @return The score value at which to award bonus lives.
	*/
	int getAwardBonusLife();

  private:

	std::string* background;
	std::string* font;
	std::string* fontbold;
	std::string* cursor;
	std::string* hiscorefile;
	std::string* checkboxactive;
	std::string* checkboxinactive;
	std::string* selectprevious;
	std::string* selectnext;
	std::string* sliderImage;
	std::string* paddle;
	std::string* ball;
	std::string* lives;
	std::string* titlemusic;
	std::string* credits;
	std::string* applicationIcon;
	int titlecolour;
	int fontcolour;
	int initiallives;
	SDL_Color* tcol;
	SDL_Color* fcol;
	std::vector<std::string>* levels;
	HighScoreTable* highScoreTable;
	int levelNameFadeTime;
	int gameOverFadeTime;
	int brickHitTime;
	int brickDestroyTime;
	int bonusScoreFadeTime;
	int bonusLifeFadeTime;

	int numberBricksHigh;
	int numberBricksWide;

	double defaultStartSpeed;
	double ballSizeRatio;
	double titleSizeRatio;
	double fontSizeRatio;

	double paddleHeightRatio;
	double paddleWidthRatio;

	double brickHitSpeedIncreaseRatio;

	int awardBonusLife;

	/**
	* @brief Process a list of levels
	*
	* @param levels The pointer to a list of levels.
	*/
	void processLevels(gchar** levels);
	/**
	* @brief Locate the correct base path to use for the game
	*
	* @return The base path.
	*/
	std::string* locateBasePath();

	/**
	* @brief Set the given field, using the given key from the gkeyfile
	* @param keyfile the file to query for the given key
	* @param field The field to set
	* @param key The key to use to determine the value to set the field.
	*/
	void setField(GKeyFile *keyfile, std::string* &field, const char* key);
	
	/**
	* @brief Set the given field, using the given key from the gkeyfile
	* @param keyfile the file to query for the given key
	* @param field The field to set
	* @param key The key to use to determine the value to set the field.
	*/
	void setField(GKeyFile *keyfile, int &field, const char* key);
	
	/**
	* @brief Set the given field, using the given key from the gkeyfile
	* @param keyfile the file to query for the given key
	* @param field The field to set
	* @param key The key to use to determine the value to set the field.
	*/
  void setField(GKeyFile *keyfile, double &field, const char* key);

};

#endif  /* GAMECONFIGURATION_H */

