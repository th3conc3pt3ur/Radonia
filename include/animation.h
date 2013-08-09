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

// Animation struct
struct Animation {
	// Constructor
	Animation(int _size, int *_tabAnim, int _delay, bool _isPlaying = false) :
		size(_size), tabAnim(_tabAnim), delay(_delay), isPlaying(_isPlaying) {}
	
	int size;		// Animation size
	int *tabAnim;	// Animation table
	int delay;		// Delay between animations
	Timer timer;	// Timer
	bool isPlaying; // Playing animation state
};

typedef Animation Animation;

#endif // ANIMATION_H
