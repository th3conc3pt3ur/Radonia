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

#include "game.h"

using namespace std;

Game::Game() {
	// Create the main window
	m_win = new sf::RenderWindow(sf::VideoMode(256, 192), "Radonia");
	
	// Set default values
	m_continue = true;
	m_paused = false;
}

Game::~Game() {
	// Delete main window
	delete m_win;
}

void Game::mainLoop() {
	while(m_win->IsOpened() && m_continue) {
		// Process events
		sf::Event event;
		
		while(m_win->GetEvent(event)) {
			// Close window: exit game
			if(event.Type == sf::Event::Closed)
				m_win->Close();
		}
		
		// Clear screen
		m_win->Clear();
		
		// Display funcs here
		
		// Update the window
		m_win->Display();
	}
}

