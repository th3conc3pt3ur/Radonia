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
#include "characterManager.h"
#include "collisionManager.h"
#include "mapManager.h"
#include "interface.h"
#include "game.h"

char *NPC::texts[NB_NPCs] = {(char*)"Hello boy!"};

NPC::NPC(u16 x, u16 y, CharacterDirection direction, u16 mapID, u16 area, NPCType type, char *filename) : Character(filename, CHARA_NPC, x, y, direction, mapID, area) {
	// Add animations to sprite
	addAnimation(2, NPC_animations[0], 250); // Down
	addAnimation(2, NPC_animations[1], 250); // Right
	addAnimation(2, NPC_animations[2], 250); // Left
	addAnimation(2, NPC_animations[3], 250); // Up
}

NPC::~NPC() {
}

void NPC::move() {
	// FIXME: To improve
	// Area to walk in
	u16 rectW = 4;
	u16 rectH = 4;
	u16 minX = m_dx - rectW * TILE_SIZE / 2;
	u16 minY = m_dy - rectH * TILE_SIZE / 2;
	u16 maxX = m_dx + rectW * TILE_SIZE / 2;
	u16 maxY = m_dy + rectH * TILE_SIZE / 2;
	
	// If it's time to move
	if(m_movementTimer.time() > 4000) {
		// Initialize random number
		int randn;
		
		// Initialize in movement boolean
		bool mvt = false;
		while(!mvt) {
			// Get a random number between 0 and 4
			randn = rand()%4;
			
			switch(randn) {
				case DIR_UP:
					if(m_y - TILE_SIZE > minY) {
						m_vy = -1;
						mvt = true;
					} break;
				case DIR_DOWN:
					if(m_y + TILE_SIZE < maxY) {
						m_vy = 1;
						mvt = true;
					} break;
				case DIR_LEFT:
					if
			}
		}
	}
}

void NPC::action() {
	Interface::newDialogBox(texts[m_id]);
}

