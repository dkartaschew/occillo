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

#ifndef MAIN_H
#define MAIN_H

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <glib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stack>

#include <locale.h>
#include "gettext.h"
#define _(string) gettext (string)

#include "Display.h"
#include "Configuration.h"
#include "IAppState.h"

#define UNUSED(x) (void)x

#ifdef  __cplusplus
extern "C" {
#endif

#define KILOBYTE 1024
#define MEGABYTE 1048576
#define GIGABYTE 1073741824

#ifdef  __cplusplus
}
#endif

/**
 * Application State stack.
 */
std::stack<IAppState*>* getApplicationState();

#endif  /* MAIN_H */
