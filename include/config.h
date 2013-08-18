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
#ifndef CONFIG_H
#define CONFIG_H

#define MAP_AREAS 3

#define NB_TILESETS 3

#define WM_SIZE 2
#define INDOOR_MAPS 3

#define NB_NPCs 1
#define NB_MONSTERS 2

#define NB_CHARACTERS (1 + NB_NPCs + NB_MONSTERS)

#define DOORS 12

#define MAP_WIDTH 40
#define MAP_HEIGHT 30

#define CHARACTER_SPEED 1

#ifdef __ANDROID__
	#define VIEWPORT
	#define PAD
#endif

#endif // CONFIG_H
