/*---------------------------------------------------------------------------------
	
	Radonia
	Copyright (C) 2013-2014 Deloptia <deloptia.devteam@gmail.com>
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
---------------------------------------------------------------------------------*/
#include <iostream>

#include "includeSDL.h"

#include "types.h"
#include "color.h"
#include "config.h"
#include "window.h"
#include "keyboard.h"
#include "timer.h"
#include "image.h"
#include "animation.h"
#include "sprite.h"
#include "weapon.h"
#include "character.h"
#include "monster.h"
#include "NPC.h"
#include "player.h"
#include "map.h"
#include "door.h"
#include "mapManager.h"
#include "doorManager.h"
#include "characterManager.h"
#include "collisionManager.h"
#include "tools.h"
#include "interface.h"
#include "game.h"

using namespace std;

Window *Game::MainWindow = NULL;

Game::Game() {
	// Initialize random seed
	srand(time(NULL));
	
#ifdef __ANDROID__
	// Initialize display mode
	SDL_DisplayMode current;
	
	// Get current display mode
	int success = SDL_GetCurrentDisplayMode(0, &current);
	
	// Log cat
	__android_log_print(ANDROID_LOG_INFO, "Radonia", "Current display: %dx%d", current.w, current.h);
	
	// Create the main window
	MainWindow = new Window((char*)"Radonia", current.w, current.h);
#else
	// Create the main window
	MainWindow = new Window((char*)"Radonia", 640, 480);
#endif
	
	// Set default values
	m_continue = true;
	m_paused = false;
	
	// Initialize characters
	CharacterManager::initAllCharacters();
	
	// Initialize maps
	MapManager::initMaps();
	
	// Initialize doors
	DoorManager::initDoors();
	
	MapManager::currentMap = MapManager::mapAreas[CharacterManager::player()->mapID()][CharacterManager::player()->area()];
	
#ifdef VIEWPORT
	// Update viewport with player position
	MainWindow->centerViewportWithObject(CharacterManager::player()->x(), CharacterManager::player()->y(), CharacterManager::player()->frameSize(), CharacterManager::player()->frameSize());
#endif
	
	// Initialize interface
	Interface::initialize();
}

Game::~Game() {
	// Unload interface
	Interface::unload();
	
	// Unload map manager
	MapManager::unload();
	
	// Delete doors
	delete[] DoorManager::doors;
	
	// Delete characters
	delete[] CharacterManager::characters;
	
	// Delete main window
	delete MainWindow;
}

void Game::mainLoop() {
	u32 lastTime = 0;
	u32 actualTime = 0;
	
	while(m_continue) {
		// Process events
		SDL_Event event;
		while(SDL_PollEvent(&event) != 0) {
			switch(event.type) {
				case SDL_QUIT:
					m_continue = false;
					break;
#ifndef __ANDROID__
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE: m_continue = false; break;
						case SDLK_p: m_paused = !m_paused; break;
						default: break;
					}
#else
				case SDL_FINGERDOWN:
				case SDL_FINGERMOTION:
					Keyboard::updatePad(&event);
					break;
				case SDL_FINGERUP:
					Keyboard::resetPad();
					break;
#endif
			}
		}
		
		if(m_paused) continue;
		
		// Update keyboard state
		Keyboard::update();
		
		// Test for map scrolling
		scroll();
		
		// Make it framerate independant
		actualTime = SDL_GetTicks();
		if(actualTime - lastTime < 15) {
			SDL_Delay(15 - (actualTime - lastTime));
			continue;
		}
		
		// Move all characters
		CharacterManager::moveCharacters();
		
		// Clear screen
		MainWindow->clear();
		
		// Render current map
		MapManager::currentMap->render();
		
		// Render all characters
		CharacterManager::renderCharacters();
		
		// Render player
		CharacterManager::player()->action();
		
		// Render HUD
		Interface::renderHUD();
		
		// Update the window
		MainWindow->update();
		
		// Update timer
		lastTime = actualTime;
	}
}

void Game::scroll() {
	s16 moveX = 0;
	s16 moveY = 0;
	u16 iMax = 0;
	s16 playerX = 0;
	s16 playerY = 0;
	
	if(CharacterManager::player()->x() > (MAP_WIDTH - 1) * 16 + 2)		 { moveX =  16; iMax = 40; playerX = -16; }
	else if(CharacterManager::player()->x() < -2)						 { moveX = -16; iMax = 40; playerX =  16; }
	else if(CharacterManager::player()->y() > (MAP_HEIGHT - 1) * 16 + 1) { moveY =  16; iMax = 30; playerY = -16; }
	else if(CharacterManager::player()->y() < -2)						 { moveY = -16; iMax = 30; playerY =  16; }
	else																 { return; }
	
	// Reset player movement vectors
	CharacterManager::player()->stop();
	
	for(u16 i = 0 ; i < iMax ; i++) {
		// Move characters
		for(std::vector<Character*>::iterator it = MapManager::currentMap->characters()->begin() ; it != MapManager::currentMap->characters()->end() ; it++) {
			if((i & 1) || !(i & 11)) (*it)->moveX(playerX); else (*it)->moveX(playerX - playerX / 16);
			if((i & 1) || !(i & 15)) (*it)->moveY(playerY); else (*it)->moveY(playerY - playerY / 16);
		}
		
		// Move view to scroll
		Map::viewRect.x += moveX;
		Map::viewRect.y += moveY;
		
#ifdef VIEWPORT
		// Update viewport
		MainWindow->updateViewportPosition(MainWindow->viewportX() - moveX, MainWindow->viewportY() - moveY);
#endif
		
		// Refresh display on time in two
		if(i & 1) {
			MainWindow->clear();
			MapManager::refreshMaps(MapManager::mapAreas[MapManager::currentMap->area()], moveX, moveY);
			CharacterManager::renderCharacters();
			Interface::renderHUD();
			MainWindow->update(false);
		}
	}
	
	// Update currentMap variable
	if(MapManager::currentMap != NULL
	&& MapManager::currentMap->x() + moveX / 16 >= 0
	&& MapManager::currentMap->x() + moveX / 16 < WM_SIZE
	&& MapManager::currentMap->y() + moveY / 16 >= 0
	&& MapManager::currentMap->y() + moveY / 16 < WM_SIZE) MapManager::currentMap = MapManager::mapAreas[MapManager::currentMap->area()][MAP_POS(MapManager::currentMap->x() + moveX / 16, MapManager::currentMap->y() + moveY / 16, MapManager::currentMap->area())];
	
	// Regen monsters and reset positions
	for(std::vector<Character*>::iterator it = MapManager::currentMap->characters()->begin() ; it != MapManager::currentMap->characters()->end() ; it++) {
		if(!(*it)->isPlayer()) (*it)->reset();
	}
}

