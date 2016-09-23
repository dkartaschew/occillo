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

#include "main.h"
#include "Texture.h"
#include <vector>

#ifndef IUIWIDGET_H
#define IUIWIDGET_H

class IUIEventListener;

/**
 * @brief General UI WIdget event types
 */
enum EventType {
    SET_FOCUS,
    LOSE_FOCUS,
    CURSOR_ACTIVATE,
    CURSOR_RELEASE,
    KEYUP,
    KEYDOWN,
    TEXT_INPUT,
    CURSOR_MOVEMENT,
    NOP
};

/**
 * @brief General UI WIdget event definition
 */
struct Event {
    /**
     * Create a new event
     * @param event The type of event
     * @param x The x co-ordinate that the event occurred.
     * @param y The y co-ordinate that the event occurred.
     */
    Event(EventType event, int32_t x, int32_t y) : event(event), x(x), y(y){};
    /**
     * Create a new event.
     * The event will have x/y set to 0.
     * @param event The type of event
     */
    Event(EventType event) : event(event), x(0), y(0){};
    /**
     * Create a new event, with default parameters.
     */
    Event() : event(NOP), x(0), y(0){};
    
    /**
     * The event type
     */
    EventType event;
    /**
     * The x co-ordinate that the event occurred.
     */
    int32_t x;
    /**
     * The y co-ordinate that the event occurred.
     */
    int32_t y;
};

/**
 * @brief General UI WIdget base class.
 */
class IUIWidget {
public:

    virtual ~IUIWidget() {
    };

    /**
     * Handle the most recent event
     * @param e The event
     * @param data The event data.
     */
    virtual void event(Event e, void* data) = 0;
    /**
     * Get the x co-ordinate of the widget;
     * @return The x co-ordinate of the widget.
     */
    virtual int getX() = 0;
    /**
     * Get the y co-ordinate of the widget;
     * @return The y co-ordinate of the widget.
     */
    virtual int getY() = 0;
    /**
     * Get the width of the widget;
     * @return The width of the widget.
     */
    virtual int getWidth() = 0;
    /**
     * Get the height of the widget;
     * @return The height of the widget.
     */
    virtual int getHeight() = 0;
    /**
     * Set the x co-ordinate of the widget;
     * @param x The x co-ordinate of the widget.
     */
    virtual void setX(int x) = 0;
    /**
     * Set the y co-ordinate of the widget;
     * @param y The y co-ordinate of the widget.
     */
    virtual void setY(int y) = 0;
    /**
     * Set the x, y co-ordinate of the widget;
     * @param x The x co-ordinate of the widget.
     * @param y The y co-ordinate of the widget.
     */
    virtual void setXY(int x, int y) = 0;
    /**
     * Render the UI element.
     * @param renderer The SDL renderer.
     */
    virtual void render(SDL_Renderer* renderer) = 0;
    /**
     * Add an event listener.
     * @param listener The listener to notify when an event occurs
     */
    virtual void addEventListener(IUIEventListener* listener) = 0;
    /**
     * Does the current widget have focus
     * @return TRUE if the widget has focus.
     */
    virtual bool hasFocus() = 0;
    /**
     * The widget can be focused for input/actions
     * @return TRUE if the widget can obtain focus.
     */
    virtual bool canFocus() = 0;
    /**
     * Is this is a composite widget?
     * @return  TRUE if a composite widget
     */
    virtual bool isComposite() = 0;
    /**
     * Get a vector of all children
     * @return  A pointer to a vector of children, or nullptr if no children.
     */
    virtual std::vector<IUIWidget*>* getChildren() = 0;

    /**
     * Determine if the event is over the widget
     * @param event The event
     * @param widget The widget
     * @return TRUE if the event overlaps the widget.
     */
    static bool overlap(Event event, IUIWidget* widget) {
        if (event.x < widget->getX() || event.x > (widget->getX() + widget->getWidth())) {
            return false;
        }
        if (event.y < widget->getY() || event.y > (widget->getY() + widget->getHeight())) {
            return false;
        }
        return true;
    }
};

/**
 * @brief Event listener interface.
 */
class IUIEventListener {
public:

    virtual ~IUIEventListener() {
    };

    /**
     * Handle the event from the given widget
     * @param widget The widget that generated the event.
     */
    virtual void event(IUIWidget* widget) = 0;

};
#endif  /* _IUIWIDGET_H */
