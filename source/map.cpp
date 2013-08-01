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
#include "timer.h"
#include "sprite.h"
#include "monster.h"
#include "NPC.h"
#include "player.h"
#include "map.h"
#include "mapManager.h"
#include "door.h"
#include "game.h"

using namespace std;

int Map::nbMaps = 0;

sf::View *Map::View = NULL;

Map::Map(sf::Texture *tileset, u16 *tilesetInfo, char *filename, u16 width, u16 height, u16 tileWidth, u16 tileHeight, u16 x, u16 y, u16 area) {
	// Set map id
	m_id = nbMaps;
	
	// Initialize sprite view
	if(View == NULL) View = new sf::View(sf::FloatRect(0, 0, 640, 480));
	
	// Update maps counter
	nbMaps++;
	
	// Set class members
	m_tileset = tileset;
	m_tilesetInfo = tilesetInfo;
	
	m_width = width;
	m_height = height;
	
	m_tileWidth = tileWidth;
	m_tileHeight = tileHeight;
	
	m_x = x;
	m_y = y;
	
	m_area = area;
	
	// Make temporary table to get map file data
	u16* table = (u16*)malloc(m_width * m_height * sizeof(u16));
	
	// Load map from file
	struct stat file_status;
	if(stat(filename, &file_status) != 0) {
		printf("Unable to load %s, file not found\n", filename);
		exit(1);
	}
	
	// Get filesize for fread
	int filesize = file_status.st_size;
	
	// Read data
	FILE* f = fopen(filename, "r");
	fread(table, 2, filesize, f);
	fclose(f);
	
	// Save data in current map
	m_data = table;
	
	// Get NPCs
	for(u16 i = 0 ; i < NB_NPCs ; i++) {
		if(Game::NPCs[i]->mapID() == m_id) m_NPCs.push_back(Game::NPCs[i]);
	}
	
	// Get monsters
	for(u16 i = 0 ; i < NB_MONSTERS ; i++) {
		if(Game::monsters[i]->mapID() == m_id) m_monsters.push_back(Game::monsters[i]);
	}
}

Map::~Map() {
	free(m_data);
}

void Map::render() {
	// Load temporary sprite to render tiles
	sf::Sprite renderedTile(*m_tileset);
	
	// Set view for drawing maps
	Game::MainWindow->setView(*View);
	
	for(u16 y = 0 ; y < m_height ; y++) {
		for(u16 x = 0 ; x < m_width ; x++) {
			// Get tile id
			u16 tile = getTile(x, y);
			
			// Get tile position
			u16 tileY = (tile / (m_tileset->getSize().x / m_tileHeight)) * m_tileHeight;
			u16 tileX = (tile - (tileY / m_tileHeight) * (m_tileset->getSize().x / m_tileHeight)) * m_tileWidth;
			
			// Set position and cut tile to display
			renderedTile.setPosition((x + m_x * MAP_WIDTH) * m_tileWidth, (y + m_y * MAP_HEIGHT) * m_tileHeight);
			renderedTile.setTextureRect(sf::IntRect(tileX, tileY, m_tileWidth, m_tileHeight));
			
			// Display the tile
			Game::MainWindow->draw(renderedTile);
		}
	}
	
	// Reset the view
	Game::MainWindow->setView(Game::MainWindow->getDefaultView());
}

u16 Map::getTile(u16 tileX, u16 tileY) {
	if(tileX + tileY * m_width < m_width * m_height) {
		return m_data[tileX + tileY * m_width];
	} else {
		return 0; // Tile isn't in the map
	}
}

void Map::renderNPCs() {
	for(std::vector<NPC*>::iterator it = m_NPCs.begin() ; it != m_NPCs.end() ; it++) {
		(*it)->move();
		(*it)->render();
	}
}

void Map::renderMonsters() {
	for(std::vector<Monster*>::iterator it = m_monsters.begin() ; it != m_monsters.end() ; it++) {
		(*it)->move();
		(*it)->render();
	}
}

