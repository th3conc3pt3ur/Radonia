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
#include "characterManager.h"
#include "tools.h"
#include "interface.h"
#include "game.h"

Font *Interface::defaultFont = NULL;
Sprite *Interface::hearts = NULL;
Image *Interface::pad = NULL;
Image *Interface::buttonA = NULL;

void Interface::titleScreen() {
	// Load title screen background
	Image *titleScreen = new Image("graphics/interface/titleScreen.png");
	
	// Wait for player
	SDL_Event event;
	bool cont = true;
	while(cont) {
		// Close window: exit game
		while(SDL_PollEvent(&event) != 0) {
			switch(event.type) {
				case SDL_QUIT: cont = false; break;
				case SDL_KEYDOWN: if(event.key.keysym.sym == SDLK_n) cont = false; break;
			}
		};
		
		// Clear main window
		Game::MainWindow->clear();
		
		// Display background
		titleScreen->render();
		
		// Update main window display
		Game::MainWindow->update();
	}
	
	// Delete title screen image
	delete titleScreen;
}

void Interface::initialize() {
	// Load default font
	defaultFont = new Font("fonts/vani.ttf");
	
	// Load hearts sprite
	hearts = new Sprite("graphics/interface/hearts.png");
	
	// Load pad image
	pad = new Image("graphics/interface/pad.png");
	pad->setAlpha(175);
	
	// Load button images
	buttonA = new Image("graphics/interface/a.png");
	buttonA->setAlpha(175);
}

void Interface::unload() {
	// Unload default font
	delete defaultFont;
	
	// Delete images
	delete hearts;
	delete pad;
	delete buttonA;
}

void Interface::renderHUD() {
	// Get viewport position
	s16 x = Game::MainWindow->viewportX();
	s16 y = Game::MainWindow->viewportY();
	
#ifdef PAD
	// Render pad
	pad->setPosRect(Game::MainWindow->viewportX() + 16, Game::MainWindow->viewportY() + Game::MainWindow->viewportH() - pad->height() - 16, pad->width(), pad->height());
	pad->render();
	
	// Render buttons
	buttonA->setPosRect(Game::MainWindow->viewportX() + Game::MainWindow->viewportW() - buttonA->width() - 16, Game::MainWindow->viewportY() + Game::MainWindow->viewportH() - buttonA->height() - 16, buttonA->width(), buttonA->height());
	buttonA->render();
#endif
	
	// Get heart data
	double entireHearts = 0;
	double piecesOfHearts = modf((double)CharacterManager::player()->lifes() / 4, &entireHearts);
	
	// Render hearts
	for(u8 i = 0 ; i < CharacterManager::player()->maxLifes() / 4 ; i++) {
		if(i < entireHearts) hearts->drawFrame(x + 16 * i, y, 4);
		else if(i == entireHearts + ceil(piecesOfHearts) - 1) hearts->drawFrame(x + 16 * i, y, ((i < entireHearts + ceil(piecesOfHearts) - 1) ? 4 : piecesOfHearts * 4));
		else if(i > entireHearts + ceil(piecesOfHearts) - 1) hearts->drawFrame(x + 16 * i, y, 0);
	}
}

void Interface::renderMonsterLife(Monster *monster) {
	// Render life bar
	Game::MainWindow->drawFillRect(monster->x() - 1, monster->y() - 6, monster->frameWidth() + 2, 4, Color::black);
	Game::MainWindow->drawFillRect(monster->x(), monster->y() - 5, monster->frameWidth(), 2, Color::white);
	Game::MainWindow->drawFillRect(monster->x(), monster->y() - 5, monster->lifes() * monster->frameWidth() / monster->maxLifes(), 2, Color::life);
}

void Interface::newDialogBox(const char *text) {
	// Draw box
	Game::MainWindow->drawFillRect((MAP_WIDTH * 16) / 8, 3 * (MAP_HEIGHT * 16) / 4, 6 * (MAP_WIDTH * 16) / 8, (MAP_HEIGHT * 16) / 4, Color::black);
	
	// Print text
	defaultFont->print(text, (MAP_WIDTH * 16) / 8 + 8, 3 * (MAP_HEIGHT * 16) / 4 + 8, FONT_LARGE, Color::white);
	defaultFont->print("Press 'A' key to continue.", 10 * MAP_WIDTH + 28, (MAP_HEIGHT - 1) * 16, FONT_SMALL, Color::white);
	
	// Update window
	Game::MainWindow->update();
	
	// Wait for attack key released
	while(Keyboard::isKeyPressed(Keyboard::GameAttack)) Keyboard::forceUpdate();
	
	// Wait for attack key pressed
	while(!Keyboard::isKeyPressed(Keyboard::GameAttack)) {
		// Process events
		SDL_Event event;
		while(SDL_PollEvent(&event) != 0) {
			switch(event.type) {
				case SDL_QUIT:
					// FIXME: Improve that: nothing is deleted
					exit(EXIT_SUCCESS);
					break;
			}
		}
		
		Game::MainWindow->update();
	}
	
	// Wait for attack key released
	while(Keyboard::isKeyPressed(Keyboard::GameAttack)) Keyboard::forceUpdate();
}

