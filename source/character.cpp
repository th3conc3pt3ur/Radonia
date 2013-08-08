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

Character::Character(char *filename, CharacterType type, s16 x, s16 y, u8 frameSize) : Sprite(filename, frameSize) {
	// Set character id
	m_id = nbCharacters;
	
	// Update characters counter
	nbCharacters++;
	
	// Setup hurt timer
	m_hurtTimer.reset();
	m_hurtTimer.start();
	
	m_timerLastValue = 0;
	
	// Set type
	m_type = type;
	
	// Set position
	m_x = x;
	m_y = y;
	
	// Set movement vectors
	m_vx = 0;
	m_vy = 0;
	
	// Initialize collision states
	m_collidedCharacter = NULL;
	m_collidedWeapon = NULL;
	m_collidedTile = 0;
}

Character::~Character() {
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

