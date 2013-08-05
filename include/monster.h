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
#ifndef MONSTER_H
#define MONSTER_H

class Monster : public Sprite {
	public:
		// Constructor and destructor
		Monster(u16 x, u16 y, u8 direction, u16 mapID, char *filename);
		~Monster();
		
		// Movement function
		void move();
		
		// Render function
		void render();
		
		// Hurt monster
		void hurt();
		
		// Get monster id
		u16 id() const { return m_id; }
		
		// Get monster position in current map
		u16 x() const { return m_x; }
		u16 y() const { return m_y; }
		
		// Get monster mapID
		u16 mapID() const { return m_mapID; }
		
		// Get monster lifes
		u16 lifes() const { return m_lifes; }
		u16 maxLifes() const { return m_maxLifes; }
		
		// Make a red monster
		static Monster *RedMonster(u16 x, u16 y, u8 direction, u16 mapID);
		
		// Initialize all monsters
		static Monster **initAll();
		
		// Monsters moves
		static int moves[NB_MONSTERS][21];
		
		// Monsters counter
		static int nbMonsters;
		
	private:
		// Monster id
		u16 m_id;
		
		// Monster position in current map
		u16 m_x;
		u16 m_y;
		
		// Monster movement vectors
		s8 m_vx;
		s8 m_vy;
		
		// Monster direction
		u8 m_direction;
		
		// Monster lifes
		u16 m_lifes;
		u16 m_maxLifes;
		
		// Monster map id
		u16 m_mapID;
		
		// Moving state
		bool m_moving;
		
		// Movement counters
		u8 m_countMoves;
		u8 m_vxCount;
		u8 m_vyCount;
		
		// Timer for movement
		Timer m_timer;
};

#endif // MONSTER_H
