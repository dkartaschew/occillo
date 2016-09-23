---
layout: default
---

A breakout style arcade game and engine.

## Features

* 25 Levels.
* Mouse, keyboard or gamepad control.
* SVG graphics for hi-dpi displays.
* Rocking soundtrack...

## Screenshots

<div class="w3-content w3-display-container" style="max-width:400px">
  <img class="mySlides" style="width:100%" src="images/menu.png">
  <img class="mySlides" style="width:100%" src="images/game.png">
  <img class="mySlides" style="width:100%" src="images/game2.png">
  <div class="w3-center w3-display-middle" style="width:100%">
    <div class="w3-left w3-padding-left " onclick="plusDivs(-1)">&#10094;</div>
    <div class="w3-right w3-padding-right " onclick="plusDivs(1)">&#10095;</div>
  </div>
  <div class="w3-center w3-display-bottomleft" style="width:100%">
    <span class="w3-badge demo w3-border" onclick="currentDiv(1)"></span>
    <span class="w3-badge demo w3-border" onclick="currentDiv(2)"></span>
    <span class="w3-badge demo w3-border" onclick="currentDiv(0)"></span>
  </div>
</div>

<script>
showDivs(slideIndex);
</script>

## Downloads

### Binary Releases

There are currently no binary releases available. Current source code can be downloaded here: 

## Support and Discussion Boards

Support and Discussion Boards can be found on the github project page located here:

## Requirements

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

## Installation

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

By default Occillo will be installed into `/usr/local/`. To override installation location, use the `--prefix` option, and compiler selection may be made using environment variables as passed to `./configure`.

#### Build Requirements

##### Arch Linux

Should build without issues once dependencies are installed:

* sdl2 
* sdl2_image 
* sdl2_mixer 
* sdl2_ttf 
* librsvg

##### Redhat 7.x / CentOS 7.x

CentOS requires SDL2 to be installed manually, or via a 3rd party repository.

Basic dependencies: 

* sdl2 
* sdl2_image 
* sdl2_mixer 
* sdl2_ttf 
* librsvg

##### Debian 8.x / Ubuntu 14.04

Should build without issues once dependencies are installed:

Development/Build dependencies:

* libsdl2-dev
* libsdl2-image-dev
* libsdl2-mixer-dev
* libsdl2-ttf2-dev
* librsvg-dev
* autoconf-archive (required for C++11 m4 macros if rebuilding .configure).

Runtime dependencies: 

* libsdl2-2.0-0 
* libsdl2-image 
* libsdl2-mixer 
* libsdl2-ttf 
* librsvg

##### OpenSuse 13.2

Should build without issues once dependencies are installed:

Development/Build dependencies:

* gcc-g++
* libSDL2-devel
* libSDL2_image-devel
* libSDL2-mixer-devel
* libSDL2-ttf2-devel
* librsvg-devel

Runtime dependencies:
 
* libSDL2-2_0-0 
* libSDL2_image 
* libSDL2_mixer 
* libSDL2_ttf 
* librsvg

##### Mac OS X 10.7+

Requires XCode to be installed with clang, as the gcc version that ships with 
Mac OS X doesn't fully support C++11.

Can use macports/homebrew to install dependencies:

* sdl2 
* sdl2_image 
* sdl2_mixer 
* sdl2_ttf 
* librsvg

Typical configuration. 

```
$ ./configure --prefix=/opt/local CXX=clang++ CC=clang CXXFLAGS='-stdlib=libc++'
$ make
# make install
```

##### Microsoft Windows 7+

Compilation on Windows requires the use of mingw via MSYS.
This is primarily due to librsvg (and dependents) only officially supported on
Windows via MSYS.

Packaging is performed via [Advanced Installer](http://www.advancedinstaller.com/).

Install mingw, autotool, autoconf, libtool. 

Install Basic dependencies: 

* sdl2 
* sdl2_image 
* sdl2_mixer 
* sdl2_ttf 
* librsvg

Run ./configure && make to build the binaries.

Run Advanced Installer and load configuration file (pkg/occillo.aip).

You can now build the MSI installer, and install the application via the MSI.

##### Solaris 11 / OpenIndiana

Oracle Solaris 11 requires SDL2 to be installed manually, or via a 3rd party repository.
OpenIndiana Hipster includes SDL2 itself in it's main repository, but not SDL2_image, SDL2_mixer and SDL2_ttf.

Solaris Studio 12.4 is required for C++11 support.

Basic dependencies: 

* sdl2 
* sdl2_image 
* sdl2_mixer 
* sdl2_ttf 
* librsvg