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

#include "HighScore.h"

HighScore::HighScore(const std::string& name, long score, int level, long datetime) {
	this->score = score;
	this->level = level;
	this->datetime = datetime;
	this->name = new std::string(name);
}

HighScore::HighScore() {
	score = 0;
	level = 1;
	datetime = 1;
	name = nullptr;
}

HighScore::HighScore(const HighScore& orig) {
	score = orig.score;
	level = orig.level;
	datetime = orig.datetime;
	name = new std::string(*(orig.name));
}

std::istream& operator>>(std::istream& in, HighScore& hiscore) {
	std::string line;
	std::getline(in, line);
	if (hiscore.name != nullptr) {
		delete hiscore.name;
	}
	hiscore.name = new std::string(line);
	in >> hiscore.score >> hiscore.level >> hiscore.datetime;
	in.ignore(1, '\n');
	return in;
}

std::ostream& operator<<(std::ostream& out, const HighScore& hiscore) {
	out << *(hiscore.name) << std::endl;
	out << hiscore.score << " " << hiscore.level << " " << hiscore.datetime << std::endl;
	return out;
}

HighScore::~HighScore() {
	if (name != nullptr) {
		delete name;
	}
}

std::string* HighScore::getName() const {
	return name;
}

long HighScore::getScore() const {
	return score;
}

int HighScore::getLevel() const {
	return level;
}

long HighScore::getDateTime() const {
	return datetime;
}


