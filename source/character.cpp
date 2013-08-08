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

