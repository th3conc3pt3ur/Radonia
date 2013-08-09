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
		Character(char *filename, CharacterType type, s16 x, s16 y, u8 direction, s16 mapID = -1, u8 frameSize = 16);
		~Character();
		
		// Move function
		void move();
		
		// Render character
		void render();
		
		// Action function
		virtual void action() = 0;
		
		// Hurt comportement
		void hurt();
		
		// Get character position
		s16 x() const { return m_x; }
		s16 y() const { return m_y; }
		
		// Get movement vectors
		s8 vx() const { return m_vx; }
		s8 vy() const { return m_vy; }
		
		// Get character lifes
		s16 lifes() const { return m_lifes; }
		u16 maxLifes() const { return m_maxLifes; }
		
		// Regenerate a character
		void regen() { m_lifes = m_maxLifes; }
		
		// Get map id
		s16 mapID() const { return m_mapID; }
		
		// Get character id
		u16 id() const { return m_id; }
		
		// Types
		bool isPlayer()  { return m_type == CHARA_PLAYER;	}
		bool isNPC()	 { return m_type == CHARA_NPC;		}
		bool isMonster() { return m_type == CHARA_MONSTER;	}
		
		// Get collision states
		Character *collidedCharacter() { return m_collidedCharacter; }
		//Weapon *collidedWeapon() { return m_collidedWeapon; }
		int collidedTile() { return m_collidedTile; }
		
		// Set position
		void x(s16 x) { m_x = x; }
		void y(s16 y) { m_y = y; }
		
		// Set movement vectors
		void vx(s8 vx) { m_vx = vx; }
		void vy(s8 vy) { m_vy = vy; }
		
		// Set direction
		void direction(s8 direction) { m_direction = direction; }
		
		// Set collision states
		void collidedCharacter(Character *character) { m_collidedCharacter = character; }
		//void collidedWeapon(Weapon *weapon) { m_collidedWeapon = weapon; }
		void collidedTile(int tile) { m_collidedTile = tile; }
		
		// Characters counter
		static u16 nbCharacters;
		
	protected:
		// Character id
		u16 m_id;
		
		// Character position
		s16 m_x;
		s16 m_y;
		
		// Movement vectors
		s8 m_vx;
		s8 m_vy;
		
		// Direction
		u8 m_direction;
		
		// Map id
		s16 m_mapID;
		
		// Moving state
		bool m_moving;
		
		// Movement counters
		u8 m_countMoves;
		u8 m_vxCount;
		u8 m_vyCount;
		
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
		//Weapon *m_collidedWeapon;
		int m_collidedTile;
		
		// Attacking state
		bool m_isAttacking;
};

#endif // CHARACTER_H
