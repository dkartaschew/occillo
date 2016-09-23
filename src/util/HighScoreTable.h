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

#ifndef HIGHSCORETABLE_H
#define HIGHSCORETABLE_H

#include <set>
#include <string>
#include <iostream>
#include <fstream>
#include "HighScore.h"

/**
* @brief The collection of high scores, that have been achieved by players.
*/
class HighScoreTable {
  public:
	/**
	* @brief Create a new high score table with number of entries
	*
	* @param numberOfEntries The number of entries
	*/
	HighScoreTable(unsigned int numberOfEntries);
	virtual ~HighScoreTable();
	/**
	* @brief Add a new high score to the table.
	*
	* @param score The new high score.
	*/
	void addScore(const HighScore* score);
	/**
	* @brief Get a pointer to set that contains the high score table
	*
	* @return A pointer to the high score table
	*/
	std::set<HighScore>* getTable();
	/**
	* @brief Load a high score file
	*
	* @param file The file to load
	*
	* @return TRUE if the table loaded correctly
	*/
	bool loadFromFile(const std::string& file);
	/**
	* @brief Save the high score table to the given file
	*
	* @param file The file to score the table to.
	*
	* @return TRUE if the table was store correctly.
	*/
	bool storeToFile(const std::string& file);

  private:
	/**
	* @brief The maximum number of entries to hold in the table
	*/
	unsigned int numEntries;
	/**
	* @brief The high score table
	*/
	std::set<HighScore>* table;
};

#endif  /* HIGHSCORETABLE_H */

