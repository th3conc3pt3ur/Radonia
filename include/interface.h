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
#ifndef INTERFACE_H
#define INTERFACE_H

namespace Interface {
	// Display title screen
	void titleScreen();
	
	// Initialize interface
	void initialize();
	
	// Unload interface
	void unload();
	
	// Display HUD
	void renderHUD();
	
	// Render monster life
	void renderMonsterLife(Monster *monster);
	
	// Display a dialog box
	void newDialogBox(const char *text);
	
	// Default font
	extern Font *defaultFont;
	
	// Hearts sprite
	extern Sprite *hearts;
	
	// Pad image
	extern Image *pad;
	
	// Buttons images
	extern Image *buttonA;
}

#endif // INTERFACE_H
