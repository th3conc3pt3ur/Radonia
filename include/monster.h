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

class Monster : public Character {
	public:
		// Constructor and destructor
		Monster(u16 x, u16 y, u8 direction, u16 mapID, char *filename);
		~Monster();
		
		// Make a red monster
		static Monster *RedMonster(u16 x, u16 y, u8 direction, u16 mapID);
		
		// Monsters moves
		static int moves[NB_MONSTERS][21];
};

#endif // MONSTER_H
