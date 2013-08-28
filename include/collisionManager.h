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
#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#define COLLISION_MATRIX(i, j, s) ((CollisionManager::collisionMatrix[(i)][(j)] >= 8)?((s) - (16 - CollisionManager::collisionMatrix[(i)][(j)])):(CollisionManager::collisionMatrix[(i)][(j)]))

namespace CollisionManager {
	// Collision matrix
	extern u16 collisionMatrix[6][4];
	
	// Test if the tile is passable
	bool passable(s16 x, s16 y);
	
	// Test if there is a character at that position
	bool collidesWithCharacter(Character *c, u8 i);
	
	// Test collisions of a character
	void doorCollisions(Character *c);
}

#endif // COLLISION_MANAGER_H
