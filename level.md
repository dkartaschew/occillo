---
layout: default
title: Occillo Level Design and Customisation
navigation_weight: 3
navigation_menu: Customisation
---

# Occillo Level Design and Customisation.

Occillo is primarily data driven in design and can be modified via configuration.

There are two main configuration files, in addition to the actual level files.
These are `${HOME}/.config/occillo/occillo.ini` and `${INSTALL}/game/game.ini`.
The first file is for user configuration items, eg. player name, audio levels, etc. There
will be an entry in this file that points to the game configuration file to be used. (This
will typically be the game.ini file that comes with Occillo).

To use an alternate game configuration, simply modify the occillo.ini file to point to the
game configuration file you wish to use.

## Game Customisation

This section primarily deals with creating your own Occillo based game.

The `${INSTALL}/game/game.ini` defines most of the parameters to be used by the game engine.
These include game name, backgrounds used on menus, colours, fonts, etc. 

This file uses a typical **key**=*value* setup found in most common INI file configurations.

Most of the entries are self explanatory, however the following entries do require additional
information:

* `numberBricksHigh` and `numberBricksWide` define the size of the play area in the number of 
bricks high and wide the play area (including the border bricks) is made up of. These values 
**must** match the brick data found in the level files. These two values also play a part in
determining the size of certain game elements, particularly the menu setup and the size of the paddle, etc.
* Time values are in milliseconds unless the key name defines a unit of measure.
* Colour values are RRGGBB values converted to integers. eg White, 255, 255, 255 is defined as 0xFFFFFF, 
which is converted to 16777215.
* Items that are textures should use either SVG or PNG. When multiple filenames are defined, these
will form an animated texture displayed at 10 frames per second. (The 10fps value is currently hardcoded).
* Ratio values are based on a single brick height or brick width as nominated.
* Sound files can be of any type as supported by the SDL and SDL_mixer installations.

## Level Customisation

All level files are simple text files of the following format:

* Line 1 = Level Name. Enclose in " to have the level name translated.
* Line 2 = Number of points on hit of a destructable brick.
* Line 3 = Probabilities on bonuses being applied for bricks. There are 7 values to be defined:
  * 100 - *x*, probability the brick will have a double score bonus.
  * 100 - *x*, probability the brick will have a triple score bonus
  * 100 - *x*, probability the brick will have a four times score bonus.
  * 100 - *x*, probability the brick will have a tens times score bonus.
  * 100 - *x*, probability the brick will have a hundred times score bonus.
  * 100 - *x*, probability the brick will have a super destruction action. (destroy bricks immediately around it).
  * 100 - *x*, probability the brick will have a extra life bonus.
* Line 4 = Initial speed of the ball ratio, based on 2/3 of the display height travelled per second.
* Line 5 = Background Music
* Line 6 = Sound to play on a ball hitting a brick.
* Line 7 = Sound to play on a ball hiiting the paddle.
* Line 8 = Sound to play on losing a ball/life.
* Line 9 = Number of textures. There must be at least 2 textures, with the first being the background image,
and entries 2+ are for bricks.
* Line 10 - 10+n = Texture filenames.
* Line 10+n+1 = Brick data.

### Brick Data Format.



