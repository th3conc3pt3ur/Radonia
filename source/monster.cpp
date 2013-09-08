/*---------------------------------------------------------------------------------
	
	Radonia
	Copyright (C) 2013-2014 Deloptia <deloptia.devteam@gmail.com>
	
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

#include "includeSDL.h"

#include "types.h"
#include "color.h"
#include "config.h"
#include "window.h"
#include "keyboard.h"
#include "font.h"
#include "sound.h"
#include "net.h"
#include "timer.h"
#include "image.h"
#include "animation.h"
#include "sprite.h"
#include "weapon.h"
#include "character.h"
#include "monster.h"
#include "NPC.h"
#include "player.h"
#include "map.h"
#include "door.h"
#include "animationManager.h"
#include "characterManager.h"
#include "mapManager.h"
#include "tools.h"
#include "game.h"

// Monsters are called Qaewans

Monster::Monster(u16 x, u16 y, CharacterDirection direction, u16 mapID, u16 area, MonsterType type, const char *filename, u8 frameWidth, u8 frameHeight) : Character(filename, CHARA_MONSTER, x, y, direction, mapID, area, frameWidth, frameHeight) {
	// Sub type
	m_subType = type;
	
	// Add animations to sprite
	addAnimation(2, Monster_animations[0], 250); // Down
	addAnimation(2, Monster_animations[1], 250); // Right
	addAnimation(2, Monster_animations[2], 250); // Left
	addAnimation(2, Monster_animations[3], 250); // Up
}

Monster::~Monster() {
}

void Monster::move() {
	// FIXME: To improve
	bool isAttacked = m_lifes < m_maxLifes;
	//bool isAttacked = true;
	
	if(isAttacked) {
		// Get player center
		u16 x = CharacterManager::player()->x() + CharacterManager::player()->frameWidth()  / 2;
		u16 y = CharacterManager::player()->y() + CharacterManager::player()->frameHeight() / 2;
		
		// Get monster center
		u16 cx = m_x + m_frameWidth / 2;
		u16 cy = m_y + m_frameHeight / 2;
		
		// Get distance between the two characters
		s16 e_x = x - cx;
		s16 e_y = y - cy;
		
		// The monster follows the character
		if(abs(e_x) < (10 * TILE_SIZE) && abs(e_y < (10 * TILE_SIZE))) {
			// Make sure movement vectors are reset
			m_vx = 0;
			m_vy = 0;
			
			// Update movement vectors
			if(SDL_GetTicks()&1) {
				if(x > cx)		m_vx =  1;
				else if(x < cx) m_vx = -1;
				if(y > cy)		m_vy =	1;
				else if(y < cy) m_vy = -1;
			}
			
			// Update moving state
			m_moving = true;
			
			// Reset movement timer
			m_movementTimer.reset();
			
			// Test collisions
			testCollisions();
			
			// Set character direction
			if(e_x > 0) m_direction = DIR_RIGHT;
			if(e_x < 0) m_direction = DIR_LEFT;
			if(e_y > 0) m_direction = DIR_DOWN;
			if(e_y < 0) m_direction = DIR_UP;
		} else {
			// Reset is attacked state
			isAttacked = false;
		}
	}
	
	if(!isAttacked) {
		// FIXME: To improve
		// Area to walk in
		u16 rectW = 4;
		u16 rectH = 4;
		u16 minX = m_dx;
		u16 minY = m_dy;
		u16 maxX = m_dx + rectW * TILE_SIZE;
		u16 maxY = m_dy + rectH * TILE_SIZE;
		
		// If it's time to move
		if(m_movementTimer.time() > 4000) {
			// Make sure movement vectors are reset
			m_vx = 0;
			m_vy = 0;
			
			// Initialize random number
			int randn;
			
			// Initialize in movement boolean
			bool mvt = false;
			while(!mvt) {
				// Get a random number between 0 and 4
				randn = rand()%4;
				
				switch(randn) {
					case DIR_UP:
						if(m_y - m_frameHeight > minY) {
							m_vy = -1;
							mvt = true;
						} break;
					case DIR_DOWN:
						if(m_y + m_frameHeight < maxY) {
							m_vy = 1;
							mvt = true;
						} break;
					case DIR_LEFT:
						if(m_x - m_frameWidth > minX) {
							m_vx = -1;
							mvt = true;
						} break;
					case DIR_RIGHT:
						if(m_x + m_frameWidth < maxX) {
							m_vx = 1;
							mvt = true;
						} break;
					default: break;
				}
			}
			
			// Update moving state
			m_moving = true;
			
			// Reset movement timer
			m_movementTimer.reset();
		}
		
		// Update counters
		m_vxCount += m_vx * m_vx;
		m_vyCount += m_vy * m_vy;
		
		// Test collisions
		testCollisions();
		
		// If the movement is finished or a collision is detected
		if(m_vxCount >= 16 || m_vyCount >= 16 || (m_moving && m_vx == 0 && m_vy == 0)) {
			// Reset counters
			m_vxCount = 0;
			m_vyCount = 0;
			
			// Reset movement timer
			m_movementTimer.reset();
			m_movementTimer.start();
			
			// Update moving state
			m_moving = false;
			
			// Reset movement vectors once the movement is finished
			m_vx = 0;
			m_vy = 0;
		}
		
		// Set character direction
		if(m_vx > 0) m_direction = DIR_RIGHT;
		if(m_vx < 0) m_direction = DIR_LEFT;
		if(m_vy > 0) m_direction = DIR_DOWN;
		if(m_vy < 0) m_direction = DIR_UP;
	}
	
	// Move character
	m_x += m_vx * CHARACTER_SPEED;
	m_y += m_vy * CHARACTER_SPEED;
}

