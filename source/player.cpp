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
int Player_animations[12][4] = {
	{4,0},
	{5,1},
	{6,2},
	{7,3}
};

NPC *Player::collidedNPC = NULL;

Player::Player() : Sprite((char*)"graphics/characters/link.png") {
	// Set class members
	m_x = 120;
	m_y = 160;
	
	m_vx = 0;
	m_vy = 0;
	
	m_direction = Direction::Right;
	
	// Add animations to sprite
	addAnimation(2, Player_animations[0], 100); // Down
	addAnimation(2, Player_animations[1], 100); // Right
	addAnimation(2, Player_animations[2], 100); // Left
	addAnimation(2, Player_animations[3], 100); // Up
}

Player::~Player() {
}

bool inDoor = false;

void Player::doorCollisions() {
	if(((m_vy < 0) && ((inTiles((m_x + 5) >> 4, (m_y + 12) >> 4, doorUp)) || (inTiles((m_x + 10) >> 4, (m_y + 12) >> 4, doorUp))))
	|| ((m_vy > 0) && ((inTiles((m_x + 5) >> 4, m_y >> 4, doorDown)) || (inTiles((m_x + 10) >> 4, m_y >> 4, doorDown))))
	|| ((inTiles((m_x + 8) >> 4, (m_y + 8) >> 4, changeMapTiles)) && (!inDoor))) {
		// Reset movement vectors
		m_vx = 0;
		m_vy = 0;
		
		// Search for the door
		s16 doorID = findDoorID(m_x, m_y, Game::currentMap->id(), Game::currentMap->area());
		
		// If door isn't found
		if(doorID == -1) {
			return;
		}
		
		// Initialize transition
		sf::RectangleShape rect1(sf::Vector2f(MAP_WIDTH * 16 / 2, MAP_HEIGHT * 16));
		sf::RectangleShape rect2(sf::Vector2f(MAP_WIDTH * 16 / 2, MAP_HEIGHT * 16));
		
		rect1.setPosition(0, 0);
		rect2.setPosition(MAP_WIDTH * 16 / 2, 0);
		
		Game::MainWindow->clear();
		Game::MainWindow->setView(*Sprite::View);
		Game::MainWindow->draw(rect1);
		Game::MainWindow->draw(rect2);
		Game::MainWindow->setView(Game::MainWindow->getDefaultView());
		Game::MainWindow->display();
		
		// Update all values
		Game::currentMap = Game::mapAreas[Game::doors[Game::doors[doorID]->nextDoorID]->mapArea][Game::doors[Game::doors[doorID]->nextDoorID]->mapID];
		m_x = Game::doors[Game::doors[doorID]->nextDoorID]->x;
		m_y = Game::doors[Game::doors[doorID]->nextDoorID]->y;
		m_direction = Game::doors[Game::doors[doorID]->nextDoorID]->direction;
		
		// Move view to display map correctly
		Map::View->setCenter(Game::currentMap->x() * MAP_WIDTH * 16 + MAP_WIDTH * 16 / 2, Game::currentMap->y() * MAP_HEIGHT * 16 + MAP_HEIGHT * 16 / 2);
		
		// Transition
		for(u16 x = 0 ; x <= MAP_HEIGHT / 2 ; x++) {
			rect1.move(-32, 0);
			rect2.move(32, 0);
			
			Game::MainWindow->clear();
			Game::currentMap->render();
			render();
			Game::MainWindow->setView(*Sprite::View);
			Game::MainWindow->draw(rect1);
			Game::MainWindow->draw(rect2);
			Game::MainWindow->setView(Game::MainWindow->getDefaultView());
			Game::MainWindow->display();
		}
		
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

void Player::actions() {
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		// Set vertical movement vector negative
		m_vy = -1;
		
		// If all other directional keys are released
		if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			// Set direction to up
			m_direction = Direction::Up;
		}
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		// Set vertical movement vector positive
		m_vy = 1;
		
		// If all other directional keys are released
		if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			// Set direction to down
			m_direction = Direction::Down;
		}
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		// Set horizontal movement vector negative
		m_vx = -1;
		
		// If all other directional keys are released
		if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			// Set direction to left
			m_direction = Direction::Left;
		}
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		// Set horizontal movement vector positive
		m_vx = 1;
		
		// If all other directional keys are released
		if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
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
	
	// If A is pressed, and the player collided a NPC, talk to him
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && collidedNPC) {
		collidedNPC->speak();
	}
}

void Player::render() {
	// If all directional keys are released
	if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		// Render a single frame
		drawFrame(m_x, m_y, m_direction);
	} else {
		// Play walk animation
		playAnimation(m_x, m_y, m_direction);
	}
}

