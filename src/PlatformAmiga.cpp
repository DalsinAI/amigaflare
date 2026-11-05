/*
Copyright © 2016 Justin Jacobs

This file is part of FLARE.

FLARE is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

FLARE is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
FLARE.  If not, see http://www.gnu.org/licenses/
*/

/*
 * AmigaOS 3.x: the data (mods/), the settings (config/) and the saves
 * (userdata/) are all in the program's drawer. Paths are built from the
 * drawer's full name ("Work:Flare/"): AmigaDOS reads "PROGDIR:/x" as the
 * parent drawer and "a//b" as a's parent, so nothing may add a second '/'.
 */

#ifdef PLATFORM_CPP_INCLUDE
#ifndef PLATFORM_CPP
#define PLATFORM_CPP

#include "Platform.h"
#include "Settings.h"
#include "SharedResources.h"
#include "Utils.h"
#include "UtilsFileSystem.h"

#include <SDL.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

extern "C" const char *amiga_program_dir(void); // amiga/amiga_start.c

Platform platform;

Platform::Platform()
	: has_exit_button(true)
	, is_mobile_device(false)
	, force_hardware_cursor(false)
	, has_lock_file(true)
	, needs_alt_escape_key(false)
	, fullscreen_bypass(false)
	, config_menu_type(CONFIG_MENU_TYPE_DESKTOP)
	, default_renderer("")
	, config_video(Platform::Video::COUNT, true)
	, config_audio(Platform::Audio::COUNT, true)
	, config_game(Platform::Game::COUNT, true)
	, config_interface(Platform::Interface::COUNT, true)
	, config_input(Platform::Input::COUNT, true)
	, config_misc(Platform::Misc::COUNT, true)
{
	// threads that load pictures run the task out of signal bits
	config_video[Platform::Video::THREADED_IMAGE_LOAD] = false;
}

Platform::~Platform() {
}

// The program's drawer as "Volume:Dir/" (or "Volume:" at a volume's root).
static std::string programDir() {
	std::string dir(amiga_program_dir());
	if (!dir.empty() && dir[dir.size() - 1] != ':' && dir[dir.size() - 1] != '/')
		dir += "/";
	return dir;
}

void Platform::setPaths() {
	std::string base = programDir();

	settings->path_conf = base + "config/";
	Filesystem::createDir(settings->path_conf);

	settings->path_user = base + "userdata/";
	Filesystem::createDir(settings->path_user);
	Filesystem::createDir(settings->path_user + "mods/");
	Filesystem::createDir(settings->path_user + "saves/");

	if (Filesystem::pathExists(settings->custom_path_data)) {
		settings->path_data = settings->custom_path_data;
	}
	else {
		if (!settings->custom_path_data.empty()) {
			Utils::logError("Platform: Could not find specified game data directory.");
			settings->custom_path_data = "";
		}
		settings->path_data = base;
	}
}

bool Platform::dirCreate(const std::string& path) {
	if (mkdir(path.c_str(), 0777) == -1) {
		std::string error_msg = "Platform::dirCreate (" + path + ")";
		perror(error_msg.c_str());
		return false;
	}
	return true;
}

bool Platform::dirRemove(const std::string& path) {
	if (rmdir(path.c_str()) == -1) {
		std::string error_msg = "Platform::dirRemove (" + path + ")";
		perror(error_msg.c_str());
		return false;
	}
	return true;
}

// unused
void Platform::FSInit() {}
bool Platform::FSCheckReady() { return true; }
void Platform::FSCommit() {}
void Platform::setScreenSize() {}
void Platform::setFullscreen(bool) {}
void Platform::setExitEventFilter() {}

#endif // PLATFORM_CPP
#endif // PLATFORM_CPP_INCLUDE
