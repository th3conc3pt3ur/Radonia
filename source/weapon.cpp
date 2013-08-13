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

/*
Weapon::Weapon(char *filename, WeaponType type, Character *owner) : Sprite(filename) {
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
	s16 mx = 0; s16 my = 0;
	
	// If S is pressed and the player isn't attacking
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !m_owner->m_isAttacking) {
		// Update attack state
		m_owner->m_isAttacking = true;
		
		// Block commands
		m_owner->m_canMove = false;
	}
	
	// If S isn't pressed
	if(!sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		// Reset sword loading timer
		m_loadingTimer.stop();
		m_loadingTimer.reset();
		
		// Deblock directions
		m_owner->m_canMove = true;
	}
	
	// If the player attacks
	if(m_owner->m_isAttacking) {
		// If the animation is at end
		if(m_owner->animationAtEnd(m_owner->m_direction + 4)) {
			// Stop attacking is S is released
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				m_owner->m_isAttacking = false;
				
				// Deblock movements
				m_owner->m_canMove = true;
				m_owner->m_canTurn = true;
				
				// Reset sword loading timer
				m_loadingTimer.stop();
				m_loadingTimer.reset();
			}
			else if(m_loadingTimer.time() == 0) {
				// Start sword loading timer
				m_loadingTimer.start();
				
				// Block movements
				m_owner->m_canMove = false;
				m_owner->m_canTurn = false;
			}
		}
		
		if(m_loadingTimer.time() == 0) {
			// Play attack animation
			playAnimation(m_x, m_y, m_direction + 4);
			
			// Get sword position
			switch(m_direction) {
				case Direction::Down:
					if(animationAtFrame(m_direction, 0))	  { mx = -12;	my = 0;  }
					else if(animationAtFrame(m_direction, 1)) { mx = -12;	my = 12; }
					else if(animationAtFrame(m_direction, 2)) { mx = 0;		my = 16; }
					else if(animationAtFrame(m_direction, 3)) { mx = 0;		my = 16; }
					break;
				case Direction::Right:
					if(animationAtFrame(m_direction, 0))	  { mx = 0;		my = -12; }
					else if(animationAtFrame(m_direction, 1)) { mx = 12;	my = -12; }
					else if(animationAtFrame(m_direction, 2)) { mx = 12;	my = 0;   }
					else if(animationAtFrame(m_direction, 3)) { mx = 12;	my = 0;   }
					break;
				case Direction::Left:
					if(animationAtFrame(m_direction, 0))	  { mx = 0;		my = -12; }
					else if(animationAtFrame(m_direction, 1)) { mx = -12;	my = -12; }
					else if(animationAtFrame(m_direction, 2)) { mx = -12;	my = 0;   }
					else if(animationAtFrame(m_direction, 3)) { mx = -12;	my = 0;   }
					break;
				case Direction::Up:	
					if(animationAtFrame(m_direction, 0))	  { mx = 12;	my = 0;  }
					else if(animationAtFrame(m_direction, 1)) { mx = 12;	my = -12; }
					else if(animationAtFrame(m_direction, 2)) { mx = 0;		my = -12; }
					else if(animationAtFrame(m_direction, 3)) { mx = 0;		my = -12; }
					break;
				default: break;
			}
			
			// Play sword animation
			playAnimation(m_x + mx, m_y + my, m_direction);
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
			drawFrame(m_x + mx, m_y + my, m_direction + 8);
		}
		
		// Test if sword collided a monster
		if((!passable(this, m_x +  2, m_y +  2)
		||  !passable(this, m_x + 14, m_y +  2)
		||  !passable(this, m_x +  2, m_y + 14)
		||  !passable(this, m_x + 14, m_y + 14))
		&& m_owner->collidedCharacter() && m_owner->collidedCharacter()->isMonster()) {
			// Hurt monster
			m_owner->collidedCharacter()->hurt();
			
			// Move it
			m_owner->collidedCharacter()->move();
		}
	}
}
*/
