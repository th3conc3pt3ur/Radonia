/*---------------------------------------------------------------------------------
	
	Radonia
	Copyright (C) 2013-2014 Deloptia <deloptia.devteam@gmail.com>
	
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

typedef enum {
	MONSTER_REDMONSTER,
	MONSTER_GOHMA
} MonsterType;

class Monster : public Character {
	public:
		// Constructor and destructor
		Monster(u16 x, u16 y, CharacterDirection direction, u16 mapID, u16 area, MonsterType type, const char *filename, u8 frameWidth = 16, u8 frameHeight = 16);
		~Monster();
		
		// Movement function
		void move();
		
		// Action function
		void action() {};
		
	private:
		// Monster type
		MonsterType m_subType;
};

#endif // MONSTER_H
