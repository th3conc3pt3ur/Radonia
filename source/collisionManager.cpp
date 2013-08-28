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
#include "mapManager.h"
#include "doorManager.h"
#include "characterManager.h"
#include "collisionManager.h"
#include "tools.h"
#include "interface.h"
#include "game.h"

// Initialize collision matrix
u16 CollisionManager::collisionMatrix[6][4] = {
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
	u16 x1 = c->x() + COLLISION_MATRIX((i + 2) ^ 6, 0, c->frameWidth());
	u16 y1 = c->y() + COLLISION_MATRIX((i + 2) ^ 6, 1, c->frameHeight());
	u16 x2 = c->x() + COLLISION_MATRIX((i + 2) ^ 6, 2, c->frameWidth());
	u16 y2 = c->y() + COLLISION_MATRIX((i + 2) ^ 6, 3, c->frameHeight());
	
	for(std::vector<Character*>::iterator it = MapManager::currentMap->characters()->begin() ; it != MapManager::currentMap->characters()->end() ; it++) {
		if((*it)->x() < x2 && (*it)->x() + (*it)->frameWidth()  > x1
		&& (*it)->y() < y2 && (*it)->y() + (*it)->frameHeight() > y1
		&& c->id() != (*it)->id()) {
			// Get characters centers
			u16 cx1 = c->x() + c->frameWidth() / 2;
			u16 cy1 = c->y() + c->frameHeight() / 2;
			
			u16 cx2 = (*it)->x() + (*it)->frameWidth() / 2;
			u16 cy2 = (*it)->y() + (*it)->frameHeight() / 2;
			
			// Test hurt
			if(c->isPlayer() && (*it)->isMonster()) c->hurt(cx1 - cx2, cy1 - cy2);
			if(c->isMonster() && (*it)->isPlayer()) (*it)->hurt(cx2 - cx1, cy2 - cy1);
			
			// Test dialog box
			if(c->isPlayer() && (*it)->isNPC() && Keyboard::isKeyPressed(Keyboard::GameAttack)) ((NPC*)(*it))->action();
			if((*it)->isPlayer() && c->isNPC() && Keyboard::isKeyPressed(Keyboard::GameAttack)) ((NPC*)c)->action();
			
			return true;
		}
	}
	
	return false;
}

