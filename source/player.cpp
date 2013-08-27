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
#include "animationManager.h"
#include "mapManager.h"
#include "doorManager.h"
#include "characterManager.h"
#include "collisionManager.h"
#include "weaponManager.h"
#include "tools.h"
#include "interface.h"
#include "game.h"

Player::Player(u16 x, u16 y, CharacterDirection direction, u16 mapID, u16 area) : Character((char*)"graphics/characters/link.png", CHARA_PLAYER, x, y, direction, mapID, area) {
	// Initialize weapon
	m_weapon = new Sword(this);
	
	// Add animations to sprite
	addAnimation(2, Player_animations[0], 125); // Down
	addAnimation(2, Player_animations[1], 125); // Right
	addAnimation(2, Player_animations[2], 125); // Left
	addAnimation(2, Player_animations[3], 125); // Up
	addAnimation(4, Player_animations[4], 100); // Attack down
	addAnimation(4, Player_animations[5], 100); // Attack right
	addAnimation(4, Player_animations[6], 100); // Attack left
	addAnimation(4, Player_animations[7], 100); // Attack up
}

Player::~Player() {
}

void Player::move() {
	if(m_hurt) {
		// Hurt player
		hurtMovement();
	}
	else if(m_canMove) {
		// Reset moving state
		m_moving = false;
		
		if(Keyboard::isKeyPressed(Keyboard::GameUp)) {
			// Set vertical movement vector negative
			if(!Keyboard::isKeyPressed(Keyboard::GameDown)) m_vy = -1;
			m_moving = true;
			
			// If all other directional keys are released
			if(!Keyboard::isKeyPressed(Keyboard::GameLeft) && !Keyboard::isKeyPressed(Keyboard::GameRight) && !Keyboard::isKeyPressed(Keyboard::GameDown)) {
				// Set direction to up
				if(m_canTurn) m_direction = DIR_UP;
			}
		}
		
		if(Keyboard::isKeyPressed(Keyboard::GameDown)) {
			// Set vertical movement vector positive
			if(!Keyboard::isKeyPressed(Keyboard::GameUp)) m_vy = 1;
			m_moving = true;
			
			// If all other directional keys are released
			if(!Keyboard::isKeyPressed(Keyboard::GameLeft) && !Keyboard::isKeyPressed(Keyboard::GameRight) && !Keyboard::isKeyPressed(Keyboard::GameUp)) {
				// Set direction to down
				if(m_canTurn) m_direction = DIR_DOWN;
			}
		}
		
		if(Keyboard::isKeyPressed(Keyboard::GameLeft)) {
			// Set horizontal movement vector negative
			if(!Keyboard::isKeyPressed(Keyboard::GameRight)) m_vx = -1;
			m_moving = true;
			
			// If all other directional keys are released
			if(!Keyboard::isKeyPressed(Keyboard::GameUp) && !Keyboard::isKeyPressed(Keyboard::GameRight) && !Keyboard::isKeyPressed(Keyboard::GameDown)) {
				// Set direction to left
				if(m_canTurn) m_direction = DIR_LEFT;
			}
		}
		
		if(Keyboard::isKeyPressed(Keyboard::GameRight)) {
			// Set horizontal movement vector positive
			if(!Keyboard::isKeyPressed(Keyboard::GameLeft)) m_vx = 1;
			m_moving = true;
			
			// If all other directional keys are released
			if(!Keyboard::isKeyPressed(Keyboard::GameLeft) && !Keyboard::isKeyPressed(Keyboard::GameUp) && !Keyboard::isKeyPressed(Keyboard::GameDown)) {
				// Set direction to right
				if(m_canTurn) m_direction = DIR_RIGHT;
			}
		}
	}
	
	// Test collisions
	testCollisions();
	doorCollisions();
	
	// Move character
	m_x += m_vx * CHARACTER_SPEED;
	m_y += m_vy * CHARACTER_SPEED;
	
	// Reset movement vectors
	m_vx = 0;
	m_vy = 0;
}

