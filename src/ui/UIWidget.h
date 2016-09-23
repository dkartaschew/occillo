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

#include "IUIWidget.h"
#include "main.h"

#ifndef _UIWIDGET_H
#define _UIWIDGET_H

/**
 * @brief Base abstract class for all UI Widgets.
 */
class UIWidget : public IUIWidget {
public:

    ~UIWidget();

    virtual void event(Event e, void* data) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;

    int getX();
    int getY();
    int getWidth();
    int getHeight();
    void setX(int x);
    void setY(int y);
    void setXY(int x, int y);
    void addEventListener(IUIEventListener* listener);
    bool hasFocus();
    bool canFocus();
    bool isComposite();
    std::vector<IUIWidget*>* getChildren();
protected:
    /**
     * @brief The base texture to display
     */
    Texture* texture;
    /**
     * @brief The x co-ordinate of the widget.
     */
    int x;
    /**
     * @brief The y co-ordinate of the widget.
     */
    int y;
    /**
     * @brief The event listener to trigger for the widget
     */
    IUIEventListener* listener;
    /**
     * Focus flag
     */
    bool focus;
};

#endif /* _UIWIDGET_H */
