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
#include <iostream>
#include <cstdio>

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "types.h"
#include "config.h"
#include "timer.h"
#include "sprite.h"
#include "monster.h"
#include "NPC.h"
#include "player.h"
#include "map.h"
#include "mapManager.h"
#include "door.h"
#include "game.h"

Door** initDoors() {
	Door** doors = new Door*[DOORS];
	
	doors[0] = new Door{0, 0, 6 << 4, 3 << 4, Direction::Down, 1};
	doors[1] = new Door{1, 0, (19 << 4) + 8, 20 << 4, Direction::Up, 0};
	
	doors[2] = new Door{1, 0, 11 << 4, 7 << 4, Direction::Down, 3};
	doors[3] = new Door{1, 1, 11 << 4, 17 << 4, Direction::Up, 2};
	
	doors[4] = new Door{0, MAP_POS(0, 1, 0), 19 << 4, 14 << 4, Direction::Down, 5};
	doors[5] = new Door{2, 0, (15 << 4) + 8, 24 << 4, Direction::Up, 4};
	
	doors[6] = new Door{0, 0, 4 << 4, 2 << 4, Direction::Down, 7};
	doors[7] = new Door{1, 0, 28 << 4, 7 << 4, Direction::Down, 6};
	
	doors[8] = new Door{0, MAP_POS(1, 1, 0), 10 << 4, 24 << 4, Direction::Up, 9};
	doors[9] = new Door{2, MAP_POS(1, 0, 2), 37 << 4, 19 << 4, Direction::Left, 8};
	
	return doors;
}

s16 findDoorID(s16 x, s16 y, u16 mapID, u16 mapArea) {
	Door** doors = Game::doors;
	for(u16 i = 0; i < DOORS ; i++) {
		//std::cout << "(" << (doors[i]->x >> 4) << ";" << (doors[i]->y >> 4) << ") & " << doors[i]->mapArea << " & " << _mid(doors[i]->mapArea, doors[i]->mapID) << " | (" << (x >> 4) << ";" << (y >> 4) << ") & " << mapArea << " & " << mapID << std::endl;
		if(((doors[i]->x >> 4 == x >> 4) || (doors[i]->x >> 4 == (x >> 4) + 1) || (doors[i]->x >> 4 == (x >> 4) - 1)) && ((doors[i]->y >> 4 == y >> 4) || (doors[i]->y >> 4 == (y >> 4) + 1)) && (doors[i]->mapArea == mapArea) && (_mid(doors[i]->mapArea, doors[i]->mapID) == mapID)) {
			return i;
		}
	}
	return -1;
}

