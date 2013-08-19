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
#include "mapManager.h"
#include "doorManager.h"
#include "animationManager.h"
#include "characterManager.h"
#include "collisionManager.h"
#include "tools.h"
#include "interface.h"
#include "game.h"

u16 Character::nbCharacters = 0;

Character::Character(char *filename, CharacterType type, s16 x, s16 y, CharacterDirection direction, u16 mapID, u16 area, u8 frameSize) : Sprite(filename, frameSize) {
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
	
	m_dx = x;
	m_dy = y;
	
	m_vx = 0;
	m_vy = 0;
	
	m_collidedCharacter = NULL;
	m_collidedTile = 0;
	
	m_direction = direction;
	
	m_mapID = mapID;
	m_area = area;
	
	m_moving = false;
	
	m_movementTimer.reset();
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
	
	// Load grass image
	m_grassEffect = new Image((char*)"graphics/animations/grassEffect.png");
	
	// Load water effect
	m_waterEffect = new Sprite((char*)"graphics/animations/waterEffect.png");
	m_waterEffect->addAnimation(3, WaterEffect_animations, 150);
}

Character::~Character() {
	// Unload all images
	delete m_grassEffect;
	delete m_waterEffect;
}

void Character::move() {
	// If the character is dead, don't move it
	if(m_lifes <= 0) return;
	
	// Move or not
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
	testCollisions();
	
	// Move character
	m_x += m_vx * CHARACTER_SPEED;
	m_y += m_vy * CHARACTER_SPEED;
	
	// Reset movement vectors
	m_vx = 0;
	m_vy = 0;
}

void Character::render() {
	// If the character is dead, don't display it
	if(m_lifes <= 0) return;
	
	if(!m_isAttacking) {
		if(m_moving) playAnimation(m_x, m_y, m_direction);
		else		 drawFrame(m_x, m_y, m_direction);
	}
	
	// Test if character is in grass
	if(inZone(m_x, m_y, 2)) {
		m_grassEffect->setPosRect(m_x, m_y, m_grassEffect->width(), m_grassEffect->height());
		m_grassEffect->render();
	}
	
	// Test if character is in water
	if(inZone(m_x, m_y, 19)) {
		m_waterEffect->playAnimation(m_x, m_y + 8, 0);
	}
}

void Character::testCollisions() {
	// 0: Right | 1: Left | 2: Up | 3:Down
	for(u8 i = 0 ; i < 4 ; i++) {
		if(((i==0)?(m_vx > 0):((i==1)?(m_vx < 0):((i==2)?(m_vy < 0):(m_vy > 0))))
		&& (!CollisionManager::passable(m_x + CollisionManager::collisionMatrix[i][0], m_y + CollisionManager::collisionMatrix[i][1])
		 || !CollisionManager::passable(m_x + CollisionManager::collisionMatrix[i][2], m_y + CollisionManager::collisionMatrix[i][3])
		 || CollisionManager::collidesWithCharacter(this))) {
			// Reset movement vector
			if(i<2) m_vx = 0;
			else	m_vy = 0;
			
			// Stop movement timer
			m_movementTimer.stop();
			
			// Obstacles
			if( CollisionManager::passable(m_x + CollisionManager::collisionMatrix[i][2], m_y + CollisionManager::collisionMatrix[i][3])
			&& !CollisionManager::passable(m_x + CollisionManager::collisionMatrix[i][0], m_y + CollisionManager::collisionMatrix[i][1])) {
				if(((i<2)?(m_vy == 0):(m_vx == 0))) {
					if(i<2)	m_vy = 1;
					else	m_vx = 1;
				}
			}
			if( CollisionManager::passable(m_x + CollisionManager::collisionMatrix[i][0], m_y + CollisionManager::collisionMatrix[i][1])
			&& !CollisionManager::passable(m_x + CollisionManager::collisionMatrix[i][2], m_y + CollisionManager::collisionMatrix[i][3])) {
				if(((i<2)?(m_vy == 0):(m_vx == 0))) {
					if(i<2) m_vy = -1;
					else	m_vx = -1;
				}
			}
		}
	}
}

void Character::hurt() {
}

void Character::reset() {
	// Reset class members
	m_hurtTimer.reset();
	m_hurtTimer.start();
	
	m_timerLastValue = 0;
	
	m_x = m_dx;
	m_y = m_dy;
	
	m_vx = 0;
	m_vy = 0;
	
	m_collidedCharacter = NULL;
	m_collidedTile = 0;
	
	m_moving = false;
	
	m_movementTimer.reset();
	m_movementTimer.start();
	
	m_countMoves = 0;
	m_vxCount = 0;
	m_vyCount = 0;
	
	m_isAttacking = false;
	
	m_canMove = true;
	m_canTurn = true;
	
	m_lifes = m_maxLifes;
}

