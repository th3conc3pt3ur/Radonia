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
#ifndef MAPMANAGER_H
#define MAPMANAGER_H

// Map position in the area
#define MAP_POS(x, y, area) (u16)((x) + (y) * sqrt(MapManager::areaSizes[(area)]))

namespace MapManager {

// Tiles tables
extern u16 nonPassableTiles[13];
extern u16 changeMapTiles[3];

// Tilesets infos
extern u16 plainInfo[256];
extern u16 indoorInfo[256];
extern u16 undergroundInfo[256];

// Tileset init function
sf::Texture **initTilesets();

// Sizes of map areas
extern u16 areaSizes[MAP_AREAS];

// Map init function
Map*** initMaps();

// Map update function
void refreshMaps(Map **maps, s16 moveX, s16 moveY);

}

// Get map id from area
u16 _mid(u16 area, u16 id);

// Functions for tiles
bool inTable(u16 *tiles, u16 id);
bool inTiles(s16 tileX, s16 tileY, u16 *tiles);

#endif // MAPMANAGER_H
