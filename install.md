---
layout: default
title: Occillo Installation
navigation_weight: 2
navigation_menu: Installation
---

# Installation

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