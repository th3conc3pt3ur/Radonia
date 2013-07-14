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
#include "game.h"

using namespace std;

sf::RenderWindow *Game::MainWindow = NULL;

Game::Game() {
	// Create the main window
	MainWindow = new sf::RenderWindow(sf::VideoMode(256, 192), "Radonia");
	
	// Set default values
	m_continue = true;
	m_paused = false;
	
	// Temp map
	sf::Image *tileset = new sf::Image;
	tileset->LoadFromFile("graphics/tilesets/plain.png");
	tileset->SetSmooth(false);
	m_map = new Map(tileset, (char*)"maps/a1.map", 16, 12, 16, 16, 0, 0);
}

Game::~Game() {
	// Delete main window
	delete MainWindow;
	
	delete m_map;
}

void Game::mainLoop() {
	while(MainWindow->IsOpened() && m_continue) {
		// Process events
		sf::Event event;
		
		while(MainWindow->GetEvent(event)) {
			// Close window: exit game
			if(event.Type == sf::Event::Closed)
				MainWindow->Close();
		}
		
		// Clear screen
		MainWindow->Clear();
		
		// Display funcs here
		m_map->render();
		
		// Update the window
		MainWindow->Display();
	}
}

