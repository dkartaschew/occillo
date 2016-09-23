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

#include "Menu.h"

Menu::Menu(Configuration* config, GameConfiguration* game, Display* display) :
config(config), game(game), display(display) {
    renderer = this->display->getRenderer();
    widgets = nullptr;
    background = nullptr;
    cursor = nullptr;
    runstate = CONTINUE;
    reactivateRequired = false;
    timeElapsed = SDL_GetTicks();
    pxPerSec = config->getDisplayWidth() * 2;
}

Menu::~Menu() {

}

bool Menu::Run() {
    timeElapsed = SDL_GetTicks();

    //Main loop flag
    runstate = CONTINUE;

    controls.clear();
    // Get our control widgets
    getControls(&controls, widgets);
    // reset focus states.
    unsigned int sz = controls.size();
    Event event = Event(LOSE_FOCUS);
    for (unsigned int i = 0; i < sz; i++) {
        controls[i]->event(event, nullptr);
    }
    // Set focused widget
    if (sz > 0) {
        event.event = SET_FOCUS;
        controls[0]->event(event, nullptr);
    }

    //Event handler
    SDL_Event e;
    int numWidgets = widgets->size();
    //While application is running
    while (runstate == CONTINUE) {

        //Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            //User requests quit
            if (e.type == SDL_QUIT) {
                runstate = EXIT_TRUE;
                break;
            }
            // screenshot
            if (e.type == SDL_KEYDOWN) {
                if ((e.key.repeat == 0) && (e.key.keysym.sym == SDLK_F12)) {
                    display->screenshot();
                }
            }
            // Process Event
            processEvent(determineEvent(&e), widgets, &e);
        }
        // Handle cursor velocity based movement
        uint32_t time = SDL_GetTicks();
        if (cursor->move((time - timeElapsed) / 1000.0f)) {
            processEvent(cursor->locationEvent(), widgets, nullptr);
        }
        timeElapsed = time;

        //Clear screen
        display->clear();

        // Render all UI Widget elements.
        background->render(renderer);
        for (int i = 0; i < numWidgets; i++) {
            (*widgets)[i]->render(renderer);
        }
        cursor->render(renderer);
        display->render();
    }
    return runstate == EXIT_TRUE;
}

void Menu::Deactivate() {
    destroy();
}

void Menu::Reactivate() {
    reactivateRequired = true;
}

void Menu::destroy() {
    SDL_ShowCursor(SDL_ENABLE);
    deleteWidgets();
    if (cursor != nullptr) {
        delete cursor;
        cursor = nullptr;
    }
    if (background != nullptr) {
        delete background;
        background = nullptr;
    }
}

void Menu::deleteWidgets() {
    if (widgets != nullptr) {
        int numWidgets = widgets->size();
        for (int i = 0; i < numWidgets; i++) {
            if ((*widgets)[i] != nullptr) {
                delete (*widgets)[i];
            }
        }
        delete widgets;
        widgets = nullptr;
    }
}

IUIWidget* Menu::getWidget(unsigned int index) {
    if (index < widgets->size()) {
        return (*widgets)[index];
    }
    return nullptr;
}

UICursor* Menu::getCursor() {
    return cursor;
}

IUIWidget* Menu::getBackground() {
    return background;
}

Event Menu::determineEvent(SDL_Event* e) {
    Event event;
    int dz = config->getControllerDeadZone();
    switch (e->type) {
        case SDL_MOUSEMOTION:
            //Get mouse position
            SDL_GetMouseState(&event.x, &event.y);
            event.event = CURSOR_MOVEMENT;
            break;

        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&event.x, &event.y);
            event.event = CURSOR_ACTIVATE;
            break;

        case SDL_MOUSEBUTTONUP:
            SDL_GetMouseState(&event.x, &event.y);
            event.event = CURSOR_RELEASE;
            break;

        case SDL_CONTROLLERAXISMOTION:
            // we get axis values, so determine amount of displacement.

            //X axis motion
            if (e->caxis.axis == 0) { //Left of dead zone
                if (e->caxis.value < -dz || e->caxis.value > dz) {
                    float delta = (float) e->caxis.value / 32768.0f;
                    cursor->setXVelocity((float) pxPerSec * delta);
                } else {
                    cursor->setXVelocity(0.0f);
                }
            } else if (e->caxis.axis == 1) { //
                if (e->caxis.value < -dz || e->caxis.value > dz) {
                    float delta = (float) e->caxis.value / 32768.0f;
                    cursor->setYVelocity((float) pxPerSec * delta);
                } else {
                    cursor->setYVelocity(0.0f);
                }
            }
            break;

        case SDL_CONTROLLERBUTTONDOWN:
            event.event = CURSOR_ACTIVATE;
            break;

        case SDL_CONTROLLERBUTTONUP:
            event.event = CURSOR_RELEASE;
            break;

        case SDL_KEYDOWN:
            event.event = KEYDOWN;
            break;

        case SDL_KEYUP:
            event.event = KEYUP;
            break;

        case SDL_TEXTINPUT:
            event.event = TEXT_INPUT;
            break;
    }
		//g_info("%s[%d] : Generate : %d %d %d", __FILE__, __LINE__, event.event, event.x, event.y);

    return event;
}

void Menu::processEvent(Event event, std::vector<IUIWidget *>* widgets, SDL_Event* e) {
    unsigned int i = 0;
    unsigned int sz = widgets->size();
		Event ev = event;
    //g_info("%s[%d] : Event : %d %d %d", __FILE__, __LINE__, ev.event, ev.x, ev.y);

    cursor->event(event, e);
    IUIWidget * widget = nullptr;
    switch (event.event) {
        case CURSOR_MOVEMENT:
            for (i = 0; i < sz; i++) {
                widget = (*widgets)[i];
                if (widget->isComposite()) {
                    processEvent(ev, widget->getChildren(), e);
                } else if (IUIWidget::overlap(event, widget) && widget->canFocus() && !widget->hasFocus()) {
                    Event nEvent = Event(SET_FOCUS, event.x, event.y);
                    widget->event(nEvent, e);
                } else if (!IUIWidget::overlap(event, widget) && widget->hasFocus() && widget->canFocus()) {
                    Event nEvent = Event(LOSE_FOCUS, event.x, event.y);
                    widget->event(nEvent, e);
                }
            }
            break;
        case CURSOR_ACTIVATE:
        case CURSOR_RELEASE:
        case TEXT_INPUT:
            widget = getFocused(widgets);
            if (widget != nullptr) {
                widget->event(event, e);
            }
            break;
        case KEYUP:
        case KEYDOWN:
            // See what key, if up/down then move the focused element.
            //If action translate into CURSOR_RELEASE/CURSOR_ACTIVATE;
            if (!handleKeyEvent(event, widgets, e)) {
                widget = getFocused(widgets);
                if (widget != nullptr) {
                    widget->event(event, e);
                }
            }
        default:
            break;
    }
}

bool Menu::handleKeyEvent(Event event, std::vector<IUIWidget *>* widgets, SDL_Event* e) {
    if (e->key.repeat != 0) {
        // ignore,
        return true;
    }
    // Handle activation
    if (e->key.keysym.sym == SDLK_KP_ENTER
            || e->key.keysym.sym == SDLK_RETURN
            || e->key.keysym.sym == SDLK_RETURN2
            || e->key.keysym.sym == SDLK_SPACE) {
        Event nEvent;
        nEvent.event = event.event == KEYUP ? CURSOR_RELEASE : CURSOR_ACTIVATE;
        nEvent.x = event.x;
        nEvent.y = event.y;
        processEvent(nEvent, widgets, e); // send new event back into the processEvent stage.
    }

    // If the event is a key down, ignore for movement
    if (event.event == KEYDOWN) {
        return false;
    }
    /*
     * Find current focused widget or first focused widget
     */
    IUIWidget* focusedWidget = getFocused(widgets);
    if (focusedWidget == nullptr) {
        if (controls.size() > 0) {
            focusedWidget = controls[0];
        }
    }
    if (focusedWidget == nullptr) {
        return true;
    }
    /*
     * Handle up/down keys
     */
    if (e->key.keysym.sym == SDLK_DOWN
            || e->key.keysym.sym == SDLK_UP
            || e->key.keysym.sym == SDLK_LEFT
            || e->key.keysym.sym == SDLK_RIGHT) {

        bool next = (e->key.keysym.sym == SDLK_DOWN) || (e->key.keysym.sym == SDLK_RIGHT);

        unsigned int index = 0;
        unsigned int sz = controls.size();
        for (unsigned int i = 0; i < sz; i++) {
            if (focusedWidget == controls[i]) {
                index = i;
                break;
            }
        }
        IUIWidget* nextWidget = nullptr;
        if (next) {
            // get the next widget
            if (index + 1 == sz) {
                // The last widget is current, so wrap.
                nextWidget = controls[0];
            } else {
                nextWidget = controls[index + 1];
            }
        } else {
            // get the prior widget
            if (index == 0) {
                // first widget current, so wrap.
                nextWidget = controls[sz - 1];
            } else {
                nextWidget = controls[index - 1];
            }
        }

        // let our old widget know it's lost focus.
        Event nEvent = Event(LOSE_FOCUS, event.x, event.y);
        focusedWidget->event(nEvent, e);
        // And let the new one it has focus
        if (nextWidget != nullptr) {
            nEvent.event = SET_FOCUS;
            nextWidget->event(nEvent, e);
        }
    }
    return true;
}

IUIWidget * Menu::getFocused(std::vector<IUIWidget *>* widgets) {
    if (widgets == nullptr) {
        return nullptr;
    }
    unsigned int sz = widgets->size();
    for (unsigned int i = 0; i < sz; i++) {
        IUIWidget * w = (*widgets)[i];
        if (w->isComposite()) {
            IUIWidget *nw = getFocused(w->getChildren());
            if (nw != nullptr) {
                return nw;
            }
        } else if (w->hasFocus()) {
            return w;
        }
    }
    return nullptr;
}

void Menu::getControls(std::vector<IUIWidget *>* controls, std::vector<IUIWidget *>* widgets) {
    if (widgets == nullptr) {
        return;
    }
    unsigned int sz = widgets->size();
    for (unsigned int i = 0; i < sz; i++) {
        IUIWidget * w = (*widgets)[i];
        if (w->isComposite()) {
            getControls(controls, w->getChildren());
        } else if (w->canFocus()) {
            controls->push_back(w);
        }
    }
}
