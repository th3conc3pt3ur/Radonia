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

Player::Player() : Character((char*)"graphics/characters/link.png", CHARA_PLAYER, 120, 160, Direction::Right) {
	// Set class members
	/*m_swordSpr = new Weapon((char*)"graphics/animations/sword.png", PLAYER_WEAPON);
	
	m_swordSpr->addAnimation(4, Sword_animations[0], 100); // Down
	m_swordSpr->addAnimation(4, Sword_animations[1], 100); // Right
	m_swordSpr->addAnimation(4, Sword_animations[2], 100); // Left
	m_swordSpr->addAnimation(4, Sword_animations[3], 100); // Up
	*/
	
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
	//delete m_swordSpr;
}

void Player::move() {
	if(m_canMove) {
		// Reset moving state
		m_moving = false;
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			// Set vertical movement vector negative
			m_vy = -1;
			m_moving = true;
			
			// If all other directional keys are released
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				// Set direction to up
				if(m_canTurn) m_direction = Direction::Up;
			}
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			// Set vertical movement vector positive
			m_vy = 1;
			m_moving = true;
			
			// If all other directional keys are released
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				// Set direction to down
				if(m_canTurn) m_direction = Direction::Down;
			}
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			// Set horizontal movement vector negative
			m_vx = -1;
			m_moving = true;
			
			// If all other directional keys are released
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				// Set direction to left
				if(m_canTurn) m_direction = Direction::Left;
			}
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			// Set horizontal movement vector positive
			m_vx = 1;
			m_moving = true;
			
			// If all other directional keys are released
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				// Set direction to right
				if(m_canTurn) m_direction = Direction::Right;
			}
		}
	}
	
	// Test collisions
	CollisionManager::doorCollisions(this);
	CollisionManager::testCollisions(this);
	
	if(m_collidedCharacter && m_collidedCharacter->isMonster() && isPlayer()) {
		hurt();
	}
	
	// Move character
	m_x += m_vx * CHARACTER_SPEED;
	m_y += m_vy * CHARACTER_SPEED;
	
	// Reset movement vectors
	m_vx = 0;
	m_vy = 0;
}

// Sword loading timer
Timer swordLoading;

/*void Player::sword() {
	TODO: Move that to Weapon
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
*/

