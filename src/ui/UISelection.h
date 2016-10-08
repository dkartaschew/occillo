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

#include <string>
#include <set>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "UIButton.h"
#include "TextureRepository.h"
#include "Menu.h"

#ifndef UI_UISELECTION_H
#define UI_UISELECTION_H

#define UISELECTION_WIDGET_PADDING 10

/**
 * @brief A selection UI Widget, that allows a user to select a item from the collection of objects.
 *
 * @tparam R The type of object that the collection hosts.
 */
template <typename R>
class UISelection : public UIWidget, public IUIEventListener {
  public:

	/**
	 * @brief Create a new selection widget
	 *
	 * @param textPrev The texture to use for the prev item button
	 * @param textNext The texture to use for the next item button
	 * @param collection The collection of items to be selected from
	 * @param renderer The SDL renderer
	 * @param font The font to use for the items as displayed
	 * @param colour The font colour to use for rendering text
	 * @param focusAlpha The alpha to apply when in focus.
	 * @param nonFocusAlpha The alpha to apply when not in focus.
	 */
	UISelection(Texture* textPrev, Texture* textNext, std::set<R>* collection, SDL_Renderer* renderer, TTF_Font* font, SDL_Color* colour,
	            Uint8 focusAlpha = OCCILLO_DEFAULT_MENUFOCUSALPHA, Uint8 nonFocusAlpha = OCCILLO_DEFAULT_MENUNONFOCUSALPHA) {
		listener = nullptr;
		texture = nullptr;
		this->collection = collection;
		value = 0;
		selectionWidth = 0;
		int index = 0;
		textures = new TextureRepository(collection->size());
		for (typename std::set<R>::iterator it = collection->begin(); it != collection->end(); ++it) {
			R r = *it;
			Texture* text = new Texture();
			if (!text->loadFromText(renderer, *(r.toString()), font, colour)) {
				g_info("%s[%d] : Failed to create element texture, setting as missing texture.", __FILE__, __LINE__);
				text->loadFromColour(renderer, Texture::getColour(), 16, 16);
			}
			selectionWidth = selectionWidth < text->getWidth() ? text->getWidth() : selectionWidth;
			textures->add(index++, text);
		}
		buttons.push_back(new UIButton(textPrev, 0, 0, focusAlpha, nonFocusAlpha));
		buttons[0]->addEventListener(this);
		buttons.push_back(new UIButton(textNext, 0, 0, focusAlpha, nonFocusAlpha));
		buttons[1]->addEventListener(this);
	}

	~UISelection() {
		if (buttons.size() > 0 && buttons[0] != nullptr) {
			delete buttons[0];
			buttons[0] = nullptr;
		}
		if (buttons.size() > 1 && buttons[1] != nullptr) {
			delete buttons[1];
			buttons[1] = nullptr;
		}
		if (collection != nullptr) {
			delete collection;
			collection = nullptr;
		}
		if (textures != nullptr) {
			delete textures;
			textures = nullptr;
		}
		// texture will be done in the super::~
	}

	void event(Event e, void* data) {
		buttons[0]->event(e, data);
		buttons[1]->event(e, data);
	}

	void render(SDL_Renderer* renderer) {
		buttons[0]->render(renderer);
		buttons[1]->render(renderer);
		int rx = x + buttons[0]->getWidth() + UISELECTION_WIDGET_PADDING;
		rx = rx + ((selectionWidth - textures->get(value)->getWidth()) / 2);
		textures->get(value)->render(renderer, rx, y);
	}

	int getWidth() {
		return selectionWidth + buttons[1]->getWidth() + buttons[0]->getWidth() + UISELECTION_WIDGET_PADDING + UISELECTION_WIDGET_PADDING;
	}

	int getHeight() {
		const int btn1 = buttons[1]->getHeight();
		const int btn2 = buttons[0]->getHeight();
		return btn1 > btn2 ? btn1 : btn2;
	}

	void setX(int x) {
		this->x = x;
		buttons[0]->setX(x);
		buttons[1]->setX(x + buttons[0]->getWidth() + UISELECTION_WIDGET_PADDING + selectionWidth + UISELECTION_WIDGET_PADDING);
	}

	void setY(int y) {
		this->y = y;
		buttons[0]->setY(y);
		buttons[1]->setY(y);
	}

	/**
	 * @brief Set the object that is the current selected item
	 *
	 * @param value The item to be marked as the currently selected item.
	 */
	void setSelection(R value) {
		int i = 0;
		for (typename std::set<R>::iterator it = collection->begin(); it != collection->end(); ++it) {
			R r = *it;
			if (value == r) {
				setIndex(i);
				return;
			}
			i++;
		}
	}

	/**
	 * @brief Set the item (by index) to be the selected item.
	 *
	 * @param index The index of the item to be selected.
	 */
	void setIndex(int index) {
		if ((index >= 0) && (index < (int) collection->size())) {
			value = index;
		}
	}

	/**
	 * @brief Get the current selected item
	 *
	 * @return The current selected item.
	 */
	R getSelection() {
		int i = 0;
		for (typename std::set<R>::iterator it = collection->begin(); it != collection->end(); ++it) {
			if (i++ == value) {
				return *it;
			}
		}
		R r;
		return r;
	}

	void event(IUIWidget* widget) {
		if (widget == buttons[0]) {
			// decrement,
			setIndex(value - 1);
			if (listener != nullptr) {
				listener->event(this);
			}
			return;
		}
		if (widget == buttons[1]) {
			// increment,
			setIndex(value + 1);
			if (listener != nullptr) {
				listener->event(this);
			}
			return;
		}
	}

	bool isComposite() {
		return true;
	}

	std::vector<IUIWidget*>* getChildren() {
		return &buttons;
	}

  private:
	/**
	 * @brief The index of the current selected item
	 */
	int value;
	/**
	 * @brief The width of the widget
	 */
	int selectionWidth;
	/**
	 * @brief The texture repository to host all the textures rendered for all items in the collection
	 */
	TextureRepository* textures;
	/**
	 * @brief The collection of items that can be selected from.
	 */
	std::set<R>* collection;
	/**
	* @brief The underlying buttons
	*/
	std::vector<IUIWidget*> buttons;
};

#endif /* UICHECKBOX_H */
