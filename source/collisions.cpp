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
#include "monster.h"
#include "NPC.h"
#include "player.h"
#include "interface.h"
#include "map.h"
#include "mapManager.h"
#include "door.h"
#include "game.h"
#include "tools.h"
#include "collisions.h"

// Initialize collision matrix
u16 collisionMatrix[4][4] = {
	{12,8,12,13},	// Right
	{3,8,3,13},		// Left
	{5,5,10,5},		// Up
	{5,15,10,15}	// Down
};

bool passable(Sprite *spr, s16 x, s16 y) {
	// Setup tileX and tileY
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
			return false;
		}
	}
	
	// Collisions with monsters
	for(u16 i = 0 ; i < Game::currentMap->monsters().size() ; i++) {
		if((Game::currentMap->monsters()[i]->x() < x && Game::currentMap->monsters()[i]->x() + 16 > x)
		&& (Game::currentMap->monsters()[i]->y() < y && Game::currentMap->monsters()[i]->y() + 16 > y)
		&& Game::currentMap->monsters()[i]->lifes() > 0) {
			spr->collidedSprite = Game::currentMap->monsters()[i];
			return false;
		}
	}
	
	// Collisions with map
	if(inTable(nonPassableTiles, Game::currentMap->tilesetInfo()[Game::currentMap->getTile(tileX, tileY)])) {
		spr->collidedTile = Game::currentMap->tilesetInfo()[Game::currentMap->getTile(tileX, tileY)];
		return false;
	} else return true;
}

void testCollisions(Sprite *spr) {
	// 0: Right | 1: Left | 2: Up | 3:Down
	for(u8 i = 0 ; i < 4 ; i++) {
		if((i==0)?(spr->vx() > 0):((i==1)?(spr->vx() < 0):((i==2)?(spr->vy() < 0):(spr->vy() > 0)))
		&& (!passable(spr, spr->x() + collisionMatrix[i][0], spr->y() + collisionMatrix[i][1])
		 || !passable(spr, spr->x() + collisionMatrix[i][2], spr->y() + collisionMatrix[i][3]))) {
			// Reset movement vector
			if(i<2) spr->vx(0);
			else	spr->vy(0);
			
			// Obstacles
			if( passable(spr, spr->x() + collisionMatrix[i][3], spr->y() + collisionMatrix[i][4])
			&& !passable(spr, spr->x() + collisionMatrix[i][0], spr->y() + collisionMatrix[i][1])) {
				if((i<2)?(spr->vy() == 0):(spr->vx() == 0) && !spr->collidedSprite) {
					if(i<2)	spr->vy(1);
					else	spr->vx(1);
				}
			}
			if( passable(spr, spr->x() + collisionMatrix[i][0], spr->y() + collisionMatrix[i][1])
			&& !passable(spr, spr->x() + collisionMatrix[i][3], spr->y() + collisionMatrix[i][4])) {
				if((i<2)?(spr->vy() == 0):(spr->vx() == 0) && !spr->collidedSprite) {
					if(i<2) spr->vy(-1);
					else	spr->vx(-1);
				}
			}
		}
	}
}

