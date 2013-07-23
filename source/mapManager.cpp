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

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "types.h"
#include "config.h"
#include "map.h"
#include "mapManager.h"
#include "timer.h"
#include "sprite.h"
#include "player.h"
#include "door.h"
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
			111: Up
			112: Down
			113: Left
			114: Right
		12: Rock, can be lifted
		13: Soft soil
		14: Stairs
		15: Stairs to underground
		16: Cane of Somaria's rock
		17: Hook's rock
		18: Block of sand 
*/

// FIXME: Adding a zero member to the tiles arrays is a
// temporary solution to prevent reading another arrays
// when reading one

u16 nonPassableTiles[13] = {
	1,3,4,5,6,8,9,10,12,16,17,18,0
};

u16 changeMapTiles[3] = {
	11,15,0
};

u16 doorUp[2] = {111,0};
u16 doorDown[2] = {112,0};
u16 doorLeft[2] = {113,0};
u16 doorRight[2] = {114,0};

u16 plainInfo[256] = {
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
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,111,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,1,1,1,1,1,1,111,6,1,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,1,
	8,9,10,7,12,13,13,1,14,15,1,16,17,0,18,1
};

u16 indoorInfo[256] = {
	0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,0,1,1,0,0,0,
	1,1,1,1,1,1,1,1,1,1,0,1,1,0,0,0,
	1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,1,0,112,112,1,1,0,
	1,1,1,1,1,1,1,1,1,1,0,112,111,0,0,0,
	1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,
	1,1,0,0,0,0,1,1,1,1,1,1,1,0,1,1,
	1,1,0,0,0,0,1,1,1,1,1,1,0,1,1,1,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	8,9,10,7,0,0,1,12,15,15,111,15,0,0,0,0
};

u16 undergroundInfo[256] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,112,0,112,112,0,0,0,0,0,0,0,0,0,0,
	8,9,10,0,0,0,0,0,0,0,0,0,0,0,0,0
};

sf::Image **initTilesets() {
	// Initialize tileset array
	sf::Image **tilesets = new sf::Image*[3];
	
	/* Initialize tilesets */
	
	// Plain
	tilesets[0] = new sf::Image;
	tilesets[0]->LoadFromFile("graphics/tilesets/plain.png");
	tilesets[0]->SetSmooth(false);
	
	// Indoor
	tilesets[1] = new sf::Image;
	tilesets[1]->LoadFromFile("graphics/tilesets/indoor.png");
	tilesets[1]->SetSmooth(false);
	
	// Underground
	tilesets[2] = new sf::Image;
	tilesets[2]->LoadFromFile("graphics/tilesets/underground.png");
	tilesets[2]->SetSmooth(false);
	
	return tilesets;
}

u16 areaSizes[MAP_AREAS] = {WM_SIZE * WM_SIZE, INDOOR_MAPS, 4};

u16 _mid(u16 area, u16 id) {
	u16 tempID = id;
	for(u16 i = 0 ; i < area ; i++)
		tempID += areaSizes[i];
	return tempID;
}

Map*** initMaps() {
	// Initialize global array
	Map ***mapAreas = new Map**[MAP_AREAS];
	
	// Initialize each area array
	mapAreas[0] = new Map*[WM_SIZE * WM_SIZE];
	mapAreas[1] = new Map*[INDOOR_MAPS];
	mapAreas[2] = new Map*[areaSizes[2]];
	
	/* Initialize area maps */
	
	// Overworld
	mapAreas[0][MAP_POS(0, 0, 0)] = new Map(Game::tilesets[0], plainInfo, (char*)"maps/a1.map", MAP_WIDTH, MAP_HEIGHT, 16, 16, 0, 0);
	mapAreas[0][MAP_POS(1, 0, 0)] = new Map(Game::tilesets[0], plainInfo, (char*)"maps/a2.map", MAP_WIDTH, MAP_HEIGHT, 16, 16, 1, 0);
	mapAreas[0][MAP_POS(0, 1, 0)] = new Map(Game::tilesets[0], plainInfo, (char*)"maps/b1.map", MAP_WIDTH, MAP_HEIGHT, 16, 16, 0, 1);
	mapAreas[0][MAP_POS(1, 1, 0)] = new Map(Game::tilesets[0], plainInfo, (char*)"maps/b2.map", MAP_WIDTH, MAP_HEIGHT, 16, 16, 1, 1);
	
	// Indoor maps
	mapAreas[1][0] = new Map(Game::tilesets[1], indoorInfo, (char*)"maps/in1.map", MAP_WIDTH, MAP_HEIGHT, 16, 16, 0, 0, 1);
	mapAreas[1][1] = new Map(Game::tilesets[1], indoorInfo, (char*)"maps/in2.map", MAP_WIDTH, MAP_HEIGHT, 16, 16, 0, 0, 1);
	
	/* Caves */
	
	mapAreas[2][MAP_POS(0, 0, 2)] = new Map(Game::tilesets[2], undergroundInfo, (char*)"maps/ca1a1.map", MAP_WIDTH, MAP_HEIGHT, 16, 16, 0, 0, 2);
	mapAreas[2][MAP_POS(1, 0, 2)] = new Map(Game::tilesets[2], undergroundInfo, (char*)"maps/ca1a2.map", MAP_WIDTH, MAP_HEIGHT, 16, 16, 1, 0, 2);
	mapAreas[2][MAP_POS(0, 1, 2)] = NULL;
	mapAreas[2][MAP_POS(1, 1, 2)] = new Map(Game::tilesets[2], undergroundInfo, (char*)"maps/ca1b2.map", MAP_WIDTH, MAP_HEIGHT, 16, 16, 1, 1, 2);
	
	return mapAreas;
}

void refreshMaps(Map **maps, s16 moveX, s16 moveY) {
	// Get next map
	Map *nextMap = maps[MAP_POS(Game::currentMap->x() + moveX / 32, Game::currentMap->y() + moveY / 32, Game::currentMap->area())];
	
	// Next map must be in the map
	if(nextMap == NULL
	   || MAP_POS(Game::currentMap->x() + moveX / 32, Game::currentMap->y(), Game::currentMap->area()) < 0
	   || MAP_POS(Game::currentMap->x() + moveX / 32, Game::currentMap->y(), Game::currentMap->area()) >= areaSizes[Game::currentMap->area()]
	   || MAP_POS(Game::currentMap->x(), Game::currentMap->y() + moveY / 32, Game::currentMap->area()) < 0
	   || MAP_POS(Game::currentMap->x(), Game::currentMap->y() + moveY / 32, Game::currentMap->area()) >= areaSizes[Game::currentMap->area()])
		return;
	
	// Render maps
	Game::currentMap->render();
	nextMap->render();
}

bool inTable(u16 tiles[], u16 id) {
	int i = 0;
	while(tiles[i]) {
		if(tiles[i] == id) {
			return true;
		}
		i++;
	}
	return false;
}

bool inTiles(s16 tileX, s16 tileY, u16 tiles[]) {
	if(inTable(tiles, Game::currentMap->tilesetInfo()[Game::currentMap->getTile(tileX, tileY)])) {
		return true;
	} else {
		return false;
	}
}

bool passable(s16 tileX, s16 tileY) {
	if(inTable(nonPassableTiles, Game::currentMap->tilesetInfo()[Game::currentMap->getTile(tileX, tileY)])) {
		return false;
	} else {
		return true;
	}
}

