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

bool CollisionManager::passable(s16 x, s16 y) {
	// Collisions with map
	return !inTable(MapManager::nonPassableTiles, MapManager::currentMap->tilesetInfo()[MapManager::currentMap->getTile(x >> 4, y >> 4)]);
}

bool CollisionManager::collidesWithCharacter(Character *c, s16 x, s16 y) {
	// FIXME: Understand why it doesn't work that way
	/*u16 x1 = c->x() + collisionMatrix[(c->direction() - 1) & 3][0];
	u16 y1 = c->y() + collisionMatrix[(c->direction() - 1) & 3][1];
	u16 x2 = c->x() + collisionMatrix[(c->direction() - 1) & 3][2];
	u16 y2 = c->y() + collisionMatrix[(c->direction() - 1) & 3][3];*/
	
	for(std::vector<Character*>::iterator it = MapManager::currentMap->characters()->begin() ; it != MapManager::currentMap->characters()->end() ; it++) {
		if((((*it)->x() < x && (*it)->x() + (*it)->frameSize() > x
		&&   (*it)->y() < y && (*it)->y() + (*it)->frameSize() > y)
		/*||	((*it)->x() < x2 && (*it)->x() + (*it)->frameSize() > x2
		&&   (*it)->y() < y2 && (*it)->y() + (*it)->frameSize() > y2)*/)
		&& c->id() != (*it)->id()) {
			return true;
		}
	}
	
	return false;
}

void CollisionManager::doorCollisions(Character *c) {
	/*if(inTiles((c->x() + 8) >> 4, (c->y() + 8) >> 4, MapManager::changeMapTiles) && !c->inDoor()) {
		// Search for the door
		s16 doorID = DoorManager::findDoorID(c->x(), c->y(), MapManager::currentMap->id(), MapManager::currentMap->area());
		
		// If door isn't found
		if(doorID == -1) return;
		
		// Reset movement vectors
		c->stop();
		c->movementTimer().stop();
		
		// Initialize transition
		Game::MainWindow->clear();
		Game::MainWindow->drawFillRect(Game::MainWindow->viewportX(), Game::MainWindow->viewportY(), Game::MainWindow->viewportW() / 2, Game::MainWindow->viewportH(), Color::white);
		Game::MainWindow->drawFillRect(Game::MainWindow->viewportX() + Game::MainWindow->viewportW() / 2, Game::MainWindow->viewportY(), Game::MainWindow->viewportW() / 2, Game::MainWindow->viewportH(), Color::white);
		Game::MainWindow->update();
		
		// Update all values
		MapManager::currentMap = Game::mapAreas[DoorManager::doors[DoorManager::doors[doorID]->nextDoorID]->mapArea][DoorManager::doors[DoorManager::doors[doorID]->nextDoorID]->mapID];
		if(!MapManager::currentMap) exit(EXIT_FAILURE);
		
		// Regen monsters and reset positions
		for(u16 i = 0 ; i < MapManager::currentMap->monsters().size() ; i++) {
			MapManager::currentMap->monsters()[i]->reset();
		}
		for(u16 i = 0 ; i < MapManager::currentMap->NPCs().size() ; i++) {
			MapManager::currentMap->NPCs()[i]->reset();
		}
		
		c->x(DoorManager::doors[DoorManager::doors[doorID]->nextDoorID]->x);
		c->y(DoorManager::doors[DoorManager::doors[doorID]->nextDoorID]->y);
		c->direction(DoorManager::doors[DoorManager::doors[doorID]->nextDoorID]->direction);
		
		// Move view to display map correctly
		Map::viewRect.x = MapManager::currentMap->x() * MAP_WIDTH * 16;
		Map::viewRect.y = MapManager::currentMap->y() * MAP_HEIGHT * 16;
		
		// Transition
		for(u16 x = 0 ; x <= MAP_HEIGHT / 1.5 ; x++) {
			Game::MainWindow->clear();
			CharacterManager::renderCharacters();
			c->render();
			Interface::renderHUD();
			Game::MainWindow->drawFillRect(Game::MainWindow->viewportX() - 32 * x, Game::MainWindow->viewportY(), Game::MainWindow->viewportW() / 2, Game::MainWindow->viewportH(), Color::white);
			Game::MainWindow->drawFillRect(Game::MainWindow->viewportX() + Game::MainWindow->viewportW() / 2 + 32 * x, Game::MainWindow->viewportY(), Game::MainWindow->viewportW() / 2, Game::MainWindow->viewportH(), Color::white);
			Game::MainWindow->update();
		}
		
		// The player is in the door
		c->inDoor(true);
	}
	
	if((!inTiles((c->x() +  2) >> 4, (c->y() +  2) >> 4, MapManager::changeMapTiles))
	&& (!inTiles((c->x() + 14) >> 4, (c->y() + 14) >> 4, MapManager::changeMapTiles)) && c->inDoor()) {
		// The player isn't in the door anymore
		c->inDoor(false);
	}*/
}

