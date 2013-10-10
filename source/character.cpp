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
#include "font.h"
#include "sound.h"
#include "net.h"
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
#include "tools.h"
#include "interface.h"
#include "game.h"

u16 Character::nbCharacters = 0;

Character::Character(const char *filename, CharacterType type, s16 x, s16 y, CharacterDirection direction, u16 mapID, u16 area, u8 frameWidth, u8 frameHeight) : Sprite(filename, frameWidth, frameHeight) {
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
	
	m_hitboxX = 3;
	m_hitboxY = 10;
	m_hitboxW = m_frameWidth - 6;
	m_hitboxH = m_frameHeight - 12;
	
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

bool passable(s16 x, s16 y) {
	// Collisions with map
	return !inTable(MapManager::nonPassableTiles, MapManager::currentMap->tilesetInfo()[MapManager::currentMap->getTile(x >> 4, y >> 4)]);
}

bool Character::inNonPassableTile() {
	return(!passable(m_x + m_hitboxX + m_vx			   , m_y + m_hitboxY + m_vy			   )
		|| !passable(m_x + m_hitboxX + m_vx + m_hitboxW, m_y + m_hitboxY + m_vy			   )
		|| !passable(m_x + m_hitboxX + m_vx			   , m_y + m_hitboxY + m_vy + m_hitboxH)
		|| !passable(m_x + m_hitboxX + m_vx + m_hitboxW, m_y + m_hitboxY + m_vy + m_hitboxH));
}

void Character::testCollisions() {
	// Ensure that movement timer is started
	m_movementTimer.start();
	
	// Ensure that collision state is reset
	m_inCollision = false;
	
	// Test map
	mapCollisions();
	
	// Test characters
	charactersCollisions();
}

void Character::mapCollisions() {
	/*// Obstacles
	if( passable(m_x + m_hitboxW, m_y + m_hitboxH)
	&& !passable(m_x + m_hitboxW, m_y + m_hitboxH)) {
		if(((i<2)?(m_vy == 0):(m_vx == 0))) {
			if(i<2)	m_vy = 1; else m_vx = 1;
		}
	}
	if( passable(m_x + m_hitboxW, m_y + m_hitboxH)
	&& !passable(m_x + m_hitboxW, m_y + m_hitboxH)) {
		if(((i<2)?(m_vy == 0):(m_vx == 0))) {
			if(i<2) m_vy = -1; else	m_vx = -1;
		}
	}*/
	
	// Up and down
	if(m_vy != 0) {
		if (!passable(m_x + m_hitboxX			 , m_y + m_hitboxY + m_vy			 )
		||  !passable(m_x + m_hitboxX + m_hitboxW, m_y + m_hitboxY + m_vy			 )
		||  !passable(m_x + m_hitboxX			 , m_y + m_hitboxY + m_vy + m_hitboxH)
		||  !passable(m_x + m_hitboxX + m_hitboxW, m_y + m_hitboxY + m_vy + m_hitboxH)) {
			/* Let player slide on tiles corners */
			// Top left
			if (!passable(m_x + m_hitboxX + m_hitboxW + 8, m_y + m_hitboxY + m_vy)
			&&	 passable(m_x + m_hitboxX				 , m_y + m_hitboxY + m_vy)) {
				m_vx = -1;
			}
			// Top right
			if (!passable(m_x + m_hitboxX - 8		 , m_y + m_hitboxY + m_vy)
			&&   passable(m_x + m_hitboxX + m_hitboxW, m_y + m_hitboxY + m_vy)) {
				m_vx = 1;
			}
			// Bottom left
			if (!passable(m_x + m_hitboxX + m_hitboxW + 8, m_y + m_hitboxY + m_hitboxH + m_vy)
			&&	 passable(m_x + m_hitboxX				 , m_y + m_hitboxY + m_hitboxH + m_vy)) {
				m_vx = -1;
			}
			// Bottom right
			if (!passable(m_x + m_hitboxX - 8		 , m_y + m_hitboxY + m_hitboxH + m_vy)
			&&   passable(m_x + m_hitboxX + m_hitboxW, m_y + m_hitboxY + m_hitboxH + m_vy)) {
				m_vx = 1;
			}
			
			// Reset vertical movement vector
			m_vy = 0;
			
			// Execute collision action
			collisionAction(NULL);
		}
	}
	
	// Left and right
	if(m_vx != 0) {
		if (!passable(m_x + m_hitboxX + m_vx			, m_y + m_hitboxY			 )
		||  !passable(m_x + m_hitboxX + m_vx + m_hitboxW, m_y + m_hitboxY			 )
		||  !passable(m_x + m_hitboxX + m_vx			, m_y + m_hitboxY + m_hitboxH)
		||  !passable(m_x + m_hitboxX + m_vx + m_hitboxW, m_y + m_hitboxY + m_hitboxH)) {
			/* Let player slide on tiles corners */
			// Top left
			if (!passable(m_x + m_hitboxX + m_vx, m_y + m_hitboxY + m_hitboxH + 0)
			&&	 passable(m_x + m_hitboxX + m_vx, m_y + m_hitboxY				 )) {
				m_vy = -1;
			}
			// Bottom left
			if (!passable(m_x + m_hitboxX + m_vx, m_y + m_hitboxY - 0		 )
			&&   passable(m_x + m_hitboxX + m_vx, m_y + m_hitboxY + m_hitboxH)) {
				m_vy = 1;
			}
			// Top right
			if (!passable(m_x + m_hitboxX + m_hitboxW + m_vx, m_y + m_hitboxY + m_hitboxH + 0)
			&&	 passable(m_x + m_hitboxX + m_hitboxW + m_vx, m_y + m_hitboxY				 )) {
				m_vy = -1;
			}
			// Bottom right
			if (!passable(m_x + m_hitboxX + m_hitboxW + m_vx, m_y + m_hitboxY - 0		 )
			&&   passable(m_x + m_hitboxX + m_hitboxW + m_vx, m_y + m_hitboxY + m_hitboxH)) {
				m_vy = 1;
			}
			
			// Reset horizontal movement vector
			m_vx = 0;
			
			// Execute collision action
			collisionAction(NULL);
		}
	}
}

void Character::charactersCollisions() {
	for(std::vector<Character*>::iterator it = MapManager::currentMap->characters()->begin() ; it != MapManager::currentMap->characters()->end() ; it++) {
		if((((*it)->x() + (*it)->hitboxX() < m_x + m_hitboxX			 && (*it)->x() + (*it)->hitboxX() + (*it)->hitboxW() > m_x + m_hitboxX)
		||  ((*it)->x() + (*it)->hitboxX() < m_x + m_hitboxX + m_hitboxW && (*it)->x() + (*it)->hitboxX() + (*it)->hitboxW() > m_x + m_hitboxX + m_hitboxW))
		&& (((*it)->y() + (*it)->hitboxY() < m_y + m_hitboxY			 && (*it)->y() + (*it)->hitboxY() + (*it)->hitboxH() > m_y + m_hitboxY)
		||  ((*it)->y() + (*it)->hitboxY() < m_y + m_hitboxY + m_hitboxH && (*it)->y() + (*it)->hitboxY() + (*it)->hitboxH() > m_y + m_hitboxY + m_hitboxH))) {
			collisionAction(*it);
		}
	}
}

void Character::collisionAction(Character *c) {
	if(!c || c->isNPC()) {
		// Stop movement timer
		m_movementTimer.stop();
		
		// Update collision state
		m_inCollision = true;
		
		// Exit function to avoid problems if c == NULL
		return;
	}
	
	if(c->isMonster() && isPlayer()) {
		// Hurt player
		hurt(m_x + m_hitboxW / 2 - c->x() + c->hitboxW() / 2, m_y + m_hitboxH / 2 - c->y() + c->hitboxH() / 2);
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
	if(m_hurtTimer.time() - m_hurtTimerLastValue > 5 && m_hurt) {
		// Change color
		SDL_SetTextureColorMod(m_texture, rand()%255, rand()%255, rand()%255);
		
		// Normalize hurt movement vectors
		s8 nhx = m_hx / ((m_hx != 0)?abs(m_hx):1);
		s8 nhy = m_hy / ((m_hy != 0)?abs(m_hy):1);
		
		// Update movement vectors
		m_vx = nhx;
		m_vy = nhy;
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

