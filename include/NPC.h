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
#ifndef NPC_H
#define NPC_H

class NPC : public Sprite {
	public:
		// Constructor and destructor
		NPC();
		~NPC();
		
		// Movement function
		void move();
		
		// Render function
		void render();
		
	private:
		// NPC position (relative to its map)
		u16 m_x;
		u16 m_y;
		
		// NPC movement vectors
		s8 m_vx;
		s8 m_vy;
		
		// NPC direction
		u8 m_direction;
};

#endif // NPC_H
