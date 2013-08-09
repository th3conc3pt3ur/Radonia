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
#include "tools.h"
#include "game.h"

// Monsters are called Qaewans

Monster::Monster(u16 x, u16 y, u8 direction, u16 mapID, MonsterType type, char *filename) : Character(filename, CHARA_MONSTER, x, y, direction, mapID) {
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

