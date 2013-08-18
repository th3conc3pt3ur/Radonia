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
	{6, 1, 0, 0, 1, 0, 1, -1, 0, 0, -1, 0, -1},
	{6, 1, 0, 0, 1, 0, 1, -1, 0, 0, -1, 0, -1}
};

Player *CharacterManager::initPlayer() {
	return new Player;
}

Monster **CharacterManager::initAllMonsters() {
	// Initialize monsters array
	Monster **monsters = new Monster*[NB_MONSTERS];
	
	// Init monsters
	monsters[0] = initRedMonster(22 << 4, 10 << 4, DIR_RIGHT, 0);
	
	return monsters;
}

NPC **CharacterManager::initAllNPCs() {
	// Initialize NPCs array
	NPC **NPCs = new NPC*[NB_NPCs];
	
	// Init NPCs
	NPCs[0] = initBlueBoy(17 << 4, 2 << 4, DIR_DOWN, 0);
	
	return NPCs;
}

/* Monsters */

Monster *CharacterManager::initRedMonster(u16 x, u16 y, CharacterDirection direction, u16 mapID) {
	return new Monster(x, y, direction, mapID, MONSTER_REDMONSTER, (char*)"graphics/monsters/red_monster.png");
}

/* NPCs */

NPC *CharacterManager::initBlueBoy(u16 x, u16 y, CharacterDirection direction, u16 mapID) {
	return new NPC(x, y, direction, mapID, NPC_BLUEBOY, (char*)"graphics/characters/blue_boy.png");
}

void CharacterManager::moveCharacters() {
	for(u16 i = 0 ; i < Game::currentMap->NPCs().size() ; i++) {
		Game::currentMap->NPCs()[i]->move();
	}
	for(u16 i = 0 ; i < Game::currentMap->monsters().size() ; i++) {
		Game::currentMap->monsters()[i]->move();
	}
	Game::player->move();
}

void CharacterManager::renderCharacters() {
	for(u16 i = 0 ; i < Game::currentMap->NPCs().size() ; i++) {
		Game::currentMap->NPCs()[i]->render();
	}
	for(u16 i = 0 ; i < Game::currentMap->monsters().size() ; i++) {
		Game::currentMap->monsters()[i]->render();
	}
	Game::player->render();
}

