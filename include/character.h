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
		Character(const char *filename, CharacterType type, s16 x, s16 y, CharacterDirection direction, u16 mapID, u16 area, u8 frameWidth = 16, u8 frameHeight = 16);
		virtual ~Character();
		
		// Movement function
		virtual void move() = 0;
		
		// Action function
		virtual void action() = 0;
		
		// Render character
		void render();
		
		// Test collisions
		void testCollisions();
		void doorCollisions();
		
		// Collisions functions
		bool inNonPassableTile();
		void mapCollisions();
		void charactersCollisions();
		void collisionAction(Character *c);
		
		// Hurt comportement
		void hurt(s16 hx, s16 hy);
		void hurtMovement();
		
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
		
		// Get character hitbox
		s16 hitboxX() const { return m_hitboxX; }
		s16 hitboxY() const { return m_hitboxY; }
		u16 hitboxW() const { return m_hitboxW; }
		u16 hitboxH() const { return m_hitboxH; }
		
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
		
		// Characters counter
		static u16 nbCharacters;
		
	protected:
		/* CHARACTER INFORMATION */
			// Character id
			u16 m_id;
			
			// Character lifes
			s16 m_lifes;
			u16 m_maxLifes;
			
			// Character type
			CharacterType m_type;
			
		/* POSITION */
			// Character position
			s16 m_x;
			s16 m_y;
			
			// Default position
			s16 m_dx;
			s16 m_dy;
			
			// Last position
			s16 m_lx;
			s16 m_ly;
			
			// Map id
			u16 m_mapID;
			
			// Map area
			u16 m_area;
			
			// In door state
			bool m_inDoor;
			
		/* MOVEMENT */
			// Movement vectors
			s8 m_vx;
			s8 m_vy;
			
			// Direction
			CharacterDirection m_direction;
			
			// Moving state
			bool m_moving;
			
			// Movement counters
			u8 m_countMoves;
			u8 m_vxCount;
			u8 m_vyCount;
			
			// Movement timer (NPC|Monsters)
			Timer m_movementTimer;
			
			// Blocked movement and direction states
			bool m_canMove;
			bool m_canTurn;
			
		/* COLLISIONS */
			// Collision state
			bool m_inCollision;
			
			// Hitbox
			s16 m_hitboxX;
			s16 m_hitboxY;
			u16 m_hitboxW;
			u16 m_hitboxH;
			
			// Hurt movement vectors
			s16 m_hx;
			s16 m_hy;
			
			// Hurt state
			bool m_hurt;
			
			// Hurt timer
			Timer m_hurtTimer;
			u16 m_hurtTimerLastValue;
			
		/* ATTACK */
			// Attacking state
			bool m_isAttacking;
			
			// Friend class Weapon
			friend class Sword;
			
			// Weapon
			Weapon *m_weapon;
			
		/* EFFECTS */
			// Grass image
			Image *m_grassEffect;
			Sprite *m_waterEffect;
};

#endif // CHARACTER_H
