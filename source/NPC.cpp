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
#include <cstdlib>

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "types.h"
#include "config.h"
#include "timer.h"
#include "animation.h"
#include "sprite.h"
#include "character.h"
#include "monster.h"
#include "NPC.h"
#include "player.h"
#include "map.h"
#include "door.h"
#include "animationManager.h"
#include "mapManager.h"
#include "interface.h"
#include "game.h"

char *NPC::texts[NB_NPCs] = {(char*)"Hello boy!"};
int NPC::moves[NB_NPCs][21] = {{6, 1, 0, 0, 1, 0, 1, -1, 0, 0, -1, 0, -1}};

NPC::NPC(u16 x, u16 y, u8 direction, u16 mapID, char *filename) : Sprite(filename, SPRITE_NPC, x, y) {
	// Add animations to sprite
	addAnimation(2, NPC_animations[0], 250); // Down
	addAnimation(2, NPC_animations[1], 250); // Right
	addAnimation(2, NPC_animations[2], 250); // Left
	addAnimation(2, NPC_animations[3], 250); // Up
}

NPC::~NPC() {
}

void NPC::action() {
	Interface::newDialogBox(texts[m_id]);
}

NPC *NPC::BlueBoy(u16 x, u16 y, u8 direction, u16 mapID) {
	return new NPC(x, y, direction, mapID, (char*)"graphics/characters/blue_boy.png");
}

NPC **NPC::initAll() {
	// Initialize NPCs array
	NPC **NPCs = new NPC*[NB_NPCs];
	
	// Init NPCs
	NPCs[0] = NPC::BlueBoy(10 << 4, 2 << 4, Direction::Down, 0);
	
	return NPCs;
}

