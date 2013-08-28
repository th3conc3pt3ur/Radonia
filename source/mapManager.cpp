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
#include "game.h"

/*	Tiles informations:
		0: Passable
		1: Non passable
		2: Slows and can be cutted
		3: Non passable but can be cutted and can be lifted
		4: Non passable but can be burned
		6: Fireplace
		7: Hole
		8: Empty chest
		9: Closed chest
		10: Placard
		11: Doors
		12: Rock, can be lifted
		13: Soft soil
		14: Stairs
		15: Stairs to underground
		16: Cane of Somaria's rock
		17: Hook's rock
		18: Block of sand 
		19: Low water
*/

// FIXME: Adding a zero member to tiles arrays is a
// temporary solution to prevent reading another arrays
// when reading one

u16 MapManager::nonPassableTiles[13] = {
	1,3,4,5,6,8,9,10,12,16,17,18,0
};

u16 MapManager::changeMapTiles[3] = {
	11,15,0
};

u16 MapManager::plainInfo[256] = {
	0,0,0,0,0,0,0,3,2,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
	0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
	0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,
	1,1,1,1,1,1,1,1,0,0,0,0,4,0,1,1,
	1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,11,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,1,1,1,1,1,1,11,6,1,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,19,19,0,0,0,0,0,0,0,0,7,1,
	8,9,10,7,12,13,13,1,14,15,1,16,17,0,18,1
};

u16 MapManager::indoorInfo[256] = {
	0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,0,1,1,0,0,0,
	1,1,1,1,1,1,1,1,1,1,0,1,1,0,0,0,
	1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,1,0,11,11,1,1,0,
	1,1,1,1,1,1,1,1,1,1,0,11,11,0,0,0,
	1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,
	1,1,0,0,0,0,1,1,1,1,1,1,1,0,1,1,
	1,1,0,0,0,0,1,1,1,1,1,1,0,1,1,1,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	19,19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	8,9,10,7,0,0,1,12,15,15,11,11,0,0,0,0
};

u16 MapManager::undergroundInfo[256] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,15,15,11,11,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,11,0,11,11,0,0,0,0,0,0,0,0,0,0,
	8,9,10,0,0,0,0,0,0,19,0,0,0,0,0,0
};

Image **MapManager::tilesets;
Map ***MapManager::mapAreas;
Map *MapManager::currentMap = NULL;

void MapManager::initTilesets() {
	// Initialize tileset array
	tilesets = new Image*[NB_TILESETS];
	
	/* Initialize tilesets */
	
	// Plain
	tilesets[0] = new Image("graphics/tilesets/plain.png");
	
	// Indoor
	tilesets[1] = new Image("graphics/tilesets/indoor.png");
	
	// Underground
	tilesets[2] = new Image("graphics/tilesets/underground.png");
}

u16 MapManager::areaSizes[MAP_AREAS] = {WM_SIZE * WM_SIZE, INDOOR_MAPS, 4};

void MapManager::initMaps() {
	// Initialize tilesets
	initTilesets();
	
	// Initialize global array
	mapAreas = new Map**[MAP_AREAS];
	
	// Initialize each area array
	mapAreas[0] = new Map*[areaSizes[0]];
	mapAreas[1] = new Map*[areaSizes[1]];
	mapAreas[2] = new Map*[areaSizes[2]];
	
	/* Initialize area maps */
	
	// Overworld
	mapAreas[0][MAP_POS(0, 0, 0)] = new Map(tilesets[0], plainInfo, "maps/a1.map", MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, TILE_SIZE, 0, 0);
	mapAreas[0][MAP_POS(1, 0, 0)] = new Map(tilesets[0], plainInfo, "maps/a2.map", MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, TILE_SIZE, 1, 0);
	mapAreas[0][MAP_POS(0, 1, 0)] = new Map(tilesets[0], plainInfo, "maps/b1.map", MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, TILE_SIZE, 0, 1);
	mapAreas[0][MAP_POS(1, 1, 0)] = new Map(tilesets[0], plainInfo, "maps/b2.map", MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, TILE_SIZE, 1, 1);
	
	// Indoor maps
	mapAreas[1][0] = new Map(tilesets[1], indoorInfo, "maps/in1.map", MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, TILE_SIZE, 0, 0, 1);
	mapAreas[1][1] = new Map(tilesets[1], indoorInfo, "maps/in2.map", MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, TILE_SIZE, 0, 0, 1);
	mapAreas[1][2] = new Map(tilesets[1], indoorInfo, "maps/in3.map", MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, TILE_SIZE, 0, 0, 1);
	
	/* Caves */
	
	mapAreas[2][MAP_POS(0, 0, 2)] = new Map(tilesets[2], undergroundInfo, "maps/ca1a1.map", MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, TILE_SIZE, 0, 0, 2);
	mapAreas[2][MAP_POS(1, 0, 2)] = new Map(tilesets[2], undergroundInfo, "maps/ca1a2.map", MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, TILE_SIZE, 1, 0, 2);
	mapAreas[2][MAP_POS(0, 1, 2)] = NULL;
	mapAreas[2][MAP_POS(1, 1, 2)] = new Map(tilesets[2], undergroundInfo, "maps/ca1b2.map", MAP_WIDTH, MAP_HEIGHT, TILE_SIZE, TILE_SIZE, 1, 1, 2);
}

void MapManager::refreshMaps(Map **maps, s16 moveX, s16 moveY) {
	// Get next map
	Map *nextMap = maps[MAP_POS(currentMap->x() + moveX / 16, currentMap->y() + moveY / 16, currentMap->area())];
	
	// Next map must be in the map
	if(nextMap == NULL
	   || MAP_POS(currentMap->x() + moveX / 16, currentMap->y(), currentMap->area()) < 0
	   || MAP_POS(currentMap->x() + moveX / 16, currentMap->y(), currentMap->area()) >= areaSizes[currentMap->area()]
	   || MAP_POS(currentMap->x(), currentMap->y() + moveY / 16, currentMap->area()) < 0
	   || MAP_POS(currentMap->x(), currentMap->y() + moveY / 16, currentMap->area()) >= areaSizes[currentMap->area()])
		return;
	
	// Render maps
	currentMap->render();
	nextMap->render();
}

void MapManager::unload() {
	// Delete maps array
	for(u16 i = 0 ; i < MAP_AREAS ; i++) {
		delete[] mapAreas[i];
	}
	delete mapAreas;
	
	// Delete tilesets
	delete[] tilesets;
}

u16 _mid(u16 area, u16 id) {
	u16 tempID = id;
	for(u16 i = 0 ; i < area ; i++)
		tempID += MapManager::areaSizes[i];
	return tempID;
}

bool inTable(u16 tiles[], u16 id) {
	for(u16 i = 0 ; tiles[i] != 0 ; i++) {
		if(tiles[i] == id) return true;
	}
	return false;
}

bool inTiles(s16 tileX, s16 tileY, u16 tiles[]) {
	return inTable(tiles, MapManager::currentMap->tilesetInfo()[MapManager::currentMap->getTile(tileX, tileY)]);
}

bool inZone(s16 x, s16 y, u16 tile) {
	return (MapManager::currentMap->tilesetInfo()[MapManager::currentMap->getTile(floor(((double)x + 4) / TILE_SIZE), floor(((double)y + 12) / TILE_SIZE))] == tile
	|| MapManager::currentMap->tilesetInfo()[MapManager::currentMap->getTile( ceil(((double)x - 4) / TILE_SIZE), floor(((double)y + 12) / TILE_SIZE))] == tile
	|| MapManager::currentMap->tilesetInfo()[MapManager::currentMap->getTile(floor(((double)x + 4) / TILE_SIZE),  ceil(((double)y - 4) / TILE_SIZE))] == tile
	|| MapManager::currentMap->tilesetInfo()[MapManager::currentMap->getTile( ceil(((double)x - 4) / TILE_SIZE),  ceil(((double)y - 4) / TILE_SIZE))] == tile);
}

bool inZones(s16 x, s16 y, u16 tiles[]) {
	for(u16 i = 0 ; tiles[i] != 0 ; i++) {
		if(inZone(x, y, tiles[i])) return true;
	}
	
	return false;
}

