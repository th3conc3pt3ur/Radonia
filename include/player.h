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
#ifndef PLAYER_H
#define PLAYER_H

class Player : public Sprite {
	public:
		// Constructor and destructor
		Player();
		~Player();
		
		// Move function
		void move();
		
		// Render function
		void render();
		
		// Test collisions functions
		void doorCollisions();
		void testCollisions();
		
		// Get player position
		s16 x() const { return m_x; }
		s16 y() const { return m_y; }
		
		// Set player position
		void x(s16 x) { m_x = x; }
		void y(s16 y) { m_y = y; }
		
		// Set player movement vectors
		void vx(s8 vx) { m_vx = vx; }
		void vy(s8 vy) { m_vy = vy; }
		
		// Player directions
		class Direction {
			public:
				static const u8 Up = 3;
				static const u8 Down = 0;
				static const u8 Left = 2;
				static const u8 Right = 1;
		};
		
	private:
		// Player position (relative to current map)
		s16 m_x;
		s16 m_y;
		
		// Player movement vectors
		s8 m_vx;
		s8 m_vy;
		
		// Player direction
		u8 m_direction;
		
		// Player lifes
		u8 m_lifes;
};

#endif // PLAYER_H
