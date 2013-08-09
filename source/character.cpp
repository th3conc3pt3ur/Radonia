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

u16 Character::nbCharacters = 0;

Character::Character(char *filename, CharacterType type, s16 x, s16 y, u8 direction, u16 mapID, u8 frameSize) : Sprite(filename, frameSize) {
	// Set character id
	m_id = nbCharacters;
	
	// Update characters counter
	nbCharacters++;
	
	// Set class members
	m_hurtTimer.reset();
	m_hurtTimer.start();
	
	m_timerLastValue = 0;
	
	m_type = type;
	
	m_x = x;
	m_y = y;
	
	m_vx = 0;
	m_vy = 0;
	
	m_collidedCharacter = NULL;
	m_collidedWeapon = NULL;
	m_collidedTile = 0;
	
	m_direction = direction;
	
	m_mapID = mapID;
	
	m_movementTimer.start();
	
	m_countMoves = 0;
	m_vxCount = 0;
	m_vyCount = 0;
	
	m_isAttacking = 0;
	
	m_canMove = true;
	m_canMove = true;
}

Character::~Character() {
}

void Character::move() {
	/*
	// If player collided NPC, don't move
	if((Game::player->collidedSprite && Game::player->collidedSprite->isPlayer())
	|| ((m_x + 2 > Game::player->x() && m_x + 2 < Game::player->x() + 16)
	&&  (m_y + 2 > Game::player->y() && m_y + 2 < Game::player->y() + 16))
	|| ((m_x + 14 > Game::player->x() && m_x + 14 < Game::player->x() + 16)
	&&  (m_y + 2 > Game::player->y() && m_y + 2 < Game::player->y() + 16))
	|| ((m_x + 2 > Game::player->x() && m_x + 2 < Game::player->x() + 16)
	&&  (m_y + 14 > Game::player->y() && m_y + 14 < Game::player->y() + 16))
	|| ((m_x + 14 > Game::player->x() && m_x + 14 < Game::player->x() + 16)
	&&  (m_y + 14 > Game::player->y() && m_y + 14 < Game::player->y() + 16))) {
		m_timer.stop();
		return;
	}
	else m_timer.start();
	
	// Move or not?
	if(m_timer.time() > 4000) {
		// Update movement vectors
		m_vx = moves[m_id][m_countMoves * 2 + 1];
		m_vy = moves[m_id][m_countMoves * 2 + 2];
		
		// Update counters
		m_vxCount += m_vx * m_vx;
		m_vyCount += m_vy * m_vy;
		
		// Update moving state
		m_moving = true;
	}
	
	if(m_vxCount >= 16 || m_vyCount >= 16) {
		// Update counter
		m_countMoves++;
		
		// Reset counters
		m_vxCount = 0;
		m_vyCount = 0;
		
		// Reset timer
		m_timer.reset();
		m_timer.start();
		
		// Update moving state
		m_moving = false;
	}
	
	if(m_countMoves >= moves[m_id][0]) {
		// Reset timer and counter
		m_countMoves = 0;
		m_timer.reset();
		m_timer.start();
	}
	
	if(m_vx > 0) m_direction = Direction::Right;
	if(m_vx < 0) m_direction = Direction::Left;
	if(m_vy > 0) m_direction = Direction::Down;
	if(m_vy < 0) m_direction = Direction::Up;
	
	// Move NPC
	m_x += m_vx;
	m_y += m_vy;
	
	// Reset movement vectors
	m_vx = 0;
	m_vy = 0;
	// PLAYER
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
	*/
}

void Character::render() {
	/*
	
	// PLAYER
	// If all directional keys are released
	if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		// Render a single frame
		if(!m_isAttacking || swordLoading.time() != 0) drawFrame(m_x, m_y, m_direction);
	} else {
		// Play walk animation
		if(!m_isAttacking || swordLoading.time() != 0) playAnimation(m_x, m_y, m_direction);
	}
	*/
}

void Character::hurt() {
	// TODO with new collision states
	/*if((collidedSprite && (collidedSprite->isPWeapon() && isMonster()))
	|| (collidedSprite && ((collidedSprite->isMWeapon() || collidedSprite->isMonster()) && isPlayer()))) {
		if(m_hurtTimer.time() - m_timerLastValue > 5) {
			if(isPlayer()) {
				// Block comamnds
				m_blockedCommands = true;
			} else {
				// Stop monster movement
				m_movementTimer.stop();
			}
			
			// Change sprite texture
			sf::Color c = m_spr.getColor();
			m_spr.setColor(invertColor(c));
			
			// Get sprite direction vectors
			s8 e_x = m_x - collidedCharacter->x();
			s8 e_y = m_y - collidedCharacter->y();
			
			// Set movement vectors
			if(!collidedTile && abs(e_x) > 8) m_vx = (e_x==0)?0:((e_x<0)?-2:2);
			if(!collidedTile && abs(e_y) > 8) m_vy = (e_y==0)?0:((e_y<0)?-2:2);
			
			// Temporary collision states
			Character *tmpCollidedCharacter = collidedCharacter;
			Weapon *tmpCollidedWeapon = collidedWeapon;
			
			// Reset collision states
			collidedCharacter = NULL;
			collidedWeapon = NULL;
			
			// Setup temporary movement vectors
			s8 t_vx = m_vx;
			s8 t_vy = m_vy;
			
			// Test collisions
			testCollisions();
			
			// Reset movement vectors
			m_vx = t_vx;
			m_vy = t_vy;
			
			// Reset collision states with temp values
			collidedCharacter = tmpCollidedCharacter;
			collidedWeapon = tmpCollidedWeapon;
			
			// Reset timer last value
			m_timerLastValue = m_hurtTimer.time();
			
			// Reset collided sprite and blocked commands states
			if(abs(e_x) > 32 || abs(e_y) > 32 || collidedTile) {
				collidedCharacter = NULL;
				collidedWeapon = NULL;
				if(isPlayer()) m_blockedCommands = false;
				else		   m_movementTimer.start();
				m_spr.setColor(m_defaultColor);
			}
		}
		
		if(m_hurtTimer.time() > 500) {
			// Hurt sprite
			m_lifes--;
			
			// Reset timer
			m_hurtTimer.reset();
			m_hurtTimer.start();
			
			// Reset timer last value
			m_timerLastValue = m_hurtTimer.time();
		}
	}*/
}

bool inDoor = false;
void Character::doorCollisions() {
	/*
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
	*/
}

void Character::testCollisions() {
	// TODO: Collisions manager
	/*// 0: Right | 1: Left | 2: Up | 3:Down
	for(u8 i = 0 ; i < 4 ; i++) {
		if(((i==0)?(m_vx > 0):((i==1)?(m_vx < 0):((i==2)?(m_vy < 0):(m_vy > 0))))
		&& (!passable(this, m_x + collisionMatrix[i][0], m_y + collisionMatrix[i][1])
		 || !passable(this, m_x + collisionMatrix[i][2], m_y + collisionMatrix[i][3]))) {
			// Reset movement vector
			if(i<2) m_vx = 0;
			else	m_vy = 0;
			
			// Obstacles
			if( passable(this, m_x + collisionMatrix[i][2], m_y + collisionMatrix[i][3])
			&& !passable(this, m_x + collisionMatrix[i][0], m_y + collisionMatrix[i][1])) {
				if(((i<2)?(m_vy == 0):(m_vx == 0)) && !collidedCharacter && !collidedWeapon) {
					if(i<2)	m_vy = 1;
					else	m_vx = 1;
				}
			}
			if( passable(this, m_x + collisionMatrix[i][0], m_y + collisionMatrix[i][1])
			&& !passable(this, m_x + collisionMatrix[i][2], m_y + collisionMatrix[i][3])) {
				if(((i<2)?(m_vy == 0):(m_vx == 0)) && !collidedSprite && !collidedWeapon) {
					if(i<2) m_vy = -1;
					else	m_vx = -1;
				}
			}
		}
	}*/
}

