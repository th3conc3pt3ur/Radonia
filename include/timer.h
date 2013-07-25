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
#ifndef TIMER_H
#define TIMER_H

class Timer {
	public:
		// Constructor and destructor
		Timer();
		~Timer();
		
		// Timer control functions
		void stop();
		void start();
		void reset();
		
		// Get time function
		int time() { return (m_isStarted) ? clock.getElapsedTime().asMilliseconds() - m_t : m_tick; }
		
		// Initialization function
		static void initTimers();
		
		// Time since global initialization (in seconds)
		static sf::Clock clock;
		
	private:
		// Time at initialization
		int m_t;
		
		// Is the timer started?
		bool m_isStarted;
		
		// Time elapsed since initialization
		int m_tick;
};

#endif // TIMER_H
