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

#include "UISlider.h"

UISlider::UISlider(Texture* textPrev, Texture* textNext, Texture* textSlider, int minValue, int maxValue, int step
, Uint8 focusAlpha, Uint8 nonFocusAlpha)
: min(minValue), max(maxValue), step(step) {
    x = 0;
    y = 0;
    value = (max - min) / 2;
    listener = nullptr;
    texture = textSlider;
    buttons.push_back(new UIButton(textPrev, 0, 0, focusAlpha, nonFocusAlpha));
    buttons[0]->addEventListener(this);
    buttons.push_back(new UIButton(textNext, 0, 0, focusAlpha, nonFocusAlpha));
    buttons[1]->addEventListener(this);
    focus = false;
}

UISlider::~UISlider() {
    if (buttons.size() > 0 && buttons[0] != nullptr) {
        delete buttons[0];
        buttons[0] = nullptr;
    }
    if (buttons.size() > 1 && buttons[1] != nullptr) {
        delete buttons[1];
        buttons[1] = nullptr;
    }
    // texture will be done in the super::~
}

void UISlider::event(Event e, void* data) {
    buttons[0]->event(e, data);
    buttons[1]->event(e, data);
}

void UISlider::event(IUIWidget* widget) {
    if (widget == buttons[0]) {
        // decrement,
        value -= step;
        if (value < min) {
            value = min;
        }
        if (listener != nullptr) {
            listener->event(this);
        }
        return;
    }
    if (widget == buttons[1]) {
        // increment,
        value += step;
        if (value > max) {
            value = max;
        }
        if (listener != nullptr) {
            listener->event(this);
        }
        return;
    }
}

void UISlider::render(SDL_Renderer* renderer) {
    buttons[0]->render(renderer);
    texture->render(renderer, x + buttons[0]->getWidth() + UISLIDER_WIDGET_PADDING,
            y - texture->getHeight() / 5
            , 0, (int) ((double) texture->getWidth() * (double) value / (double) max)
            , 0, texture->getHeight());
    buttons[1]->render(renderer);
}

int UISlider::getWidth() {
    if (texture == nullptr) {
        return 0;
    }
    return texture->getWidth() + buttons[1]->getWidth() + buttons[0]->getWidth();

}

int UISlider::getHeight() {
    if (texture == nullptr) {
        return 0;
    }
    const int th = texture->getHeight();
    const int btn1 = buttons[1]->getHeight();
    const int btn2 = buttons[0]->getHeight();
    int h = th > btn1 ? th : btn1;
    h = h > btn2 ? h : btn2;
    return h;
}

void UISlider::setX(int x) {
    this->x = x;
    buttons[0]->setX(x);
    buttons[1]->setX(x + buttons[0]->getWidth() + texture->getWidth() + UISLIDER_WIDGET_PADDING + UISLIDER_WIDGET_PADDING);
}

void UISlider::setY(int y) {
    this->y = y;
    buttons[0]->setY(y);
    buttons[1]->setY(y);
}

void UISlider::setValue(int value) {
    if (value >= min && value <= max) {
        this->value = value;
    }
}

int UISlider::getValue() {
    return value;
}

bool UISlider::isComposite(){
    return true;
}

std::vector<IUIWidget*>* UISlider::getChildren(){
    return &buttons;
}

