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
#include "mapManager.h"
#include "doorManager.h"
#include "animationManager.h"
#include "characterManager.h"
#include "collisionManager.h"
#include "tools.h"
#include "interface.h"
#include "game.h"

u16 Character::nbCharacters = 0;

Character::Character(char *filename, CharacterType type, s16 x, s16 y, CharacterDirection direction, u16 mapID, u16 area, u8 frameWidth, u8 frameHeight) : Sprite(filename, frameWidth, frameHeight) {
	// Set character id
	m_id = nbCharacters;
	
	// Update characters counter
	nbCharacters++;
	
	// Set class members
	m_type = type;
	
	m_x = x;
	m_y = y;
	
	m_dx = x;
	m_dy = y;
	
	m_vx = 0;
	m_vy = 0;
	
	m_direction = direction;
	
	m_mapID = mapID;
	m_area = area;
	
	m_moving = false;
	
	m_movementTimer.start();
	
	m_countMoves = 0;
	m_vxCount = 0;
	m_vyCount = 0;
	
	m_inDoor = true;
	
	m_isAttacking = false;
	
	m_canMove = true;
	m_canTurn = true;
	
	m_hx = 0;
	m_hy = 0;
	
	m_hurtTimer.start();
	m_hurtTimerLastValue = m_hurtTimer.time();
	
	m_hurt = false;
	
	// FIXME: Temporary values
	m_lifes = 32;
	m_maxLifes = 32;
	
	// Load grass image
	m_grassEffect = new Image((char*)"graphics/animations/grassEffect.png");
	
	// Load water effect
	m_waterEffect = new Sprite((char*)"graphics/animations/waterEffect.png");
	m_waterEffect->addAnimation(3, WaterEffect_animations, 150);
}

Character::~Character() {
	// Unload effects images
	delete m_grassEffect;
	delete m_waterEffect;
}

void Character::render() {
	// If the character is dead, don't display it
	if(m_lifes <= 0) return;
	
	if(!m_isAttacking) {
		if(m_moving) playAnimation(m_x, m_y, m_direction);
		else		 drawFrame(m_x, m_y, m_direction);
	}
	
	// Test if character is in grass
	if(inZone(m_x, m_y, 2)) {
		m_grassEffect->setPosRect(m_x, m_y, m_grassEffect->width(), m_grassEffect->height());
		m_grassEffect->render();
	}
	
	// Test if character is in water
	if(inZone(m_x, m_y, 19)) {
		m_waterEffect->playAnimation(m_x, m_y + 8, 0);
	}
}

void Character::testCollisions() {
	// Ensure that movement timer is started
	m_movementTimer.start();
	
	// Ensure that collision state is reset
	m_inCollision = false;
	
	// 0: Right | 1: Left | 2: Up | 3:Down
	for(u8 i = 0 ; i < 4 ; i++) {
		if(((i==0)?(m_vx > 0):((i==1)?(m_vx < 0):((i==2)?(m_vy < 0):(m_vy > 0))))
		&& (!CollisionManager::passable(m_x + CollisionManager::collisionMatrix[i][0], m_y + CollisionManager::collisionMatrix[i][1])
		 || !CollisionManager::passable(m_x + CollisionManager::collisionMatrix[i][2], m_y + CollisionManager::collisionMatrix[i][3])
		 || CollisionManager::collidesWithCharacter(this, i)
		 )) {
			// Reset movement vectors
			if(i<2) m_vx = 0; else m_vy = 0;
			
			// Stop movement timer
			m_movementTimer.stop();
			
			// Update collision state
			m_inCollision = true;
			
			// Obstacles
			if( CollisionManager::passable(m_x + CollisionManager::collisionMatrix[i][2], m_y + CollisionManager::collisionMatrix[i][3])
			&& !CollisionManager::passable(m_x + CollisionManager::collisionMatrix[i][0], m_y + CollisionManager::collisionMatrix[i][1])) {
				if(((i<2)?(m_vy == 0):(m_vx == 0))) {
					if(i<2)	m_vy = 1; else m_vx = 1;
				}
			}
			if( CollisionManager::passable(m_x + CollisionManager::collisionMatrix[i][0], m_y + CollisionManager::collisionMatrix[i][1])
			&& !CollisionManager::passable(m_x + CollisionManager::collisionMatrix[i][2], m_y + CollisionManager::collisionMatrix[i][3])) {
				if(((i<2)?(m_vy == 0):(m_vx == 0))) {
					if(i<2) m_vy = -1; else	m_vx = -1;
				}
			}
		}
	}
}

void Character::doorCollisions() {
	if(inZones(m_x, m_y, MapManager::changeMapTiles) && !m_inDoor) {
		// Search for the door
		s16 doorID = DoorManager::findDoorID(m_x, m_y, MapManager::currentMap->id(), MapManager::currentMap->area());
		
		// If door isn't found
		if(doorID == -1) return;
		
		// Reset movement vectors
		m_vx = 0;
		m_vy = 0;
		m_movementTimer.stop();
		
		// Initialize transition
		Game::MainWindow->clear();
		Game::MainWindow->drawFillRect(Game::MainWindow->viewportX(), Game::MainWindow->viewportY(), Game::MainWindow->viewportW() / 2, Game::MainWindow->viewportH(), Color::white);
		Game::MainWindow->drawFillRect(Game::MainWindow->viewportX() + Game::MainWindow->viewportW() / 2, Game::MainWindow->viewportY(), Game::MainWindow->viewportW() / 2, Game::MainWindow->viewportH(), Color::white);
		Game::MainWindow->update();
		
		// Update all values
		MapManager::currentMap = MapManager::mapAreas[DoorManager::doors[DoorManager::doors[doorID]->nextDoorID]->mapArea][DoorManager::doors[DoorManager::doors[doorID]->nextDoorID]->mapID];
		if(!MapManager::currentMap) exit(EXIT_FAILURE);
		
		// Regen monsters and reset positions
		std::vector<Character*> *v = MapManager::currentMap->characters();
		for(std::vector<Character*>::iterator it = v->begin() ; it != v->end() ; it++) {
			if(!(*it)->isPlayer()) (*it)->reset();
		}
		
		m_x = DoorManager::doors[DoorManager::doors[doorID]->nextDoorID]->x;
		m_y = DoorManager::doors[DoorManager::doors[doorID]->nextDoorID]->y;
		m_direction = DoorManager::doors[DoorManager::doors[doorID]->nextDoorID]->direction;
		
		// Move view to display map correctly
		Map::viewRect.x = MapManager::currentMap->x() * MAP_WIDTH * 16;
		Map::viewRect.y = MapManager::currentMap->y() * MAP_HEIGHT * 16;
		
		// Transition
		for(u16 x = 0 ; x <= MAP_HEIGHT / 1.5 ; x++) {
			Game::MainWindow->clear();
			MapManager::currentMap->render();
			CharacterManager::renderCharacters();
			Interface::renderHUD();
			Game::MainWindow->drawFillRect(Game::MainWindow->viewportX() - 32 * x, Game::MainWindow->viewportY(), Game::MainWindow->viewportW() / 2, Game::MainWindow->viewportH(), Color::white);
			Game::MainWindow->drawFillRect(Game::MainWindow->viewportX() + Game::MainWindow->viewportW() / 2 + 32 * x, Game::MainWindow->viewportY(), Game::MainWindow->viewportW() / 2, Game::MainWindow->viewportH(), Color::white);
			Game::MainWindow->update();
		}
		
		// The player is in the door
		m_inDoor = true;
	}
	
	if(!inZones(m_x, m_y, MapManager::changeMapTiles) && m_inDoor) {
		// The player isn't in the door anymore
		m_inDoor = false;
	}
}

void Character::hurt(s16 hx, s16 hy) {
	// Update hurt state
	m_hurt = true;
	
	// Update hurt movement vectors
	m_hx = hx;
	m_hy = hy;
}

void Character::hurtMovement() {
	if(m_hurtTimer.time() - m_hurtTimerLastValue > 5) {
		// Change color
		SDL_SetTextureColorMod(m_texture, rand()%255, rand()%255, rand()%255);
		
		// Normalize hurt movement vectors
		s8 nhx = m_hx / ((m_hx != 0)?abs(m_hx):1);
		s8 nhy = m_hy / ((m_hy != 0)?abs(m_hy):1);
		
		// Update movement vectors
		m_vx = nhx;
		m_vy = nhy;
		
		// Test collisions
		testCollisions();
	}
	
	if(m_hurtTimer.time() > 500) {
		// Hurt character
		m_lifes--;
		
		// Reset timer
		m_hurtTimer.reset();
		m_hurtTimer.start();
		
		// Reset timer last value
		m_hurtTimerLastValue = m_hurtTimer.time();
		
		// Reset color
		SDL_SetTextureColorMod(m_texture, 255, 255, 255);
		
		// Reset hurt movement vectors
		m_hx = 0;
		m_hy = 0;
		
		// Reset hurt state
		m_hurt = false;
	}
}

void Character::reset() {
	// Reset class members
	m_x = m_dx;
	m_y = m_dy;
	
	m_vx = 0;
	m_vy = 0;
	
	m_moving = false;
	
	m_movementTimer.reset();
	m_movementTimer.start();
	
	m_countMoves = 0;
	m_vxCount = 0;
	m_vyCount = 0;
	
	m_inDoor = true;
	
	m_isAttacking = false;
	
	m_canMove = true;
	m_canTurn = true;
	
	m_hx = 0;
	m_hy = 0;
	
	m_hurtTimer.reset();
	m_hurtTimer.start();
	
	m_hurt = false;
	
	m_inCollision = false;
	
	m_lifes = m_maxLifes;
}

