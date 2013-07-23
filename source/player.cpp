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
#include "map.h"
#include "mapManager.h"
#include "timer.h"
#include "sprite.h"
#include "player.h"
#include "door.h"
#include "game.h"

// Set animations table
int animations[12][4] = {
	{4,0},
	{5,1},
	{6,2},
	{7,3},
	{8,12,16,16},
	{9,13,17,17},
	{10,14,18,18},
	{11,15,19,19},
	{20,24},
	{21,25},
	{22,26},
	{23,27}
};

Player::Player() : Sprite((char*)"graphics/characters/link.png") {
	// Set class members
	m_x = 120;
	m_y = 160;
	
	m_vx = 0;
	m_vy = 0;
	
	m_direction = Direction::Right;
	
	// Add animations to sprite
	addAnimation(2, animations[0], 100); // Down
	addAnimation(2, animations[1], 100); // Right
	addAnimation(2, animations[2], 100); // Left
	addAnimation(2, animations[3], 100); // Up
	addAnimation(4, animations[4], 50);  // Walk down
	addAnimation(4, animations[5], 50);  // Walk right
	addAnimation(4, animations[6], 50);  // Walk left
	addAnimation(4, animations[7], 50);  // Walk up
}

Player::~Player() {
}

bool inDoor = false;

void Player::doorCollisions() {
	printf("INDOOR: %d\n", inDoor);
	if(((m_vy < 0) && ((inTiles((m_x + 5) >> 4, (m_y + 12) >> 4, doorUp)) || (inTiles((m_x + 10) >> 4, (m_y + 12) >> 4, doorUp))))
	|| ((m_vy > 0) && ((inTiles((m_x + 5) >> 4, m_y >> 4, doorDown)) || (inTiles((m_x + 10) >> 4, m_y >> 4, doorDown))))
	|| ((inTiles((m_x + 8) >> 4, (m_y + 8) >> 4, changeMapTiles)) && (!inDoor))) {
		// Reset movement vectors
		m_vx = 0;
		m_vy = 0;
		
		// Search for the door
		s16 doorID = findDoorID(m_x, m_y, Game::currentMap->id());
		
		// If door isn't found
		if(doorID == -1) {
			printf("Fatal error. Code: 02\n");
			printf("%d ; %d", m_x >> 4, m_y >> 4);
			exit(EXIT_FAILURE);
		}
		//printf("door id: %d\n", doorID);
		
		// Update all values
		Game::currentMap = Game::maps[Game::doors[Game::doors[doorID]->nextDoorID]->mapID];
		m_x = Game::doors[Game::doors[doorID]->nextDoorID]->x;
		m_y = Game::doors[Game::doors[doorID]->nextDoorID]->y;
		m_direction = Game::doors[Game::doors[doorID]->nextDoorID]->direction;
		
		// Move view to display map correctly
		/*Game::MainWindow->GetDefaultView().SetCenter(Game::currentMap->x() * MAP_WIDTH * 16 + MAP_WIDTH * 16 / 2, Game::currentMap->y() * MAP_HEIGHT * 16 + MAP_HEIGHT * 16 / 2);
		
		// Initialize transition
		sf::Image transition(MAP_WIDTH * 16, MAP_HEIGHT * 16, sf::Color(255, 255, 255, 255));
		transition.SetSmooth(false);
		
		sf::Sprite renderTrans(transition);
		
		// Transition
		for(u16 x = 0 ; x < MAP_WIDTH / 2 ; x++) {
			Game::MainWindow->Clear();
			render();
			Game::currentMap->render();
			renderTrans.SetImage(transition);
			Game::MainWindow->Draw(renderTrans);
			Game::MainWindow->Display();
			for(u16 y = 0 ; y < MAP_HEIGHT * 16 ; y++) {
				for(u16 xx = 0 ; xx < 16 ; xx++) {
					transition.SetPixel(MAP_WIDTH / 2 + x + xx, y, sf::Color(0, 0, 0, 0));
					transition.SetPixel(MAP_WIDTH / 2 - 1 - x + xx, y, sf::Color(0, 0, 0, 0));
				}
			}
		}*/
		
		// The player is in the door
		inDoor = true;
	}
	
	if((!inTiles((m_x + 2) >> 4, (m_y + 2) >> 4, changeMapTiles))
	&& (!inTiles((m_x + 14) >> 4, (m_y + 14) >> 4, changeMapTiles))) {
		// The player isn't in the door anymore
		inDoor = false;
	}
}

// NOTE: In that func there is a lot of ">> 4", it's the same thing than "/ 16"
void Player::testCollisions() {
	// Right
	if((m_vx > 0) && ((!passable((m_x + 12) >> 4, (m_y + 8) >> 4)) || (!passable((m_x + 12) >> 4, (m_y + 13) >> 4)))) {
		// Reset movement vector to stop player
		m_vx = 0;
		
		// Obstacle up
		if((!passable((m_x + 12) >> 4, (m_y + 8) >> 4)) && passable((m_x + 12) >> 4, (m_y + 13) >> 4)) {
			if(m_vy == 0) m_vy = 1;
		}
		
		// Obstacle down
		if((!passable((m_x + 12) >> 4, (m_y + 13) >> 4)) && passable((m_x + 12) >> 4, (m_y + 8) >> 4)) {
			if(m_vy == 0) m_vy = -1;
		}
	}
	
	// Left
	if((m_vx < 0) && ((!passable((m_x + 3) >> 4, (m_y + 8) >> 4)) || (!passable((m_x + 3) >> 4, (m_y + 13) >> 4)))) {
		// Reset movement vector to stop player
		m_vx = 0;
		
		// Obstacle up
		if((!passable((m_x + 3) >> 4, (m_y + 8) >> 4)) && passable((m_x + 3) >> 4, (m_y + 13) >> 4)) {
			if(m_vy == 0) m_vy = 1;
		}
		
		// Obstacle down
		if((!passable((m_x + 3) >> 4, (m_y + 13) >> 4)) && passable((m_x + 3) >> 4, (m_y + 8) >> 4)) {
			if(m_vy == 0) m_vy = -1;
		}
	}
	
	// Up
	if((m_vy < 0) && ((!passable((m_x + 5) >> 4, (m_y + 5) >> 4)) || (!passable((m_x + 10) >> 4, (m_y + 5) >> 4)))) {
		// Reset movement vector to stop player
		m_vy = 0;
		
		// Obstacle left
		if((!passable((m_x + 5) >> 4, (m_y + 5) >> 4)) && passable((m_x + 10) >> 4, (m_y + 5) >> 4)) {
			if(m_vx == 0) m_vx = 1;
		}
		
		// Obstacle right
		if((!passable((m_x + 10) >> 4, (m_y + 5) >> 4)) && passable((m_x + 5) >> 4, (m_y + 5) >> 4)) {
			if(m_vx == 0) m_vx = -1;
		}
	}
	
	// Down
	if((m_vy > 0) && ((!passable((m_x + 5) >> 4, (m_y + 15) >> 4)) || (!passable((m_x + 10) >> 4, (m_y + 15) >> 4)))) {
		// Reset movement vector to stop player
		m_vy = 0;
		
		// Obstacle left
		if((!passable((m_x + 5) >> 4, (m_y + 15) >> 4)) && passable((m_x + 10) >> 4, (m_y + 15) >> 4)) {
			if(m_vx == 0) m_vx = 1;
		}
		
		// Obstacle right
		if((!passable((m_x + 10) >> 4, (m_y + 15) >> 4)) && passable((m_x + 5) >> 4, (m_y + 15) >> 4)) {
			if(m_vx == 0) m_vx = -1;
		}
	}
}

void Player::move() {
	if(Game::Input->IsKeyDown(sf::Key::Up)) {
		// Set vertical movement vector negative
		m_vy = -1;
		
		// If all other directional keys are released
		if(!Game::Input->IsKeyDown(sf::Key::Left) && !Game::Input->IsKeyDown(sf::Key::Right) && !Game::Input->IsKeyDown(sf::Key::Down)) {
			// Set direction to up
			m_direction = Direction::Up;
		}
	}
	
	if(Game::Input->IsKeyDown(sf::Key::Down)) {
		// Set vertical movement vector positive
		m_vy = 1;
		
		// If all other directional keys are released
		if(!Game::Input->IsKeyDown(sf::Key::Left) && !Game::Input->IsKeyDown(sf::Key::Right) && !Game::Input->IsKeyDown(sf::Key::Up)) {
			// Set direction to down
			m_direction = Direction::Down;
		}
	}
	
	if(Game::Input->IsKeyDown(sf::Key::Left)) {
		// Set horizontal movement vector negative
		m_vx = -1;
		
		// If all other directional keys are released
		if(!Game::Input->IsKeyDown(sf::Key::Up) && !Game::Input->IsKeyDown(sf::Key::Right) && !Game::Input->IsKeyDown(sf::Key::Down)) {
			// Set direction to left
			m_direction = Direction::Left;
		}
	}
	
	if(Game::Input->IsKeyDown(sf::Key::Right)) {
		// Set horizontal movement vector positive
		m_vx = 1;
		
		// If all other directional keys are released
		if(!Game::Input->IsKeyDown(sf::Key::Left) && !Game::Input->IsKeyDown(sf::Key::Up) && !Game::Input->IsKeyDown(sf::Key::Down)) {
			// Set direction to right
			m_direction = Direction::Right;
		}
	}
	
	// Test collisions
	doorCollisions();
	testCollisions();
	
	// Move the player
	m_x += m_vx * PLAYER_SPEED;
	m_y += m_vy * PLAYER_SPEED;
	
	// Reset movement vectors
	m_vx = 0;
	m_vy = 0;
}

void Player::render() {
	// If all directional keys are released
	if(!Game::Input->IsKeyDown(sf::Key::Left) && !Game::Input->IsKeyDown(sf::Key::Up) && !Game::Input->IsKeyDown(sf::Key::Right) && !Game::Input->IsKeyDown(sf::Key::Down)) {
		// Render a single frame
		drawFrame(m_x, m_y, m_direction);
	} else {
		// Play walk animation
		playAnimation(m_x, m_y, m_direction);
	}
}

