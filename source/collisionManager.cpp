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
#include "mapManager.h"
#include "doorManager.h"
#include "characterManager.h"
#include "collisionManager.h"
#include "tools.h"
#include "interface.h"
#include "game.h"

// Initialize collision matrix
u16 CollisionManager::collisionMatrix[4][4] = {
	{12,8,12,13},	// Right
	{3,8,3,13},		// Left
	{5,5,10,5},		// Up
	{5,15,10,15}	// Down
};

bool CollisionManager::passable(Character *c, s16 x, s16 y) {
	// Setup tileX and tileY
	s16 tileX = x >> 4;
	s16 tileY = y >> 4;
	
	// Reset collision states
	c->collidedCharacter(NULL);
	c->collidedTile(0);
	
	// Collisions with NPCs
	if(!c->isNPC()) {
		for(u16 i = 0 ; i < Game::currentMap->NPCs().size() ; i++) {
			if((Game::currentMap->NPCs()[i]->x() < x && Game::currentMap->NPCs()[i]->x() + 16 > x)
			&& (Game::currentMap->NPCs()[i]->y() < y && Game::currentMap->NPCs()[i]->y() + 16 > y)) {
				c->collidedCharacter(Game::currentMap->NPCs()[i]);
					Game::currentMap->NPCs()[i]->collidedCharacter(c);
				return false;
			}
		}
	}
	
	// Collisions with monsters
	if(!c->isMonster()) {
		for(u16 i = 0 ; i < Game::currentMap->monsters().size() ; i++) {
			if((Game::currentMap->monsters()[i]->x() < x && Game::currentMap->monsters()[i]->x() + 16 > x)
			&& (Game::currentMap->monsters()[i]->y() < y && Game::currentMap->monsters()[i]->y() + 16 > y)
			&& Game::currentMap->monsters()[i]->lifes() > 0) {
				c->collidedCharacter(Game::currentMap->monsters()[i]);
				Game::currentMap->monsters()[i]->collidedCharacter(c);
				return false;
			}
		}
	}
	
	// Collisions with player
	if(!c->isPlayer()) {
		if(Game::player->x() < x && Game::player->x() + 16 > x
		&& Game::player->y() < y && Game::player->y() + 16 > y) {
			c->collidedCharacter(Game::player);
			return false;
		}
	}
	
	// Collisions with map
	if(c->isMonster() || c->isNPC() || c->isPlayer()) {
		if(inTable(MapManager::nonPassableTiles, Game::currentMap->tilesetInfo()[Game::currentMap->getTile(tileX, tileY)])) {
			c->collidedTile(Game::currentMap->tilesetInfo()[Game::currentMap->getTile(tileX, tileY)]);
			return false;
		} else return true;
	} else {
		return true;
	}
}

void CollisionManager::testCollisions(Character *c){
	// 0: Right | 1: Left | 2: Up | 3:Down
	for(u8 i = 0 ; i < 4 ; i++) {
		if(((i==0)?(c->vx() > 0):((i==1)?(c->vx() < 0):((i==2)?(c->vy() < 0):(c->vy() > 0))))
		&& (!passable(c, c->x() + collisionMatrix[i][0], c->y() + collisionMatrix[i][1])
		 || !passable(c, c->x() + collisionMatrix[i][2], c->y() + collisionMatrix[i][3]))) {
			// Reset movement vector
			if(i<2) c->vx(0);
			else	c->vy(0);
			
			// Obstacles
			if( passable(c, c->x() + collisionMatrix[i][2], c->y() + collisionMatrix[i][3])
			&& !passable(c, c->x() + collisionMatrix[i][0], c->y() + collisionMatrix[i][1])) {
				if(((i<2)?(c->vy() == 0):(c->vx() == 0)) && !c->collidedCharacter()/* && !collidedWeapon*/) {
					if(i<2)	c->vy(1);
					else	c->vx(1);
				}
			}
			if( passable(c, c->x() + collisionMatrix[i][0], c->y() + collisionMatrix[i][1])
			&& !passable(c, c->x() + collisionMatrix[i][2], c->y() + collisionMatrix[i][3])) {
				if(((i<2)?(c->vy() == 0):(c->vx() == 0)) && !c->collidedCharacter()/* && !collidedWeapon*/) {
					if(i<2) c->vy(-1);
					else	c->vx(-1);
				}
			}
		}
	}
}
bool inDoor = false;
void CollisionManager::doorCollisions(Character *c) {
	if(((inTiles((c->x() + 8) >> 4, (c->y() + 8) >> 4, MapManager::changeMapTiles)) && (!inDoor))) {
		// Reset movement vectors
		c->vx(0);
		c->vy(0);
		
		// Search for the door
		s16 doorID = DoorManager::findDoorID(c->x(), c->y(), Game::currentMap->id(), Game::currentMap->area());
		
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
		
		c->x(Game::doors[Game::doors[doorID]->nextDoorID]->x);
		c->y(Game::doors[Game::doors[doorID]->nextDoorID]->y);
		c->direction(Game::doors[Game::doors[doorID]->nextDoorID]->direction);
		
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
			c->render();
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
	
	if((!inTiles((c->x() + 2) >> 4, (c->y() + 2) >> 4, MapManager::changeMapTiles))
	&& (!inTiles((c->x() + 14) >> 4, (c->y() + 14) >> 4, MapManager::changeMapTiles))) {
		// The player isn't in the door anymore
		inDoor = false;
	}
}

