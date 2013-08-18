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
#ifndef CHARACTER_MANAGER_H
#define CHARACTER_MANAGER_H

namespace CharacterManager {
	// Movements
	extern s16 moves[][21];
	
	// Characters array
	extern Character **characters;
	
	// Initialize all characters
	void initAllCharacters();
	
	// Get current map characters
	std::vector<Character*> *getCharactersInMap(u16 id, u16 area);
	
	/* Monsters */
	Monster *initRedMonster(u16 x, u16 y, CharacterDirection direction, u16 mapID, u16 area);
	
	/* NPCs */
	NPC *initBlueBoy(u16 x, u16 y, CharacterDirection direction, u16 mapID, u16 area);
	
	// Sort characters by y coordinate for rendering
	bool sortCharacters(Character *c1, Character *c2);
	
	// Get player
	Player *player();
	
	// Move all characters
	void moveCharacters();
	
	// Render all characters
	void renderCharacters();
}

#endif // CHARACTER_MANAGER_H
