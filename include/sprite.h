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

class Sprite {
	public:
		// Constructor and destructor
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
		// Sprite texture
		sf::Texture m_tex;
		
		// Sprite frame to display
		sf::Sprite m_spr;
		
		// Sprite frame size
		u8 m_frameSize;
		
		// Animations table
		std::vector<Sprite_Animation*> m_animations;
};

#endif // SPRITE_H
