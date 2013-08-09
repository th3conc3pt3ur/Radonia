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

class NPC : public Character {
	public:
		// Constructor and destructor
		NPC(u16 x, u16 y, u8 direction, u16 mapID, char *filename);
		~NPC();
		
		// Speak function
		void action();
		
		// Make a blue boy
		static NPC *BlueBoy(u16 x, u16 y, u8 direction, u16 mapID);
		
		// NPCs texts
		static char *texts[NB_NPCs];
		
		// NPCs moves
		static int moves[NB_NPCs][21];
};

#endif // NPC_H
