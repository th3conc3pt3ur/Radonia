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
#ifndef ANIMATION_H
#define ANIMATION_H

class Animation {
	public:
		// Constructor and destructor
		Animation(int size, int *tabAnim, int delay);
		~Animation();
		
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
		
		// Set if the animation is playing or not
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

#endif // ANIMATION_H
