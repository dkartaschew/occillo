# Occillo 1.0.0

Yet another implementation of the classic breakout game.

Released under the GPLv3.0.

Some contributed 3rd party art/sound/music asset under CC-BY.

See COPYING and AUTHORS for full license details.

## Features

* 20 Levels.
* Mouse, keyboard or gamepad control.
* SVG graphics for hi-dpi displays.
* Rocking soundtrack...

## Requirements

* GNU/Linux, Apple OS X 10.10+, Microsoft Windows 7+, Solaris 11/OpenIndiana
* Intel Atom N570 or better.
* 2GB RAM.
* 50MB HDD space.
* 640x480 32bit colour display.

## Build requirements.

* C++11 compiler (gcc, Solaris Studio, clang)
* SDL 2.0
* SDL_image 2.0
* SDL_ttf 2.0
* SDL_mixer 2.0
* librsvg
  * Cairo
  * Cairo-SVG
  * glib
* cppunit (tests only)

Occillo builds on Windows (mingw only), Linux, MacOS X (XCode/clang), Solaris 11 and FreeBSD.

### Build from source code

Please ensure all build requirements are available.

```
$ ./configure
$ make
# make install
```

Then run

```
$ occillo
```

to start the application. Additionally, a menu item should be available under Games.

See INSTALL for further details for each compatible platform.
