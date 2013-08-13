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
#include <iostream>

#include "includeSDL.h"

#include "types.h"
#include "config.h"
#include "timer.h"
#include "animation.h"

// Player animations
u16 Player_animations[8][4] = {
	{4,0},
	{5,1},
	{6,2},
	{7,3},
	{8,12,12,12},
	{9,13,13,13},
	{10,14,14,14},
	{11,15,15,15}
};

// Sword animations
u16 Sword_animations[4][4] = {
	{0,4,8,8},
	{1,5,9,9},
	{2,6,10,10},
	{3,7,11,11}
};

// NPC animations
u16 NPC_animations[4][4] = {
	{4,0},
	{5,1},
	{6,2},
	{7,3}
};

// Monster animations
u16 Monster_animations[12][4] = {
	{4,0},
	{5,1},
	{6,2},
	{7,3}
};

