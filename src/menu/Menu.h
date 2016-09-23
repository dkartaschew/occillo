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


#ifndef _MENU_H
#define _MENU_H

#include "config.h"

#include <cstdlib>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// main.h includes the gettext, sdl, etc items
#include "main.h"
#include "IAppState.h"
#include "TextureRepository.h"
#include "Configuration.h"
#include "GameConfiguration.h"
#include "IUIWidget.h"
#include "UIButton.h"
#include "UICursor.h"
#include "UILabel.h"
#include "UIImage.h"
#include "UICheckBox.h"
#include "UISlider.h"
#include "Resolution.h"
#include "UISelection.h"

enum RUNSTATE {
    CONTINUE, EXIT_TRUE, EXIT_FALSE
};

#define MENU_WIDGET_PADDING 20
#define MENU_ROWS 20
#define MENU_COLUMNS 32

/**
 * @brief Abstract Menu class to allow implementation of menu screens
 */
class Menu : public IAppState, public IUIEventListener {
public:
    /**
     * @brief Construct a menu screen
     *
     * @param config The application/engine configuration
     * @param game The game configuration
     * @param display The current display configuration.
     */
    Menu(Configuration* config, GameConfiguration* game, Display* display);
    ~Menu();
    /**
     * @brief Destroy the internal state of the menu
     */
    virtual void destroy();
    bool Run();
    void Deactivate();
    /**
     * @brief Set the reactivate flag to true to have the internal state destoryed and reinitialised.
     */
    void Reactivate();
    /**
     * @brief Get the UI widget for the given index
     *
     * @param index The index of the item to get
     *
     * @return The UI widget, or NULL if the index is invalid.
     */
    IUIWidget* getWidget(unsigned int index);
    /**
     * Get the cursor widget
     * @return The cursor
     */
    UICursor* getCursor();
    /**
     * Get the background widget
     * @return The background
     */
    IUIWidget* getBackground();

protected:

    /**
     * @brief The application/engine configuration
     */
    Configuration* config;
    /**
     * @brief The game configuration
     */
    GameConfiguration* game;
    /**
     * @brief The current display instance
     */
    Display* display;
    /**
     * @brief The current SDL renderer.
     */
    SDL_Renderer* renderer;
    /**
     * @brief The vector of widgets for this menu.
     */
    std::vector<IUIWidget *>* widgets;
    /**
     * @brief The vector of control widgets.
     */
    std::vector<IUIWidget *> controls;
    /**
     * @brief The cursor widget
     */
    UICursor * cursor;
    /**
     * @brief The background widget
     */
    IUIWidget * background;
    /**
     * @brief The runstate for the menu.
     */
    RUNSTATE runstate;
    /**
     * @brief Flag to indicate if the internal state is invalid and requires to be reactived.
     */
    bool reactivateRequired;
    /**
    * @brief The time elapsed since the last frame.
    */
    uint32_t timeElapsed;
    /**
     * The number of pixels to move per second.
     */
    uint32_t pxPerSec;
    /**
     * Process the event
     * @param event The base event
     * @param widgets The collection of widgets
     * @param e The original SDL event.
     */
    void processEvent(Event event, std::vector<IUIWidget *>* widgets, SDL_Event* e);
    /**
     * Process the keyboard event, either change focused item or mutate to cursor activation.
     * @param event The base event
     * @param widgets The collection of widgets
     * @param e The original SDL event.
     * @return TRUE if handled.
     */
    bool handleKeyEvent(Event event, std::vector<IUIWidget *>* widgets, SDL_Event* e);
    /**
     * Delete all widget instances.
     */
    void deleteWidgets();
    /**
     * Get the focused widget (DFS)
     * @param widgets The collection of widgets
     * @return The current widget that has focus or nullptr is none have focus.
     */
    IUIWidget * getFocused(std::vector<IUIWidget *>* widgets);
    /**
     * Generate a list of controls from the collection widgets
     * @param controls The vector to add controls to
     * @param widgets The collection of widgets to scan through.
     */
    void getControls(std::vector<IUIWidget *>* controls, std::vector<IUIWidget *>* widgets);
    /**
     * Determine the type of event
     * @param e The SDL Event 
     * @return The type of event.
     */
    Event determineEvent(SDL_Event* e);
};

#endif /* MAINMENU_H */
