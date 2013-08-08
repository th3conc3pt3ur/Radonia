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
#ifndef CHARACTER_H
#define CHARACTER_H

typedef enum {
	CHARA_PLAYER,
	CHARA_NPC,
	CHARA_MONSTER
} CharacterType;

class Character : public Sprite {
	public:
		// Constructor and destructor
		Character();
		~Character();
		
		// Render character
		void render();
		
		// Hurt comportement
		void hurt();
		
		// Test collisions
		void testCollisions();
		
		// Action function
		virtual void action() = 0;
		
		// Move function
		void move();
		
		// Get character position
		s16 x() const { return m_x; }
		s16 y() const { return m_y; }
		
		// Get character lifes
		s16 lifes() const { return m_lifes; }
		u16 maxLifes() const { return m_maxLifes; }
		
		// Types
		bool isPlayer()  { return m_type == CHARA_PLAYER;	}
		bool isNPC()	 { return m_type == CHARA_NPC;		}
		bool isMonster() { return m_type == CHARA_MONSTER;	}
		
		// Get collision states
		Character *collidedCharacter() { return m_collidedCharacter; }
		Weapon *collidedWeapon() { return m_collidedWeapon; }
		int collidedTile() { return m_collidedTile; }
		
		// Set collision states
		void collidedCharacter(Character *character) { m_collidedCharacter = character; }
		void collidedWeapon(Weapon *weapon) { m_collidedWeapon = weapon; }
		void collidedTile(int tile) { m_collidedTile = tile; }
		
	private:
		// Character position
		s16 m_x;
		s16 m_y;
		
		// Movement vectors
		s8 m_vx;
		s8 m_vy;
		
		// Character hurt timer
		Timer m_hurtTimer;
		u16 m_timerLastValue;
		
		// Character lifes
		s16 m_lifes;
		u16 m_maxLifes;
		
		// Character type
		CharacterType m_type;
		
		// Movement timer (NPC|Monsters)
		Timer m_movementTimer;
		
		// Blocked movement and direction states
		bool m_canMove;
		bool m_canTurn;
		
		// Collision states
		Character *m_collidedCharacter;
		Weapon *m_collidedWeapon;
		int m_collidedTile;
};

#endif // CHARACTER_H
