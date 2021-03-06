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
#include "mapManager.h"
#include "doorManager.h"
#include "characterManager.h"
#include "weaponManager.h"
#include "tools.h"
#include "interface.h"
#include "game.h"

Weapon::Weapon(const char *filename, WeaponType type, Character *owner) : Sprite(filename) {
	// Set class members
	m_type = type;
	
	m_owner = owner;
}

Weapon::~Weapon() {
}

Sword::Sword(Character *owner) : Weapon((char*)"graphics/animations/sword.png", WEAPON_MELEE, owner) {
	addAnimation(4, Sword_animations[0], 100); // Down
	addAnimation(4, Sword_animations[1], 100); // Right
	addAnimation(4, Sword_animations[2], 100); // Left
	addAnimation(4, Sword_animations[3], 100); // Up
}

Sword::~Sword() {
}

void Sword::action() {
	// Sword position
	s8 mx = 0; s8 my = 0;
	
	// Player movement
	s8 px = 0; s8 py = 0;
	
	// Get sword position
	switch(m_owner->m_direction) {
		case DIR_DOWN:	mx = 0;		my = 16;	py =  3; break;
		case DIR_RIGHT:	mx = 12;	my = 0;		px =  4; break;
		case DIR_LEFT:	mx = -12;	my = 0;		px = -4; break;
		case DIR_UP:	mx = 0;		my = -12;	py = -3; break;
		default: break;
	}
	
	if(Keyboard::isKeyPressed(Keyboard::GameAttack) || m_owner->m_isAttacking) {
		if(!m_owner->m_isAttacking) {
			// Update owner states
			m_owner->m_isAttacking = true;
			m_owner->m_canMove = false;
			
			// Reset player animation
			m_owner->resetAnimation(m_owner->m_direction + 4);
			m_owner->startAnimation(m_owner->m_direction + 4);
			
			// Reset sword animation
			resetAnimation(m_owner->m_direction);
			startAnimation(m_owner->m_direction);
			
			// Play attack animation
			m_owner->playAnimation(m_owner->m_x + px, m_owner->m_y + py, m_owner->m_direction + 4);
		} else if(!m_owner->animationAtEnd(m_owner->m_direction + 4)) {
			// Play attack animation
			m_owner->playAnimation(m_owner->m_x + px, m_owner->m_y + py, m_owner->m_direction + 4);
	
			// Get sword position
			switch(m_owner->m_direction) {
				case DIR_DOWN:
					if(animationAtFrame(m_owner->m_direction, 0))			{ mx = -12;	my =  0; }
					else if(animationAtFrame(m_owner->m_direction, 1))		{ mx = -12;	my = 12; }
					else if(animationAtFrame(m_owner->m_direction, 2))		{ mx =   0;	my = 16; }
					else if(animationAtFrame(m_owner->m_direction, 3))		{ mx =   0;	my = 16; }
					break;
				case DIR_RIGHT:
					if(animationAtFrame(m_owner->m_direction, 0))			{ mx =  0;	my = -12; }
					else if(animationAtFrame(m_owner->m_direction, 1))		{ mx = 12;	my = -12; }
					else if(animationAtFrame(m_owner->m_direction, 2))		{ mx = 12;	my =   0; }
					else if(animationAtFrame(m_owner->m_direction, 3))		{ mx = 12;	my =   0; }
					break;
				case DIR_LEFT:
					if(animationAtFrame(m_owner->m_direction, 0))			{ mx =   0;	my = -12; }
					else if(animationAtFrame(m_owner->m_direction, 1))		{ mx = -12;	my = -12; }
					else if(animationAtFrame(m_owner->m_direction, 2))		{ mx = -12;	my =   0; }
					else if(animationAtFrame(m_owner->m_direction, 3))		{ mx = -12;	my =   0; }
					break;
				case DIR_UP:	
					if(animationAtFrame(m_owner->m_direction, 0))			{ mx = 12;	my =   0; }
					else if(animationAtFrame(m_owner->m_direction, 1))		{ mx = 12;	my = -12; }
					else if(animationAtFrame(m_owner->m_direction, 2))		{ mx =  0;	my = -12; }
					else if(animationAtFrame(m_owner->m_direction, 3))		{ mx =  0;	my = -12; }
					break;
				default: break;
			}
			
			// Play sword animation
			if(!animationAtEnd(m_owner->m_direction)) playAnimation(m_owner->m_x + mx + px, m_owner->m_y + my + py, m_owner->m_direction);
		} else if(m_timer.time() == 0) {
			// Start loading timer
			m_timer.start();
			
			// Update owner states
			m_owner->m_canMove = true;
			m_owner->m_canTurn = false;
			
			// Play attack animation
			m_owner->drawFrame(m_owner->m_x, m_owner->m_y, m_owner->m_direction);
			
			// Play sword animation
			drawFrame(m_owner->m_x + mx, m_owner->m_y + my, m_owner->m_direction + 8);
		} else {
			// If player don't move draw frame, else, play animation
			if(m_owner->m_moving) {
				m_owner->playAnimation(m_owner->m_x, m_owner->m_y, m_owner->m_direction);
			} else {
				m_owner->drawFrame(m_owner->m_x, m_owner->m_y, m_owner->m_direction);
			}
			
			// Draw sword
			drawFrame(m_owner->m_x + mx, m_owner->m_y + my, m_owner->m_direction + 8);
		}
		
		/*// Test if sword collided a character
		Character *collidedCharacter = NULL;
		if((m_collidedCharacter
		|| (((collidedCharacter = CollisionManager::getCollidedCharacter(m_owner->m_x + mx +  2, m_owner->m_y + my +  2, m_owner))
		||   (collidedCharacter = CollisionManager::getCollidedCharacter(m_owner->m_x + mx + 14, m_owner->m_y + my +  2, m_owner))
		||   (collidedCharacter = CollisionManager::getCollidedCharacter(m_owner->m_x + mx +  2, m_owner->m_y + my + 14, m_owner))
		||   (collidedCharacter = CollisionManager::getCollidedCharacter(m_owner->m_x + mx + 14, m_owner->m_y + my + 14, m_owner)))
		&& collidedCharacter))) {
			// Set collided character
			if(collidedCharacter) m_collidedCharacter = collidedCharacter;
			m_collidedCharacter->m_collidedCharacter = m_owner;
			
			// Hurt character
			m_collidedCharacter->hurt();
		}
		
		if(m_collidedCharacter && m_collidedCharacter->m_collidedCharacter == m_owner) {
			// Hurt character
			m_collidedCharacter->hurt();
		} else {
			// Reset collided character state
			m_collidedCharacter = NULL;
		}*/
	}
	
	if(!Keyboard::isKeyPressed(Keyboard::GameAttack) && m_owner->m_isAttacking && m_owner->animationAtEnd(m_owner->m_direction + 4)) {
		// Reset timer
		m_timer.reset();
		
		// Reset owner states
		m_owner->m_isAttacking = false;
		m_owner->m_canMove = true;
		m_owner->m_canTurn = true;
	}
}

