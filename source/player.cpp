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
#include "monster.h"
#include "NPC.h"
#include "player.h"
#include "interface.h"
#include "map.h"
#include "mapManager.h"
#include "door.h"
#include "game.h"

// Set animations table
int Player_animations[8][4] = {
	{4,0},
	{5,1},
	{6,2},
	{7,3},
	{8,12,12,12},
	{9,13,13,13},
	{10,14,14,14},
	{11,15,15,15}
};

int Sword_animations[4][4] = {
	{0,4,8,8},
	{1,5,9,9},
	{2,6,10,10},
	{3,7,11,11}
};

NPC *Player::collidedNPC = NULL;
Monster *Player::collidedMonster = NULL;
int Player::collidedTile = 0;

Player::Player() : Sprite((char*)"graphics/characters/link.png") {
	// Set class members
	m_x = 120;
	m_y = 160;
	
	m_vx = 0;
	m_vy = 0;
	
	m_direction = Direction::Right;
	
	m_maxLifes = 5;
	m_lifes = 3 * 4 + 3;
	
	m_hurtTimer.reset();
	m_hurtTimer.start();
	
	m_timerLastValue = 0;
	
	m_defaultColor = m_spr.getColor();
	
	m_isAttacking = false;
	
	m_swordSpr = new Sprite((char*)"graphics/animations/sword.png");
	
	m_swordSpr->addAnimation(4, Sword_animations[0], 50); // Down
	m_swordSpr->addAnimation(4, Sword_animations[1], 50); // Right
	m_swordSpr->addAnimation(4, Sword_animations[2], 50); // Left
	m_swordSpr->addAnimation(4, Sword_animations[3], 50); // Up
	
	// Add animations to sprite
	addAnimation(2, Player_animations[0], 100); // Down
	addAnimation(2, Player_animations[1], 100); // Right
	addAnimation(2, Player_animations[2], 100); // Left
	addAnimation(2, Player_animations[3], 100); // Up
	addAnimation(4, Player_animations[4], 50); // Attack down
	addAnimation(4, Player_animations[5], 50); // Attack right
	addAnimation(4, Player_animations[6], 50); // Attack left
	addAnimation(4, Player_animations[7], 50); // Attack up
}

Player::~Player() {
	delete m_swordSpr;
}

bool inDoor = false;
void Player::doorCollisions() {
	if(((inTiles((m_x + 8) >> 4, (m_y + 8) >> 4, changeMapTiles)) && (!inDoor))) {
		// Reset movement vectors
		m_vx = 0;
		m_vy = 0;
		
		// Search for the door
		s16 doorID = findDoorID(m_x, m_y, Game::currentMap->id(), Game::currentMap->area());
		
		// If door isn't found
		if(doorID == -1) {
			return;
		}
		
		// Initialize transition
		sf::RectangleShape rect1(sf::Vector2f(MAP_WIDTH * 16 / 2, MAP_HEIGHT * 16));
		sf::RectangleShape rect2(sf::Vector2f(MAP_WIDTH * 16 / 2, MAP_HEIGHT * 16));
		
		rect1.setPosition(0, 0);
		rect2.setPosition(MAP_WIDTH * 16 / 2, 0);
		
		Game::MainWindow->clear();
		Game::MainWindow->setView(*Sprite::View);
		Game::MainWindow->draw(rect1);
		Game::MainWindow->draw(rect2);
		Game::MainWindow->setView(Game::MainWindow->getDefaultView());
		Game::MainWindow->display();
		
		// Update all values
		Game::currentMap = Game::mapAreas[Game::doors[Game::doors[doorID]->nextDoorID]->mapArea][Game::doors[Game::doors[doorID]->nextDoorID]->mapID];
		if(!Game::currentMap) exit(EXIT_FAILURE);
		
		m_x = Game::doors[Game::doors[doorID]->nextDoorID]->x;
		m_y = Game::doors[Game::doors[doorID]->nextDoorID]->y;
		m_direction = Game::doors[Game::doors[doorID]->nextDoorID]->direction;
		
		// Move view to display map correctly
		Map::View->setCenter(Game::currentMap->x() * MAP_WIDTH * 16 + MAP_WIDTH * 16 / 2, Game::currentMap->y() * MAP_HEIGHT * 16 + MAP_HEIGHT * 16 / 2);
		
		// Transition
		for(u16 x = 0 ; x <= MAP_HEIGHT / 1.5 ; x++) {
			rect1.move(-32, 0);
			rect2.move(32, 0);
			
			Game::MainWindow->clear();
			Game::currentMap->render();
			Game::currentMap->renderNPCs();
			Game::currentMap->renderMonsters();
			render();
			Interface::renderHUD();
			Game::MainWindow->setView(*Sprite::View);
			Game::MainWindow->draw(rect1);
			Game::MainWindow->draw(rect2);
			Game::MainWindow->setView(Game::MainWindow->getDefaultView());
			Game::MainWindow->display();
		}
		
		// The player is in the door
		inDoor = true;
	}
	
	if((!inTiles((m_x + 2) >> 4, (m_y + 2) >> 4, changeMapTiles))
	&& (!inTiles((m_x + 14) >> 4, (m_y + 14) >> 4, changeMapTiles))) {
		// The player isn't in the door anymore
		inDoor = false;
	}
}

void Player::testCollisions() {
	// Right
	if((m_vx > 0) && ((!passable(m_x + 12, m_y + 8)) || (!passable(m_x + 12, m_y + 13)))) {
		// Reset movement vector to stop player
		m_vx = 0;
		
		// Obstacle up
		if(passable(m_x + 12, m_y + 13) && (!passable(m_x + 12, m_y + 8))) {
			if(m_vy == 0 && !collidedMonster && !collidedNPC) m_vy = 1;
		}
		
		// Obstacle down
		if(passable(m_x + 12, m_y + 8) && (!passable(m_x + 12, m_y + 13))) {
			if(m_vy == 0 && !collidedMonster && !collidedNPC) m_vy = -1;
		}
	}
	
	// Left
	if((m_vx < 0) && ((!passable(m_x + 3, m_y + 8)) || (!passable(m_x + 3, m_y + 13)))) {
		// Reset movement vector to stop player
		m_vx = 0;
		
		// Obstacle up
		if(passable(m_x + 3, m_y + 13) && (!passable(m_x + 3, m_y + 8))) {
			if(m_vy == 0 && !collidedMonster && !collidedNPC) m_vy = 1;
		}
		
		// Obstacle down
		if(passable(m_x + 3, m_y + 8) && (!passable(m_x + 3, m_y + 13))) {
			if(m_vy == 0 && !collidedMonster && !collidedNPC) m_vy = -1;
		}
	}
	
	// Up
	if((m_vy < 0) && ((!passable(m_x + 5, m_y + 5)) || (!passable(m_x + 10, m_y + 5)))) {
		// Reset movement vector to stop player
		m_vy = 0;
		
		// Obstacle left
		if(passable(m_x + 10, m_y + 5) && (!passable(m_x + 5, m_y + 5))) {
			if(m_vx == 0 && !collidedMonster && !collidedNPC) m_vx = 1;
		}
		
		// Obstacle right
		if(passable(m_x + 5, m_y + 5) && (!passable(m_x + 10, m_y + 5))) {
			if(m_vx == 0 && !collidedMonster && !collidedNPC) m_vx = -1;
		}
	}
	
	// Down
	if((m_vy > 0) && ((!passable(m_x + 5, m_y + 15)) || (!passable(m_x + 10, m_y + 15)))) {
		// Reset movement vector to stop player
		m_vy = 0;
		
		// Obstacle left
		if(passable(m_x + 10, m_y + 15) && (!passable(m_x + 5, m_y + 15))) {
			if(m_vx == 0 && !collidedMonster && !collidedNPC) m_vx = 1;
		}
		
		// Obstacle right
		if(passable(m_x + 5, m_y + 15) && (!passable(m_x + 10, m_y + 15))) {
			if(m_vx == 0 && !collidedMonster && !collidedNPC) m_vx = -1;
		}
	}
}

void Player::sword() {
	// If S is pressed
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		// Update attack state
		m_isAttacking = true;
	}
	
	// If the player attacks
	if(m_isAttacking) {
		// If the animation is at end
		if(animationAtEnd(m_direction + 4)) {
			m_isAttacking = false;
		}
		
		// Play attack animation
		playAnimation(m_x, m_y, m_direction + 4);
		
		// Get sword position
		s16 mx = 0; s16 my = 0;
		
		// Play sword animation
		switch(m_direction) {
			case Direction::Down:
				if(m_swordSpr->animationAtFrame(m_direction, 0))	  { mx = -12;	my = 0;  }
				else if(m_swordSpr->animationAtFrame(m_direction, 1)) { mx = -12;	my = 12; }
				else if(m_swordSpr->animationAtFrame(m_direction, 2)) { mx = 0;		my = 16; }
				else if(m_swordSpr->animationAtFrame(m_direction, 3)) { mx = 0;		my = 16; }
				break;
			case Direction::Right:
				if(m_swordSpr->animationAtFrame(m_direction, 0))	  { mx = 0;		my = -12; }
				else if(m_swordSpr->animationAtFrame(m_direction, 1)) { mx = 14;	my = -12; }
				else if(m_swordSpr->animationAtFrame(m_direction, 2)) { mx = 14;	my = 0;   }
				else if(m_swordSpr->animationAtFrame(m_direction, 3)) { mx = 14;	my = 0;   }
				break;
			case Direction::Left:
				if(m_swordSpr->animationAtFrame(m_direction, 0))	  { mx = 0;		my = -12; }
				else if(m_swordSpr->animationAtFrame(m_direction, 1)) { mx = -14;	my = -12; }
				else if(m_swordSpr->animationAtFrame(m_direction, 2)) { mx = -14;	my = 0;   }
				else if(m_swordSpr->animationAtFrame(m_direction, 3)) { mx = -14;	my = 0;   }
				break;
			case Direction::Up:	
				if(m_swordSpr->animationAtFrame(m_direction, 0))	  { mx = 12;	my = 0;  }
				else if(m_swordSpr->animationAtFrame(m_direction, 1)) { mx = 12;	my = -12; }
				else if(m_swordSpr->animationAtFrame(m_direction, 2)) { mx = 0;		my = -12; }
				else if(m_swordSpr->animationAtFrame(m_direction, 3)) { mx = 0;		my = -12; }
				break;
			default: break;
		}
		m_swordSpr->playAnimation(m_x + mx, m_y + my, m_direction);
	}
}

bool blockedCommands = false;
void Player::actions() {
	if(!blockedCommands) {
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			// Set vertical movement vector negative
			m_vy = -1;
			
			// If all other directional keys are released
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				// Set direction to up
				m_direction = Direction::Up;
			}
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			// Set vertical movement vector positive
			m_vy = 1;
			
			// If all other directional keys are released
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				// Set direction to down
				m_direction = Direction::Down;
			}
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			// Set horizontal movement vector negative
			m_vx = -1;
			
			// If all other directional keys are released
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				// Set direction to left
				m_direction = Direction::Left;
			}
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			// Set horizontal movement vector positive
			m_vx = 1;
			
			// If all other directional keys are released
			if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				// Set direction to right
				m_direction = Direction::Right;
			}
		}
		
		// Sword attack
		sword();
	}
	
	// Test collisions
	doorCollisions();
	testCollisions();
	
	// If player collided a monster, hurt him
	if(collidedMonster) {
		if(m_hurtTimer.time() - m_timerLastValue > 5) {
			// Block commands
			blockedCommands = true;
			
			// Change player texture
			sf::Color c = m_spr.getColor();
			m_spr.setColor(sf::Color(255-c.r, 255-c.g, 255-c.b));
			
			// Get enemy direction vectors
			s8 e_x = m_x - collidedMonster->x();
			s8 e_y = m_y - collidedMonster->y();
			
			// Set movement vectors
			if(!collidedTile && abs(e_x) > 8) m_vx = (e_x==0)?0:((e_x<0)?-2:2);
			if(!collidedTile && abs(e_y) > 8) m_vy = (e_y==0)?0:((e_y<0)?-2:2);
			
			// Temporary collided monster
			Monster *tmpCollidedMonster = collidedMonster;
			
			// Reset collided monster
			collidedMonster = NULL;
			
			// Test collisions
			doorCollisions();
			testCollisions();
			
			// Reset collided monster with temp value
			collidedMonster = tmpCollidedMonster;
			
			// Reset timer last value
			m_timerLastValue = m_hurtTimer.time();
			
			// Reset collided monster and blocked commands states
			if(abs(e_x) > 24 || abs(e_y) > 24 || collidedTile) {
				collidedMonster = NULL;
				blockedCommands = false;
				m_spr.setColor(m_defaultColor);
			}
		}
		
		if(m_hurtTimer.time() > 500) {
			// Hurt player
			m_lifes--;
			
			// Reset timer
			m_hurtTimer.reset();
			m_hurtTimer.start();
			
			// Reset timer last value
			m_timerLastValue = m_hurtTimer.time();
		}
	}
	
	// Move the player
	m_x += m_vx * PLAYER_SPEED;
	m_y += m_vy * PLAYER_SPEED;
	
	// Reset movement vectors
	m_vx = 0;
	m_vy = 0;
	
	// If A is pressed, and the player collided a NPC, talk to him
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && collidedNPC) {
		collidedNPC->speak();
	}
}

void Player::render() {
	// If all directional keys are released
	if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		// Render a single frame
		if(!m_isAttacking) drawFrame(m_x, m_y, m_direction);
	} else {
		// Play walk animation
		if(!m_isAttacking) playAnimation(m_x, m_y, m_direction);
	}
}

