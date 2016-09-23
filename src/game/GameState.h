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

#ifndef GAME_GAMESTATE_H
#define GAME_GAMESTATE_H

/**
* @brief Game run state enumerations
*/
enum GAME_RUNSTATE {
	/**
	* @brief The game is a run state, or game continue state during the run of a level.
	*/
	GAME_CONTINUE,
	/**
	* @brief The game has now ended, and the EndGame will be displayed.
	*/
	GAME_END,
	/**
	* @brief The current level is now complete, and the next level should be loaded.
	*/
	NEXT_LEVEL
};

/**
* @brief Class to hold the current game state, including the scores, lives, level number, run state, etc.
*/
class GameState {
  public:
	/**
	* @brief Create a new Game State instance. Defaults are 0 score, 0 lives, level 0.
	*/
	GameState();
	virtual ~GameState();
	/**
	* @brief Increment the score by the given amount
	*
	* @param delta The amount to increment the score.
	*/
	void addScore(int delta);
	/**
	* @brief Get the current score.
	*
	* @return The current score.
	*/
	int getScore();
	/**
	* @brief Increment the level number by 1 and return the new level number.
	*
	* @return The new level number.
	*/
	int incLevel();
	/**
	* @brief Get the current level number
	*
	* @return The current level number.
	*/
	int getLevel();
	/**
	* @brief Set the run state of the game
	*
	* @param state The new run state of the game.
	*/
	void setState(GAME_RUNSTATE state);
	/**
	* @brief Get the current run state of the game
	*
	* @return The current run state.
	*/
	GAME_RUNSTATE getState();
	/**
	* @brief Get the number of lives left for the player
	*
	* @return The number of lives for the player.
	*/
	int getLives();
	/**
	* @brief Add a number of lives to the internal state.
	*
	* @param delta The number of lives to add (positive) or remove (negative).
	*/
	void addLives(int delta);
	/**
	* @brief Get the number of lives awarded to the player
	*
	* @return The number of lives awarded
	*/
	int getBonusLivesAwarded();
	/**
	* @brief Add a number of lives awarded 
	*
	* @param delta The number of lives awarded to add (positive) or remove (negative).
	*/
	void addBonusLivesAwarded(int delta);

  private:
	/**
	* @brief The current score.
	*/
	int score;
	/**
	* @brief The current number of lives.
	*/
	int lives;
	/**
	* @brief The current level number.
	*/
	int level;
	/**
	* @brief The current runstate.
	*/
	GAME_RUNSTATE state;
	/**
	* The number of bonus live awarded.
	*/
	int bonusLifeAwarded;
};

#endif  /* GAME_GAMESTATE_H */

