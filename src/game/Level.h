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

#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include "config.h"

#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <sstream>

#include "Configuration.h"
#include "GameConfiguration.h"
#include "GameState.h"
#include "Display.h"
#include "TextureRepository.h"
#include "Brick.h"
#include "Paddle.h"
#include "Ball.h"
#include "GameAnimation.h"
#include "TextureFadeAnimation.h"
#include "BrickDestructionAnimation.h"

#include "IAppState.h"

/**
 * @brief Class that represents the Level.
 */
class Level : public IAppState {
  public:
	/**
	 * Initialise a Level with the given configuration and display.
	 * @param config The application configuration.
	 * @param game The game configuration.
	 * @param display The SDL Display used to assist in rendering to display.
	 * @param state The game state.
	 * @param path The location of the file that defines the level.
	 */
	Level(Configuration* config, GameConfiguration* game, Display* display, GameState* state, const std::string& path);
	virtual ~Level();

	bool Run();
	bool Activate();
	void Deactivate();

  private:
	/**
	 * @brief The application/engine configuration
	 */
	Configuration* config;
	/**
	 * @brief The game configuration.
	 */
	GameConfiguration* gameConfig;
	/**
	 * @brief The current display instance.
	 */
	Display* display;
	/**
	 * @brief The current game state. (score, lives, etc).
	 */
	GameState* gameState;
	/**
	 * @brief The filename of the level definition.
	 */
	std::string levelPath;
	/**
	 * @brief The texture repository for the game elements.
	 */
	TextureRepository* textures;
	/**
	 * @brief The number of textures used.
	 */
  int numTextures;

	/**
	* @brief The number of bricks wide the play area is.
	*/
	const int bricksWidth;

	/**
	* @brief The number of bricks high the play area is.
	*/
	const int bricksHigh;
	/**
	 * @brief The instance of the background music.
	 */
	Mix_Music* backgroundMusic;
	/**
	 * @brief the sound instance to play when the ball collides with a brick.
	 */
	Mix_Chunk* brickHitSound;
	/**
	 * @brief the sound instance to play when the ball collides with the paddle.
	 */
	Mix_Chunk* paddleHitSound;
	/**
	 * @brief the sound instance to play when the player loses a ball.
	 */
	Mix_Chunk* ballLossSound;
	/**
	 * @brief The SDL renderer instance.
	 */
	SDL_Renderer* renderer;
	/**
	 * @brief The name of the level.
	 */
	std::string levelName;
	/**
	 * @brief The initial speed of the ball.
	 */
	double initialSpeed;
	/**
	 * @brief the base value for all bricks.
	 */
	int baseValue;
	/**
	 * @brief The basic probability that a brick will have a bonus
	 */
	int bonusProbability;
	/**
	 * The bonus probabilities
	 */
	int bonusProbabilities[6];
	/**
	 * @brief The array of all bricks in the level.
	 */
	Brick** bricks;
	/**
	 * @brief The paddle instance.
	 */
	Paddle* paddle;
	/**
	 * @brief The ball instance
	 */
	Ball* ball;
	/**
	 * @brief The font to use to render the score and lives texture.
	 */
	TTF_Font* fontScore;
	/**
	 * @brief The font to use to render the level name and game over settings.
	 */
	TTF_Font* fontTitle;
	/**
	 * @brief The font to use to render the game bonus elements.
	 */
	TTF_Font* fontBonus;
	/**
	 * @brief The texture that shows the score of the player.
	 */
	Texture* scoreTexture;
	/**
	 * @brief The texture that shows the number of lives the player has.
	 */
	Texture* livesTexture;
	/**
	 * @brief The value that the scoreTexture denotes.
	 */
	int lastScore;
	/**
	 * @brief The value that the livesTexture denotes.
	 */
	int lastLives;
	/**
	 * @brief Flag to indicate if the ball is in free movement.
	 */
	bool ballInPlay;
	/**
	 * Flag to indicate that the ball should be bumped 10'.
	 */
	bool bumpBall;
	/**
	 * @brief Flag to indicate if we have finished displaying the level name.
	 */
	bool doneEndGameAnimation;
	/**
	 * @brief The time elapsed since the last frame.
	 */
	uint32_t timeElapsed;

	std::vector<GameAnimation*> animations;
	/**
	 * Process the line as a row of bricks.
	 * @param i The row id
	 * @param line The line
	 * @return TRUE if the line processed correctly.
	 */
	bool processLine(int i, const std::string& line);
	/**
	 * @brief Cleanup all resources for the level
	 */
	void cleanup();
	/**
	 * @brief Update the internal state of the level. (Move ball, check collisions, etc).
	 */
	void updateState();
	/**
	 * Load the level from the given file.
	 * @param path The location of the file.
	 * @return TRUE if the level loaded correctly.
	 */
	bool loadFromFile(const std::string& path);
	/**
	 * Render the level.
	 */
	void render();
	/**
	 * Process the current sdl event
	 * @param event the event to process;
	 * @return the new run state;
	 */
	void event(SDL_Event& event);
	/**
	 * @brief Create a new texture from the given str, value and font
	 *
	 * @param str The original string
	 * @param value The value to insert in the string
	 * @param font The font to use.
	 *
	 * @return A new texture pointer with the texture.
	 */
	Texture* createStringTexture(const std::string& str, const int value, TTF_Font* font);

	/**
	 * @brief Create a new bonus animation sequence
	 * @param str The text to display
	 * @param brick The brick to place the value over
	 * @param animationTime The time for animation.
	 */
	void createBonusAnimation(const std::string& str, Brick* brick, uint32_t animationTime);

	/**
	 * @brief Create a new bonus animation sequence
	 * @param str The text to display
	 * @param x The x co-ordinate to display
	     * @param y The y co-ordinate to display
	 * @param animationTime The time for animation.
	 */
	void createBonusAnimation(const std::string& str, int x, int y, uint32_t animationTime);

	/**
	 *
	 * @param activeBricks
	 * @param ctime
	 * @param elapsed
	     * @param cBrickX
	     * @param cBrickY
	 * @return
	 */
	Brick* findCollision(int &activeBricks, double &ctime, double elapsed, int &cBrickX, int &cBrickY);

	/**
	 * Get the bonus to apply
	 * @return The bonus to apply.
	 */
	int getBonus();

	/**
	 * Perform the animation/state changes on the given brick for a collision event.
	 *
	 * @param brick the brick that collided.
	 */
	void handleCollision(Brick* brick);
	/**
	 * Get the number of active bricks in the game of play.
	 * @return The number of destructable bricks.
	 */
	int getActiveBricks();
};

#endif  /* LEVEL_H */

