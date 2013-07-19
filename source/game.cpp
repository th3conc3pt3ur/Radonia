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
#include "game.h"

using namespace std;

sf::RenderWindow *Game::MainWindow = NULL;
const sf::Input *Game::Input = NULL;
Map *Game::currentMap = NULL;
Player *Game::player = NULL;

Game::Game() {
	// Create the main window
	MainWindow = new sf::RenderWindow(sf::VideoMode(640, 480), "Radonia", sf::Style::Close);
	
	// Set inputs
	Input = &MainWindow->GetInput();
	
	// Set default values
	m_continue = true;
	m_paused = false;
	
	// Initialize overworld maps
	m_maps = initOverworldMaps();
	
	currentMap = m_maps[0];
	
	// Initialize player
	player = new Player();
}

Game::~Game() {
	// Delete main window
	delete MainWindow;
	
	// Delete overworld maps
	delete[] m_maps;
}

void Game::mainLoop() {
	while(MainWindow->IsOpened() && m_continue) {
		// Process events
		sf::Event event;
		
		while(MainWindow->GetEvent(event)) {
			// Close window: exit game
			if(event.Type == sf::Event::Closed) {
				MainWindow->Close();
			}
		}
		
		// Temporary scrolling function
		scroll();
		
		// Move player
		player->move();
		
		// Clear screen
		MainWindow->Clear();
		
		// Render overworld maps
		renderMaps(m_maps);
		player->render();
		
		// Update the window
		MainWindow->Display();
	}
}

void Game::scroll() {
	s16 moveX = 0;
	s16 moveY = 0;
	u16 iMax = 0;
	
	if(Input->IsKeyDown(sf::Key::Q)) { moveX = -32;	iMax = 20; }
	if(Input->IsKeyDown(sf::Key::D)) { moveX = 32;	iMax = 20; }
	if(Input->IsKeyDown(sf::Key::Z)) { moveY = -32;	iMax = 15; }
	if(Input->IsKeyDown(sf::Key::S)) { moveY = 32;	iMax = 15; }
	
	for(u16 i = 0 ; i < iMax ; i++) {
		// Move view to scroll
		MainWindow->GetDefaultView().Move(moveX, moveY);
		
		// Refresh window
		MainWindow->Clear();
		refreshMaps(m_maps, moveX, moveY);
		player->render();
		MainWindow->Display();
	}
	
	// Update currentMap variable
	if(currentMap != NULL
	   && currentMap->x() + moveX / 32 >= 0
	   && currentMap->x() + moveX / 32 < WM_SIZE
	   && currentMap->y() + moveY / 32 >= 0
	   && currentMap->y() + moveY / 32 < WM_SIZE) currentMap = m_maps[MAP_POS(currentMap->x() + moveX / 32, currentMap->y() + moveY / 32)];
}

