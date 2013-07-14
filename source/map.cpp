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
#include "map.h"

using namespace std;

int Map::nbMaps = 0;

Map::Map(Tileset *tileset, char *filename, u16 width, u16 height, u16 tileWidth, u16 tileHeight, s16 mapX, s16 mapY) {
	// Set map id
	m_id = nbMaps;
	
	// Update maps counter
	nbMaps++;
	
	// Set class members
	m_tileset = tileset;
	
	m_width = width;
	m_height = height;
	
	m_tileWidth = tileWidth;
	m_tileHeight = tileHeight;
	
	m_mapX = mapX;
	m_mapY = mapY;
	
	// Make temporary table to get map file data
	u16* table = (u16*)malloc(m_width * m_height * sizeof(u16));
	
	// Load map from file
	struct stat file_status;
	if(stat(filename, &file_status) != 0){
		printf("Unable to load %s", m_filename);
	}
	int filesize = file_status.st_size;
	
	FILE* f = fopen(filename, "r");
	fread(table, 2, filesize, f);
	fclose(f);
	
	m_data = table;
	
	m_mapY = ((mapY == -1) ? (m_id / WM_SIZE) : (mapY));
	m_mapX = ((mapX == -1) ? (m_id - m_mapY * WM_SIZE) : (mapX));
}

Map::~Map() {
}

void Map::render() {
	
}

