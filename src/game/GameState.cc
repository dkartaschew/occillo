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

#include "GameState.h"

GameState::GameState() :
	score(0), lives(0), level(0), state(GAME_CONTINUE), bonusLifeAwarded(0) {
}

GameState::~GameState() {
	// NOP
}

void GameState::addScore(int delta) {
	score += delta;
}

int GameState::getScore() {
	return score;
}

int GameState::incLevel() {
	level++;
	return level;
}

int GameState::getLevel() {
	return level;
}

void GameState::setState(GAME_RUNSTATE state) {
	this->state = state;
}

GAME_RUNSTATE GameState::getState() {
	return state;
}

void GameState::addLives(int delta) {
	lives += delta;
}

int GameState::getLives() {
	return lives;
}

void GameState::addBonusLivesAwarded(int delta) {
	bonusLifeAwarded += delta;
}

int GameState::getBonusLivesAwarded() {
	return bonusLifeAwarded;
}


