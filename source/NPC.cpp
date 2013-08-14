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
#include "image.h"
#include "animation.h"
#include "sprite.h"
#include "weapon.h"
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

NPC::NPC(u16 x, u16 y, CharacterDirection direction, u16 mapID, NPCType type, char *filename) : Character(filename, CHARA_NPC, x, y, direction, mapID) {
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

