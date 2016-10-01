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

#include "config.h"

// main.h includes the gettext, sdl, etc items
#include "main.h"
#include "IAppState.h"
#include "Texture.h"
#include "Configuration.h"
#include "Display.h"
#include "TextureRepository.h"
#include "Level.h"
#include "MainMenu.h"

/*
 * Private method definitions.
 */

/**
 * Main application initialisation function<P>
 * Reads the application configuration, sets up the display, and
 * creates the initial menu application state.
 * @return TRUE if initialized correctly
 */
bool MainInit();
/**
 * @brief Main application exit/cleanup code.
 */
void MainExit();

/**
 * file name for the configuration
 */
std::string configFile;
/**
 * Application configuration
 */
Configuration* config = nullptr;
/**
 * Application Display Manager object.
 */
Display* display = nullptr;
/**
 * The game configuration
 */
GameConfiguration* game = nullptr;
/**
 * The application state stack.
 */
std::stack<IAppState*>* applicationState = nullptr;

// ****************************************************************************



/**
 * Main Function
 * @return exit code
 */
int main(void) {
	bool res = MainInit();
	// Start main outer loop
	if (res) {
		// Keep running whilst we have an application state.
		while (!applicationState->empty()) {
			// Get the current application state, activate and run it. If it's complete, deactivate it, and
			// pop the application state stack.
			IAppState* state = applicationState->top();
			if (state->Activate()) {
				if (state->Run()) {
					state->Deactivate();
					applicationState->pop();
					delete state;
				}
			} else {
				// State failed to activate...
				applicationState->pop();
				delete state;
			}
		}
	}

	//Free resources and close SDL
	MainExit();
	if (res) {
		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}
} // end main()

#ifdef _WIN32

#include <Windows.h>

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
) {
	return main();
}

#endif

/**
 * Application State Stack.
 */
std::stack<IAppState*>* getApplicationState() {
	return applicationState;
}

/**
 * Main application initialisation function<P>
 * Reads the application configuration, sets up the display, and
 * creates the initial menu application state.
 * @return TRUE if initialized correctly
 */
bool MainInit() {
#ifdef ENABLE_NLS
	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE_NAME, LOCALEDIR);
	bind_textdomain_codeset(PACKAGE_NAME, "UTF-8");
	textdomain(PACKAGE_NAME);
#endif
	g_info("%s[%d] : %s", __FILE__, __LINE__, PACKAGE_NAME " v" PACKAGE_VERSION);

	// rand..
	std::srand(std::time(NULL));

	config = new Configuration();
	display = new Display();

	configFile = PACKAGE_NAME".ini";
	g_info("%s[%d] : %s %s", __FILE__, __LINE__, _("Configuration File "), configFile.c_str());
	config->load(configFile);
	g_info("%s[%d] : %s %s", __FILE__, __LINE__, _("Data directory is "), (config->getDataPath()->c_str()));
	g_info("%s[%d] : %s %s", __FILE__, __LINE__, _("Game definition file is "), (config->getGameConfiguration()->c_str()));

	// Get the game configuration
	std::string gameConfig = config->locateResource(*(config->getGameConfiguration()));
	if (gameConfig.empty()) {
		g_warning("%s[%d] : %s", __FILE__, __LINE__, _("Unable to locate game configuration, exiting application"));
		return false;
	}
	game = new GameConfiguration();
	game->load(gameConfig);


	//Start up SDL and create window
	if (!display->init(config, game)) {
		g_warning("%s[%d] : %s", __FILE__, __LINE__, _("Failed to initialize, exiting application"));
		return false;
	}

	//Set textdomain if set.
	std::string* td = game->getTextDomain();
	if ((td != nullptr) && (!td->empty())) {
	  bindtextdomain(td->c_str(), LOCALEDIR);
		bind_textdomain_codeset(td->c_str(), "UTF-8");
		textdomain(td->c_str());
	}

	// Setup the initial application state stack.
	applicationState = new std::stack<IAppState*>();

	// Add in the Menu Application State.
	IAppState* state = new MainMenu(config, game, display);
	applicationState->push(state);

	/* Note, the menu will create the Game Application State on demand and
	 will place it on the top of the application state stack as needed. */
	return true;
}

/**
 * Free application resources;
 */
void MainExit() {
	display->close();
	config->store(configFile);
	if (applicationState != nullptr) {
		while (!applicationState->empty()) {
			IAppState* state = applicationState->top();
			applicationState->pop();
			if (state != nullptr) {
				state->Deactivate();
				delete state;
			}
		}
	}
	if (game != nullptr) {
		delete game;
	}
	delete config;
	delete display;
	if (applicationState != nullptr) {
		delete applicationState;
	}
}

