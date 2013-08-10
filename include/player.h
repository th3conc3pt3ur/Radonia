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

// TODO when back: put all movement/rendering/basic actions functions in Character

class Player : public Character {
	public:
		// Constructor and destructor
		Player();
		~Player();
		
		// Movement function
		void move();
		
		// Action function
		void action() {};
		
		// Sword attack
		//void sword(); TODO: Weapon class
		
		// Get sword sprite
		//Sprite *swordSpr() const { return m_swordSpr; }
		
	private:
		// Sword sprite
		//Sprite *m_swordSpr;
};

#endif // PLAYER_H
