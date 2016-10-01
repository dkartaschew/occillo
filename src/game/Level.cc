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


#include "Level.h"

Level::Level(Configuration *config, GameConfiguration* game, Display* display, GameState* state, const std::string& path) :
	config(config), gameConfig(game), display(display), gameState(state), levelPath(path),
	bricksWidth(game->getNumberBricksWide()), bricksHigh(game->getNumberBricksHigh()) {
	textures = nullptr;
	numTextures = 0;
	renderer = display->getRenderer();
	bricks = nullptr;
	paddle = nullptr;
	ball = nullptr;
	lastLives = -1;
	lastScore = -1;
	fontScore = nullptr;
	fontTitle = nullptr;
	fontBonus = nullptr;
	livesTexture = nullptr;
	scoreTexture = nullptr;
	timeElapsed = SDL_GetTicks();
	ballInPlay = false;
	backgroundMusic = nullptr;
	brickHitSound = nullptr;
	paddleHitSound = nullptr;
	ballLossSound = nullptr;
	initialSpeed = 1.0;
	doneEndGameAnimation = false;
	baseValue = 10;
	bumpBall = false;
	bonusProbability = 10;
	bonusProbabilities[0] = 40;
	bonusProbabilities[1] = 60;
	bonusProbabilities[2] = 80;
	bonusProbabilities[3] = 90;
	bonusProbabilities[4] = 94;
	bonusProbabilities[5] = 99;
}

Level::~Level() {
	cleanup();
}

void Level::cleanup() {
	g_info("%s[%d] : Level cleanup", __FILE__, __LINE__);
	if (textures != nullptr) {
		// free the texture repo.
		delete textures;
		textures = nullptr;
	}
	if (ball != nullptr) {
		delete ball;
		ball = nullptr;
	}
	if (paddle != nullptr) {
		delete paddle;
		paddle = nullptr;
	}
	if (livesTexture != nullptr) {
		delete livesTexture;
		livesTexture = nullptr;
	}
	if (scoreTexture != nullptr) {
		delete scoreTexture;
		scoreTexture = nullptr;
	}
	if (fontScore != nullptr) {
		TTF_CloseFont(fontScore);
		fontScore = nullptr;
	}
	if (fontTitle != nullptr) {
		TTF_CloseFont(fontTitle);
		fontTitle = nullptr;
	}
	if (fontBonus != nullptr) {
		TTF_CloseFont(fontBonus);
		fontBonus = nullptr;
	}
	const int n = bricksWidth * bricksHigh;
	if (bricks != nullptr) {
		for (int i = 0; i < n; i++) {
			Brick* b = bricks[i];
			if (b != nullptr) {
				delete b;
			}
			bricks[i] = nullptr;
		}
		delete[] bricks;
		bricks = nullptr;
	}
	// stop all sounds
	Mix_HaltMusic();
	Mix_HaltChannel(-1);
	if (backgroundMusic != nullptr) {
		Mix_FreeMusic(backgroundMusic);
		backgroundMusic = nullptr;
	}
	if (brickHitSound != nullptr) {
		Mix_FreeChunk(brickHitSound);
		brickHitSound = nullptr;
	}
	if (paddleHitSound != nullptr) {
		Mix_FreeChunk(paddleHitSound);
		paddleHitSound = nullptr;
	}
	if (ballLossSound != nullptr) {
		Mix_FreeChunk(ballLossSound);
		ballLossSound = nullptr;
	}
}

bool Level::Activate() {
	return loadFromFile(levelPath);
}

void Level::Deactivate() {
	cleanup();
}

bool Level::Run() {
	// Setup break ground music...
	if (backgroundMusic != nullptr) {
		if (Mix_PlayingMusic() != 0) {
			Mix_HaltMusic();
		}
		Mix_PlayMusic(backgroundMusic, -1);
	}
	// Set the music volume
	Mix_VolumeMusic(config->getMusicVolume());
	Mix_Volume(-1, config->getSoundEffectVolume());

	//Event handler
	SDL_Event e;
	//While application is running
	while (gameState->getState() == GAME_CONTINUE) {

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0) {
			//User requests quit
			if (e.type == SDL_QUIT) {
				gameState->setState(GAME_END);
			}
			// screenshot or end game.
			if (e.type == SDL_KEYDOWN) {
				if ((e.key.repeat == 0) && (e.key.keysym.sym == SDLK_F12)) {
					display->screenshot();
				} else if ((e.key.repeat == 0) && (e.key.keysym.sym == SDLK_ESCAPE)) {
					gameState->setState(GAME_END);
				}
			}
			// Process the event
			event(e);
		}

		//Clear screen and render
		display->clear();
		render();
		display->render();
	}
	return true;
}

bool Level::loadFromFile(const std::string& path) {
	cleanup();

	std::string filename = config->locateResource(path);

	g_info("%s[%d] : Loading level file %s", __FILE__, __LINE__, path.c_str());

	const int brickWidth = config->getDisplayWidth() / bricksWidth;
	const int brickHeight = config->getDisplayHeight() / bricksHigh;
	std::string line;

	std::ifstream lvl(filename.c_str());
	// ensure we have the file.
	if (!lvl.is_open()) {
		return false;
	}
	// Level name
	std::getline(lvl, levelName);
	g_info("%s[%d] : Level name: %s", __FILE__, __LINE__, levelName.c_str());
	// If the level name starts and ends with "<name>" quotes, then strip the quotes.
	int l = levelName.size();
	char& start = levelName.front();
	char& end = levelName.back();
	if (start == '"' && end == '"') {
		levelName = levelName.substr(1, l - 2);
	}

	// base value for bricks.
	std::getline(lvl, line);
	g_info("%s[%d] : Base brick hit value ratio : %s", __FILE__, __LINE__, line.c_str());
	try {
		baseValue = std::stoi(line);
	} catch (...) {
		// ignore.
		g_info("%s[%d] : Base brick hit value failed to convert", __FILE__, __LINE__);
	}

	// bonus probabilities.
	std::getline(lvl, line);
	g_info("%s[%d] : Base brick bonus probability : %s", __FILE__, __LINE__, line.c_str());
	try {
		std::istringstream iss(line);
		std::string probValue;
		iss >> probValue;
		g_info("%s[%d] : Base brick apply bonus value ratio : %s", __FILE__, __LINE__, probValue.c_str());
		bonusProbability = std::stoi(probValue);
		// do bonus probabilities.
		int count = 0;
		while (iss && (count < 6)) {
			iss >> probValue;
			try {
				bonusProbabilities[count] = std::stoi(probValue);
				g_info("%s[%d] : Base brick bonus value ratio : %d = %d", __FILE__, __LINE__, count,
				       bonusProbabilities[count]);
			} catch (...) {
				g_info("%s[%d] : Base brick bonus value failed to convert", __FILE__, __LINE__);
			}
			count++;
		}

	} catch (...) {
		// ignore.
		g_info("%s[%d] : Base brick bonus value failed to convert", __FILE__, __LINE__);
	}

	// initial speed;
	std::getline(lvl, line);
	g_info("%s[%d] : Initial Speed ratio : %s", __FILE__, __LINE__, line.c_str());
	try {
		initialSpeed = std::stod(line);
	} catch (...) {
		// ignore.
		g_info("%s[%d] : Initial Speed ratio failed to convert", __FILE__, __LINE__);
	}

	// Background Music
	std::getline(lvl, line);
	g_info("%s[%d] : Background Music : %s", __FILE__, __LINE__, line.c_str());
	if (!line.empty()) {
		line = config->locateResource(line);
		backgroundMusic = Mix_LoadMUS(line.c_str());
		if (backgroundMusic == nullptr) {
			g_info("%s[%d] Could not load background music! SDL_Mixer Error: %s", __FILE__, __LINE__, Mix_GetError());
		}
	}

	// Brick Hit
	std::getline(lvl, line);
	g_info("%s[%d] : Brick Hit Sound : %s", __FILE__, __LINE__, line.c_str());
	if (!line.empty()) {
		line = config->locateResource(line);
		brickHitSound = Mix_LoadWAV(line.c_str());
		if (brickHitSound == nullptr) {
			g_info("%s[%d] Could not load brick hit sound! SDL_Mixer Error: %s", __FILE__, __LINE__, Mix_GetError());
		}
	}

	// Paddle Hit
	std::getline(lvl, line);
	g_info("%s[%d] : Paddle Hit Sound : %s", __FILE__, __LINE__, line.c_str());
	if (!line.empty()) {
		line = config->locateResource(line);
		paddleHitSound = Mix_LoadWAV(line.c_str());
		if (paddleHitSound == nullptr) {
			g_info("%s[%d] Could not load paddle hit sound! SDL_Mixer Error: %s", __FILE__, __LINE__, Mix_GetError());
		}
	}

	// Loss of ball sound
	std::getline(lvl, line);
	g_info("%s[%d] : Ball Loss Sound : %s", __FILE__, __LINE__, line.c_str());
	if (!line.empty()) {
		line = config->locateResource(line);
		ballLossSound = Mix_LoadWAV(line.c_str());
		if (ballLossSound == nullptr) {
			g_info("%s[%d] Could not load ball loss sound! SDL_Mixer Error: %s", __FILE__, __LINE__, Mix_GetError());
		}
	}

	// Number of textures.
	std::getline(lvl, line);
	const int textureCount = atoi(line.c_str());
	if (textureCount < 2) {
		lvl.close();
		return false;
	}

	// Textures
	textures = new TextureRepository(textureCount);
	numTextures = textureCount;
	// texture 0 is background
	getline(lvl, line);
	line = config->locateResource(line);
	g_info("%s[%d] : Texture %d name: %s", __FILE__, __LINE__, 0, line.c_str());
	textures->add(0, renderer, line, config->getDisplayWidth(), config->getDisplayHeight());
	// load the textures
	for (int i = 1; i < textureCount; i++) {
		if (!std::getline(lvl, line)) {
			cleanup();
			lvl.close();
			return false;
		}
		g_info("%s[%d] : Texture %d name: %s", __FILE__, __LINE__, i, line.c_str());
		/*
		 * Determine if file exists... if not, then try to prepend config->getDataPath() to it.
		 */
		line = config->locateResource(line);
		bool res = g_file_test(line.c_str(), G_FILE_TEST_EXISTS);
		if (res) {
			textures->add(i, renderer, line, brickWidth, brickHeight);
		} else {
			g_warning("%s[%d] : Texture %d name: %s missing!", __FILE__, __LINE__, i, line.c_str());
		}
	}

	if (bricks == nullptr) {
		const int n = bricksWidth * bricksHigh;
		bricks = new Brick*[n];
		for (int i = 0; i < n; i++) {
			bricks[i] = nullptr;
		}
	}

	// Level layout. (OCCILLO_BRICK_UNIT_HEIGHT lines)
	for (int i = 0; i < bricksHigh; i++) {
		if (!std::getline(lvl, line)) {
			g_info("%s[%d] : Line %u missing?", __FILE__, __LINE__, i);
			cleanup();
			lvl.close();
			return false;
		}
		// build the line.
		g_info("%s[%d] : Line %d Row: %s", __FILE__, __LINE__, i, line.c_str());
		if (!processLine(i, line)) {
			cleanup();
			lvl.close();
			return false;
		}
	}

	lvl.close();

	/*
	 * Paddle
	 */
	g_info("%s[%d] : Load Paddle %s", __FILE__, __LINE__, gameConfig->getPaddleImage()->c_str());
	Texture* text = new Texture();
	text->loadFromFile(renderer, config->locateResource(*(gameConfig->getPaddleImage())),
	                   brickWidth * gameConfig->getPaddleWidthRatio(),
	                   brickHeight * gameConfig->getPaddleHeightRatio());

	paddle = new Paddle(text,
	                    config->getDisplayWidth() / 2 - text->getWidth() / 2,
	                    (brickHeight * bricksHigh) - text->getHeight(),
	                    config->getDisplayWidth() * 2,
	                    brickWidth,
	                    brickWidth * (bricksWidth - 1),
	                    config->getControllerDeadZone());
	/*
	 * Ball
	 */
	text = new Texture();
	text->loadFromFile(renderer, config->locateResource(*(gameConfig->getBallImage())),
	                   brickHeight * gameConfig->getBallSizeRatio(),
	                   brickHeight * gameConfig->getBallSizeRatio());
	ball = new Ball(text, brickWidth, brickHeight, (config->getDisplayHeight() * 2) / 3);

	/*
	 * In game fonts.
	 */

	fontTitle = TTF_OpenFont(config->locateResource(*(gameConfig->getFontBold())).c_str(), (brickHeight * gameConfig->getTitleSizeRatio()));
	fontScore = TTF_OpenFont(config->locateResource(*(gameConfig->getFontBold())).c_str(), (brickHeight * 6 / 10));
	fontBonus = TTF_OpenFont(config->locateResource(*(gameConfig->getFontBold())).c_str(), (brickHeight * gameConfig->getFontSizeRatio()));

	/*
	 * Level name
	 */
	text = new Texture();
	text->loadFromText(renderer, _(levelName.c_str()), fontTitle, gameConfig->getFontColour(), gameConfig->getTitleFontColour());
	text->setBlendMode(SDL_BLENDMODE_BLEND);
	int x = config->getDisplayWidth() / 2 - text->getWidth() / 2;
	int y = config->getDisplayHeight() / 2 - text->getHeight() / 2;
	animations.push_back(new TextureFadeAnimation(text, x, y, gameConfig->getLevelNameFadeTime())); // 5 secs.

	return true;
}

/**
 * Convert a single hex encoded nibble to int.
 * @param c A sing hex character
 * @return The int representation.
 */
uint8_t hextCharToInt(char c) {
	uint8_t value = (uint8_t) c;
	if (value >= 'a') {
		return (value - 'a' + 10) & 0xf;
	}
	if (value >= 'A') {
		return (value - 'A' + 10) & 0xf;
	}
	return (value - '0') & 0xf;
}

bool Level::processLine(int i, const std::string& l) {
	// If the length of the line is less that number of bricks wide, then exit.
	if (l.size() < (size_t)(bricksWidth * 4)) {
		g_info("%s[%d] : Line too short? %lu ", __FILE__, __LINE__, l.size());
		return false;
	}

	const int brickWidth = config->getDisplayWidth() / bricksWidth;
	const int brickHeight = config->getDisplayHeight() / bricksHigh;

	for (int j = 0; j < bricksWidth; j++) {
		// each 4 chars are texture index and flags;
		uint8_t ti = ((hextCharToInt(l[4 * j])) << 4) + hextCharToInt((l[(4 * j) + 1]));
		uint8_t flags = ((hextCharToInt(l[(4 * j) + 2])) << 4) + hextCharToInt(l[(4 * j) + 3]);
		if ((flags == 0) && (ti != 0)) {
			int apply = std::rand() % 101;
			if (apply <= bonusProbability) {
				flags = getBonus();
				g_info("%s[%d] : Bonus being applied to %d x %d = %d", __FILE__, __LINE__, i , j, flags);
			}
		}

		if (ti != 0 && textures->get(ti) != nullptr) {
			bricks[(i * bricksWidth) + j] = new Brick(textures->get(ti), j * brickWidth, i * brickHeight, flags, baseValue);
		} else {
			bricks[(i * bricksWidth) + j] = nullptr;
		}
	}
	return true;
}

void Level::render() {

	// Update the level state. (move paddle, move ball, brick collison, etc).
	updateState();

	//Render background texture to screen
	textures->get(0)->render(renderer, 0, 0);

	// Render bricks
	const int n = bricksWidth * bricksHigh;
	for (int i = 0; i < n; i++) {
		if (bricks[i] != nullptr) {
			bricks[i]->render(renderer);
		}
	}

	// render ball...
	ball->render(renderer);

	// render paddle;
	paddle->render(renderer);

	// render game state.
	int lives = gameState->getLives();
	int score = gameState->getScore();


	// Update the textures used for the number of player lives and current scores.
	if (lives != lastLives) {
		if (livesTexture != nullptr) {
			delete livesTexture;
		}
		livesTexture = createStringTexture(_("Lives : %d"), lives, fontScore);
		lastLives = lives;
	}
	if (score != lastScore) {
		if (scoreTexture != nullptr) {
			delete scoreTexture;
		}
		scoreTexture = createStringTexture(_("Score: %08d"), score, fontScore);
		lastScore = score;
	}

	const int bw = config->getDisplayWidth() / bricksWidth;
	const int bh = config->getDisplayHeight() / bricksHigh;

	// render lives and score
	livesTexture->render(renderer, bw / 3, bh / 10);
	scoreTexture->render(renderer,
	                     (bw * bricksWidth) - scoreTexture->getWidth() - (bw / 3),
	                     bh / 10);

	// render any animations
	for (std::vector<GameAnimation*>::iterator anItr = animations.begin(); anItr != animations.end();) {
		GameAnimation* animation = *anItr;
		if (animation->isComplete()) {
			anItr = animations.erase(anItr);
			delete animation;
		} else {
			animation->render(renderer);
			++anItr;
		}
	}

	// If out of lives and all animations finished, exit.
	if ((gameState->getLives() == 0) && animations.empty() && doneEndGameAnimation) {
		gameState->setState(GAME_END);
	}

}

void Level::updateState() {
	double elapsed = (SDL_GetTicks() - timeElapsed) / 1000.0f;
	paddle->move(elapsed);
	if (bumpBall) {
		double direction = ((ball->getDirection() / M_PI) * 180.0);
		if (direction < 0.0) {
			direction += 360.0;
		}
		direction += 10;
		if (direction > 360.0) {
			direction -= 360.0;
		}
		ball->setDirection(direction);
		bumpBall = false;
		g_info("%s[%d] : Bump ball.", __FILE__, __LINE__);
		std::string value = _("Bump!");
		createBonusAnimation(value, ball->getX(), ball->getY(), gameConfig->getBonusScoreFadeTime());

	}
	if (gameState->getLives() == 0) {
		// enable end game animation...
		if (!doneEndGameAnimation) {
			/*
			 * Game Over text
			 */
			std::string gameOver = _("Game Over");
			Texture *text = new Texture();
			text->loadFromText(renderer, gameOver, fontTitle, gameConfig->getFontColour(), gameConfig->getTitleFontColour());
			text->setBlendMode(SDL_BLENDMODE_BLEND);
			int x = config->getDisplayWidth() / 2 - text->getWidth() / 2;
			int y = config->getDisplayHeight() / 2 - text->getHeight() / 2;
			animations.push_back(new TextureFadeAnimation(text, x, y, gameConfig->getGameOverFadeTime())); // 4 secs.

			doneEndGameAnimation = true;
		}
		timeElapsed = SDL_GetTicks();
		return;
	} else if (!ballInPlay) {
		const int x = paddle->getX() + (paddle->getWidth() / 2) - (ball->getWidth() / 2);
		const int y = paddle->getY();
		ball->setXY(x, y - ball->getHeight() - 1);
		// Set initial direction...
		double direction = (double) x / (double) (config->getDisplayWidth());
		// range is between 200 and 340.
		direction = 340.0 - (direction * 160.0);
		ball->setDirection(direction);
		// set initial speed...
		double speed = (double) config->getDisplayHeight() * gameConfig->getDefaultStartSpeed();
		speed = speed * initialSpeed;
		ball->setSpeed(speed);
		if ((getActiveBricks() == 0) && animations.empty()) {
			gameState->setState(NEXT_LEVEL);
			elapsed = 0.0;
			g_info("%s[%d] : Next level!", __FILE__, __LINE__);
		}
	} else {

		// see if the ball left the field of play.
		if (ball->getY() > config->getDisplayHeight() || ball->getY() < 0
		        || ball->getX() > config->getDisplayWidth() || ball->getX() < 0) {
			ballInPlay = false;
			if (getActiveBricks() != 0) {
				gameState->addLives(-1);
				g_info("%s[%d] : Lost ball. Lives = %d ", __FILE__, __LINE__, gameState->getLives());

				if (ballLossSound != nullptr) {
					Mix_PlayChannel(-1, ballLossSound, 0);
				}
			} else {
				g_info("%s[%d] : Ignore lost ball, no active bricks", __FILE__, __LINE__);
				elapsed = 0.0;
			}
		} else {
			while (elapsed > 0.0) {
				// Determine if the ball moving will collide with something?
				double ctime = 0.0;
				int activeBricks = 0;
				int cBrickX = 0;
				int cBrickY = 0;
				Brick* cBrick = findCollision(activeBricks, ctime, elapsed, cBrickX, cBrickY);
				// If no more bricks, then set next level...
				if (activeBricks == 0 && animations.empty()) {
					gameState->setState(NEXT_LEVEL);
					elapsed = 0.0;
					g_info("%s[%d] : Next level!", __FILE__, __LINE__);

				} else if (cBrick != nullptr) {
					// We have a collision.
					if (brickHitSound != nullptr) {
						Mix_PlayChannel(-1, brickHitSound, 0);
					}
					handleCollision(cBrick);
					// Quad destroy
					if (cBrick->isQuadDestroy()) {
#if DEBUG
						g_info("%s[%d] : Quad Hit Brick Collision %d %d", __FILE__, __LINE__, cBrickY, cBrickX);
#endif
						handleCollision(bricks[(cBrickY - 1) * bricksWidth + (cBrickX - 1)]);
						handleCollision(bricks[(cBrickY - 1) * bricksWidth + (cBrickX)]);
						handleCollision(bricks[(cBrickY - 1) * bricksWidth + (cBrickX + 1)]);
						handleCollision(bricks[(cBrickY) * bricksWidth + (cBrickX - 1)]);
						handleCollision(bricks[(cBrickY) * bricksWidth + (cBrickX + 1)]);
						handleCollision(bricks[(cBrickY + 1) * bricksWidth + (cBrickX - 1)]);
						handleCollision(bricks[(cBrickY + 1) * bricksWidth + (cBrickX)]);
						handleCollision(bricks[(cBrickY + 1) * bricksWidth + (cBrickX + 1)]);
					} else if (cBrick->isAllBrickDestroy()) {
						// All Brick hit
#if DEBUG
						g_info("%s[%d] : All Hit Brick Collision %d %d", __FILE__, __LINE__, cBrickY, cBrickX);
#endif
						const int n = bricksWidth * bricksHigh;
						for (int i = 0; i != n; i++) {
							Brick* b = bricks[i];
							handleCollision(b);
						}
					}

					ball->move(elapsed * ctime);
					ball->collision(cBrick);
					elapsed = elapsed * (1.0 - ctime);
					// Increase the speed by a factor of initial speed / total possible bricks.
					if (!cBrick->isWall()) {
						ball->setSpeed(ball->getSpeed() +
						               (gameConfig->getBrickHitSpeedIncreaseRatio() *
						                (((double)config->getDisplayHeight() * gameConfig->getDefaultStartSpeed())
						                 / (double) (bricksWidth * bricksHigh))));
					}
				} else {
					// Check for paddle collision.
					ctime = ball->collision(paddle, elapsed);
					if ((ctime > 0.0) && (ctime < 1.0)) {
						// paddle collision...
						if (paddleHitSound != nullptr) {
							Mix_PlayChannel(-1, paddleHitSound, 0);
						}
#if DEBUG
						g_info("%s[%d] : Paddle Collision %f", __FILE__, __LINE__, ctime);
#endif
						ball->move(elapsed * ctime);
						ball->collision(paddle);
						elapsed = elapsed * (1.0 - ctime);
					} else if (ctime <= 0.0 || ball->overlaps(paddle)) {
						// ball overlaps the paddle.
						ball->collision(paddle);
						// paddle collision...
						if (paddleHitSound != nullptr) {
							Mix_PlayChannel(-1, paddleHitSound, 0);
						}
#if DEBUG
						g_info("%s[%d] : Paddle Overlap/Collision %f", __FILE__, __LINE__, ctime);
						if (ctime <= 0.0) {
							g_info("%s[%d] : Paddle %d x %d : %d x %d", __FILE__, __LINE__,
							       paddle->getX(), paddle->getY(), paddle->getWidth(), paddle->getHeight());
							g_info("%s[%d] : Ball %d x %d : %d x %d", __FILE__, __LINE__,
							       ball->getX(), ball->getY(), ball->getWidth(), ball->getHeight());
						}
#endif
					} else {
						// No brick collision, do full movement.
						ball->move(elapsed);
						elapsed = 0.0;
					}
				}
			}
		}
	}
	timeElapsed = SDL_GetTicks();
}

void Level::event(SDL_Event & event) {
	paddle->event(event);
	// if we pressed fire, set ball in play to true;
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (!ballInPlay) {
			ballInPlay = true;
		}
	} else if ((event.type == SDL_KEYUP) && (event.key.repeat == 0)) {
		//Adjust the velocity
		switch (event.key.keysym.sym) {
		case SDLK_SPACE:
			ballInPlay = true;
			break;
		case SDLK_F11:
			bumpBall = true;
			break;
		}
	} else if (event.type == SDL_CONTROLLERBUTTONUP) {
		if (!ballInPlay) {
			ballInPlay = true;
		}
	}
}

Texture* Level::createStringTexture(const std::string & str, const int value, TTF_Font * font) {

	char dest[100] = "";
#ifdef _WIN32
	_snprintf(dest, 100, str.c_str(), value);
#else
	std::snprintf(dest, 100, str.c_str(), value);
#endif
	std::string dstr = dest;

	Texture* text = new Texture();
	text->loadFromText(renderer, dstr, font, gameConfig->getTitleFontColour());
	text->setBlendMode(SDL_BLENDMODE_BLEND);
	return text;
}

void Level::createBonusAnimation(const std::string& str, Brick* brick, uint32_t animationTime) {
	Texture *text = new Texture();
	text->loadFromText(renderer, str, fontBonus, gameConfig->getFontColour(), gameConfig->getTitleFontColour());
	text->setBlendMode(SDL_BLENDMODE_BLEND);
	int x = brick->getX() + (brick->getWidth() / 2 - text->getWidth() / 2);
	int y = brick->getY() + (brick->getHeight() / 2 - text->getHeight() / 2);
	animations.push_back(new TextureFadeAnimation(text, x, y, animationTime, config->getDisplayHeight() / bricksHigh));
}

void Level::createBonusAnimation(const std::string& str, int x, int y, uint32_t animationTime) {
	Texture *text = new Texture();
	text->loadFromText(renderer, str, fontBonus, gameConfig->getFontColour(), gameConfig->getTitleFontColour());
	text->setBlendMode(SDL_BLENDMODE_BLEND);
	animations.push_back(new TextureFadeAnimation(text, x, y, animationTime, config->getDisplayHeight() / bricksHigh));

}

Brick* Level::findCollision(int &activeBricks, double &ctime, double elapsed, int &cBrickX, int &cBrickY) {
	double direction = ball->getDirection();
	double xDelta = std::cos(direction);
	double yDelta = std::sin(direction);
	Brick* cBrick = nullptr;
	int iStart = 0;
	int iEnd = 0;
	int iDelta = 1;
	int jStart = 0;
	int jEnd = 0;
	int jDelta = 1;
	// Determine the scan order testing against bricks.
	if (yDelta > 0) {
		// scan top to bottom;
		iEnd = bricksHigh;
	} else {
		iStart = bricksHigh - 1;
		iEnd = -1;
		iDelta = -1;
	}
	if (xDelta > 0) {
		// Scan left to right;
		jEnd = bricksWidth;
	} else {
		jStart = bricksWidth - 1;
		jEnd = -1;
		jDelta = -1;
	}

	// Count the number of destructable bricks
	double ltime = 1.0;
	for (int i = iStart; i != iEnd; i += iDelta) {
		for (int j = jStart; j != jEnd; j += jDelta) {
			Brick* b = bricks[(i * bricksWidth) + j];
			if ((b != nullptr)) {
				ctime = ball->collision(b, elapsed);
				if (ctime < 1.0) {
					// The ball on the move will collide with this brick;
					if (cBrick == nullptr) {
						// no collision so far.
						cBrick = b;
						cBrickX = j;
						cBrickY = i;
						ltime = ctime;
					} else {
						// already have a collision, check if this one is earlier!
						if (ctime < ltime) {
							cBrick = b;
							cBrickX = j;
							cBrickY = i;
							ltime = ctime;
						}
					}
#if DEBUG
					g_info("%s[%d] : Collision %f @ %d x %d", __FILE__, __LINE__, ctime, i, j);
#endif
				}
			}
			if (b != nullptr) {
				if (!b->isWall() && b->getHitCount() > 0) {
					activeBricks++;
				}
			}
		}
	}
	ctime = ltime;
	return cBrick;
}

int Level::getBonus() {
	int bonus = std::rand() % 101;
	if (bonus < bonusProbabilities[0]) {
		return OCCILLO_BRICK_FLAG_DOUBLE_SCORE ;
	}
	if (bonus < bonusProbabilities[1]) {
		return OCCILLO_BRICK_FLAG_TRIPLE_SCORE ;
	}
	if (bonus < bonusProbabilities[2]) {
		return OCCILLO_BRICK_FLAG_QUAD_SCORE ;
	}
	if (bonus < bonusProbabilities[3]) {
		return OCCILLO_BRICK_FLAG_TEN_SCORE ;
	}
	if (bonus < bonusProbabilities[4]) {
		return OCCILLO_BRICK_FLAG_HUNDRED_SCORE ;
	}
	if (bonus < bonusProbabilities[5]) {
		return OCCILLO_BRICK_FLAG_BOMB_QUAD ;
	}
	return OCCILLO_BRICK_FLAG_EXTRA_LIFE ;
}


void Level::handleCollision(Brick* brick) {

	if (brick == nullptr) {
		return;
	}
	if ((brick->getHitCount() == 0) || (brick->isWall())) {
		return;
	}

	brick->collision();
	gameState->addScore(brick->getValue());

	if (brick->getHitCount() == 0) {
		// destruction animination.
		int time = gameConfig->getBrickDestroyTime();
		if (brick->getValue() > baseValue) {
			time *= 2; // double the time if a bonus score brick
		}
		animations.push_back(new BrickDestructionAnimation(brick->getTexture(),
		                     brick->getX(), brick->getY(), time, brick->getWidth()));
	} else if (!brick->isWall()) {
		// hit animation.
		animations.push_back(new BrickDestructionAnimation(brick->getTexture(),
		                     brick->getX(), brick->getY(), gameConfig->getBrickHitTime(), brick->getHeight(), true));
	}

	// Display bonus value animations.
	if (brick->getValue() > baseValue) {
		// bonus animation.
		try {
			std::string value = std::to_string(brick->getValue());
			createBonusAnimation(value, brick, gameConfig->getBonusScoreFadeTime());
		} catch (...) {
			// ignore...
		}
	}
	// Display extra life animation.
	if (brick->isExtraLife()) {
		std::string value = _("+1 Life");
		createBonusAnimation(value, brick, gameConfig->getBonusLifeFadeTime());
		gameState->addLives(1);
	}

	// See if bonus life is to be awarded.
	int bonus = gameConfig->getAwardBonusLife();
	if (bonus != -1) {
		int score = gameState->getScore();
		int multiple = score / bonus;
		int awarded = gameState->getBonusLivesAwarded();
		if (multiple > awarded) {
			gameState->addBonusLivesAwarded(multiple - awarded);
			gameState->addLives(multiple - awarded);

			Texture *text = new Texture();
			std::string value = _("+1 Life");
			text->loadFromText(renderer, value, fontTitle, gameConfig->getFontColour(), gameConfig->getTitleFontColour());
			text->setBlendMode(SDL_BLENDMODE_BLEND);
			int x = config->getDisplayWidth() / 2 - text->getWidth() / 2;
			int y = config->getDisplayHeight() / 2 - text->getHeight() / 2;
			animations.push_back(new TextureFadeAnimation(text, x, y, gameConfig->getBonusScoreFadeTime(),
			                     config->getDisplayHeight() / bricksHigh));
		}
	}

}

int Level::getActiveBricks() {
	int activeBricks = 0;
	const int n = bricksWidth * bricksHigh;
	for (int i = 0; i != n; i++) {
		Brick* b = bricks[i];
		if (b != nullptr) {
			if (!b->isWall() && b->getHitCount() > 0) {
				activeBricks++;
			}
		}
	}
	return activeBricks;
}

