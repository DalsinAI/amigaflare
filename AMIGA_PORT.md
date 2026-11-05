Flare for AmigaOS 3.x
=====================

Flare 1.15 (Free/Libre Action Roleplaying Engine,
https://github.com/flareteam/flare-engine, GPL 3 or later) with its game,
Empyrean Campaign (https://github.com/flareteam/flare-game, the art and
data CC BY-SA 3.0), for AmigaOS 3.x on a 68040 or 68060 with an FPU and an
RTG screen, on OpenGPU's SDL 2.

Base: tag v1.15 of both repositories (engine
9c0e784ce8b0c8c41cca8c5b31c6cf6845269908, game
2ef474f5f5f368628bc526f9e56f936dac743e49). Branch team2/flare-amiga.

Status: built, not yet run in the lab.


What you need
-------------

- A 68040 or 68060 with an FPU, AmigaOS 3.x, an RTG card (or the
  AmigaChrome lab).
- OpenGPU 0.6 or later: opengpu.library and LIBS:OpenGPU/SDL2.module,
  SDL2_image.module, SDL2_mixer.module and SDL2_ttf.module (installed by
  OpenUp).
- About 620 MB of disk (the game's art and music).


How to play
-----------

Open the Flare drawer and double-click Flare, or from a Shell:

    cd Flare
    Flare

The settings are kept in config/ and the saved games in userdata/, both
in the drawer (made the first time).


Building
--------

GFOpenAmigaGCC 16.2.0-open4 or later, the OpenGPU developer kit, and a
checkout of flare-game at the same tag:

    make -f amiga/Makefile.amigaos -j2 dist \
         STOVE=/path/to/openamigagcc-16.2.0-open4 \
         NDK=/path/to/OpenNDK/Include_H SDL2KIT=/path/to/opengpu-sdk \
         GAME=/path/to/flare-game

The defaults are CradleLab's paths. Output: build/amigaos/Flare (stripped;
Flare.debug beside it keeps the symbols); 'dist' makes dist/Flare/ and
dist/Flare.info. Making the drawer needs Python 3 (amiga/tools/png_rgba.py).


What was changed
----------------

- amiga/Makefile.amigaos: CMake's sources, flags and definitions (C++98,
  no exceptions); links the kit's SDL2, SDL2_image, SDL2_mixer and
  SDL2_ttf.
- src/PlatformAmiga.cpp (included from src/main.cpp, as each platform's
  file is): the data, settings and saves are named from the drawer's full
  name ("Work:Flare/"), never "PROGDIR:/" (the parent on AmigaDOS).
- amiga/amiga_start.c: main() first moves into the drawer and gives the
  drawer's name; at exit, the screens SDL2.module 0.6 can leave after a
  full screen are closed (amiga/amiga_screens.c). amiga/stack.c: 512 KB
  of stack.
- The game's 8-bit RGB PNGs are stored as RGBA in the drawer
  (amiga/tools/png_rgba.py): OpenGPU's SDL2_image.module 0.6 damages the
  host's heap decoding some of them.


Licences
--------

The engine: GPL 3 or later (COPYING.txt in the drawer). The game's art,
music and data: CC BY-SA 3.0 (LICENSE-data.txt), with every author in
CREDITS-data.txt. The amiga/ files are the AmiCradle Team's, GPL 3 or
later.
