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

#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <string>
#include <iostream>
#include <fstream>

/**
* @brief A single High Score entry.
*/
class HighScore {
  public:
	/**
	* @brief Create an empty HighScore entry
	*/
	HighScore();
	/**
	* @brief Create a high score entry
	*
	* @param name The players name
	* @param score The players score
	* @param level The level reached
	* @param datetime The timestamp of the entry
	*/
	HighScore(const std::string& name, long score, int level, long datetime);
	/**
	* @brief Copy a high score entry
	*
	* @param orig The entry to copy
	*/
	HighScore(const HighScore& orig);

	virtual ~HighScore();
	/**
	* @brief Get the players name
	*
	* @return The players name.
	*/
	std::string* getName() const;
	/**
	* @brief Get the players score
	*
	* @return The players score
	*/
	long getScore() const;
	/**
	* @brief Get the level the player reached/
	*
	* @return  The level the player reached
	*/
	int getLevel() const;
	/**
	* @brief Get the timestamp of the score entry
	*
	* @return The timestamp of the entry
	*/
	long getDateTime() const;

	/**
	* @brief Serialise the highscore to ostream
	*
	* @param out The output stream to serialise to.
	* @param hiscore The highscore.
	*
	* @return The ostream.
	*/
	friend std::ostream& operator<<(std::ostream& out, const HighScore& hiscore);
	/**
	* @brief Deserialise the highscore from the input stream
	*
	* @param in The input stream
	* @param hiscore The highscore to poopulate
	*
	* @return The input stream.
	*/
	friend std::istream& operator>>(std::istream& in, HighScore& hiscore);

	/**
	* @brief Oerator overload for comparison of high scores.
	*
	* @param rhs The right hand side.
	*
	* @return TRUE is score is above 'rhs', or if equal, the oldest timestamp.
	*/
	bool operator<(const HighScore &rhs) const {
		if (score != rhs.score) {
			return score > rhs.score;
		} else {
			return datetime < rhs.datetime;
		}
	}

  private:
	/**
	* @brief The players name.
	*/
	std::string* name;
	/**
	* @brief The players score.
	*/
	long score;
	/**
	* @brief The level the player reached.
	*/
	int level;
	/**
	* @brief The timestamp reached.
	*/
	long datetime;
};

#endif  /* HIGHSCORE_H */

