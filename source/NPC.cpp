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
#include <cstdio>
#include <cstdlib>

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "types.h"
#include "config.h"
#include "timer.h"
#include "sprite.h"
#include "NPC.h"
#include "player.h"
#include "map.h"
#include "mapManager.h"
#include "door.h"
#include "game.h"

// Set animations table
int NPC_animations[12][4] = {
	{4,0},
	{5,1},
	{6,2},
	{7,3}
};

int NPC::nbNPCs = 0;

char *NPC::texts[NB_NPCs] = {(char*)"Hello boy!"};

NPC::NPC(u16 x, u16 y, u8 direction, u16 mapID, char *filename) : Sprite(filename) {
	// Set NPC id
	m_id = nbNPCs;
	
	// Update NPCs counter
	nbNPCs++;
	
	// Set class members
	m_x = x;
	m_y = y;
	
	m_direction = direction;
	
	// Add animations to sprite
	addAnimation(2, NPC_animations[0], 250); // Down
	addAnimation(2, NPC_animations[1], 250); // Right
	addAnimation(2, NPC_animations[2], 250); // Left
	addAnimation(2, NPC_animations[3], 250); // Up
}

NPC::~NPC() {
}

void NPC::move() {
	// Move NPC
	m_x += m_vx;
	m_y += m_vy;
	
	// Reset movement vectors
	m_vx = 0;
	m_vy = 0;
}

void NPC::render() {
	// Play animation
	playAnimation(m_x, m_y, m_direction);
}

void NPC::speak() {
	//std::cout << NPC::texts[m_id] << std::endl;
	
	// Initialize string
	sf::Text text(NPC::texts[m_id], *Game::defaultFont);
	text.setPosition((MAP_WIDTH * 16) / 8 + 8, 3 * (MAP_HEIGHT * 16) / 4 + 8);
	
	// Initialize box
	sf::RectangleShape box;
	box.setPosition((MAP_WIDTH * 16) / 8, 3 * (MAP_HEIGHT * 16) / 4);
	box.setSize(sf::Vector2f(6 * (MAP_WIDTH * 16) / 8, (MAP_HEIGHT * 16) / 4));
	box.setFillColor(sf::Color::Black);
	sf::Text pressKeyText((char*)"Press Z key to continue.", *Game::defaultFont, 10);
	pressKeyText.setPosition(11 * MAP_WIDTH + 12, (MAP_HEIGHT * 16) - 13);
	
	// Render string
	Game::MainWindow->setView(*Sprite::View);
	Game::MainWindow->draw(box);
	Game::MainWindow->draw(pressKeyText);
	Game::MainWindow->draw(text);
	Game::MainWindow->display();
	
	// Wait for Z pressed
	sf::Event event;
	while(Game::MainWindow->isOpen() && !(Game::MainWindow->pollEvent(event) && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z)) {
		// Close window: exit game
		if((event.type == sf::Event::Closed) || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
			Game::MainWindow->close();
		}
		
		Game::MainWindow->display();
	}
	
	// Reset main window
	Game::MainWindow->clear();
	Game::MainWindow->setView(Game::MainWindow->getDefaultView());
}

NPC *NPC::BlueBoy(u16 x, u16 y, u8 direction, u16 mapID) {
	return new NPC(x, y, direction, mapID, (char*)"graphics/characters/blue_boy.png");
}

NPC **NPC::initAll() {
	// Initialize NPCs array
	NPC **NPCs = new NPC*[NB_NPCs];
	
	// Init NPCs
	NPCs[0] = NPC::BlueBoy(10 << 4, 2 << 4, Direction::Down, 0);
	
	return NPCs;
}

