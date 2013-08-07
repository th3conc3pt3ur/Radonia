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
#ifndef SPRITE_H
#define SPRITE_H

class Sprite_Animation {
	public:
		// Constructor and destructor
		Sprite_Animation(int size, int *tabAnim, int delay);
		~Sprite_Animation();
		
		// Get animation size
		int size() const { return m_size; }
		
		// Get animations table
		int *tabAnim() const { return m_tabAnim; }
		
		// Get delay between animations
		int delay() const { return m_delay; }
		
		// Get timer
		Timer *tmr() const { return m_tmr; }
		
		// Get if the animation is playing or not
		bool isPlaying() const { return m_isPlaying; }
		
		// Set if the animations is playing or not
		void isPlaying(bool isPlaying) { m_isPlaying = isPlaying; }
		
	private:
		// Animation size
		int m_size;
		
		// Animations table
		int *m_tabAnim;
		
		// Delay between animations
		int m_delay;
		
		// Timer
		Timer *m_tmr;
		
		// Variable for telling the animation to play or not
		bool m_isPlaying;
};

typedef enum {
	SPRITE_NONE,
	SPRITE_PLAYER,
	SPRITE_NPC,
	SPRITE_MONSTER,
	SPRITE_PWEAPON,
	SPRITE_MWEAPON
} SpriteType;

class Sprite {
	public:
		// Constructor and destructor
		Sprite(char *filename, SpriteType type = SPRITE_NONE, s16 x = -1, s16 y = -1, u8 frameSize = 16);
		~Sprite();
		
		// Draw sprite
		void drawFrame(s16 x, s16 y, int frame);
		
		// Animations
		void addAnimation(int size, int *tabAnim, int delay);
		void resetAnimation(int anim);
		void startAnimation(int anim);
		void stopAnimation(int anim);
		bool animationAtEnd(int anim);
		bool animationAtFrame(int anim, int frame);
		void playAnimation(s16 x, s16 y, int anim);
		
		// Hurt sprite
		void hurt();
		
		// Speak
		void speak() {};
		
		// Get sprite
		sf::Sprite spr() { return m_spr; }
		
		// Get default color
		sf::Color defaultColor() const { return m_defaultColor; }
		
		// Get sprite position
		s16 x() const { return m_x; }
		s16 y() const { return m_y; }
		
		// Get sprite movement vectors
		s8 vx() const { return m_vx; }
		s8 vy() const { return m_vy; }
		
		// Get sprite lifes
		s16 lifes() const { return m_lifes; }
		u16 maxLifes() const { return m_maxLifes; }
		
		// Types
		bool isNone()	 { return m_type == SPRITE_NONE;	}
		bool isPlayer()  { return m_type == SPRITE_PLAYER;	}
		bool isNPC()	 { return m_type == SPRITE_NPC;		}
		bool isMonster() { return m_type == SPRITE_MONSTER;	}
		bool isPWeapon() { return m_type == SPRITE_PWEAPON;	}
		bool isMWeapon() { return m_type == SPRITE_MWEAPON;	}
		
		// Set sprite position
		void x(s16 x) { m_x = x; }
		void y(s16 y) { m_y = y; }
		
		// Set sprite movement vectors
		void vx(s8 vx) { m_vx = vx; }
		void vy(s8 vy) { m_vy = vy; }
		
		// Sprite view
		static sf::View *View;
		
		// Which sprite the sprite is colliding
		Sprite *collidedSprite;
		
		// Which tile the sprite is colliding
		int collidedTile;
		
	protected:
		// Sprite texture
		sf::Texture m_tex;
		
		// Sprite frame to display
		sf::Sprite m_spr;
		
		// Default texture color
		sf::Color m_defaultColor;
		
		// Sprite frame size
		u8 m_frameSize;
		
		// Animations table
		std::vector<Sprite_Animation*> m_animations;
		
		// Sprite position
		s16 m_x;
		s16 m_y;
		
		// Movement vectors
		s8 m_vx;
		s8 m_vy;
		
		// Sprite hurt timer
		Timer m_hurtTimer;
		u16 m_timerLastValue;
		
		// Sprite lifes
		s16 m_lifes;
		u16 m_maxLifes;
		
		// Sprite type
		SpriteType m_type;
};

#endif // SPRITE_H
