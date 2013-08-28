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
#include "font.h"
#include "sound.h"
#include "net.h"
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

Character **CharacterManager::characters;

void CharacterManager::initAllCharacters() {
	// Initialize characters array
	characters = new Character*[NB_CHARACTERS];
	
	// Init player
	characters[0] = new Player(17 << 4, 21 << 4, DIR_RIGHT, 0, 0);
	
	// Init monsters
	characters[1] = initRedMonster(22 << 4, 10 << 4, DIR_RIGHT, 0, 0);
	characters[3] = initGohma(16 << 4, 3 << 4, DIR_DOWN, 1, 0); 
	
	// Init NPCs
	characters[2] = initBlueBoy(17 << 4, 2 << 4, DIR_DOWN, 0, 0);
}

std::vector<Character*> CharacterManager::getCharactersInMap(u16 id) {
	std::vector<Character*> c;
	
	for(u16 i = 0 ; i < NB_CHARACTERS ; i++) {
		if(characters[i]->mapID() == id
		|| characters[i]->isPlayer()) {
			c.push_back(characters[i]);
		}
	}
	
	return c;
}

/* Monsters */

Monster *CharacterManager::initRedMonster(u16 x, u16 y, CharacterDirection direction, u16 mapID, u16 area) {
	return new Monster(x, y, direction, mapID, area, MONSTER_REDMONSTER, "graphics/monsters/red_monster.png");
}

Monster *CharacterManager::initGohma(u16 x, u16 y, CharacterDirection direction, u16 mapID, u16 area) {
	return new Monster(x, y, direction, mapID, area, MONSTER_GOHMA, "graphics/monsters/gohma.png", 48, 24);
}

/* NPCs */

NPC *CharacterManager::initBlueBoy(u16 x, u16 y, CharacterDirection direction, u16 mapID, u16 area) {
	return new NPC(x, y, direction, mapID, area, NPC_BLUEBOY, "graphics/characters/blue_boy.png");
}

bool CharacterManager::sortCharacters(Character *c1, Character *c2) {
	return (c1->y() < c2->y());
}

Player *CharacterManager::player() {
	return (Player*)characters[0];
}

void CharacterManager::moveCharacters() {
	std::vector<Character*> *v = MapManager::currentMap->characters();
	for(std::vector<Character*>::iterator it = v->begin() ; it != v->end() ; it++) {
		(*it)->move();
	}
}

void CharacterManager::renderCharacters() {
	std::vector<Character*> *v = MapManager::currentMap->characters();
	std::sort(v->begin(), v->end(), sortCharacters);
	for(std::vector<Character*>::iterator it = v->begin() ; it != v->end() ; it++) {
		if((*it)->isPlayer()) ((Player*)(*it))->action();
		if((*it)->lifes() > 0 && (*it)->isMonster()) Interface::renderMonsterLife((Monster*)(*it));
		(*it)->render();
	}
}

