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

#define MAP_POS(x, y) ((x) + (y) * WM_SIZE)

// Tiles tables
extern u16 nonPassableTiles[13];
extern u16 changeMapTiles[3];

extern u16 doorUp[2];
extern u16 doorDown[2];
extern u16 doorLeft[2];
extern u16 doorRight[2];

// Map init functions
Map** initOverworldMaps();

// Map update functions
void renderMaps(Map **maps);
void refreshMaps(Map **maps, s16 moveX, s16 moveY);

// Functions for tiles
bool inTable(u16 *tiles, u16 id);
bool passable(s16 tileX, s16 tileY);
bool inTiles(s16 tileX, s16 tileY, u16 *tiles);

#endif // MAPMANAGER_H
