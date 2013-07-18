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
#include <SFML/System.hpp>

#include "timer.h"

sf::Clock Timer::clock;

Timer::Timer() {
	m_t = clock.GetElapsedTime() * 1000;
	m_isStarted = false;
	m_tick = 0;
}

Timer::~Timer() {
	reset();
	stop();
}

void Timer::stop() {
	if(m_isStarted) {
		m_isStarted = false;
		m_tick = clock.GetElapsedTime() * 1000 - m_t;
	}
}

void Timer::start() {
	if(!m_isStarted) {
		m_isStarted = true;
		m_t = clock.GetElapsedTime() * 1000 - m_tick;
	}
}

void Timer::reset() {
	m_t = clock.GetElapsedTime() * 1000;
	m_isStarted = false;
	m_tick = 0;
}

void Timer::initTimers() {
	clock.Reset();
}

