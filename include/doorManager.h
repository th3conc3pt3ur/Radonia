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
#ifndef DOOR_MANAGER_H
#define DOOR_MANAGER_H

namespace DoorManager {
	// Doors array
	extern Door **doors;
	
	// Initialize doors
	void initDoors();
	
	// Find the id of a door with its position
	s16 findDoorID(s16 x, s16 y, u16 mapID, u16 mapArea);
}

#endif // DOOR_MANAGER_H
