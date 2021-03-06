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
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <sys/stat.h>

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
#include "tools.h"
#include "interface.h"
#include "game.h"

using namespace std;

u16 Map::nbMaps = 0;

// Initialize view rect
SDL_Rect Map::viewRect = {0, 0, MAP_WIDTH << 4, MAP_HEIGHT << 4};

Map::Map(Image *tileset, u16 *tilesetInfo, const char *filename, u16 width, u16 height, u16 tileWidth, u16 tileHeight, u16 x, u16 y, u16 area) {
	// Set map id
	m_id = nbMaps;
	
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
	
	// Get filesize for SDL_RWread
	int filesize;
	struct stat file_status;
	if(stat(filename, &file_status) != 0) {
		filesize = sizeof(table) * m_width * m_height;
	} else {
		filesize = file_status.st_size;
	}
	
	// Load map from file
	SDL_RWops *f = SDL_RWFromFile(filename, "r");
	if(!f) {
		fprintf(stderr, "Unable to open file %s: %s\n", filename, SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_RWread(f, table, 2, filesize);
	SDL_RWclose(f);
	
	// Save data in current map
	m_data = table;
	
	// Get characters
	m_characters = CharacterManager::getCharactersInMap(m_id);
}

Map::~Map() {
	// Free map data
	free(m_data);
}

void Map::renderTile(u16 x, u16 y) {
			// Get position
			u16 posX = (x + m_x * MAP_WIDTH) * m_tileWidth - viewRect.x;
			u16 posY = (y + m_y * MAP_HEIGHT) * m_tileHeight - viewRect.y;
			
			// If the tile isn't in the viewport: don't display it
			if(posX < Game::MainWindow->viewportX() - m_tileWidth * (int)ceil((double)Game::MainWindow->viewportW() / (double)Game::MainWindow->viewportH()) || posX > Game::MainWindow->viewportX() + Game::MainWindow->viewportW()) return;
			if(posY < Game::MainWindow->viewportY() - m_tileHeight * (int)ceil((double)Game::MainWindow->viewportW() / (double)Game::MainWindow->viewportH()) || posY > Game::MainWindow->viewportY() + Game::MainWindow->viewportH()) return;
			
			// Get tile id
			u16 tile = getTile(x, y);
			
			// Get tile position in the tileset
			u16 tileY = (tile / (m_tileset->width() / m_tileHeight)) * m_tileHeight;
			u16 tileX = (tile - (tileY / m_tileHeight) * (m_tileset->width() / m_tileHeight)) * m_tileWidth;
			
			// Set position and clip tile to display
			m_tileset->setPosRect(posX, posY, m_tileWidth, m_tileHeight);
			m_tileset->setClipRect(tileX, tileY, m_tileWidth, m_tileHeight);
			
			// Display the tile
			m_tileset->render();
}

void Map::render() {
	for(u16 y = 0 ; y < m_height ; y++) {
		for(u16 x = 0 ; x < m_width ; x++) {
			// Render the tile
			renderTile(x, y);
		}
	}
}

void Map::update() {
	for(std::vector<Character*>::iterator it = m_characters.begin() ; it != m_characters.end() ; it++) {
		// Get character position
		u16 x = (*it)->x() / m_tileWidth;
		u16 y = (*it)->y() / m_tileHeight;
		
		// Draw tiles around the character
		for(s8 i = -((*it)->frameWidth() / m_tileWidth) - 1 ; i <= (*it)->frameWidth() / m_tileWidth + 1 ; i++) {
			for(s8 j = -((*it)->frameHeight() / m_tileHeight) - 1 ; j <= (*it)->frameHeight() / m_tileHeight + 1 ; j++) {
				renderTile(x + i, y + j);
			}
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

