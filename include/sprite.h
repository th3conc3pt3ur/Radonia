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
		Sprite_Animation(int size, int *tabAnim, int delay);
		~Sprite_Animation();
		
		int size() const { return m_size; }
		int *tabAnim() const { return m_tabAnim; }
		int delay() const { return m_delay; }
		Timer *tmr() const { return m_tmr; }
		bool isPlaying() const { return m_isPlaying; }
		
		void isPlaying(bool isPlaying) { m_isPlaying = isPlaying; }
		
	private:
		int m_size;
		int *m_tabAnim;
		int m_delay;
		Timer *m_tmr;
		bool m_isPlaying;
};

class Sprite {
	public:
		Sprite(char *filename, u8 frameSize = 16);
		~Sprite();
		
		// Draw sprite
		void drawFrame(s16 x, s16 y, int frame);
		
		// Animations
		void addAnimation(int size, int *tabAnim, int delay);
		void resetAnimation(int anim);
		void startAnimation(int anim);
		void stopAnimation(int anim);
		bool animationAtEnd(int anim);
		void playAnimation(s16 x, s16 y, int anim);
		
		// Sprite view
		static sf::View *View;
		
	protected:
		// Sprite image
		sf::Image m_img;
		
		// Sprite frame to display
		sf::Sprite m_spr;
		
		// Sprite frame size
		u8 m_frameSize;
		
		// Animations table
		std::vector<Sprite_Animation*> m_animations;
};

#endif // SPRITE_H
