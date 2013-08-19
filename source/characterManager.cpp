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
#include <algorithm>

#include "includeSDL.h"

#include "types.h"
#include "color.h"
#include "config.h"
#include "window.h"
#include "keyboard.h"
#include "timer.h"
#include "animation.h"
#include "image.h"
#include "sprite.h"
#include "weapon.h"
#include "character.h"
#include "monster.h"
#include "NPC.h"
#include "player.h"
#include "map.h"
#include "door.h"
#include "mapManager.h"
#include "doorManager.h"
#include "characterManager.h"
#include "collisionManager.h"
#include "tools.h"
#include "interface.h"
#include "game.h"

// Movements are defined like that:
// {nbMovements, vx1, vy1, vx2, vy2, vx3, vy3, etc...}

s16 CharacterManager::moves[][21] = {
	{0},
	{6, 1, 0, 0, 1, 0, 1, -1, 0, 0, -1, 0, -1},
	{6, 1, 0, 0, 1, 0, 1, -1, 0, 0, -1, 0, -1},
	{6, 1, 0, 0, 1, 0, 1, -1, 0, 0, -1, 0, -1}
};

Character **CharacterManager::characters;

void CharacterManager::initAllCharacters() {
	// Initialize characters array
	characters = new Character*[NB_CHARACTERS];
	
	// Init player
	characters[0] = new Player(17 << 4, 21 << 4, DIR_RIGHT, 0, 0);
	
	// Init monsters
	characters[1] = initRedMonster(22 << 4, 10 << 4, DIR_RIGHT, 0, 0);
	characters[3] = initRedMonster(25 << 4, 10 << 4, DIR_RIGHT, 1, 0); 
	
	// Init NPCs
	characters[2] = initBlueBoy(17 << 4, 2 << 4, DIR_DOWN, 0, 0);
}

std::vector<Character*> CharacterManager::getCharactersInMap(u16 id, u16 area) {
	std::vector<Character*> c;
	
	for(u16 i = 0 ; i < NB_CHARACTERS ; i++) {
		if((characters[i]->mapID() == id
		&&	characters[i]->area()  == area)
		|| characters[i]->isPlayer()) {
			c.push_back(characters[i]);
		}
	}
	
	return c;
}

/* Monsters */

Monster *CharacterManager::initRedMonster(u16 x, u16 y, CharacterDirection direction, u16 mapID, u16 area) {
	return new Monster(x, y, direction, mapID, area, MONSTER_REDMONSTER, (char*)"graphics/monsters/red_monster.png");
}

/* NPCs */

NPC *CharacterManager::initBlueBoy(u16 x, u16 y, CharacterDirection direction, u16 mapID, u16 area) {
	return new NPC(x, y, direction, mapID, area, NPC_BLUEBOY, (char*)"graphics/characters/blue_boy.png");
}

bool CharacterManager::sortCharacters(Character *c1, Character *c2) {
	return (c1->y() < c2->y());
}

Player *CharacterManager::player() {
	return (Player*)characters[0];
}

void CharacterManager::moveCharacters() {
	for(std::vector<Character*>::iterator it = MapManager::currentMap->characters()->begin() ; it != MapManager::currentMap->characters()->end() ; it++) {
		(*it)->move();
	}
}

void CharacterManager::renderCharacters() {
	std::sort(MapManager::currentMap->characters()->begin(), MapManager::currentMap->characters()->end(), sortCharacters);
	for(std::vector<Character*>::iterator it = MapManager::currentMap->characters()->begin() ; it != MapManager::currentMap->characters()->end() ; it++) {
		if((*it)->lifes() > 0 && (*it)->isMonster()) Interface::renderMonsterLife((Monster*)(*it));
		(*it)->render();
	}
}

