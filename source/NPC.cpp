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
#include "sprite.h"
#include "NPC.h"
#include "player.h"
#include "interface.h"
#include "map.h"
#include "mapManager.h"
#include "door.h"
#include "game.h"

// Set animations table
int NPC_animations[12][4] = {
	{4,0},
	{5,1},
	{6,2},
	{7,3}
};

int NPC::nbNPCs = 0;

char *NPC::texts[NB_NPCs] = {(char*)"Hello boy!"};
int NPC::moves[NB_NPCs][21] = {{6, 0, 1, 1, 0, 0, 1, -1, 0, 0, -1, 0, -1}};

NPC::NPC(u16 x, u16 y, u8 direction, u16 mapID, char *filename) : Sprite(filename) {
	// Set NPC id
	m_id = nbNPCs;
	
	// Update NPCs counter
	nbNPCs++;
	
	// Set class members
	m_x = x;
	m_y = y;
	
	m_direction = direction;
	
	m_countMoves = 0;
	m_vxCount = 0;
	m_vyCount = 0;
	
	// Restart clock
	m_timer.start();
	
	// Add animations to sprite
	addAnimation(2, NPC_animations[0], 250); // Down
	addAnimation(2, NPC_animations[1], 250); // Right
	addAnimation(2, NPC_animations[2], 250); // Left
	addAnimation(2, NPC_animations[3], 250); // Up
}

NPC::~NPC() {
}

void NPC::move() {
	// If player collided NPC, don't move
	if(Player::collidedNPC) { m_timer.stop(); return; }
	else m_timer.start();
	
	// Move or not?
	if(m_timer.time() > 4000) {
		// Update movement vectors
		m_vx = moves[m_id][m_countMoves * 2 + 1];
		m_vy = moves[m_id][m_countMoves * 2 + 2];
		
		// Update counters
		m_vxCount += m_vx;
		m_vyCount += m_vy;
	}
	
	if(m_vxCount >= 16 || m_vyCount >= 16) {
		// Update counter
		m_countMoves++;
		
		// Reset counters
		m_vxCount = 0;
		m_vyCount = 0;
		
		// Reset timer
		m_timer.reset();
		m_timer.start();
	}
	
	if(m_countMoves >= moves[m_id][0]) {
		// Reset timer and counter
		m_countMoves = 0;
		m_timer.reset();
		m_timer.start();
	}
	
	if(m_vx > 0) m_direction = Direction::Right;
	if(m_vx < 0) m_direction = Direction::Left;
	if(m_vy > 0) m_direction = Direction::Down;
	if(m_vy < 0) m_direction = Direction::Up;
	
	// Move NPC
	m_x += m_vx;
	m_y += m_vy;
	
	// Reset movement vectors
	m_vx = 0;
	m_vy = 0;
}

void NPC::render() {
	// Play animation
	playAnimation(m_x, m_y, m_direction);
}

void NPC::speak() {
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

