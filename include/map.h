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

class Map {
	public:
		// Constructor and destructor
		Map(sf::Image *tileset, u16 *tilesetInfo, char *filename, u16 width, u16 height, u16 tileWidth, u16 tileHeight, u16 mapX, u16 mapY);
		~Map();
		
		// Map render function
		void render();
		
		// Get tile id
		u16 getTile(u16 tileX, u16 tileY);
		
		// Get map position in the overworld
		u16 x() const { return m_x; }
		u16 y() const { return m_y; }
		
		// Get tileset information
		u16 *tilesetInfo() const { return m_tilesetInfo; }
		
		// Overworld maps counter
		static int nbMaps;
		
	private:
		// Map id
		u16 m_id;
		
		// Map tileset
		sf::Image *m_tileset;
		u16 *m_tilesetInfo;
		
		// Map data
		u16 *m_data;
		
		// Map size (tiles)
		u16 m_width;
		u16 m_height;
		
		// Map tile size
		u16 m_tileWidth;
		u16 m_tileHeight;
		
		// Map position in the overworld
		u16 m_x;
		u16 m_y;
};

#endif // MAP_H
