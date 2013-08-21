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
#ifndef CHARACTER_H
#define CHARACTER_H

typedef enum {
	CHARA_PLAYER,
	CHARA_NPC,
	CHARA_MONSTER
} CharacterType;

// Characters directions
typedef enum {
	DIR_DOWN,
	DIR_RIGHT,
	DIR_LEFT,
	DIR_UP
} CharacterDirection;

class Character : public Sprite {
	public:
		// Constructor and destructor
		Character(char *filename, CharacterType type, s16 x, s16 y, CharacterDirection direction, u16 mapID, u16 area, u8 frameWidth = 16, u8 frameHeight = 16);
		virtual ~Character();
		
		// Movement function
		virtual void move() = 0;
		
		// Render character
		void render();
		
		// Test collisions
		void testCollisions();
		void doorCollisions();
		
		// Action function
		virtual void action() = 0;
		
		// Hurt comportement
		void hurt();
		
		// Reset character attributes
		void reset();
		
		// Other movement functions
		void moveX(u16 x) { m_x += x; }
		void moveY(u16 y) { m_y += y; }
		void stop() { m_vx = 0; m_vy = 0; }
		
		// Get character id
		u16 id() const { return m_id; }
		
		// Get character position
		s16 x() const { return m_x; }
		s16 y() const { return m_y; }
		
		// Get character movement vectors
		s8 vx() const { return m_vx; }
		s8 vy() const { return m_vy; }
		
		// Get character direction
		CharacterDirection direction() const { return m_direction; }
		
		// Get character lifes
		s16 lifes() const { return m_lifes; }
		u16 maxLifes() const { return m_maxLifes; }
		
		// Get map id
		u16 mapID() const { return m_mapID; }
		
		// Get map area
		u16 area() const { return m_area; }
		
		// Types
		bool isPlayer()  { return m_type == CHARA_PLAYER;	}
		bool isNPC()	 { return m_type == CHARA_NPC;		}
		bool isMonster() { return m_type == CHARA_MONSTER;	}
		
		// Increase or decrease hurt movement vectors
		void hxm(s16 value) { m_hx += value; m_hurt = true; }
		void hym(s16 value) { m_hy += value; m_hurt = true; }
		
		// Characters counter
		static u16 nbCharacters;
		
	protected:
		// Character id
		u16 m_id;
		
		// Character position
		s16 m_x;
		s16 m_y;
		
		// Default position
		s16 m_dx;
		s16 m_dy;
		
		// Movement vectors
		s8 m_vx;
		s8 m_vy;
		
		// Direction
		CharacterDirection m_direction;
		
		// Map id
		u16 m_mapID;
		
		// Map area
		u16 m_area;
		
		// Moving state
		bool m_moving;
		
		// Movement counters
		u8 m_countMoves;
		u8 m_vxCount;
		u8 m_vyCount;
		
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
		
		// Hurt movement vectors
		s16 m_hx;
		s16 m_hy;
		
		// Hurt state
		bool m_hurt;
		
		// Collision state
		bool m_inCollision;
		
		// Hurt timer
		Timer m_hurtTimer;
		u16 m_hurtTimerLastValue;
		
		// Attacking state
		bool m_isAttacking;
		
		// In door state
		bool m_inDoor;
		
		// Friend class Weapon
		friend class Sword;
		
		// Weapon
		Weapon *m_weapon;
		
		// Grass image
		Image *m_grassEffect;
		Sprite *m_waterEffect;
};

#endif // CHARACTER_H
