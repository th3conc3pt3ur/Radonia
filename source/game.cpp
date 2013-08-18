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
//sf::Font *Game::defaultFont = NULL;
Image **Game::tilesets = NULL;
Map ***Game::mapAreas = NULL;
Map **Game::maps = NULL;
Map *Game::currentMap = NULL;
Door **Game::doors = NULL;
NPC **Game::NPCs = NULL;
Monster **Game::monsters = NULL;
Player *Game::player = NULL;

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
	
	// Setup default font
/*	defaultFont = new sf::Font();
	if(!defaultFont->loadFromFile((char*)"fonts/Vani.ttf")) {
		std::cout << "FATAL ERROR: Unable to load default font." << std::endl;
		exit(EXIT_FAILURE);
	}
*/
	
	// Set default values
	m_continue = true;
	m_paused = false;
	
	// Initialize tilesets
	tilesets = MapManager::initTilesets();
	
	// Initialize NPCs
	NPCs = CharacterManager::initAllNPCs();
	
	// Initialize monsters
	monsters = CharacterManager::initAllMonsters();
	
	// Initialize maps
	mapAreas = MapManager::initMaps();
	maps = mapAreas[0];
	
	// Initialize doors
	doors = DoorManager::initDoors();
	
	currentMap = maps[0];
	
	// Initialize player
	player = CharacterManager::initPlayer();
	
#ifdef VIEWPORT
	// Update viewport with player position
	MainWindow->centerViewportWithObject(player->x(), player->y(), player->frameSize(), player->frameSize());
#endif
	
	// Initialize interface
	Interface::initialize();
}

Game::~Game() {
	// Unload interface
	Interface::unload();
	
	// Delete main window
	delete MainWindow;
	
	// Delete default font
	//delete defaultFont;
	
	// Delete maps
	delete[] mapAreas;
	
	// Delete doors
	delete[] doors;
	
	// Delete NPCs
	delete[] NPCs;
	
	// Delete monsters
	delete[] monsters;
	
	// Delete player
	delete player;
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
#ifdef __ANDROID__
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
		
		// Update keyboard state
		Keyboard::update();
		
		// Test for map scrolling
		scroll();
		
		// Make if framerate independant
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
		currentMap->render();
		
		// Render all characters
		CharacterManager::renderCharacters();
		
		// Render player
		player->action();
		
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
	
	if(player->x() > (MAP_WIDTH - 1) * 16 + 2)		 { moveX =  16; iMax = 40; playerX = -16; }
	else if(player->x() < -2)						 { moveX = -16; iMax = 40; playerX =  16; }
	else if(player->y() > (MAP_HEIGHT - 1) * 16 + 1) { moveY =  16; iMax = 30; playerY = -16; }
	else if(player->y() < -2)						 { moveY = -16; iMax = 30; playerY =  16; }
	else											 { return; }
	
	// Reset player movement vectors
	player->vx(0);
	player->vy(0);
	
	for(u16 i = 0 ; i < iMax ; i++) {
		// Move player
		if((i & 1) || !(i & 11)) player->x(player->x() + playerX); else player->x(player->x() + playerX - playerX / 16);
		if((i & 1) || !(i & 15)) player->y(player->y() + playerY); else player->y(player->y() + playerY - playerY / 16);
		
		// Move NPCs and monsters
		for(u16 j = 0 ; j < currentMap->NPCs().size() ; j++) {
			if((i & 1) || !(i & 11)) currentMap->NPCs()[j]->x(currentMap->NPCs()[j]->x() + playerX); else currentMap->NPCs()[j]->x(currentMap->NPCs()[j]->x() + playerX - playerX / 16);
			if((i & 1) || !(i & 15)) currentMap->NPCs()[j]->y(currentMap->NPCs()[j]->y() + playerY); else currentMap->NPCs()[j]->y(currentMap->NPCs()[j]->y() + playerY - playerY / 16);
		}
		for(u16 j = 0 ; j < currentMap->monsters().size() ; j++) {
			if((i & 1) || !(i & 11)) currentMap->monsters()[j]->x(currentMap->monsters()[j]->x() + playerX); else currentMap->monsters()[j]->x(currentMap->monsters()[j]->x() + playerX - playerX / 16);
			if((i & 1) || !(i & 15)) currentMap->monsters()[j]->y(currentMap->monsters()[j]->y() + playerY); else currentMap->monsters()[j]->y(currentMap->monsters()[j]->y() + playerY - playerY / 16);
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
			MapManager::refreshMaps(mapAreas[currentMap->area()], moveX, moveY);
			CharacterManager::renderCharacters();
			Interface::renderHUD();
			MainWindow->update(false);
		}
	}
	
	// Update currentMap variable
	if(currentMap != NULL
	   && currentMap->x() + moveX / 16 >= 0
	   && currentMap->x() + moveX / 16 < WM_SIZE
	   && currentMap->y() + moveY / 16 >= 0
	   && currentMap->y() + moveY / 16 < WM_SIZE) currentMap = mapAreas[currentMap->area()][MAP_POS(currentMap->x() + moveX / 16, currentMap->y() + moveY / 16, currentMap->area())];
	
	// Regen monsters and reset positions
	for(u16 i = 0 ; i < currentMap->monsters().size() ; i++) {
		currentMap->monsters()[i]->reset();
	}
	for(u16 i = 0 ; i < currentMap->NPCs().size() ; i++) {
		currentMap->NPCs()[i]->reset();
	}
}

