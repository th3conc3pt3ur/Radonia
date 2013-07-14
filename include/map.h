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
#ifndef MAP_H
#define MAP_H

typedef sf::Image Tileset;

class Map {
	public:
		// Constructor and destructor
		Map(Tileset *tileset, char *filename, u16 width, u16 height, u16 tileWidth, u16 tileHeight, s16 mapX, s16 mapY);
		~Map();
		
		// Map render function
		void render();
		
	private:
		// Map tileset
		Tileset *m_tileset;
		
		// Map data
		u16 *m_data;
		
		// Map size
		u16 m_width;
		u16 m_height;
		
		// Map tile size
		u16 m_tileWidth;
		u16 m_tileHeight;
		
		// Map position in the overworld
		u16 m_mapX;
		u16 m_mapY;
};

#endif // MAP_H
