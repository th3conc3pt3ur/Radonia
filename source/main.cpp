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
#include "init.h"
#include "window.h"
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
#include "interface.h"
#include "game.h"

using namespace std;

int main(int argc, char* argv[]) {
	// Initialize SDL
	initSDL();
	
	// Initialize game
	Game *game = new Game;
	
	// Display title screen
	//Interface::titleScreen();
	
	// Start the game loop
	game->mainLoop();
	
	// Delete all game objects
	delete game;
	
	// Exit SDL
	exitSDL();
	
	return 0;
}

