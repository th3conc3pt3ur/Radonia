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
#include "game.h"

Map** initOverworldMaps() {
	// Initialize overworld maps array
	Map **maps = new Map*[WM_SIZE * WM_SIZE];
	
	// Initialize default tileset
	sf::Image *tileset = new sf::Image;
	tileset->LoadFromFile("graphics/tilesets/plain.png");
	tileset->SetSmooth(false);
	
	// Load maps
	for(u16 y = 0 ; y < WM_SIZE ; y++) {
		for(u16 x = 0 ; x < WM_SIZE ; x++) {
			// Format filename string to match maps filenames
			char *filename = new char[13];
			sprintf(filename, "maps/%c%d.map", y+'a', x+1);
			
			// Load map into maps array
			maps[MAP_POS(x, y)] = new Map(tileset, filename, 40, 30, 16, 16, x, y);
		}
	}
	
	return maps;
}


void renderMaps(Map **maps) {
	// Get current map position
	s16 x = Game::currentMap->x();
	s16 y = Game::currentMap->y();
	
	// Render surrounding maps
	Game::currentMap->render();
	if(maps[MAP_POS(x - 1, y)] != NULL && x - 1 >= 0)		maps[MAP_POS(x - 1, y)]->render();
	if(maps[MAP_POS(x + 1, y)] != NULL && x + 1 < WM_SIZE)	maps[MAP_POS(x + 1, y)]->render();
	if(maps[MAP_POS(x, y - 1)] != NULL && y - 1 >= 0)		maps[MAP_POS(x, y - 1)]->render();
	if(maps[MAP_POS(x, y + 1)] != NULL && y + 1 < WM_SIZE)	maps[MAP_POS(x, y + 1)]->render();
}

void refreshMaps(Map **maps, s16 moveX, s16 moveY) {
	// Get next map
	Map *nextMap = maps[MAP_POS(Game::currentMap->x() + moveX / 32, Game::currentMap->y() + moveY / 32)];
	
	// Next map must be in the map
	if(nextMap == NULL
	   || Game::currentMap->x() + moveX / 32 < 0
	   || Game::currentMap->x() + moveX / 32 >= WM_SIZE
	   || Game::currentMap->y() + moveY / 32 < 0
	   || Game::currentMap->y() + moveY / 32 >= WM_SIZE)
		return;
	
	// Render maps
	Game::currentMap->render();
	nextMap->render();
}

