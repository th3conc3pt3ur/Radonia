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
#include "mapManager.h"
#include "doorManager.h"
#include "characterManager.h"
#include "collisionManager.h"
#include "tools.h"
#include "interface.h"
#include "game.h"

// Initialize collision matrix
u16 CollisionManager::collisionMatrix[4][4] = {
	{12,8,12,13},	// Right
	{3,8,3,13},		// Left
	{5,5,10,5},		// Up
	{5,15,10,15}	// Down
};

bool CollisionManager::passable(Character *character, s16 x, s16 y) {
	/*// Setup tileX and tileY
	s16 tileX = x >> 4;
	s16 tileY = y >> 4;
	
	// Reset collision states
	spr->collidedSprite = NULL;
	spr->collidedTile = 0;
	
	// Collisions with NPCs
	for(u16 i = 0 ; i < Game::currentMap->NPCs().size() ; i++) {
		if((Game::currentMap->NPCs()[i]->x() < x && Game::currentMap->NPCs()[i]->x() + 16 > x)
		&& (Game::currentMap->NPCs()[i]->y() < y && Game::currentMap->NPCs()[i]->y() + 16 > y)) {
			spr->collidedSprite = Game::currentMap->NPCs()[i];
			Game::currentMap->NPCs()[i]->collidedSprite = spr;
			return false;
		}
	}
	
	// Collisions with monsters
	for(u16 i = 0 ; i < Game::currentMap->monsters().size() ; i++) {
		if((Game::currentMap->monsters()[i]->x() < x && Game::currentMap->monsters()[i]->x() + 16 > x)
		&& (Game::currentMap->monsters()[i]->y() < y && Game::currentMap->monsters()[i]->y() + 16 > y)
		&& Game::currentMap->monsters()[i]->lifes() > 0) {
			spr->collidedSprite = Game::currentMap->monsters()[i];
			Game::currentMap->monsters()[i]->collidedSprite = spr;
			return false;
		}
	}
	
	// Collisions with player
	if(spr->isMonster() || spr->isNPC() || spr->isMWeapon()) {
		if(Game::player->x() < x && Game::player->x() + 16 > x
		&& Game::player->y() < y && Game::player->y() + 16 > y) {
			spr->collidedSprite = Game::player;
			return false;
		}
	}
	
	// Collisions with map
	if(spr->isMonster() || spr->isNPC() || spr->isPlayer()) {
		if(inTable(nonPassableTiles, Game::currentMap->tilesetInfo()[Game::currentMap->getTile(tileX, tileY)])) {
			spr->collidedTile = Game::currentMap->tilesetInfo()[Game::currentMap->getTile(tileX, tileY)];
			return false;
		} else return true;
	} else {
		return true;
	}*/
	
	return true;
}

