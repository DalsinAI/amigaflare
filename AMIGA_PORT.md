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

Status: lab-tested in a third run (below). First lab run (Instance-45, an emulated 68040 with an FPU, RTG,
5 November 2026): it opens a 640x480 screen, the first-start questions
(language, mouse movement) and the title screen draw and answer the
mouse, a new character is made, the intro plays, the first map
("Arrival") loads in about a minute and a half, the hero walks where
clicked, and Save & Exit then Exit Game save the game and return to the
Workbench (the lock file's count back at 0). Known from that run:

- Item pictures (images/loot/) did not load: "No signal bits
  available", 90 times. Flare loads pictures in threads by default,
  and each thread takes a signal bit. Threaded loading is now off by
  default on the Amiga and not offered in the configuration menu; in
  the second run there were no such errors.
- The pause menu (Esc) was laid out wider than 640 pixels: the game
  (fantasycore's engine/resolutions.txt) asks for 1280x720. OpenGPU's
  SDL 2 opens its full screens at 640x480 whatever is asked for, so on
  the Amiga Flare now opens a 1280x720 window on the Workbench by
  default (full screen can still be chosen under Configuration).
- Second run (5 November, windowed 1280x720): the title screen draws
  whole. "Play Game" with a saved game stayed on "Loading..." for over
  five minutes: SDL2.module's SDL_AtomicLock spinning (with
  SDL_Delay), the audio thread (SDL_SDLAudioP2) busy, free memory
  flat.
- Third run (5 November, the build now in the repository, fresh
  settings): a 1280x720 window by default; the first-start questions,
  the title, the saved-game screen, Load Game (about 100 seconds to the
  first map), the hero on "Arrival" with the HUD and minimap whole, the
  pause menu with all its tabs on screen, Save & Exit and Exit Game (the
  lock count back at 0). No picture failed to load. The second run's
  hang did not happen again; it came after the lab had been stopped with
  Flare running, from settings written in a 640x480 full screen. If
  Flare ever stops at "Loading...", that is the first thing to report.
- If Flare is stopped from outside (a reset, or the lab stopped),
  config/flare_lock keeps a count of 1 and Flare then quits at once at
  every start: set the number in that file back to 0.
- In a window, OpenGPU's SDL 2 reports the mouse about 16 pixels right
  and 32 down of where the pointer is; Flare's own pointer shows where
  a click will land.
- images/menus/game_over.png is asked for but is not in the game's
  data (as upstream ships it): one error in flare_log.txt, harmless.


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
- src/Settings.cpp: a 1280x720 window by default (fullscreen=0);
  src/SDLHardwareRenderDevice.cpp asks for SDL_WINDOW_FULLSCREEN, not
  _DESKTOP, when full screen is chosen.
- src/Settings.cpp, src/PlatformAmiga.cpp: threaded picture loading is
  off by default and not offered (each loading thread takes a signal
  bit, and the task ran out of them).
- The game's 8-bit RGB PNGs are stored as RGBA in the drawer
  (amiga/tools/png_rgba.py): OpenGPU's SDL2_image.module 0.6 damages the
  host's heap decoding some of them.


Licences
--------

The engine: GPL 3 or later (COPYING.txt in the drawer). The game's art,
music and data: CC BY-SA 3.0 (LICENSE-data.txt), with every author in
CREDITS-data.txt. The amiga/ files are the AmiCradle Team's, GPL 3 or
later.
