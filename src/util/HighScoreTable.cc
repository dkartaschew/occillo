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

#include "HighScoreTable.h"

HighScoreTable::HighScoreTable(unsigned int numberOfEntries) {
	numEntries = numberOfEntries;
	table = new std::set<HighScore>();
	// Add a set of blank entries;
	for (unsigned int i = 0; i < numEntries; i++) {
		table->insert(HighScore("Player", (i + 1) * 10, 1, i));
	}
}

HighScoreTable::~HighScoreTable() {
	delete table;
}

void HighScoreTable::addScore(const HighScore* score) {
	table->insert(*score);
	while (table->size() > numEntries) {
		std::set<HighScore>::reverse_iterator it = table->rbegin();
		table->erase(*it);
	}
}

std::set<HighScore>* HighScoreTable::getTable() {
	return table;
}

bool HighScoreTable::loadFromFile(const std::string& file) {
	std::ifstream filestream (file);
	if (filestream.is_open()) {
		HighScore score;
		table->clear();
		while (filestream >> score) {
			table->insert(score);
		}
		filestream.close();
		return true;
	}
	return false;
}

bool HighScoreTable::storeToFile(const std::string& file) {
	std::ofstream filestream (file, std::ofstream::out);
	if (filestream.is_open()) {
		for (std::set<HighScore>::iterator it = table->begin(); it != table->end(); ++it) {
			filestream << *it;
		}
		filestream.close();
		return true;
	}
	return false;
}
