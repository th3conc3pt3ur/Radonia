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
#include "tools.h"
#include "game.h"

// Monsters are called Qaewans

int Monster::nbMonsters = 0;

int Monster::moves[NB_MONSTERS][21] = {{6, 1, 0, 0, 1, 0, 1, -1, 0, 0, -1, 0, -1}};

Monster::Monster(u16 x, u16 y, u8 direction, u16 mapID, char *filename) : Sprite(filename, SPRITE_MONSTER, x, y) {
	// Set monster id
	m_id = nbMonsters;
	
	// Update monsters counter
	nbMonsters++;
	
	m_direction = direction;
	
	m_lifes = 100;
	m_maxLifes = 100;
	
	m_moving = false;
	
	m_countMoves = 0;
	m_vxCount = 0;
	m_vyCount = 0;
	
	// Start movement timer
	m_movementTimer.start();
	
	// Add animations to sprite
	addAnimation(2, Monster_animations[0], 250); // Down
	addAnimation(2, Monster_animations[1], 250); // Right
	addAnimation(2, Monster_animations[2], 250); // Left
	addAnimation(2, Monster_animations[3], 250); // Up
}

Monster::~Monster() {
}

void Monster::move() {
	// If player collided monster, don't move
	if(((m_x + 2 > Game::player->x() && m_x + 2 < Game::player->x() + 16)
	&&  (m_y + 2 > Game::player->y() && m_y + 2 < Game::player->y() + 16))
	|| ((m_x + 14 > Game::player->x() && m_x + 14 < Game::player->x() + 16)
	&&  (m_y + 2 > Game::player->y() && m_y + 2 < Game::player->y() + 16))
	|| ((m_x + 2 > Game::player->x() && m_x + 2 < Game::player->x() + 16)
	&&  (m_y + 14 > Game::player->y() && m_y + 14 < Game::player->y() + 16))
	|| ((m_x + 14 > Game::player->x() && m_x + 14 < Game::player->x() + 16)
	&&  (m_y + 14 > Game::player->y() && m_y + 14 < Game::player->y() + 16))) {
		m_movementTimer.stop();
		Game::player->collidedSprite = this;
		return;
	}
	else m_movementTimer.start();
	
	// Move or not?
	if(m_movementTimer.time() > 4000) {
		// Update movement vectors
		m_vx = moves[m_id][m_countMoves * 2 + 1];
		m_vy = moves[m_id][m_countMoves * 2 + 2];
		
		// Update counters
		m_vxCount += m_vx * m_vx;
		m_vyCount += m_vy * m_vy;
		
		// Update moving state
		m_moving = true;
	}
	
	if(m_vxCount >= 16 || m_vyCount >= 16) {
		// Update counter
		m_countMoves++;
		
		// Reset counters
		m_vxCount = 0;
		m_vyCount = 0;
		
		// Reset timer
		m_movementTimer.reset();
		m_movementTimer.start();
		
		// Update moving state
		m_moving = false;
	}
	
	if(m_countMoves >= moves[m_id][0]) {
		// Reset timer and counter
		m_countMoves = 0;
		m_movementTimer.reset();
		m_movementTimer.start();
	}
	
	if(m_vx > 0) m_direction = Direction::Right;
	if(m_vx < 0) m_direction = Direction::Left;
	if(m_vy > 0) m_direction = Direction::Down;
	if(m_vy < 0) m_direction = Direction::Up;
	
	// Move monster
	m_x += m_vx;
	m_y += m_vy;
	
	// Reset movement vectors
	m_vx = 0;
	m_vy = 0;
}

void Monster::render() {
	// Render monster
	if(m_moving) playAnimation(m_x, m_y, m_direction);
	else drawFrame(m_x, m_y, m_direction);
}

Monster *Monster::RedMonster(u16 x, u16 y, u8 direction, u16 mapID) {
	return new Monster(x, y, direction, mapID, (char*)"graphics/monsters/red_monster.png");
}

Monster **Monster::initAll() {
	// Initialize monsters array
	Monster **monsters = new Monster*[NB_MONSTERS];
	
	// Init monsters
	monsters[0] = Monster::RedMonster(22 << 4, 14 << 4, Direction::Right, 0);
	
	return monsters;
}

