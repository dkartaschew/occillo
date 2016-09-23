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

#include "UIWidget.h"

UIWidget::~UIWidget() {
    if (texture != nullptr) {
        delete texture;
        texture = nullptr;
    }
}

int UIWidget::getX() {
    return x;
}

int UIWidget::getY() {
    return y;
}

int UIWidget::getWidth() {
    if (texture == nullptr) {
        return 0;
    }
    return texture->getWidth();
}

int UIWidget::getHeight() {
    if (texture == nullptr) {
        return 0;
    }
    return texture->getHeight();
}

void UIWidget::setX(int x) {
    this->x = x;
}

void UIWidget::setY(int y) {
    this->y = y;
}

void UIWidget::setXY(int x, int y) {
    setX(x);
    setY(y);
}

void UIWidget::addEventListener(IUIEventListener* listener) {
    this->listener = listener;
}

bool UIWidget::hasFocus() {
    return focus;
}

bool UIWidget::canFocus(){
    return true;
}

bool UIWidget::isComposite(){
    return false;
}

std::vector<IUIWidget*>* UIWidget::getChildren(){
    return nullptr;
}