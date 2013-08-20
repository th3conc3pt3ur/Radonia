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
#include "characterManager.h"
#include "collisionManager.h"
#include "tools.h"
#include "interface.h"
#include "game.h"

// Initialize collision matrix
u16 CollisionManager::collisionMatrix[6][4] = {
	//{12,8,12,14},		// Right
	//{3,8,3,14},		// Left
	//{5,5,10,5},		// Up
	//{5,15,10,15},		// Down
	//{12,8,12,10},		// Special right (mobs)
	//{3,8,3,10}		// Special left (mobs)
	// FIXME: Adapt for taller monsters
	{12,8,12,14},	// Right
	{3,8,3,14},		// Left
	{5,5,10,5},		// Up
	{5,15,10,15},	// Down
	{12,8,12,10},	// Special right (mobs)
	{3,8,3,10}		// Special left (mobs)
};

bool CollisionManager::passable(s16 x, s16 y) {
	// Collisions with map
	return !inTable(MapManager::nonPassableTiles, MapManager::currentMap->tilesetInfo()[MapManager::currentMap->getTile(x >> 4, y >> 4)]);
}

bool CollisionManager::collidesWithCharacter(Character *c, u8 i) {
	u16 x1 = c->x() + collisionMatrix[(i + 2) ^ 6][0];
	u16 y1 = c->y() + collisionMatrix[(i + 2) ^ 6][1];
	u16 x2 = c->x() + collisionMatrix[(i + 2) ^ 6][2];
	u16 y2 = c->y() + collisionMatrix[(i + 2) ^ 6][3];
	
	for(std::vector<Character*>::iterator it = MapManager::currentMap->characters()->begin() ; it != MapManager::currentMap->characters()->end() ; it++) {
		if((((*it)->x() < x1 && (*it)->x() + (*it)->frameWidth() > x1
		&&   (*it)->y() < y1 && (*it)->y() + (*it)->frameHeight() > y1)
		||	((*it)->x() < x2 && (*it)->x() + (*it)->frameWidth() > x2
		&&   (*it)->y() < y2 && (*it)->y() + (*it)->frameHeight() > y2))
		&& c->id() != (*it)->id()) {
			return true;
		}
	}
	
	return false;
}

