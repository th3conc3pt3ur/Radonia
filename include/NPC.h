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
		NPC(u16 x, u16 y, u8 direction, u16 mapID, char *filename);
		~NPC();
		
		// Movement function
		void move();
		
		// Render function
		void render();
		
		// Speaking function
		void speak();
		
		// Get NPC id
		u16 id() const { return m_id; }
		
		// Get NPC mapID
		u16 mapID() const { return m_mapID; }
		
		// Make a blue boy
		static NPC *BlueBoy(u16 x, u16 y, u8 direction, u16 mapID);
		
		// Initialize all NPCs
		static NPC **initAll();
		
		// NPCs texts
		static char *texts[NB_NPCs];
		
		// NPCs moves
		static int moves[NB_NPCs][21];
		
		// NPCs counter
		static int nbNPCs;
		
	private:
		// NPC id
		u16 m_id;
		
		// NPC direction
		u8 m_direction;
		
		// NPC map id
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

#endif // NPC_H
