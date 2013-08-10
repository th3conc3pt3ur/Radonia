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
#include "mapManager.h"
#include "doorManager.h"
#include "characterManager.h"
#include "collisionManager.h"
#include "tools.h"
#include "interface.h"
#include "game.h"

u16 Character::nbCharacters = 0;

Character::Character(char *filename, CharacterType type, s16 x, s16 y, CharacterDirection direction, s16 mapID, u8 frameSize) : Sprite(filename, frameSize) {
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
	//m_collidedWeapon = NULL;
	m_collidedTile = 0;
	
	m_direction = direction;
	
	m_mapID = mapID;
	
	m_moving = false;
	
	m_movementTimer.start();
	
	m_countMoves = 0;
	m_vxCount = 0;
	m_vyCount = 0;
	
	m_isAttacking = false;
	
	m_canMove = true;
	m_canTurn = true;
	
	// FIXME: TEMP
	m_lifes = 27;
	m_maxLifes = 32;
}

Character::~Character() {
}

void Character::move() {
	// If player collided monster, don't move
	if(((m_x + 2 > Game::player->x() && m_x + 2 < Game::player->x() + 16)
	&&  (m_y + 2 > Game::player->y() && m_y + 2 < Game::player->y() + 16))
	|| ((m_x + 14 > Game::player->x() && m_x + 14 < Game::player->x() + 16)
	&&  (m_y + 2 > Game::player->y() && m_y + 2 < Game::player->y() + 16))
	|| ((m_x + 2 > Game::player->x() && m_x + 2 < Game::player->x() + 16)
	&&  (m_y + 14 > Game::player->y() && m_y + 14 < Game::player->y() + 16))
	|| ((m_x + 14 > Game::player->x() && m_x + 14 < Game::player->x() + 16)
	&&  (m_y + 14 > Game::player->y() && m_y + 14 < Game::player->y() + 16))) {
		m_movementTimer.stop();
		Game::player->collidedCharacter(this);
		return;
	}
	else m_movementTimer.start();
	
	// Move or not?
	if(m_movementTimer.time() > 4000) {
		// Update movement vectors
		m_vx = CharacterManager::moves[m_id][m_countMoves * 2 + 1];
		m_vy = CharacterManager::moves[m_id][m_countMoves * 2 + 2];
		
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
		m_movementTimer.reset();
		m_movementTimer.start();
		
		// Update moving state
		m_moving = false;
	}
	
	if(m_countMoves >= CharacterManager::moves[m_id][0]) {
		// Reset timer and counter
		m_countMoves = 0;
		m_movementTimer.reset();
		m_movementTimer.start();
	}
	
	// Set character direction
	if(m_vx > 0) m_direction = DIR_RIGHT;
	if(m_vx < 0) m_direction = DIR_LEFT;
	if(m_vy > 0) m_direction = DIR_DOWN;
	if(m_vy < 0) m_direction = DIR_UP;
	
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

void Character::render() {
	if(m_moving) playAnimation(m_x, m_y, m_direction);
	else drawFrame(m_x, m_y, m_direction);
}

void Character::hurt() {
	if(m_hurtTimer.time() - m_timerLastValue > 5) {
		if(isPlayer()) {
			// Block comamnds
			m_canMove = false;
		} else {
			// Stop monster movement
			m_movementTimer.stop();
		}
		
		// Change sprite texture
		sf::Color c = m_spr.getColor();
		m_spr.setColor(invertColor(c));
		
		// Get character direction vectors
		s8 e_x = m_x - m_collidedCharacter->x();
		s8 e_y = m_y - m_collidedCharacter->y();
		
		// Set movement vectors
		if(!m_collidedTile && abs(e_x) > 8) m_vx = (e_x==0)?0:((e_x<0)?-2:2);
		if(!m_collidedTile && abs(e_y) > 8) m_vy = (e_y==0)?0:((e_y<0)?-2:2);
		
		// Temporary collision states
		Character *tmpCollidedCharacter = m_collidedCharacter;
		//Weapon *tmpCollidedWeapon = m_collidedWeapon;
		
		// Reset collision states
		m_collidedCharacter = NULL;
		//m_collidedWeapon = NULL;
		
		// Setup temporary movement vectors
		s8 t_vx = m_vx;
		s8 t_vy = m_vy;
		
		// Test collisions
		CollisionManager::testCollisions(this);
		
		// Reset movement vectors
		m_vx = t_vx;
		m_vy = t_vy;
		
		// Reset collision states with temp values
		m_collidedCharacter = tmpCollidedCharacter;
		//m_collidedWeapon = tmpCollidedWeapon;
		
		// Reset timer last value
		m_timerLastValue = m_hurtTimer.time();
		
		// Reset collided sprite and blocked commands states
		if(abs(e_x) > 32 || abs(e_y) > 32 || m_collidedTile) {
			m_collidedCharacter = NULL;
			//m_collidedWeapon = NULL;
			if(isPlayer()) m_canMove = true;
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
}

