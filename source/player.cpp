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
#include "animation.h"
#include "sprite.h"
#include "character.h"
#include "monster.h"
#include "NPC.h"
#include "player.h"
#include "map.h"
#include "door.h"
#include "animationManager.h"
#include "mapManager.h"
#include "doorManager.h"
#include "characterManager.h"
#include "collisionManager.h"
#include "tools.h"
#include "interface.h"
#include "game.h"

Player::Player() : Sprite((char*)"graphics/characters/link.png", SPRITE_PLAYER, 120, 160) {
	// Set class members
	m_direction = Direction::Right;
	
	m_maxLifes = 5;
	m_lifes = 3 * 4 + 3;
	
	m_isAttacking = false;
	
	m_blockedCommands = false;
	m_blockedDirections = false;
	
	m_swordSpr = new Sprite((char*)"graphics/animations/sword.png", SPRITE_PWEAPON);
	
	m_swordSpr->addAnimation(4, Sword_animations[0], 100); // Down
	m_swordSpr->addAnimation(4, Sword_animations[1], 100); // Right
	m_swordSpr->addAnimation(4, Sword_animations[2], 100); // Left
	m_swordSpr->addAnimation(4, Sword_animations[3], 100); // Up
	
	// Add animations to sprite
	addAnimation(2, Player_animations[0], 100); // Down
	addAnimation(2, Player_animations[1], 100); // Right
	addAnimation(2, Player_animations[2], 100); // Left
	addAnimation(2, Player_animations[3], 100); // Up
	addAnimation(4, Player_animations[4], 100); // Attack down
	addAnimation(4, Player_animations[5], 100); // Attack right
	addAnimation(4, Player_animations[6], 100); // Attack left
	addAnimation(4, Player_animations[7], 100); // Attack up
}

Player::~Player() {
	delete m_swordSpr;
}

bool inDoor = false;
void Player::doorCollisions() {
	if(((inTiles((m_x + 8) >> 4, (m_y + 8) >> 4, changeMapTiles)) && (!inDoor))) {
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
		if(!Game::currentMap) exit(EXIT_FAILURE);
		
		m_x = Game::doors[Game::doors[doorID]->nextDoorID]->x;
		m_y = Game::doors[Game::doors[doorID]->nextDoorID]->y;
		m_direction = Game::doors[Game::doors[doorID]->nextDoorID]->direction;
		
		// Move view to display map correctly
		Map::View->setCenter(Game::currentMap->x() * MAP_WIDTH * 16 + MAP_WIDTH * 16 / 2, Game::currentMap->y() * MAP_HEIGHT * 16 + MAP_HEIGHT * 16 / 2);
		
		// Transition
		for(u16 x = 0 ; x <= MAP_HEIGHT / 1.5 ; x++) {
			rect1.move(-32, 0);
			rect2.move(32, 0);
			
			Game::MainWindow->clear();
			Game::currentMap->render();
			Game::currentMap->renderNPCs();
			Game::currentMap->renderMonsters();
			render();
			Interface::renderHUD();
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

// Sword loading timer
Timer swordLoading;

void Player::sword() {
	// Sword position
	s16 mx = 0; s16 my = 0;
	
	// If S is pressed and the player isn't attacking
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !m_isAttacking) {
		// Update attack state
		m_isAttacking = true;
		
		// Block commands
		m_blockedCommands = true;
	}
	
	// If S isn't pressed
	if(!sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		// Reset sword loading timer
		swordLoading.stop();
		swordLoading.reset();
		
		// Deblock directions
		m_blockedDirections = false;
	}
	
	// If the player attacks
	if(m_isAttacking) {
		// If the animation is at end
		if(animationAtEnd(m_direction + 4)) {
			// Stop attacking is S is released
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				m_isAttacking = false;
				
				// Deblock commands
				m_blockedCommands = false;
				
				// Reset sword loading timer
				swordLoading.stop();
				swordLoading.reset();
				
				// Deblock directions
				m_blockedDirections = false;
			}
			else if(swordLoading.time() == 0) {
				// Start sword loading timer
				swordLoading.start();
				
				// Deblock commands
				m_blockedCommands = false;
				
				// Block directions
				m_blockedDirections = true;
			}
		}
		
		if(swordLoading.time() == 0) {
			// Play attack animation
			playAnimation(m_x, m_y, m_direction + 4);
			
			// Get sword position
			switch(m_direction) {
				case Direction::Down:
					if(m_swordSpr->animationAtFrame(m_direction, 0))	  { mx = -12;	my = 0;  }
					else if(m_swordSpr->animationAtFrame(m_direction, 1)) { mx = -12;	my = 12; }
					else if(m_swordSpr->animationAtFrame(m_direction, 2)) { mx = 0;		my = 16; }
					else if(m_swordSpr->animationAtFrame(m_direction, 3)) { mx = 0;		my = 16; }
					break;
				case Direction::Right:
					if(m_swordSpr->animationAtFrame(m_direction, 0))	  { mx = 0;		my = -12; }
					else if(m_swordSpr->animationAtFrame(m_direction, 1)) { mx = 12;	my = -12; }
					else if(m_swordSpr->animationAtFrame(m_direction, 2)) { mx = 12;	my = 0;   }
					else if(m_swordSpr->animationAtFrame(m_direction, 3)) { mx = 12;	my = 0;   }
					break;
				case Direction::Left:
					if(m_swordSpr->animationAtFrame(m_direction, 0))	  { mx = 0;		my = -12; }
					else if(m_swordSpr->animationAtFrame(m_direction, 1)) { mx = -12;	my = -12; }
					else if(m_swordSpr->animationAtFrame(m_direction, 2)) { mx = -12;	my = 0;   }
					else if(m_swordSpr->animationAtFrame(m_direction, 3)) { mx = -12;	my = 0;   }
					break;
				case Direction::Up:	
					if(m_swordSpr->animationAtFrame(m_direction, 0))	  { mx = 12;	my = 0;  }
					else if(m_swordSpr->animationAtFrame(m_direction, 1)) { mx = 12;	my = -12; }
					else if(m_swordSpr->animationAtFrame(m_direction, 2)) { mx = 0;		my = -12; }
					else if(m_swordSpr->animationAtFrame(m_direction, 3)) { mx = 0;		my = -12; }
					break;
				default: break;
			}
			
			// Play sword animation
			m_swordSpr->playAnimation(m_x + mx, m_y + my, m_direction);
		} else {
			// Get sword position
			switch(m_direction) {
				case Direction::Down:	mx = 0;		my = 16;	break;
				case Direction::Right:	mx = 12;	my = 0;		break;
				case Direction::Left:	mx = -12;	my = 0;		break;
				case Direction::Up:		mx = 0;		my = -12;	break;
				default: break;
			}
			
			// Draw sword
			m_swordSpr->drawFrame(m_x + mx, m_y + my, m_direction + 8);
		}
		
		// Test if sword collided a monster
		if((!passable(m_swordSpr, m_swordSpr->x() +  2, m_swordSpr->y() +  2)
		||  !passable(m_swordSpr, m_swordSpr->x() + 14, m_swordSpr->y() +  2)
		||  !passable(m_swordSpr, m_swordSpr->x() +  2, m_swordSpr->y() + 14)
		||  !passable(m_swordSpr, m_swordSpr->x() + 14, m_swordSpr->y() + 14))
		&& m_swordSpr->collidedSprite && m_swordSpr->collidedSprite->isMonster()) {
			// Hurt monster
			m_swordSpr->collidedSprite->hurt();
			
			// Move it
			m_swordSpr->collidedSprite->updatePosition();
		}
	}
}

void Player::actions() {
	if(!m_blockedCommands) {
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			// Set vertical movement vector negative
			m_vy = -1;
			
			// If all other directional keys are released
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				// Set direction to up
				if(!m_blockedDirections) m_direction = Direction::Up;
			}
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			// Set vertical movement vector positive
			m_vy = 1;
			
			// If all other directional keys are released
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				// Set direction to down
				if(!m_blockedDirections) m_direction = Direction::Down;
			}
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			// Set horizontal movement vector negative
			m_vx = -1;
			
			// If all other directional keys are released
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				// Set direction to left
				if(!m_blockedDirections) m_direction = Direction::Left;
			}
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			// Set horizontal movement vector positive
			m_vx = 1;
			
			// If all other directional keys are released
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				// Set direction to right
				if(!m_blockedDirections) m_direction = Direction::Right;
			}
		}
	}
	
	// Sword attack
	sword();
	
	// Test collisions
	doorCollisions();
	testCollisions();
	
	// If player collided a monster, hurt him
	hurt();
	
	// Move the player
	m_x += m_vx * PLAYER_SPEED;
	m_y += m_vy * PLAYER_SPEED;
	
	// Reset movement vectors
	m_vx = 0;
	m_vy = 0;
	
	// If A is pressed, and the player collided a NPC, talk to him
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && collidedSprite && collidedSprite->isNPC()) {
		collidedSprite->speak();
	}
}

void Player::render() {
	// If all directional keys are released
	if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		// Render a single frame
		if(!m_isAttacking || swordLoading.time() != 0) drawFrame(m_x, m_y, m_direction);
	} else {
		// Play walk animation
		if(!m_isAttacking || swordLoading.time() != 0) playAnimation(m_x, m_y, m_direction);
	}
}

