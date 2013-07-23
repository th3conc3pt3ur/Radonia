/*---------------------------------------------------------------------------------
	
	Radonia
	Copyright (C) 2013 Deloptia <deloptia.devteam@gmail.com>
	
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

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "types.h"
#include "config.h"
#include "map.h"
#include "mapManager.h"
#include "timer.h"
#include "sprite.h"
#include "player.h"
#include "door.h"
#include "game.h"

using namespace std;

sf::RenderWindow *Game::MainWindow = NULL;
const sf::Input *Game::Input = NULL;
sf::Image **Game::tilesets = NULL;
Map ***Game::mapAreas = NULL;
Map **Game::maps = NULL;
Map *Game::currentMap = NULL;
Door **Game::doors = NULL;
Player *Game::player = NULL;

Game::Game() {
	// Create the main window
	MainWindow = new sf::RenderWindow(sf::VideoMode(640, 480), "Radonia", sf::Style::Close);
	
	// Set inputs
	Input = &MainWindow->GetInput();
	
	// Set default values
	m_continue = true;
	m_paused = false;
	
	// Initialize tilesets
	tilesets = initTilesets();
	
	// Initialize maps
	mapAreas = initMaps();
	maps = mapAreas[0];
	
	// Initialize doors
	doors = initDoors();
	
	currentMap = maps[0];
	
	// Initialize player
	player = new Player();
}

Game::~Game() {
	// Delete main window
	delete MainWindow;
	
	// Delete sprite view
	delete Sprite::View;
	
	// Delete maps
	delete[] mapAreas;
	
	// Delete player
	delete player;
	
	// Delete doors
	delete[] doors;
}

void Game::mainLoop() {
	while(MainWindow->IsOpened() && m_continue) {
		// Process events
		sf::Event event;
		
		while(MainWindow->GetEvent(event)) {
			// Close window: exit game
			if((event.Type == sf::Event::Closed) || (event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Escape)) {
				MainWindow->Close();
			}
		}
		
		// Test for map scrolling
		scroll();
		
		// Move player
		player->move();
		
		// Clear screen
		MainWindow->Clear();
		
		// Render current map
		Game::currentMap->render();
		
		// Render player
		player->render();
		
		// Update the window
		MainWindow->Display();
	}
}

void Game::scroll() {
	s16 moveX = 0;
	s16 moveY = 0;
	u16 iMax = 0;
	s16 playerX = 0;
	s16 playerY = 0;
	
	if(player->x() > (MAP_WIDTH - 1) * 16 + 2)		 { moveX =  32; iMax = 20; playerX = -32; }
	else if(player->x() < -2)						 { moveX = -32; iMax = 20; playerX =  32; }
	else if(player->y() > (MAP_HEIGHT - 1) * 16 + 1) { moveY =  32; iMax = 15; playerY = -32; }
	else if(player->y() < -2)						 { moveY = -32; iMax = 15; playerY =  32; }
	else											 { return; }
	
	// Reset player movement vectors
	player->vx(0);
	player->vy(0);
	
	for(u16 i = 0 ; i < iMax ; i++) {
		// Move player
		if((i & 1) || !(i & 11)) player->x(player->x() + playerX); else player->x(player->x() + playerX - playerX / 16);
		if((i & 1) || !(i & 15)) player->y(player->y() + playerY); else player->y(player->y() + playerY - playerY / 16);
		
		// Move view to scroll
		MainWindow->GetDefaultView().Move(moveX, moveY);
		
		// Refresh display on time in two
		if(i & 1) {
			MainWindow->Clear();
			refreshMaps(mapAreas[currentMap->area()], moveX, moveY);
			player->render();
			MainWindow->Display();
		}
	}
	
	// Update currentMap variable
	if(currentMap != NULL
	   && currentMap->x() + moveX / 32 >= 0
	   && currentMap->x() + moveX / 32 < WM_SIZE
	   && currentMap->y() + moveY / 32 >= 0
	   && currentMap->y() + moveY / 32 < WM_SIZE) currentMap = mapAreas[currentMap->area()][MAP_POS(currentMap->x() + moveX / 32, currentMap->y() + moveY / 32)];
}

