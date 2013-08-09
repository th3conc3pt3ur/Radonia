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
#ifndef CHARACTER_MANAGER_H
#define CHARACTER_MANAGER_H

namespace CharacterManager {
	// Initialize player
	Player *initPlayer();
	
	// Initialize all monsters
	Monster **initAllMonsters();
	
	// Initialize all NPCs
	NPC **initAllNPCs();
	
	/* Monsters */
	Monster *initRedMonster(u16 x, u16 y, u8 direction, u16 mapID);
	
	/* NPCs */
	NPC *initBlueBoy(u16 x, u16 y, u8 direction, u16 mapID);
}

#endif // CHARACTER_MANAGER_H
