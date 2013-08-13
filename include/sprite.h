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
#ifndef SPRITE_H
#define SPRITE_H

#include <vector>

class Sprite : public Image {
	public:
		// Constructor and destructor
		Sprite(char *filename, u8 frameSize = 16);
		~Sprite();
		
		// Draw sprite
		void drawFrame(s16 x, s16 y, u16 frame);
		
		// Animations
		void addAnimation(u16 size, u16 *tabAnim, u16 delay);
		void resetAnimation(u16 anim);
		void startAnimation(u16 anim);
		void stopAnimation(u16 anim);
		bool animationAtEnd(u16 anim);
		bool animationAtFrame(u16 anim, u16 frame);
		void playAnimation(s16 x, s16 y, u16 anim);
		
	protected:
		// Sprite frame size
		u8 m_frameSize;
		
		// Animations table
		std::vector<Animation*> m_animations;
};

#endif // SPRITE_H
