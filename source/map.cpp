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
#include <cmath>
#include <sys/stat.h>

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "types.h"
#include "config.h"
#include "map.h"
#include "game.h"

using namespace std;

int Map::nbMaps = 0;

Map::Map(sf::Image *tileset, char *filename, u16 width, u16 height, u16 tileWidth, u16 tileHeight, s16 mapX, s16 mapY) {
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
	
	// Make temporary table to get map file data
	u16* table = (u16*)malloc(m_width * m_height * sizeof(u16));
	
	// Load map from file
	struct stat file_status;
	if(stat(filename, &file_status) != 0){
		printf("Unable to load %s", filename);
	}
	int filesize = file_status.st_size;
	
	FILE* f = fopen(filename, "r");
	fread(table, 2, filesize, f);
	fclose(f);
	
	m_data = table;
	
	// NOTE: mapX/Y == -1 when it's an outdoor map
	m_mapY = ((mapY == -1) ? (m_id / WM_SIZE) : (mapY));
	m_mapX = ((mapX == -1) ? (m_id - m_mapY * WM_SIZE) : (mapX));
}

Map::~Map() {
}

void Map::render() {
	// Load temporary sprite to render tiles
	sf::Sprite renderedTile;
	renderedTile.SetImage(*m_tileset);
	
	for(u16 y = 0 ; y < m_height ; y++) {
		for(u16 x = 0 ; x < m_width ; x++) {
			// Get tile id
			u16 tile = getTile(x, y);
			
			// Get tile position
			u16 tileY = (tile / (m_tileset->GetHeight() / m_tileHeight)) * m_tileHeight;
			u16 tileX = (tile - (tileY / m_tileHeight) * (m_tileset->GetHeight() / m_tileHeight)) * m_tileWidth;
			
			// Set position and cut tile to display
			renderedTile.SetPosition(x * m_tileWidth, y * m_tileHeight);
			renderedTile.SetSubRect(sf::IntRect(tileX, tileY, tileX + m_tileWidth, tileY + m_tileHeight));
			
			// Display the tile
			Game::MainWindow->Draw(renderedTile);
		}
	}
}

u16 Map::getTile(u16 tileX, u16 tileY) {
	if(tileX + tileY * m_width < m_width * m_height) {
		return m_data[tileX + tileY * m_width];
	} else {
		return 0; // Tile isn't in the map
	}
}

