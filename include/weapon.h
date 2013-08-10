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
#ifndef WEAPON_H
#define WEAPON_H

typedef enum {
	WEAPON_MELEE,
	WEAPON_TOOL,
	WEAPON_PROJECTILE
} WeaponType;

class Weapon : public Sprite {
	public:
		// Constructor and destructor
		Weapon(WeaponType type);
		~Weapon();
		
		// Action function
		void action();
		
		// Get types
		bool isMelee()		{ return m_type == WEAPON_MELEE;	  }
		bool isTool()		{ return m_type == WEAPON_TOOL;		  }
		bool isProjectile() { return m_type == WEAPON_PROJECTILE; }
		
	private:
		// Type
		WeaponType type;
		
;

#endif // WEAPON_H
