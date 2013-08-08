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
		
		// Get sprite
		sf::Sprite spr() { return m_spr; }
		
		// Get default color
		sf::Color defaultColor() const { return m_defaultColor; }
		
		// Sprite view
		static sf::View *View;
		
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
		std::vector<Animation*> m_animations;
};

#endif // SPRITE_H
