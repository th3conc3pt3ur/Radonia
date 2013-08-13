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
#include "keyboard.h"

const u8 *Keyboard::state = NULL;

const u8 *Keyboard::getState() {
	return SDL_GetKeyboardState(NULL);
}

bool Keyboard::isKeyPressed(u32 key) {
	if(state[key]) return true;
	else		   return false;
}

void Keyboard::update() {
	state = getState();
}

void Keyboard::forceUpdate() {
	SDL_PumpEvents();
	update();
}

